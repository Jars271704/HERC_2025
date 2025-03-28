#include "DriverControl.h"

void DriverControl::start()
{
}

float DriverControl::update(CONTROLERS::CONTROL* control, float error)
{
    // Definir el periodo de muestreo en segundos
    const float T = 0.02f;

    control->integral += error* T;

    // Calcular la salida del controlador PI
    float output = control->KP * error + control->KI * control->integral;

    // Aplicar anti-windup limitando solo la integral si hay saturación
    if (output > 1.0f) {
        output = 1.0f;
        control->integral -= error * T;  // Evitar acumulación excesiva
    } else if (output < -1.0f) {
        output = -1.0f;
        control->integral -= error * T;
    }

    // Guardar el error actual para la próxima iteración
    control->previous_error = error;

    // Actualizar la salida del controlador
    control->control_output = output;

    return output;
}
