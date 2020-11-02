import numpy as np 
import matplotlib.pyplot as plt 

theta = 45
A = [np.cos(theta), -np.sin(theta)], [np.sin(theta), np.cos(theta)]
transform1= np.zeros([2,1])
transform2 = transform1

x = np.array([[1], [3]])
y2 = np.array([[1],[3]])
y1 = np.array([[3], [1]])


for i in range(len(A)):

    for j in range(1):

        for k in range(2):
            transform1[i][j] += A[i][k] * x[k][j]
            
for r in transform1:
    print(r)


plt.plot(x, y2)
plt.plot(x, y1)
plt.plot(transform1, y1)

plt.show()