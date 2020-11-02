import numpy as np
import matplotlib.pyplot as plt 

x1= np.array([[1], [1]])
x3= np.array([[3],[3]])
y1= np.array([[1],[3]])

x2= np.array([[1], [3]])
y2= np.array([[2], [2]])

c1= np.cos(np.linspace(0, 2*np.pi, 100)) + 5
c2= np.sin(np.linspace(0, 2*np.pi, 100)) + 2

xi= np.array([[7],[7]])
yi= np.array([[1],[3]])

print(xi, yi, c1, c2, x2, y2, x1, y1, x3)

HOI= plt.plot(xi, yi, c1, c2, x2, y2, x1, y1, x3, y1)


plt.show()
