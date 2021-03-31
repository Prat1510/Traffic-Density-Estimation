import matplotlib.pyplot as plt
# from scipy.interpolate import make_interp_spline
import numpy as np

f = open("graph.txt","r")
x = []
y = []
param = []
line = []
lines = f.readlines()
for i in lines:
	line = i.split(",")
	param.append(line[1])
	x.append(float(line[0]))
	y.append(float(line[2][:-1]))
f.close()

plt.title("Number of Threads_vs_Runtime")
plt.xlabel("NUM_OF_THREADS")
plt.ylabel("RUNTIME")
plt.scatter(x, y, label='Data')

plt.plot(x, y, linestyle = 'dashed')
for i in range(len(x)):
	plt.annotate(" " + param[i], (x[i], y[i]))
plt.grid()

plt.show()
