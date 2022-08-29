import serial
import numpy as np
import cv2
import math

# abre a primeira porta disponível
SerialPortOK = 0
SerialReadOK = 0
accessState = 0
user = 0

font = cv2.FONT_HERSHEY_SIMPLEX
fontScale = 1

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
        splitMsg = byte_message.split(",")
        accessState = int(splitMsg[0])
        user = int(splitMsg[1])

        print(byte_message)
        SerialReadOK = 1
    except:
        SerialReadOK = 0

    if SerialReadOK == 1:
        if accessState == 1:
            if user == 1:
                img = cv2.imread("usuarios/padrao.png")
                org = (250, 470)
                img = cv2.putText(img, 'USUARIO LIBERADO', org, font, fontScale, (0, 0, 255), 2, cv2.LINE_AA)
                cv2.imshow('Single Channel Window', img)

            elif user == 2:
                img = cv2.imread("usuarios/user1.png")
                org = (250, 470)
                img = cv2.putText(img, 'USUARIO LIBERADO', org, font, fontScale, (0, 0, 255), 2, cv2.LINE_AA)
                cv2.imshow('Single Channel Window', img)

        elif accessState == 0:
            cv2.destroyAllWindows()


    if cv2.waitKey(1) == ord('q'):
        # press q to terminate the loop
        cv2.destroyAllWindows()
        ser.close()
        break


