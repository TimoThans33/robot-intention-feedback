import numpy as np 
import matplotlib.pyplot as plt

# Change the radius of the projected circle
RADIUS = 100
DATAPOINTS = 100

x = 100 * np.cos(np.linspace(0, 2*np.pi, 100)) + 2000
y = 100 * np.sin(np.linspace(0, 2*np.pi, 100)) + 2000
x = x.round()
y = y.round()
print(x, y)

file1 = open("data.txt", "a")
file1.seek(0)
file1.truncate()
file1.write("{")

for i in range (DATAPOINTS):
    file1.write("{:d},".format(int(x[i])))
file1.write("}")
file1.write("\n")
file1.write("{")
for i in range (DATAPOINTS):
    file1.write("{:d},".format(int(y[i])))
file1.write("}")
file1.write("\n")

plt.plot(x, y)
plt.show()