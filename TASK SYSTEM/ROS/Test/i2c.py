import smbus

# Comunicacion I2C Bidireccional
# el maestro envia un primer mensaje con la 7 bits de la direccion del esclavo y un bit especificando si es lectura (1) escritura (0)
# el esclavo responde con SDA LOW y empieza la comunicacion, si no responde no se da la comunicacion

# Seleccionamos el I2C (SMBus) 1
bus = smbus.SMBus(1)  
address = 0x48        # Dirección del dispositivo I2C

# Leer un byte de datos
data = bus.read_byte_data(address, 0)
print("Datos recibidos:", data)