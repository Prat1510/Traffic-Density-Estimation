import matplotlib.pyplot as plt
# from scipy.interpolate import make_interp_spline
# import numpy as np

f = open("Utility_vs_Runtime.txt","r")
y = []
x = []
param = []
line = []
lines = f.readlines()
for i in lines:
	line = i.split(",")
	param.append(line[0])
	x.append(float(line[1]))
	y.append(float(line[2][:-1]))
f.close()
# x.reverse()
# y.reverse()
# x = np.array(x)
# y = np.array(y)
# X_ = np.linspace(x.min(), x.max(), 500)
# X_Y_Spline = make_interp_spline(x, y)
# Y_ = X_Y_Spline(X_)

plt.title("Utility runtime tradeoff: Method1 - Sub-sampling frames")
plt.ylabel("Run-time")
plt.xlabel("Utility")

plt.plot(x, y, marker = 'o', linestyle = 'dashed')
for i in range(len(x)):
	plt.annotate(" " + param[i], (x[i], y[i]))
	
plt.grid()

plt.show()

# import matplotlib.pyplot as plt

# x =[80, 85, 90, 95, 100, 105, 110, 115, 120, 125]
# y =[240, 250, 260, 270, 280, 290, 300, 310, 320, 330]

