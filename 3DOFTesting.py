# Patrick Johnson 202
# 3DOF turret command testing

import serial


# get serial connection
connected = False
while not connected:
    try:
        robotComms = (serial.Serial('COM23', 115200, timeout=1))
        connected = True
    except:
        pass
        
command = 'VS'
joint = '1'
direction = '1' #forward
tuningWord = '1000'
duration = '500'

msg = command + joint + direction + tuningWord + duration

print(msg)
