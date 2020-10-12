import serial
import time
import numpy as np

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
time.sleep(1)

i = time.time()
print(i)
while True:
    rec_data = ser.readline()
    if rec_data:
        print(rec_data)
    user_in = input("Projection :")
    print(user_in)
    if user_in == "helloworld" :
        for i in range (100):
            send_data = bytearray([3, 10])
            ser.write(send_data)
    if user_in == "circle" :
        send_data = bytearray([2, 10]) 
        ser.write(send_data)
    if user_in == "taylor" :
        for i in range(100) :
            send_data = bytearray([0, 20])
            ser.write(send_data)
    if user_in == "TUD" :
        for i in range(100):
            send_data = bytearray([5, 10])
            ser.write(send_data)
    if user_in == "arrow" :
        for i in range(100):
            send_data = bytearray([6, 10])
            ser.write(send_data)
