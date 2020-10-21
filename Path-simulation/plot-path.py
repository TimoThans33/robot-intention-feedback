import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# reading csv file
data = pd.read_csv("trajectories/trajectory_1_fpg_out.txt", skiprows= 11)

WIDTH = 0.02
HEIGHT = 0.02

# show shape
print(data)
print(data.shape)
print(data.ndim)

t = 0.2

angle = data["yaw_angle"]
velocity = data["velocity"]
x_robot = data["x_out"]
y_robot = data["y_out"]
plt.plot(data["x_out"], data["y_out"])
for i in range(len(x_robot)):
    x_kernel = [np.cos(angle[i])*WIDTH+x_robot[i]+WIDTH, -np.cos(angle[i])*WIDTH + x_robot[i] - WIDTH,
                -np.cos(angle[i])*HEIGHT + x_robot[i] + WIDTH, np.cos(angle[i])*HEIGHT + x_robot[i] - WIDTH]
    y_kernel = [np.sin(angle[i])*WIDTH + y_robot[i], -np.sin(angle[i])*WIDTH + y_robot[i], 
                np.sin(angle[i])*HEIGHT + y_robot[i]+HEIGHT,-np.sin(angle[i])*HEIGHT + y_robot[i] + HEIGHT]
    plt.plot(x_kernel, y_kernel)
    plt.show()


