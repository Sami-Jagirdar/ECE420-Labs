cd "/home/user_20/Lab1/Development Kit Lab1"
make all

# basic tests

echo "n=100, p=25"
./matrixgen -s 100 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=100 (b=500), p=25"
./matrixgen -s 100 -b 500 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=100 (b=1), p=25"
./matrixgen -s 100 -b 1 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=100 (b=10000), p=25"
./matrixgen -s 100 -b 10000 && ./demo 25
echo "Verifying answer..."
python ../tests/validate_outputs.py


# small sizes/threadcounts

./matrixgen -s 1 && ./demo 1
echo "Verifying answer..."
python ../tests/validate_outputs.py

./matrixgen -s 2 && ./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py

./matrixgen -s 4 -b 1000 && ./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py

./matrixgen -s 3 -b 1000000 && ./demo 1
echo "Verifying answer..."
python ../tests/validate_outputs.py

./matrixgen -s 10 -b 0 && ./demo 10
echo "Verifying answer..."
python ../tests/validate_outputs.py


# large sizes/threadcounts

echo "n=2048, p=1024"
./matrixgen -s 2048 && ./demo 1024
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=3362, p=1681"
./matrixgen -s 3362 && ./demo 1681
echo "Verifying answer..."
python ../tests/validate_outputs.py

echo "n=3362, p=1681"
./matrixgen -s 10800 && ./demo 3600
echo "Verifying answer..."
python ../tests/validate_outputs.py