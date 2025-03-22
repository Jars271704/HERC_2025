import spidev

spi = spidev.SpiDev()
spi.open(0, 0)  # Abrir el bus SPI0, dispositivo CE0

# Enviar y recibir datos
data = spi.xfer2([0x01, 0x02, 0x03])
print("Datos recibidos:", data)