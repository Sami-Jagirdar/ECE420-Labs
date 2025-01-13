cd "/home/user_20/Labs/Lab1/Development Kit Lab1"
make all

echo "n=100"
./matrixgen -s 100
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./demo 4
echo "> PARALLEL p=25"
./demo 25
echo "> PARALLEL p=100"
./demo 100

echo "n=1000"
./matrixgen -s 1000
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./demo 4
echo "> PARALLEL p=25"
./demo 25
echo "> PARALLEL p=100"
./demo 100

echo "n=2048"
./matrixgen -s 2048
echo "> SEQUENTIAL"
./demo_seq
echo "> PARALLEL p=4"
./demo 4
echo "> PARALLEL p=64"
./demo 64
echo "> PARALLEL p=1024"
./demo 1024