import matplotlib.pyplot as plt
import numpy as np

f = open("graph.txt","r")
x = []
y = []
param = []
line = []
lines = f.readlines()
for i in lines:
	line = i.split(",")
	param.append(float(line[1]))
	x.append(float(line[0]))
	y.append(float(line[2][:-1]))
f.close()

fig, (ax1,ax2) = plt.subplots(nrows =2, ncols =1)
ax1.set_title("Runtime_vs_Number of Threads and Utility_vs_Runtime")
ax1.set_xlabel("NUM_OF_THREADS")
ax1.set_ylabel("RUNTIME")
ax1.set_ylim([70,80])
ax1.scatter(x, y, label='Data')
ax1.plot(x, y, linestyle = 'dashed')
ax1.grid()

ax2.set_ylabel("UTILITY")
ax2.set_xlabel("RUNTIME")
ax2.scatter(y, param, label='Data')
ax2.set_ylim([0.98,1.02])
ax2.grid()
plt.tight_layout()
plt.show()
