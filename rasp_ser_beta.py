import serial
import os
import time
from datetime import datetime

#private
testing_state = True

#handlers
force = True
Turn  = False


#ser = serial.Serial('COM5', 9600, timeout=5) #using COM5 for testing purposes on laptop only
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=5) #using /dev/ttyACM0- serial comm for rasp usb connection to esp
conectsec = ser.readline()

def get_time(): #get current time for multi use
    current_time = f'{datetime.now(): %H_%M_%S}'
    return current_time

def info_dump():
    f = open(filename, 'a')
    f.write("-------" + get_time() + "-------" + '\n' + '\n')
    f.close()
    while(1):
        f = open(filename, 'a')
        input_str = ser.readline().decode("utf-8").strip()
        if input_str == "sent_complete":
            ser.write(b'done\n')
            return 0
        else:
            f.write(input_str + '\n')
            f.close()
            

filename = ('sensors_data.txt')
f = open(filename, 'a')

while(1):
    input_str = ser.readline().decode("utf-8").strip()
    print(input_str) #TODO only for debuggin purposes
    if input_str == "ready" or force == True: #force for when comm is stuck or desicronissed
        while(True):
            ser.write(b'on\n') #testing purposes
            input_str = ser.readline().decode("utf-8").strip()
            if(input_str == "received"):
                info_dump()
                break
    elif(input_str == "."):
        force = True
    elif(input_str == "sent_complete"):
        ser.write(b'done\n')