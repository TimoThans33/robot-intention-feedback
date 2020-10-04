import numpy as np
import matplotlib.pyplot as plt

L= 5

phi = np.linspace(0,np.pi,21)

xs = np.zeros([21,2])
ys = np.zeros([21,2])

for i in range (21):
    xs[i] = [0, L*np.cos(phi[i])]
    ys[i] = [0, L*np.sin(phi[i])]

for j in range (21):
    plt.plot(xs[j], ys[j])
    print(xs[j],ys[j])

plt.show()

