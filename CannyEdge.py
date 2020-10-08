import numpy as numpy
import cv2 as cv
from matplotlib import pyplot as plt

img = cv.imread('PrimeVision.jpg')
edges = cv.Canny(img, 100, 200)
ans_x = []
ans_y =[]

for x in range(0, edges.shape[0]):
    for y in range(0, edges.shape[1]):
        if edges[x, y] != 0:
            ans_x.append(x)
            ans_y.append(y)

plt.subplot(121), plt.imshow(img)
plt.title('Original Image'), plt.xticks([]), plt.yticks([])
plt.subplot(122), plt.scatter(ans_x,ans_y, linewidth=0.001)
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
print(ans_x)
print(ans_y)
plt.show()