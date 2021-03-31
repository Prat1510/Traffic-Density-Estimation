import matplotlib.pyplot as plt

f = open("Utility_vs_Runtime.txt","r")
y = []
x = []
param = []
line = []
lines = f.readlines()
for i in lines:
	line = i.split(",")
	param.append(float(line[0]))
	x.append(round(float(line[1]),3))
	y.append(round(float(line[2][:-1]),1))
f.close()

fig, (ax1,ax2) = plt.subplots(nrows =1, ncols =2)
ax1.set_title("Sub-sampling frames: Utility runtime tradeoff")
ax1.set_ylabel("Run-time")
ax1.set_xlabel("Utility")
ax1.scatter(x, y, label='Data')
ax1.plot(x, y, linestyle = 'dashed')
for i in range(len(x)):
	ax1.annotate(" (" + str(x[i]) + "," + str(y[i]) + ")" ,(x[i], y[i]), horizontalalignment='right')
ax1.grid()

ax2.set_title("Sub-sampling frames: Runtime vs Sub-sampling parameter")
ax2.set_ylabel("Run-time")
ax2.set_xlabel("Sub-sampling parameter")
ax2.scatter(param,y, label='Data')
ax2.plot(param,y, linestyle = 'dashed')
for i in range(len(y)):
	ax2.annotate(" (" + str(param[i]) + "," + str(y[i]) + ")" ,(param[i], y[i]), horizontalalignment='right')
ax2.grid()
plt.tight_layout()
plt.show()
