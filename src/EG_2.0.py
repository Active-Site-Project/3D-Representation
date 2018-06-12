from numpy import pi, cos, sin, arccos, arange
from math import ceil
import sys
import os

radius = float(sys.argv[1])
filename = str(sys.argv[2])+".txt"
num_pts =  int(radius ** 2 * 4 * pi * 4)
indices = arange(0, num_pts, dtype=float) + 0.5
path = "ElectronClouds/"
outFile = open(path + filename, 'w+')
xv = []
yv = []
zv = []
phi = arccos(1 - 2*indices/num_pts)
theta = pi * (1 + 5**0.5) * indices
x, y, z = cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi);


for i in range(len(x)):
    xr = ceil(x[i] * 1000) / 1000.0
    yr = ceil(y[i] * 1000) / 1000.0
    zr = ceil(z[i] * 1000) / 1000.0
    for j in range(1, int(radius*10)):
        xf = ceil((1/10 * j * xr)*1000) / 1000.0
        yf = ceil((1/10 * j * yr)*1000) / 1000.0
        zf = ceil((1/10 * j * zr)*1000) / 1000.0
        xv.append(xf)
        yv.append(yf)
        zv.append(zf)


for i in range(len(xv)):
    outFile.write(str(xv[i])+", "+str(yv[i])+", "+str(zv[i]))
    outFile.write("\n")

outFile.close()
