import matplotlib.pyplot as plt
# from scipy.interpolate import make_interp_spline
import numpy as np

f = open("Error_vs_Runtime.txt","r")
x = []
y = []
param = []
line = []
lines = f.readlines()
for i in lines:
	line = i.split(",")
	param.append(line[0])
	y.append(float(line[1]))
	x.append(float(line[2][:-1]))
f.close()
# x.reverse()
# y.reverse()
# x = np.array(x)
# y = np.array(y)
# X_ = np.linspace(x.min(), x.max(), 500)
# X_Y_Spline = make_interp_spline(x, y)
# Y_ = X_Y_Spline(X_)
# from scipy import optimize

# def test_func(x, a, b):
#     arr = []
#     for i in x:
#     	arr.append(a*np.sin(b*i))
#     return np.array(arr)

# params, params_covariance = optimize.curve_fit(test_func, x, y, p0=[2, 2])

plt.title("Utility runtime tradeoff: Method1")
plt.xlabel("Run-time")
plt.ylabel("Error")
plt.scatter(x, y, label='Data')

plt.plot(x, y, linestyle = 'dashed')
for i in range(len(x)):
	# plt.text(x[i],y[i],str(param[i]))
	plt.annotate(" " + param[i], (x[i], y[i]))
plt.grid()

plt.show()

# import matplotlib.pyplot as plt

# x =[80, 85, 90, 95, 100, 105, 110, 115, 120, 125]
# y =[240, 250, 260, 270, 280, 290, 300, 310, 320, 330]

