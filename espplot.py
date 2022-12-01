import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
from time import sleep
from time import time

#initialize serial port
sleep(1)
ser = serial.Serial()
ser.port = '/dev/ttyUSB0' #Arduino serial port
ser.baudrate = 74880
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters
datalist = []

tmr = 15
timeout = time() + tmr

plt.ion()
while True:
    data = ser.readline()
    data_f = float(data)
    datalist.append(data_f)

    if time() > timeout:
        break
    
data_np = np.array(datalist)
data_size = data_np.shape
time_np = np.linspace(0, tmr, data_size[0])
plt.plot(time_np, data_np)
plt.draw()
plt.show(block=True)

    

    