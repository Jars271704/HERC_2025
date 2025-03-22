import pigpio
import time

# La Libreria pigpio trabaja con numeracion BCM
pi = pigpio.pi()

# Salida Digital
pi.set_mode(4, pigpio.OUTPUT)
pi.set_mode(18, pigpio.OUTPUT)

# Iniciamos PWM por hardware (100 Hz, 500 000 porcentaje de senal en HIGH, el maximo es 1 000 000)
pi.hardware_PWM(18, 100, 500000)
time.sleep(5)
# Detenemos PWM 
pi.hardware_PWM(18, 0, 0)  

# Cierre de funciones
pi.stop()