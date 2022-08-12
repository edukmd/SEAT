import serial
import numpy as np
import cv2
import math

# abre a primeira porta disponível
deuruim = 1
try:
    ser = serial.Serial(port='COM7', baudrate=115200)
except:
    print("Deu BO")
    deuruim = 0


raio = 200
start_point = (300,300)
angulo = -45
pi = 3.14


x = int(raio*math.cos(angulo * math.pi/180))
y = int(raio*math.sin(angulo * math.pi/180))
print(math.pi)
print(x,y)
end_point =(300 + x, 300 + y)
line_thickness = 2
height = 600
width = 600
image = np.ones((height, width)) * 255
img1 = cv2.line(image, start_point, end_point, (0, 255, 0), thickness=line_thickness)

#cv2.imshow('Single Channel Window', img1)
#cv2.waitKey(0)
#cv2.destroyAllWindows()
while 1:

    # escreve a string teste nesta porta
    if deuruim == 1:
        linhaLida = ser.readline()
        byte_message = linhaLida.decode()
        angulo = int(byte_message)
        angulo = angulo - 180
        print(angulo)


    x = int(raio * math.cos(angulo * math.pi / 180))
    y = int(raio * math.sin(angulo * math.pi / 180))
    end_point =(300 + x, 300 + y)
    image = np.ones((height, width)) * 255
    img1 = cv2.line(image, start_point, end_point, (0, 255, 0), thickness=line_thickness)

    cv2.imshow('Single Channel Window', img1)
    if cv2.waitKey(1) == ord('q'):
        # press q to terminate the loop
        cv2.destroyAllWindows()
        break
    #cv2.destroyAllWindows()
    #cv2.waitKey(0)
    #

    # le a linha enviada pelo disposito remoto como resposta a string Teste


    # fecha a porta
ser.close()