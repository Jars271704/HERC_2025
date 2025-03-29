#include <DriverMotor.h>
#include <DriverEncoder.h>
#include <DriverControl.h>
#include <SerialControl.h>

// CONEXIONES DE PINOUT PARA UN MOTOR PARA PRUEBAS
// PIN 12 a R_PWM, PIN_13 a L_PWM
// PIN 4 a cable amarillo del encoder, PIN 5 a cable blanco del encoder


// Primero debes iniciazliar todas las estructuras con tu configuracion de pines

// Control terminado, ahora es necesario una interfaz que resiba las velocidades y lo mande al control a actuar
void setup() {
    SerialControl::start();
}

void loop() {
    SerialControl::loop();
}



/*#include <DriverMotor.h>
#include <DriverEncoder.h>
#include <DriverControl.h>

// CONEXIONES DE PINOUT PARA UN MOTOR PARA PRUEBAS
// PIN 12 a R_PWM, PIN_13 a L_PWM
// PIN 4 a cable amarillo del encoder, PIN 5 a cable blanco del encoder


// Primero debes iniciazliar todas las estructuras con tu configuracion de pines

// Control terminado, ahora es necesario una interfaz que resiba las velocidades y lo mande al control a actuar
void setup() {

    // Inicializando
    DriverMotor::start();
    DriverEncoder::start();
    Serial.begin(9600);
    // Para cambiar la velocidad un motor se debe tanto escribir el duty como configurar su modo de encoder, + forward, - backward
    DriverMotor::setDuty(&DriverMotor::MOTORS::FORWARD_LEFT, 0);
    DriverEncoder::ENCODERS::FORWARD_LEFT.MODE_FORWARD = true;
    DriverControl::CONTROLERS::FORWARD_LEFT.set_point = -8.0f;
}

void loop() {
    DriverControl::update(&DriverControl::CONTROLERS::FORWARD_LEFT);
    delay(200);
}*/