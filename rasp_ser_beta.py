import serial
import os
import time
from datetime import datetime


#handlers
force = True
Turn  = False


ser = serial.Serial('COM5', 9600, timeout=5) #using COM5 for testing purposes only

def get_time(): #get current time for multi use
    current_time = f'{datetime.now(): %H_%M_%S}'
    return current_time

def info_dump():
    f.write(get_time() + '\n')
    while(1):
        input_str = ser.readline().decode("utf-8").strip()
        if input_str == "no_m_up":
            return 0
        else:
            f.write(input_str + '\n')
            
def lill_test(val, Turn):
    if val == 1:
        if Turn == True:
            ser.write(b'on\n')
        else:
            ser.write(b'off\n')
    else:
        if Turn == True:
            Turn = False
        else:
            Turn = False

filename = ('sensors_data.txt')
f = open(filename, 'w')

while(1):
    #lill_test(2, Turn) all this is in comment because i dont hecking know what i was on last night
    input_str = ser.readline().decode("utf-8").strip()
    print(input_str) #TODO only for debuggin purposes
    if input_str == "ready" or force == True: #force for when comm is stuck or desicronissed
        lill_test(1, Turn) #TODO sub for turn or smth
        while(True):
            ser.write(b'on\n') #testing purposes
            input_str = ser.readline().decode("utf-8").strip()
            if(input_str == "received"):
                break 
            #input_str = ser.readline().decode("utf-8").strip()
            #lill_test(1, Turn)
            ##if input_str == "update_r": #TODO don't forget to take this out and just imediatly start info_dump()
            #    force = False
            #    info_dump()
            #    if info_dump == 0:
            #        break
            #elif input_str == "received":
            #    print("it's working :D")
            #    force = False
            #    break
    elif(input_str == "."):
        force = True