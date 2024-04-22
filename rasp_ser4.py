import serial
import os
import time
from datetime import datetime

#vars
ready_go = False
Turn = True
#handlers
force = False

ser = serial.Serial('COM5', 9600, timeout=5)

current_time = f'{datetime.now(): %H_%M_%S}'
filename = ('log' + current_time + '.txt')
f = open(filename, 'w')

#TODO: optimise the ser read thingy by turning the input_str line into
#       a funct and maybe there add the code log thing
#while(ready_go == False):
#    ser.write(b'value\n')
#    input_str = ser.readline().decode("utf-8").strip()
#    Turn = input_str
#    if Turn == True or Turn == False: #for reference Turn= false: esp32 turn
#        ready_go = True               #                  = true : rasp  turn  

#if ready_go == True:
while(1):
    input_str = ser.readline().decode("utf-8").strip()
    print(input_str)
    if input_str == "ready" or force == True:
        if Turn == True: #rasp turn
            while(1):
                ser.write(b'off\n')
                input_str = ser.readline().decode("utf-8").strip()
                if input_str == "received" or input_str == "off":
                    Turn = False
                    force = False
                    break
            #go to the other process ig
            print("¯\_(ツ)_/¯")
        elif Turn == False: #esp turn
            while(1):
                ser.write(b'on\n')
                input_str = ser.readline().decode("utf-8").strip()
                if input_str == "received" or input_str == "on":
                    Turn = True
                    force = False
                    break
    elif input_str == ".":
        force = True
    else:
        ...