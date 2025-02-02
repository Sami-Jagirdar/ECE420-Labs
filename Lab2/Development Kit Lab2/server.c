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

// Idea is to have a read write lock on each element of the array since if
// one element is being written to / read from, the other elements aren't affected

char** theArray;
int n;
struct sockaddr_in sock_var;
int serverFileDescriptor;
int clientFileDescriptor;
int i;
pthread_rwlock_t *rwlocks;


void *RequestHandler(void *args) {
    int clientFileDescriptor = (int) args;
    char clientMessage[COM_BUFF_SIZE]; // "XXX-Y-SSSSSS\0"
    char buffer[COM_BUFF_SIZE];

    read(clientFileDescriptor, clientMessage, COM_BUFF_SIZE);
    printf("Client %d sent request: %s\n", clientFileDescriptor, clientMessage);

    // parse the message
    ClientRequest request;
    ParseMsg(clientMessage, &request);

    if (request.is_read) {
        // acquire a read lock and perform read on array element
        pthread_rwlock_rdlock(rwlocks + request.pos);
        getContent(buffer, request.pos, (char**)theArray);
        pthread_rwlock_unlock(rwlocks + request.pos);       
    } else {
        // acquire a write lock and perform write on array element
        pthread_rwlock_wrlock(rwlocks + request.pos);
        setContent(request.msg, request.pos, (char**)theArray);
        getContent(buffer, request.pos, (char**)theArray);
        pthread_rwlock_unlock(rwlocks + request.pos);
    }

    // Send content back to the client
    write(clientFileDescriptor, buffer, COM_BUFF_SIZE);

    close(clientFileDescriptor);
    return NULL;
    
}


int main(int argc, char* argv[]) {

    if (argc != 4) {
        printf("Usage: %s <sizeOfArray> <serverIP> <portNumber>\n", argv[0]);
        exit(1);
    }
    
    n = atoi(argv[1]);
    if (n < 1) {
        printf("INCORRECT INPUT: The number of elements in the array must be at least 1.\n");
        return 1;
    }

    char* serverIPAddr = argv[2];
    int serverPort = atoi(argv[3]);

    printf("%s\n", serverIPAddr);
    printf("%d\n", serverPort);

    sock_var.sin_addr.s_addr=inet_addr(serverIPAddr);
    sock_var.sin_port = serverPort;
    sock_var.sin_family = AF_INET;

    if ((serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0)) < 0)
        printf("Server: socket creation failed");

    pthread_t tids[COM_NUM_REQUEST];
    rwlocks = malloc(n * sizeof(pthread_rwlock_t));
    theArray = (char**) malloc(n * sizeof(char*));
    for (i = 0; i < n; i ++)
        theArray[i] = (char*) malloc(COM_BUFF_SIZE * sizeof(char));

    for (i = 0; i < n; i++)
        pthread_rwlock_init(rwlocks+i, NULL);
    

    if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    {
        printf("socket has been created\n");
        listen(serverFileDescriptor,2000); 
        while(1)
        {
            for(i = 0; i < COM_NUM_REQUEST; i++)
            {
                clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
                printf("Connected to client %d\n",clientFileDescriptor);
                pthread_create(&tids[i],NULL,RequestHandler,(void *)(long)clientFileDescriptor);
            }

            for (i = 0; i < COM_NUM_REQUEST; i++)
                pthread_join(tids[i], NULL);

        }
        close(serverFileDescriptor);
        
    }
    else{
        printf("Server: socket bind creation failed\n");
    }

    for (i = 0; i < n; i++)
        pthread_rwlock_destroy(rwlocks+i);

    for (i = 0; i < n; i++)
        free(theArray[i]);
    
    free(theArray);
    
    return 0;

}