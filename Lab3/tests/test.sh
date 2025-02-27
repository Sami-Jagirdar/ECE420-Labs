source /home/user_20/Labs/Lab3/tests/lab3_test_venv/bin/activate
make all

echo "> Normal tests"
for i in $(seq 1 30); do
    ./datagen && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

echo "> Small matrices"

for i in $(seq 1 10); do
    ./datagen -s 1 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

for i in $(seq 1 10); do
    ./datagen -s 2 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

for i in $(seq 1 10); do
    ./datagen -s 3 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

echo "> Large matrices"

for i in $(seq 1 10); do
    ./datagen -s 1000 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

# ./datagen -s 10000 && ./main && python /home/user_20/Labs/Lab3/tests/validate.py

echo "> Small values"
for i in $(seq 1 10); do
    ./datagen -b 1 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done

echo "> Large values"
for i in $(seq 1 10); do
    ./datagen -b 10000 && ./main 8 && python /home/user_20/Labs/Lab3/tests/validate.py
done