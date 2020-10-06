import numpy as np 
import matplotlib.pyplot as plt
import cv2

CIRCLE = 0
TRANSCIRCLE = 1
HELLOWORLD = 2
TRANSHELLOWORLD = 3

# Give a flag for which projection you want
flag = HELLOWORLD
print(flag)
if flag == 1:
    # Change the radius of the projected circle
    RADIUS = 300
    DATAPOINTS = 100
    THETA = 30
    OMEGA = 0
    PHI = 0

    x = RADIUS * np.cos(np.linspace(0, 2*np.pi, DATAPOINTS)) + 2200
    y = RADIUS * np.sin(np.linspace(0, 2*np.pi, DATAPOINTS)) + 2000
    x = x.round()
    y = y.round()

    AffineTransformMatrix = np.array([[1 , 0, 0], [0, np.cos(THETA/180*np.pi), -np.sin(THETA/180*np.pi)], [0, np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi)]])
    print(AffineTransformMatrix)
    #result = cv2.warpAffine([x, y, np.ones(DATAPOINTS)], AffineTransformMatrix, DATAPOINTS )
    result = np.zeros([3, len(x)])
    for i in range(DATAPOINTS):
        result[:,i] = np.matmul([x[i]-2200, y[i]-2000, 1], AffineTransformMatrix)

    x = result[1] + 2200
    y = result[0] + 2000

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

    #plt.plot(x,y)
    #plt.show()
if flag == 2 :
    RADIUS = 300

    # Resolution of Projected Image
    DATAPOINTSI = 15
    DATAPOINTSII = 40
    SIZE = 300
    DST = 50
    x = np.zeros([4*DATAPOINTSI+DATAPOINTSII])
    y = np.zeros([4*DATAPOINTSI+DATAPOINTSII])
    print(len(x))
    stripe = np.linspace(-1,1, DATAPOINTSI)
    circleY = np.sin(np.linspace(-0.5*np.pi, 1.5*np.pi, DATAPOINTSII))
    circleX = np.cos(np.linspace(-0.5*np.pi, 1.5*np.pi, DATAPOINTSII))
    stripeI = np.linspace(0,1, DATAPOINTSI)

    # Draw the letter I
    x[0:DATAPOINTSI] = 2200 + RADIUS + DST
    y[0:DATAPOINTSI] = -stripe * SIZE + 2000
    print(x[0:DATAPOINTSI],y[0:DATAPOINTSI])
    plt.plot(x[0:DATAPOINTSI],y[0:DATAPOINTSI])
    plt.show()
    # Draw the letter O
    x[DATAPOINTSI:DATAPOINTSI+DATAPOINTSII] = RADIUS * circleX + 2200
    y[DATAPOINTSI:DATAPOINTSI+DATAPOINTSII] = RADIUS * circleY + 2000
    print(x[0:DATAPOINTSI+DATAPOINTSII],y[0:DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:DATAPOINTSI+DATAPOINTSII],y[0:DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the | of H
    x[DATAPOINTSI+DATAPOINTSII:2*DATAPOINTSI+DATAPOINTSII] = -RADIUS - DST + 2200
    y[DATAPOINTSI+DATAPOINTSII:2*DATAPOINTSI+DATAPOINTSII] =  stripe * SIZE + 2000
    print(x[0:2*DATAPOINTSI+DATAPOINTSII],y[0:2*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:2*DATAPOINTSI+DATAPOINTSII],y[0:2*DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the - of H
    x[2*DATAPOINTSI+DATAPOINTSII:3*DATAPOINTSI+DATAPOINTSII] = -RADIUS -DST + 2200 - stripeI * SIZE 
    y[2*DATAPOINTSI+DATAPOINTSII:3*DATAPOINTSI+DATAPOINTSII] = 2000
    print(x[0:3*DATAPOINTSI+DATAPOINTSII],y[0:3*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:3*DATAPOINTSI+DATAPOINTSII],y[0:3*DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the | of H
    x[3*DATAPOINTSI+DATAPOINTSII:4*DATAPOINTSI+DATAPOINTSII] = -RADIUS -DST - SIZE + 2200
    y[3*DATAPOINTSI+DATAPOINTSII:4*DATAPOINTSI+DATAPOINTSII] = stripe*SIZE + 2000
    print(x[0:4*DATAPOINTSI+DATAPOINTSII],y[0:4*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:4*DATAPOINTSI+DATAPOINTSII],y[0:4*DATAPOINTSI+DATAPOINTSII])
    plt.show()

    file1 = open("data.txt", "a")
    file1.seek(0)
    file1.truncate()
    file1.write("{")

    for i in range (4*DATAPOINTSI + DATAPOINTSII):
        file1.write("{:d},".format(int(x[i])))
    file1.write("}")
    file1.write("\n")
    file1.write("{")
    for i in range (4*DATAPOINTSI + DATAPOINTSII):
        file1.write("{:d},".format(int(y[i])))
    file1.write("}")
    file1.write("\n")

if flag == 3 :
    RADIUS = 300

    # Resolution of Projected Image
    DATAPOINTSI = 15
    DATAPOINTSII = 40
    SIZE = 300
    DST = 50

    THETA = 45

    x = np.zeros([4*DATAPOINTSI+DATAPOINTSII])
    y = np.zeros([4*DATAPOINTSI+DATAPOINTSII])
    print(len(x))
    stripe = np.linspace(-1,1, DATAPOINTSI)
    circleY = np.sin(np.linspace(-0.5*np.pi, 1.5*np.pi, DATAPOINTSII))
    circleX = np.cos(np.linspace(-0.5*np.pi, 1.5*np.pi, DATAPOINTSII))
    stripeI = np.linspace(0,1, DATAPOINTSI)

    # Draw the letter I
    x[0:DATAPOINTSI] = 2200 + RADIUS + DST
    y[0:DATAPOINTSI] = -stripe * SIZE + 2000
    print(x[0:DATAPOINTSI],y[0:DATAPOINTSI])
    plt.plot(x[0:DATAPOINTSI],y[0:DATAPOINTSI])
    plt.show()
    # Draw the letter O
    x[DATAPOINTSI:DATAPOINTSI+DATAPOINTSII] = RADIUS * circleX + 2200
    y[DATAPOINTSI:DATAPOINTSI+DATAPOINTSII] = RADIUS * circleY + 2000
    print(x[0:DATAPOINTSI+DATAPOINTSII],y[0:DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:DATAPOINTSI+DATAPOINTSII],y[0:DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the | of H
    x[DATAPOINTSI+DATAPOINTSII:2*DATAPOINTSI+DATAPOINTSII] = -RADIUS - DST + 2200
    y[DATAPOINTSI+DATAPOINTSII:2*DATAPOINTSI+DATAPOINTSII] =  stripe * SIZE + 2000
    print(x[0:2*DATAPOINTSI+DATAPOINTSII],y[0:2*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:2*DATAPOINTSI+DATAPOINTSII],y[0:2*DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the - of H
    x[2*DATAPOINTSI+DATAPOINTSII:3*DATAPOINTSI+DATAPOINTSII] = -RADIUS -DST + 2200 - stripeI * SIZE 
    y[2*DATAPOINTSI+DATAPOINTSII:3*DATAPOINTSI+DATAPOINTSII] = 2000
    print(x[0:3*DATAPOINTSI+DATAPOINTSII],y[0:3*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:3*DATAPOINTSI+DATAPOINTSII],y[0:3*DATAPOINTSI+DATAPOINTSII])
    plt.show()
    # Draw the | of H
    x[3*DATAPOINTSI+DATAPOINTSII:4*DATAPOINTSI+DATAPOINTSII] = -RADIUS -DST - SIZE + 2200
    y[3*DATAPOINTSI+DATAPOINTSII:4*DATAPOINTSI+DATAPOINTSII] = stripe*SIZE + 2000
    print(x[0:4*DATAPOINTSI+DATAPOINTSII],y[0:4*DATAPOINTSI+DATAPOINTSII])
    plt.plot(x[0:4*DATAPOINTSI+DATAPOINTSII],y[0:4*DATAPOINTSI+DATAPOINTSII])
    plt.show()

    AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , 0, np.sin(THETA/180*np.pi)], [0, 1, 0], [-np.sin(THETA/180*np.pi), 0, np.cos(THETA/180*np.pi)]])
    #AffineTransformMatrix = np.array([[np.cos(THETA/180*np.pi) , -np.sin(THETA/180*np.pi), 0], [np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi), 0], [0, 0, 1]])
    #AffineTransformMatrix = np.array([[1 , 0, 0], [0, np.cos(THETA/180*np.pi), -np.sin(THETA/180*np.pi)], [0, np.sin(THETA/180*np.pi), np.cos(THETA/180*np.pi)]])

    print(AffineTransformMatrix)

    #result = cv2.warpAffine([x, y, np.ones(DATAPOINTS)], AffineTransformMatrix, DATAPOINTS )
    result = np.zeros([3, len(x)])

    for i in range(4*DATAPOINTSI+DATAPOINTSII):
        result[:,i] = np.matmul([x[i]-2200, y[i]-2000, 1], AffineTransformMatrix)
    x = result[0] + 2200
    y = result[1] + 2000

    plt.plot(x,y)
    plt.show()

    file1 = open("data.txt", "a")
    file1.seek(0)
    file1.truncate()
    file1.write("{")

    for i in range (4*DATAPOINTSI+DATAPOINTSII):
        file1.write("{:d},".format(int(x[i])))
    file1.write("}")
    file1.write("\n")
    file1.write("{")
    for i in range (4*DATAPOINTSI+DATAPOINTSII):
        file1.write("{:d},".format(int(y[i])))
    file1.write("}")
    file1.write("\n")