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
    time.sleep(5)
    send_data = bytearray([3, 20])
    ser.write(send_data)
    time.sleep(5)
    send_data = bytearray([5, 20])
    ser.write(send_data)
    time.sleep(5)

# Projective Transformaties
# Zoek wat meer uit wat al bestaat...
# Vooral focusen op de technologie en impact

# Impact , bestaande technologie, Omschrijving van de 2 projectie methode, Criteria, (misschien de beoordeling)
# Week 43 (23 Oktober) 1ste verslagje inleveren
# 27 Oktober meeting inplannen 14:00
# Standaard dinsdag om 14:00 meeting
