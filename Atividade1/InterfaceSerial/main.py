import serial
import numpy as np
import cv2
import math

# abre a primeira porta disponível
SerialPortOK = 1
try:
    ser = serial.Serial(port='COM5', baudrate=115200)
except:
    SerialPortOK = 0


raio = 200
start_point = (300,300)
angulo = -45
pi = 3.14
line_thickness = 2
height = 600
width = 600
image = np.ones((height, width)) * 255

font = cv2.FONT_HERSHEY_SIMPLEX
org = (100, 350)
fontScale = 1
color = (0, 255, 0)
thickness = 2
modo = 0
updateimg = 0
while 1:
    # escreve a string teste nesta porta
    if SerialPortOK == 1:
        updateimg = 0
        try:
            linhaLida = ser.readline()
            updateimg = 1
        except:
            updateimg = 0

        byte_message = linhaLida.decode()
        splitMsg = byte_message.split(",")
        try:
            angulo = int(splitMsg[0])
            angulo = angulo * -1
            modo = int(splitMsg[1])
        except:
            if angulo > 180:
                angulo = 180
            elif angulo < 0:
                angulo = 0

        if angulo <=-170:
            angulo = -180
        elif angulo == -84:
            angulo = -90
        elif angulo < 0 and angulo > -10:
            angulo = 0




    if updateimg == 1:
        x = int(raio * math.cos(angulo * math.pi / 180))
        y = int(raio * math.sin(angulo * math.pi / 180))
        end_point =(300 + x, 300 + y)
        image = np.zeros((height, width, 3), np.uint8)
        img1 = cv2.line(image, start_point, end_point, (0, 0, 255), thickness=line_thickness)
        img1 = cv2.line(img1, (0,300), (600,300), (255, 255, 255), thickness=line_thickness)
        img1 = cv2.circle(img1,end_point,3,(0,0,255),5)

        if modo == 0:
            org = (120,400)
            img1 = cv2.putText(img1, 'Calibrar LDR esquerdo', org, font,fontScale, (0,255,255), thickness, cv2.LINE_AA)
        elif modo == 1:
            org = (140, 400)
            img1 = cv2.putText(img1, 'Calibrar LDR direito', org, font,fontScale, (0,255,255), thickness, cv2.LINE_AA)
        elif modo == 2:
            org = (225, 400)
            img1 = cv2.putText(img1, 'OPERANDO', org, font,fontScale, color, thickness, cv2.LINE_AA)


        cv2.imshow('Single Channel Window', img1)
    if cv2.waitKey(1) == ord('q'):
        # press q to terminate the loop
        cv2.destroyAllWindows()
        ser.close()
        break