import serial
import time
import numpy as np

ser = serial.Serial('/dev/ttyACM1', 115200, timeout=0.1)
time.sleep(1)

k = 1
i = 0

while True:
    rec_data = ser.readline()
    send_data = bytearray([i, i])
    ser.write(send_data)
    if rec_data:
        print(rec_data)
    if i==256:
        k = -1
    if i==0:
        k = 1
    i+= k
    time.sleep(0.001)