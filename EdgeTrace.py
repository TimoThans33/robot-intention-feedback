import numpy as np 
import cv2
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

img = cv2.imread('Test-Figures/pijl.png')
imgray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(imgray, 100, 200)
plt.imshow(edges, cmap = 'gray')
plt.show()
ret, thresh = cv2.threshold(edges, 170, 255, 0)
contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

SIZE = 6

img = cv2.drawContours(img, contours, -1, (0,255,0), 3)
#plt.imshow(img)
#cnt = contours[4]
#img = cv2.drawContours(img, [cnt], 0, (0,255,0), 3)
print(len(contours))

x = []
y = []
print(contours[0][1][0][1])
for i in range(len(contours)) :
    for j in range(len(contours[i])) :
        for k in range(len(contours[i][j])) :
            x.append(contours[i][j][0][0])
            y.append(contours[i][j][0][1])

THETA = 0
#AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , 0, np.sin(THETA/180*np.pi)], [0, 1, 0], [-np.sin(THETA/180*np.pi), 0, np.cos(THETA/180*np.pi)]])
AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , -np.sin(THETA/180*np.pi), 0], [np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi), 0], [0, 0, 1]])
#AffineTransformMatrix = np.array([[1 , 0, 0], [0, np.cos(THETA/180*np.pi), -np.sin(THETA/180*np.pi)], [0, np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi)]])
result = np.zeros([3, len(x)])
for i in range(len(x)):
    result[:,i] = np.matmul([round(x[i]*SIZE), round(y[i]*SIZE), 1], AffineTransformMatrix)
x = result[0] + 2000
y = result[1] + 1800
#print(x[0][1])
#plt.imshow(img)
plt.plot(x,y)

print(len(x))

file1 = open("data.txt", "a")
file1.seek(0)
file1.truncate()
file1.write("{")

for i in range (len(x)):
    file1.write("{:d},".format(int(x[i])))
file1.write("}")
file1.write("\n")
file1.write("{")
for i in range (len(y)):
    file1.write("{:d},".format(int(y[i])))
file1.write("}")
file1.write("\n")

plt.show()