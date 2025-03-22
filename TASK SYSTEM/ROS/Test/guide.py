
# Rasberry PI OS System GNU/Linux based on Debian

# Guia de Pines
# 5 V PWR
# 3.3 V PWR
# GND
# GPIO (General Purpose Input/Output)
# I2C1 SDA (3) SCL (5)
# SPI 0 MOSI (19) MISO (21) SCK (23)        CS0 (24) CS1 (26)
# SPI 0 MOSI (38) MISO (35) SCK (40)        CS0 (36)
# UART TX (8) RX (10)
# #

# Todos soportan Digital, ninguno soporta analogico
# PWM (implementado en el hardware) (32) (33)
##


# Los pines GPIO Trabajan a 3.3V, 16mA
# En la comunicacion SPI se pueden configurar mas pines de los gpio como CS (Chip Select) / SS (Slave Select) para mas dispositivos 
# Los pines indicados de CS, son los que vienen configurados por defecto



# LIBRERIAS
# RPi.GPIO
# pigpio

# Lenguajes y Compiladores
# sudo apt install python3
# sudo apt install build-essential