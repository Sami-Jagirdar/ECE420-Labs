import numpy as np
import os


INPUT = "./data_input"
OUTPUT = "./data_output"

# get input mats
with open(INPUT, 'r') as f:
    lines = f.readlines()
    n = int(lines[0])
    lines_A = lines[2:2+n]
    lines_B = lines[3+n:3+2*n]

mat_A = np.array([x.strip().split('\t') for x in lines_A], dtype=int)
mat_B = np.array([x.strip().split('\t') for x in lines_B], dtype=int)

# get output mat
with open(OUTPUT, 'r') as f:
    lines_C = f.readlines()[2:-1]

mat_C = np.array([x.strip().split('\t') for x in lines_C], dtype=int)

# multiply
my_C = np.matmul(mat_A, mat_B)

# print(my_C)
# print(mat_C)
if np.all(my_C == mat_C):
    print("answer ok")
else:
    print("ANSWER INCORRECT")