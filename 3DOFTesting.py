# Patrick Johnson 202
# 3DOF turret command testing
import serial
import time

# get serial connection
connected = False
while not connected:
    try:
        robotComms = (serial.Serial('COM5', 115200, timeout=1))
        connected = True
    except:
        pass
        
robotComms.flushInput()
robotComms.reset_output_buffer()
robotComms.write(b'\r')
robotComms.flush()
command = 'VS'
joint = '1'
direction = '1' #forward
tuningWord = '1000'
duration = '500'

msg = command + joint + direction + tuningWord + duration

print(msg)
key = ' '
num = 0

while key != 'q':
    key = input()
    if key == 'T': 
        while True:
            #get positions of all joints
            robotComms.flushInput()
            robotComms.reset_output_buffer()
            robotComms.write(b'GP1\r')
            robotComms.flush()
            #time.sleep(0.1)
            joint_1 = robotComms.readline()
            robotComms.write(b'GP2\r')
            robotComms.flush()
            #time.sleep(0.1)
            joint_2 = robotComms.readline()
            robotComms.write(b'GP3\r')
            robotComms.flush()
            #time.sleep(0.1)
            joint_3 = robotComms.readline()
            print("Joint 1: " + str(joint_1) + ' ' + str(num))
            num = num + 1
            print("Joint 2: " + str(joint_2))
            print("Joint 3: " + str(joint_3))
            #time.sleep(0.1)

robotComms.close()
