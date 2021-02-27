def area(Points):
	if len(Points) < 1:
		return "Error: No of Points must be greater than 0."
	if len(Points) == 1 :
		return 0
	x1 , y1 = Points[0]
	# x = 0
	area = 0
	for i in range(1,len(Points)):
		x2, y2 = Points[i]
		if x1 > x2:
			return "Error: Points must be sorted."
		if y1*y2 >= 0:
			area += abs((x2-x1)*(y1+y2)*0.5)
		else :
			x = (x2-x1)*y1/(y1-y2)
			area += abs(x*y1*0.5) + abs((x2-x1-x)*y2*0.5)
		x1,y1 = x2,y2	
	return area
file = input("Enter testcase file name: ")
fout = open("out_" + file + ".txt", "w") 
	
with open(file + ".txt", "r") as fin:
	line_array = fin.readlines()
	fin.close()
array = []
for i in line_array:
	for elem in i.split():
		array.append(elem)
	
array = array[::-1]
t = int(array.pop())
for j in range (t):
	n = int(array.pop())
	Points = []
	for i in range(n):
		point = [int(array.pop()),int(array.pop())]
		Points.append(point)	
	Area = area(Points)
	fout.write(str(Area) + "\n")

fout.close()