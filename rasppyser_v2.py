import threading
import os
import serial
import time
from datetime import datetime
global life_line
life_line = False
def get_time():
    current_time = f'{datetime.now(): %H_%M_%S}'
    return current_time

filename = ('log' + get_time() + '.txt')
ser = serial.Serial('COM5', 9600, timeout=5)

#for reference purpuses !!!!!!!
# rasp_turn = 1
# esp_turn  = 0

def first_run(life_line):
    life_line = False
    while(life_line == False):
        input_str = ser.readline().decode("utf-8").strip()
        if(input_str == "hello_world"):
            ser.write(b'hello_back\n')
            life_line = True
            hub_task.start()
        if(life_line == True):
            hub_task.start()


def rasp_turn():
    global current_status
    current_status = 1
    ser.write(b'off\n') #for experimental purposes ussing off to signal rasp's turn
    return(current_status)
    bluetick.start()
        
def esp_turn():
    global current_status
    current_status = 0
    ser.write(b'on\n') #for experimental purposes ussing on to signal esp's turn
    return(current_status)
    bluetick.start()
    
def bluetick():
    'current_status'.join()
    reply = False
    while(reply == False):
        ser.write(b'current_status\n')
        input_str = ser.readline().decode("utf-8").strip()
        if(input_str == "done"):
            reply = True
    if(reply == True):
        hub_task.start()
        
    
def hub_task():
    first_run.join()
    while 1:
        input_str = ser.readline().decode("utf-8").strip()
        if(input_str == ""):
            print("no info")
            
        else:
            input_str = ser.readline().decode("utf-8").strip()
            if(input_str == "rasp_turn"):
                rasp_turn.start()
            elif(input_str == "esp_turn"):
                esp_turn.start()
            else:
                print("out of turn")
                
        
            
    
if __name__ == '__main__':
    rasp_turn = threading.Thread(target=rasp_turn, args=(0,))
    esp_turn = threading.Thread(target=esp_turn, args=(0,))
    bluetick = threading.Thread(target=bluetick, args=(0,))
    hub_task = threading.Thread(target=hub_task, args=(0,))
    first_run = threading.Thread(target=first_run, args=(life_line,))
    first_run.start()
