cd "/home/user_20/Lab1/Development Kit Lab1"
make all


# basic tests

echo "> n=100, p=25"
./matrixgen -s 100 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=100 (b=500), p=25"
./matrixgen -s 100 -b 500 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=100 (b=1), p=25"
./matrixgen -s 100 -b 1 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=100 (b=10000), p=25"
./matrixgen -s 100 -b 10000 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=100 (b=0), p=10"
./matrixgen -s 100 -b 0 && ./demo 10
echo "Verifying answer..."
python ../tests/validate_outputs.py


# small sizes/threadcounts

echo "> n=1, p=1"
./matrixgen -s 1 && ./demo 1
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=2, p=4"
./matrixgen -s 2 && ./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=4 (b=1000), p=4"
./matrixgen -s 4 -b 1000 && ./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "> n=2 (b=1000000), p=4"
./matrixgen -s 2 -b 1000000 && ./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py


# large sizes/threadcounts

echo "n=1728, p=576"
./matrixgen -s 1728 && ./demo 576
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=2048, p=1024"
./matrixgen -s 2048 && ./demo 1024
echo "Verifying answer..."
python ../tests/validate_outputs.py