import serial
import numpy as np
import cv2
import math

# abre a primeira porta disponível
SerialPortOK = 0
SerialReadOK = 0

while(SerialPortOK == 0):
    try:
        ser = serial.Serial(port='COM7', baudrate=115200)
        SerialPortOK = 1
        print("Foi")

    except:
        SerialPortOK = 0
        print("Ainda nao foi")

while 1:

    # escreve a string teste nesta porta
    SerialReadOK = 0
    try:
        linhaLida = ser.readline()
        byte_message = linhaLida.decode()
        #splitMsg = byte_message.split(",")
        print(byte_message)
        SerialReadOK = 1
    except:
        SerialReadOK = 0

    if SerialReadOK == 1:

