# make demo_long

echo "> n=2 (b=50000), p=4"
# ./matrixgen -s 2 -b 2
./demo 4
echo "Verifying answer..."
python ../tests/validate_outputs.py