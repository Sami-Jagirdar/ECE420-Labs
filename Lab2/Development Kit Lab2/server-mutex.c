#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "common.h"
#include <bits/pthreadtypes.h>
#include <sys/time.h>
#include "timer.h"

// Idea is to have a read write lock on each element of the array since if
// one element is being written to / read from, the other elements aren't affected

char** theArray;
int n;
struct sockaddr_in sock_var;
int serverFileDescriptor;
int clientFileDescriptor;
int i;
pthread_mutex_t *mutexes;

struct thread_args {
    int clientFileDescriptor; // client file to send to
    int times_idx; // index in times array to write to
};

// Mutex and array to handle recording times

double times[COM_NUM_REQUEST] = {0};
int num_times;
pthread_mutex_t times_mutex;

void record_time(double my_time) {
    // add my_time to times array and rerun saveTimes function.
    // we were not asked to optimize this, so use full-array mutex for simplicity
    pthread_mutex_lock(&times_mutex);
        times[num_times] = my_time;
        num_times++;
    pthread_mutex_unlock(&times_mutex);
}

void *RequestHandler(void * arg) {
    int clientFileDescriptor = (int) arg;
    char clientMessage[COM_BUFF_SIZE]; // "XXX-Y-SSSSSS\0"
    char buffer[COM_BUFF_SIZE];
    double time_start, time_end;

    read(clientFileDescriptor, clientMessage, COM_BUFF_SIZE);
    printf("Client %d sent request: %s\n", clientFileDescriptor, clientMessage);

    // parse the message
    ClientRequest request;
    ParseMsg(clientMessage, &request);

    GET_TIME(time_start);

    if (request.is_read) {
        // acquire a read lock and perform read on array element
        pthread_mutex_lock(mutexes + request.pos);
        getContent(buffer, request.pos, theArray);
        pthread_mutex_unlock(mutexes + request.pos);       
    } else {
        // acquire a write lock and perform write on array element
        pthread_mutex_lock(mutexes + request.pos);
        setContent(request.msg, request.pos, theArray);
        getContent(buffer, request.pos, theArray); //?
        pthread_mutex_unlock(mutexes + request.pos);
    }

    GET_TIME(time_end);
    record_time(time_end - time_start);

    // Send content back to the client
    write(clientFileDescriptor, buffer, COM_BUFF_SIZE);
    close(clientFileDescriptor);

    return NULL;
}

int main(int argc, char* argv[]) {

    // Take in input arguments

    if (argc != 4) {
        printf("Usage: %s <sizeOfArray> <serverIP> <portNumber>\n", argv[0]);
        exit(1);
    }
    
    n = atoi(argv[1]);
    if (n < 1) {
        printf("INCORRECT INPUT: The number of elements in the array must be at least 1.\n");
        return 1;
    }

    // Set up server to receive requests

    char* serverIPAddr = argv[2];
    int serverPort = atoi(argv[3]);

    printf("Server IP: %s\n", serverIPAddr);
    printf("Server Port: %d\n", serverPort);

    sock_var.sin_addr.s_addr = inet_addr(serverIPAddr);
    sock_var.sin_port = serverPort;
    sock_var.sin_family = AF_INET;

    if ((serverFileDescriptor = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("Server: Socket creation failed");
        return 1;
    }

    // Dynamically allocate n-element array
    
    theArray = (char**) malloc(n * sizeof(char*));
    for (i = 0; i < n; i++) {
        theArray[i] = (char*) malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(theArray[i], "String %d: the initial value", i);
    }

    // Allow for COM_NUM_REQUEST threads to handle requests

    pthread_t tids[COM_NUM_REQUEST];

    // Set up r/w lock to protect array

    mutexes = malloc(n * sizeof(pthread_mutex_t));

    for (i = 0; i < n; i++)
        pthread_mutex_init(mutexes+i, NULL);

    // Set up times mutex for time calculations
    pthread_mutex_init(&times_mutex, NULL);
    
    // Run server

    if (bind(serverFileDescriptor, (struct sockaddr*) &sock_var, sizeof(sock_var)) >= 0)
    {
        printf("Server: Socket has been created\n");
        listen(serverFileDescriptor,2000);
        
        while (1) 
        {
            for (i = 0; i < COM_NUM_REQUEST; i++)
            {
                clientFileDescriptor = accept(serverFileDescriptor, NULL, NULL);
                printf("Connected to client %d, request #%d\n", clientFileDescriptor, i);

                // create thread to handle request
                pthread_create(&tids[i], NULL, RequestHandler, (void *) (long) clientFileDescriptor);
            }

            // join all threads and record their runtime
            for (i = 0; i < COM_NUM_REQUEST; i++)
                pthread_join(tids[i], NULL);

            // save the aggregated runtime to file and reset latency array
            saveTimes(times, num_times);
            num_times = 0;
        }
        
        close(serverFileDescriptor);
        
    }
    else {
        printf("Server: socket bind creation failed\n");
        return 1;
    }

    // join all threads and record their runtime
    for (i = 0; i < COM_NUM_REQUEST; i++)
        pthread_join(tids[i], NULL);

    // Free memory

    for (i = 0; i < n; i++)
        pthread_mutex_destroy(mutexes+i);

    pthread_mutex_destroy(&times_mutex);

    for (i = 0; i < n; i++)
        free(theArray[i]);

    free(theArray);
    
    return 0;

}