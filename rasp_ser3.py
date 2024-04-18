import threading
import os
import serial
import time
from datetime import datetime

global Flag 
Flag = True
global fready 
fready = False
global out 
out = False

ser = serial.Serial('COM5', 9600, timeout=5)

def get_time():
    current_time = f'{datetime.now(): %H_%M_%S}'
    return current_time

def esp_turn():
    while(out==False):
        ser.write(b'off\n')
        input_str = ser.readline().decode("utf-8").strip()
        if(input_str == "received" or input_str == "ready"):
            out = True
        
def rasp_turn():
    while(out==False):
        ser.write(b'on\n')
        input_str = ser.readline().decode("utf-8").strip()
        if(input_str == "received" or input_str == "ready"):
            out = True
    

def listener(num):
    filename = ('log' + get_time() + '.txt')
    f = open(filename, "w")
    while(1):
        input_str = ser.readline().decode("utf-8").strip()
        print(input_str)
        f.write(input_str + '\n')
        if input_str == "ready" or fready == True:
            fready = False
            if Flag:
                out = False
                f.write("!said off!")
                esp_turn()
            else:
                ser.write(b'on\n')
                f.write("!said on!")
        elif input_str == ".":
            fready = True
            
if __name__ == '__main__':
    listener = threading.Thread(target=listener, args=(10,   ))
    listener.start()