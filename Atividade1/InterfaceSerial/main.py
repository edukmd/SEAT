import serial  # necessário para importar a biblioteca pyserial

# abre a primeira porta disponível
ser = serial.Serial(port='COM7',baudrate=115200)
while 1:


    # escreve a string teste nesta porta
    #ser.write("teste")

    # le a linha enviada pelo disposito remoto como resposta a string Teste
    linhaLida = ser.readline()
    byte_message = linhaLida.decode()
    angulo = int(byte_message)


    print(angulo)

    # fecha a porta
ser.close()