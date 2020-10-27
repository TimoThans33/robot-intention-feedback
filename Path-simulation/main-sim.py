import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import json
import socket
import queue
import threading

HOST =  '127.0.0.1' # localhost
PORT = 8090

WIDTH = 0.2/2
HEIGHT = 0.2
T = 40

class sockets:
    def __init__(self, host, port):
        threading.Thread.__init__(self)
        self.host = host
        self.port = port
    def connect(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen()
            self.conn, self.addr = s.accept()
            with self.conn:
                while True:
                    print('connected by', self.addr)
                    self.data_client = self.conn.recv(4096)
                    if not self.data_client:
                        break
                    self.data_x = np.linspace(0,10, 100)
                    self.data_y = np.linspace(0,10, 100)
                    self.data_json = json.dumps({})
                    self.conn.send(self.data_json.encode())
            self.conn.close()

class simulation:
    def __init__(self,  width, height, t):
        threading._main_thread
        self.data = pd.read_csv("trajectories/trajectory_1_fpg_out.txt", skiprows=11)
        self.width = width
        self.height = height
        self.t = t

        self.theta = 0
        self.angle = self.data["yaw_angle"]
        self.velocity = self.data["velocity"]
        self.x_robot = self.data["x_out"]
        self.y_robot = self.data["y_out"]
        self.kernel = np.array([[0, self.width],[self.height, self.width], [self.height, -self.width], [0, -self.width], [0, self.width]])

        print("[message] Initializing simulation...")
        print("[message] Showing data type:")
        print(self.data.shape)
        print(self.data.ndim)
        print(self.data)
    def processing(self):
        for i in range(len(self.x_robot)):
            #print("Iteration: ", i)
            # read current angle and create the rotation matrix
            self.theta = self.angle[i]
            self.rot_mat = np.array([[np.cos(self.theta), np.sin(self.theta)],[-np.sin(self.theta), np.cos(self.theta)]])

            # rotate the kernel to the robot-frame
            self.kernel_rot = np.matmul(self.kernel, self.rot_mat) + [self.x_robot[i], self.y_robot[i]]

            # read the coordinate of the 't' following time steps (future path)
            self.coord = np.array([self.x_robot[1+i:self.t+i], self.y_robot[1+i:self.t+i]])

            # create the rotation matrix again
            self.theta = -self.theta
            self.rot_mat = np.array([[np.cos(self.theta), np.sin(self.theta)],[-np.sin(self.theta), np.cos(self.theta)]])

            # rotate the coordinates to the robot-frame
            self.rot_coord = np.matmul(np.transpose(self.coord), self.rot_mat)
        print("[message] simulation is finished...")

def testing():
    i = 0
    while i<100:
        print("test",i)
        i+=1
    print("[message] test is finished")

if __name__ == "__main__":
    # creating objects of classes
    socket_programming = sockets(HOST, PORT)
    path_simulation = simulation(WIDTH, HEIGHT, T)
