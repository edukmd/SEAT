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

height = 600
width = 600
image = np.zeros((height, width, 3), np.uint8)




#Circle configuration
center_coordinates = (300,600)
radius_max = 260
radius_1 = int(radius_max / 4)
radius_2 = int(radius_max * 2 / 4)
radius_3 = int(radius_max * 3 / 4)
radius_4 = radius_max
color = (0,255,0)
thickness = 2

#Dist configuration
dist_max = 100 #cm
dist_1 = dist_max / 4
dist_2 = dist_max * 2 / 4
dist_3 = dist_max * 3 / 4
dist_4 = dist_max

actualAngle = 0
lastAngle = 0
lastlastAngle = 0
actualDist = 0

x_60 = 0
y_60 = 0

x_120 = 0
y_120 = 0

fontScale = 0.3
font = cv2.FONT_HERSHEY_SIMPLEX



def drawLines(image):
    color_angle = (255, 255, 0)
    # Draw angle lines
    # 90

    angle = 90
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
             thickness)

    center_text = (center_coordinates[0] - x - 5, center_coordinates[1] - y - 10)

    image = cv2.putText(image, "90", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    # 60
    angle = 60
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
             thickness)

    center_text = (center_coordinates[0] - x - 10, center_coordinates[1] - y - 10)

    image = cv2.putText(image, "60", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    # 120
    angle = 120
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
             thickness)

    center_text = (center_coordinates[0] - x, center_coordinates[1] - y - 10)

    image = cv2.putText(image, "120", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    # 30
    angle = 30
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
             thickness)

    center_text = (center_coordinates[0] - x - 20, center_coordinates[1] - y - 5)

    image = cv2.putText(image, "30", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    # 135
    angle = 150
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
             thickness)

    center_text = (center_coordinates[0] - x + 5, center_coordinates[1] - y - 5)

    image = cv2.putText(image, "150", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    center_text = (center_coordinates[0] - radius_max - 20, 595)

    image = cv2.putText(image, "0", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

    center_text = (center_coordinates[0] + radius_max + 10, 595)

    image = cv2.putText(image, "180", center_text, font, fontScale, color_angle, 1, cv2.LINE_AA)

def drawCircles(image):
    image = cv2.circle(image, center_coordinates, radius_1, color, thickness)
    image = cv2.circle(image, center_coordinates, radius_2, color, thickness)
    image = cv2.circle(image, center_coordinates, radius_3, color, thickness)
    image = cv2.circle(image, center_coordinates, radius_4, color, thickness)


def drawAlphaEffect():
    global image
    global lastAngle
    global lastlastAngle

    angle = lastlastAngle
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), (255,0,0),
             thickness)

    overlay = image.copy()
    alpha = 0.5
    image = cv2.addWeighted(overlay, alpha, image, 1 - alpha, 0)
    lastlastAngle = lastAngle
    lastAngle = actualAngle

    angle = lastAngle
    x = int(radius_max * math.cos(angle * math.pi / 180))
    y = int(radius_max * math.sin(angle * math.pi / 180))
    cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), (0,0,255),
             thickness)
    lastlastAngle = lastAngle
    lastAngle = actualAngle

def drawTexts(image):

    global dist_1
    global dist_2
    global dist_3
    global dist_4

    center_text = (300 - radius_1 + 10, 595)
    image = cv2.putText(image, str(dist_1) + "cm", center_text, font, fontScale, (0, 255, 255), 1, cv2.LINE_AA)

    center_text = (300 - radius_2 + 10, 595)
    image = cv2.putText(image, str(dist_2) + "cm", center_text, font, fontScale, (0, 255, 255), 1, cv2.LINE_AA)

    center_text = (300 - radius_3 + 10, 595)
    image = cv2.putText(image, str(dist_3) + "cm", center_text, font, fontScale, (0, 255, 255), 1, cv2.LINE_AA)

    center_text = (300 - radius_4 + 10, 595)
    image = cv2.putText(image, str(dist_4) + "cm", center_text, font, fontScale, (0, 255, 255), 1, cv2.LINE_AA)


def on_change(value):
    global image
    global dist_max
    global dist_1
    global dist_2
    global dist_3
    global dist_4
    dist_max = float(value)
    dist_1 = dist_max / 4
    dist_2 = dist_max * 2 / 4
    dist_3 = dist_max * 3 / 4
    dist_4 = dist_max
    image = np.zeros((height, width, 3), np.uint8)

windowname = "Single Channel Window"
cv2.imshow(windowname, image)
cv2.createTrackbar('Dist. max.',windowname , 20, 300, on_change)

while(SerialPortOK == 0):
    try:
        ser = serial.Serial(port='COM5', baudrate=115200)
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
        try:
            splitMsg = byte_message.split(",")
            actualAngle = float(splitMsg[0])
            actualDist = float(splitMsg[1])
            SerialReadOK = 1
            print(actualAngle,actualDist)
        except:
            print("problema na msg")
            SerialReadOK = 0

    except:
        SerialReadOK = 0

    if SerialReadOK == 1:
        image_original = np.zeros((height, width, 3), np.uint8)
        #Draw circles
        image = image_original

        drawCircles(image)


        drawLines(image)

        drawTexts(image)

        #Draw Actual Angle
        angle = actualAngle
        x = int(radius_max * math.cos(angle * math.pi / 180))
        y = int(radius_max * math.sin(angle * math.pi / 180))
        cv2.line(image, center_coordinates, (center_coordinates[0] - x, center_coordinates[1] - y), color,
                 thickness)

        # Draw actual pos
        if(actualDist < dist_max):
            raio = (radius_max /dist_max) * actualDist
            x = int(raio * math.cos(angle * math.pi / 180))
            y = int(raio * math.sin(angle * math.pi / 180))
            center = (300 - x,600 - y)
            image = cv2.circle(image, center, 5, (0,0,255), 5)

        cv2.imshow(windowname, image)


    if cv2.waitKey(1) == ord('q'):
        # press q to terminate the loop
        cv2.destroyAllWindows()
        ser.close()
        break




