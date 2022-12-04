import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
from time import sleep
from time import time

sleep(1)
ser = serial.Serial()
ser.port = '/dev/ttyUSB0'
ser.baudrate = 74880
ser.timeout = 10
ser.open()
if ser.is_open==True:
	print("\nSerial Port Initialised:\n Configuration:")
	print(ser, "\n")
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

data1 = datalist[::2]
data2 = datalist[1::2]
data1_np = np.array(datalist)
data2_np = np.array(data2)
data1_size = data1_np.shape
data2_size = data2_np.shape
time1_np = np.linspace(0, tmr, data1_size[0])
time2_np = np.linspace(0, tmr, data2_size[0])
plt.plot(time1_np, data1_np)
plt.plot(time2_np, data2_np)


plt.draw()
plt.show(block=True)   

#led_interval = 2000
#adc_interval = 10
#interval = int(led_interval/adc_interval)

#data1 = datalist[0:interval]
#data1_n = 0
#data1_start = 0
#data2 = datalist[interval:interval]
#data2_n = 0
#data2_start = 1

#while (len(data1) < (len(datalist)/2)):
#    data1_start = data1_n*interval
#    data1 += datalist[data1_start:interval]
#    data1_start += 1

#while (len(data2) < (len(datalist)/2)):
#    data2_start = data2_n*interval
#    data2 += datalist[data2_start:interval]
#    data1_start += 1

#data1_np = np.array(data1)
#data2_np = np.array(data2)
#data1_size = data1_np.shape
#data2_size = data2_np.shape
#time1_np = np.linspace(0, tmr, data1_size[0])
#time2_np = np.linspace(0, tmr, data2_size[0])
#plt.plot(time1_np, data1_np)
#plt.plot(time2_np, data2_np)