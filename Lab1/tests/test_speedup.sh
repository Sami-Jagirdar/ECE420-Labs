make all

echo "--------- n=100 ---------"
./matrixgen -s 100
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./main 4
echo "> PARALLEL p=25"
./main 25
echo "> PARALLEL p=100"
./main 100

echo "--------- n=1000 ---------"
./matrixgen -s 1000
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./main 4
echo "> PARALLEL p=25"
./main 25
echo "> PARALLEL p=100"
./main 100

echo "--------- n=2048 ---------"
./matrixgen -s 2048
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./main 4
echo "> PARALLEL p=64"
./main 64
echo "> PARALLEL p=1024"
./main 1024