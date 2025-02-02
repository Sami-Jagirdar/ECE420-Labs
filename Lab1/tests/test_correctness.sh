source /home/user_20/Labs/Lab1/tests/lab1_test_venv/bin/activate
make all

# basic tests

echo "> n=100, p=25"
./matrixgen -s 100 && ./main 25
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=100 (b=500), p=25"
./matrixgen -s 100 -b 500 && ./main 25
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=100 (b=1), p=25"
./matrixgen -s 100 -b 1 && ./main 25
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=100 (b=10000), p=25"
./matrixgen -s 100 -b 10000 && ./main 25
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py


# small sizes/threadcounts

echo "> n=1, p=1"
./matrixgen -s 1 && ./main 1
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=2, p=4"
./matrixgen -s 2 && ./main 4
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=4 (b=1000), p=4"
./matrixgen -s 4 -b 1000 && ./main 4
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "> n=2 (b=40000), p=4"
./matrixgen -s 2 -b 40000 && ./main 4
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py


# large sizes/threadcounts

echo "n=1728, p=576"
./matrixgen -s 1728 && ./main 576
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py

echo "n=2048, p=1024"
./matrixgen -s 2048 && ./main 1024
echo "Verifying answer..."
python /home/user_20/Labs/Lab1/tests/validate_outputs.py