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

kernel = np.array([[WIDTH, 0],[-WIDTH, 0],[WIDTH, HEIGH], [-WIDTH, HEIGHT]])
rot_mat = np.array([np.cos(angle), -np.sin(angle)],[np.sin(angle), np.cos(angle)])

for i in range(len(x_robot)):
    kernel = np.matmul(kernel, rot_mat) + [x_robot[i], y_robot[i]]
    plt.plot(kernel[0], kernel[1])
    plt.show()


