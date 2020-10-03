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
    send_data = bytearray([2, 20])
    ser.write(send_data)
    time.sleep(5)
    send_data = bytearray([0, 30])
    ser.write(send_data)
