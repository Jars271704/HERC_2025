#include <SerialControl.h>

void SerialControl::start()
{
    // Start Drivers y Serial
    DriverMotor::start();
    DriverEncoder::start();
    DriverControl::start();
    Serial.begin(CONFIG::SERIAL_RATE);

    // Estado Inicial de los motores
    DriverMotor::setDuty(&DriverMotor::MOTORS::FORWARD_LEFT, 0);
    DriverEncoder::ENCODERS::FORWARD_LEFT.MODE_FORWARD = true;
    DriverControl::CONTROLERS::FORWARD_LEFT.set_point = 0.0f;
}

void SerialControl::loop()
{
    // Esperar de Serial Wait el ratio y la speed
    float ratio = 0; float speed_rad = 0;
    if(waitSerial(&ratio, &speed_rad)) {
        // Cambiar setpoints
        DriverControl::CONTROLERS::FORWARD_LEFT.set_point = speed_rad;
        // Recibir las velocidades de los 6 motores
    }
    // Update Controlers
    DriverControl::update(&DriverControl::CONTROLERS::FORWARD_LEFT);
}

bool SerialControl::waitSerial(float* ratio, float* speed_rad)
{
    // Esperar por comunicacion Serial
    // Crear el Buffer y variables
    static char inputBuffer[CONFIG::BUFFER_SIZE];
    unsigned long startTime = millis();
    bool bandera_read = false;
    uint8_t index = 0;

    // Bucle de espera de entrada serial
    while (millis() - startTime < CONFIG::TIME_QUANTUM) {   // Esperar el tiempo definido
        if (Serial.available()) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') {
                if (index > 0) { // Solo procesar si hay datos en el buffer
                    inputBuffer[index] = '\0';
                    int parsed = sscanf(inputBuffer, "%f %f", ratio, speed_rad);
                    bandera_read = (parsed == 2);
                    break; // Salir del bucle una vez leído el dato
                }
            } else if (index < sizeof(inputBuffer) - 1) {
                inputBuffer[index++] = c;
            }
        }
    }
    // Durar un tiempo constante en la funcion
    unsigned long time_rest = CONFIG::TIME_QUANTUM - (millis() - startTime);
    if(time_rest > 0) delay(time_rest);

    return bandera_read;
}
