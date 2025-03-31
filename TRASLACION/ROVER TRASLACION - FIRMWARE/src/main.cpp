#include <DriverMotor.h>
#include <DriverEncoder.h>
#include <DriverControl.h>
#include <SerialControl.h>

// CONEXIONES DE PINOUT PARA UN MOTOR CONVENCION DE SIGNOS
// PIN_FORWARD a R_PWM, PIN_BACKWARD a L_PWM
// PIN_A a cable amarillo del encoder, PIN_B a cable blanco del encoder

// PINOUT
// FORWARD 
// PWM FORWARD 12       PWM BACKWARD 13
// PIN_A 4              PIN_B 5
// MIDDLE
// PWM FORWARD 14       PWM BACKWARD 15
// PIN_A 21              PIN_B 22
// BACKWARD
// PWM FORWARD 16       PWM BACKWARD 17
// PIN_A 25              PIN_B 26

// Una esp para los 3 motores del lado izquierdo, otra para el derecho,
// su configuracion de pines es la misma, pero reciben distintas speeds por la raspy

// SerialControl se encarga de recibir 3 floats que indican velocidades de forward middle backward
// Y Actualizar esto en los controlers

void setup() {
    SerialControl::start();
}

void loop() {
    SerialControl::loop();
}

