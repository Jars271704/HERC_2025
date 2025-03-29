#include <DriverControl.h>

namespace DriverControl {
    namespace CONTROLERS {
        CONTROL FORWARD_LEFT = CONTROL(&DriverEncoder::ENCODERS::FORWARD_LEFT, &DriverMotor::MOTORS::FORWARD_LEFT, 40.0, -12.0);
    }
}

void DriverControl::start()
{
    // De momento no es necesaria
}

void DriverControl::update(CONTROLERS::CONTROL *control)
{
    // Funcion de Loop que se llama con setpoint previamente definido en control

    // Proteger de set points muy bajos
    if(abs(control->set_point) < 0.5f) {
        DriverMotor::setDuty(control->motor, 0);
        control->encoder->MODE_FORWARD = true;
        Serial.println("SET_POINT BAJO");
        Serial.println("----------------------------------");
    }
    else {
        float speed_Rad = DriverEncoder::getVelocity(control->encoder) * PI * 2.0f;
        if(control->encoder->MODE_FORWARD == false) speed_Rad *= -1;
        float error = control->set_point - speed_Rad;
        float output = control->KP * error + control->KI * control->previous_error + control->previous_output;
        
        if (output > DriverMotor::CONFIG::RESOLUTION_MAX) output = DriverMotor::CONFIG::RESOLUTION_MAX;
        if (output < -DriverMotor::CONFIG::RESOLUTION_MAX) output = -DriverMotor::CONFIG::RESOLUTION_MAX;
    
        // Actualiza valores previous
        control->previous_output = output;
        control->previous_error = error;
       
        // Escribir la salida en el motor, y actualizar el modo del encoder
        DriverMotor::setDuty(control->motor, output);
        control->encoder->MODE_FORWARD = (output >= 0);
    
        // Codigo de depuracion
        Serial.println(control->encoder->MODE_FORWARD);
        Serial.println(control->previous_error);
        Serial.println(speed_Rad);
        Serial.println(output);
        Serial.println("----------------------------------");
    }
}
