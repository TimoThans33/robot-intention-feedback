import time
import serial
import sys

#0xFF 0xFF 8 0 1 0 1 0x0F 0xA0 0x0F 0xA0 0x4A

def build_message(x,y,num,size):
    msg = [0xFF,0xFF]
    msg.append(x >> 8)
    msg.append(x % 256)
    msg.append(y >> 8)
    msg.append(y % 256)
    msg.append(num >> 8)
    msg.append(num % 256)
    msg.append(size >> 8)
    msg.append(size % 256)
    
    #calc CRC
    msg.append(255-((sum(msg)-255-255)%255))
    return msg
   

    
my_msg = build_message(1,1,4000,4000)

print(my_msg)

ser = serial.Serial('COM8', 112800, writeTimeout=0, parity=serial.PARITY_EVEN, rtscts=1)

print(ser.isOpen())


ser.write(my_msg)

ser.close()

sys.exit()  
    


N = 3
X_LIM_LOW = 1000
Y_LIM_LOW = 1000
X_LIM = 3000
Y_LIM = 3000


speeds = [[-20,20], [-17,15], [-15, 15], [17,15], [15, -15], [-17,-15], [-15, 15], [-17,15], [15,-15]]
coords = [[2000, 2000], [2400, 1500], [1300, 2000], [1400, 1000], [2000, 3000], [1700, 300], [1700, 2000], [1600, 1400], [2000, 3000], ]

try:
	while True:
		#update positions
		send_string = str(N+1)
		
		for i in range(0,N):
			send_string = send_string + ";"
			coords[i][0] = coords[i][0]+speeds[i][0]
			coords[i][1] = coords[i][1]+speeds[i][1]
			#check limits
			if coords[i][0] > X_LIM:
				speeds[i][0] = speeds[i][0] * -1
				coords[i][0] = X_LIM
			if coords[i][0] < X_LIM_LOW:
				speeds[i][0] = speeds[i][0] * -1
				coords[i][0] = Y_LIM_LOW
			if coords[i][1] > Y_LIM:
				speeds[i][1] = speeds[i][1] * -1
				coords[i][1] = Y_LIM
			if coords[i][1] < Y_LIM_LOW:
				speeds[i][1] = speeds[i][1] * -1
				coords[i][1] = Y_LIM_LOW
			#add it to string
			send_string = send_string + str(coords[i][0]) + "," + str(coords[i][1]) + ",2,30"
			
            
			#b"4;2000,2000,3,60;2000,2000,3,60;2000,2000,3,60;2000,2000,3,60\n"
			
			
		string = bytes(send_string + ";2000,2000,2,30", 'utf-8') 
		print(string)
		ser.write(string)
		time.sleep(0.08)
		#while ser.inWaiting() > 0:
		#	out += ser.read(1)
			
		#if out != '':
		#	print(out)
except KeyboardInterrupt:
	pass