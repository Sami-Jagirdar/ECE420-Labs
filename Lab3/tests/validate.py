import numpy as np
import os

INPUT = "/home/user_20/Labs/Lab3/Development Kit Lab3/data_input"
OUTPUT = "/home/user_20/Labs/Lab3/Development Kit Lab3/data_output"

# get input mats
with open(INPUT, 'r') as f:
    lines = f.readlines()
    n = int(lines[0])
    lines_G = lines[2:2+n]
    lines_x = lines[3+n:3+2*n]

G = np.array([a.strip().split() for a in lines_G], dtype=np.double)
b = np.array([a.strip() for a in lines_x], dtype=np.float64)

# get output mat
with open(OUTPUT, 'r') as f:
    lines_x = f.readlines()[1:2]

x = np.array([a.strip() for a in lines_x[0].split()], dtype=np.double)

# test multiplication
tol = np.average(np.abs(b)) * 0.001
cond = (G.dot(x) - b < tol).all()

if cond:
    # print("Validation OK")
    pass
else:
    print("Validation failed :(")
    print("G[0:5]:", G[0:5])
    print("b:", b)
    print("x:", x)
    print("G*x", G.dot(x))