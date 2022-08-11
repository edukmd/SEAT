import serial  # necessário para importar a biblioteca pyserial
import numpy as np
import matplotlib.pyplot as plt

# abre a primeira porta disponível
ser = serial.Serial(port='COM7',baudrate=115200)
plt.axis([0, 10, 0, 1])
raio = 50;
while 1:
    for i in range(10):
        y = np.random.random()
        plt.scatter(i, y)
        plt.pause(0.05)

    plt.show()


    # escreve a string teste nesta porta
    #ser.write("teste")

    # le a linha enviada pelo disposito remoto como resposta a string Teste
    linhaLida = ser.readline()
    byte_message = linhaLida.decode()
    angulo = int(byte_message)


    print(angulo)

    # fecha a porta
ser.close()