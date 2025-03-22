import serial

# Configurar UART
ser = serial.Serial("/dev/serial0", baudrate=9600)

# Enviar datos
ser.write(b"Hola")  

# Leer respuesta
response = ser.readline()  

# Imprimir
print("Respuesta:", response)