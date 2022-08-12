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

#cv2.imshow('Single Channel Window', img1)
#cv2.waitKey(0)
#cv2.destroyAllWindows()
while 1:

    # escreve a string teste nesta porta
    if deuruim == 1:
        linhaLida = ser.readline()
        byte_message = linhaLida.decode()
        try:
            angulo = int(byte_message)
            print(angulo)
        except:
            angulo = 84


    x = int(raio * math.cos(angulo * math.pi / 180))
    y = int(raio * math.sin(angulo * math.pi / 180))
    end_point =(300 + x, 300 + y)
    image = np.ones((height, width)) * 255
    img1 = cv2.line(image, start_point, end_point, (0, 255, 0), thickness=line_thickness)

    cv2.imshow('Single Channel Window', img1)
    if cv2.waitKey(1) == ord('q'):
        # press q to terminate the loop
        cv2.destroyAllWindows()
        ser.close()
        break