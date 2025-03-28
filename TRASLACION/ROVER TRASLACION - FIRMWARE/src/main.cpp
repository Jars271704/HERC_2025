#include <DriverMotor.h>
#include <DriverEncoder.h>
#include <DriverControl.h>

// CONEXIONES DE PINOUT PARA UN MOTOR PARA PRUEBAS
// PIN 12 a R_PWM, PIN_13 a L_PWM
// PIN 4 a cable amarillo del encoder, PIN 5 a cable blanco del encoder

void setup() {
    // Inicializando
    DriverMotor::start();
    DriverEncoder::start();
    Serial.begin(9600);
    // Para cambiar la velocidad un motor se debe tanto escribir el duty como configurar su modo de encoder, + forward, - backward
    DriverMotor::setDuty(DriverMotor::MOTORS::FORWARD_LEFT, DriverMotor::CONFIG::RESOLUTION_MAX);
    DriverEncoder::ENCODERS::FORWARD_LEFT.MODE_FORWARD = true;
}

void loop() {
    // Para obtener la velocidad
    DriverEncoder::getVelocity(&DriverEncoder::ENCODERS::FORWARD_LEFT);
    // Mostrando
    std::string msg = std::to_string(DriverEncoder::ENCODERS::FORWARD_LEFT.speed);
    Serial.println(msg.c_str());
    Serial.println("-----------------------");
    delay(200);
}
