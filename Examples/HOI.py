import numpy as np
import matplotlib.pyplot as plt 

phi = np.linspace(0, 2*np.pi, 100) #hoek
#c1= np.cos(phi) + 5
#c2= np.sin(phi) + 2

c3 = np.zeros([100,1])
c4 = np.zeros([100,1])
x = np.zeros([109,1])
y = np.zeros([109,1])

x[0] = 7
x[1] = 7
x[2] = 1 
x[3] = 3
x[105] = 1
x[106] = 1
x[107] = 3
x[108] = 3

y[0] = 1
y[1] = 3
y[2] = 2
y[3] = 2
y[105] = 1
y[106] = 3
y[107] = 1
y[108] = 3

for i in range(100):
    c3[i] = [np.cos(phi[i]) +5] 
    c4[i] = [np.sin(phi[i]) +2] 

    x[i+4] = c3[i]
    y[i+4] = c4[i]



for j in range(109):
    print(x[j], y[j])
    plt.plot(x[j], y[j])
    
#x = np.array([[7],[7],[1],[3],[1],[1],[3],[3]])
#y = np.array([[1],[3],[2],[2],[1],[3],[1],[3]])




print(x, y)

HOI= plt.plot(x,y)
plt.show()
