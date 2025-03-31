#include <DriverControl.h>

namespace DriverControl {
    namespace CONTROLERS {
        CONTROL FORWARD = CONTROL(&DriverEncoder::ENCODERS::FORWARD, &DriverMotor::MOTORS::FORWARD, 40.0, -12.0);
        CONTROL MIDDLE = CONTROL(&DriverEncoder::ENCODERS::MIDDLE, &DriverMotor::MOTORS::MIDDLE, 40.0, -12.0);
        CONTROL BACKWARD = CONTROL(&DriverEncoder::ENCODERS::BACKWARD, &DriverMotor::MOTORS::BACKWARD, 40.0, -12.0);
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
    }
    else {
        // Aniadir filtros de kalman
        float speed_Rad = DriverEncoder::getVelocity(control->encoder) * PI * 2.0f;

        float error = control->set_point - speed_Rad;
        float output = control->KP * error + control->KI * control->previous_error + control->previous_output;
        
        if (output > DriverMotor::CONFIG::RESOLUTION_MAX) output = DriverMotor::CONFIG::RESOLUTION_MAX;
        if (output < -DriverMotor::CONFIG::RESOLUTION_MAX) output = -DriverMotor::CONFIG::RESOLUTION_MAX;
    
        // Actualiza valores previous
        control->previous_output = output;
        control->previous_error = error;
       
        // Escribir la salida en el motor
        DriverMotor::setDuty(control->motor, output);

        // Codigo de Depuracion
        //Serial.println(error);
        //Serial.println(output);
        //Serial.println("-----------------------------");
    
    }
}
