#include <DriverMotor.h>

namespace DriverMotor {
    namespace MOTORS {
        MOTORS::MOTOR FORWARD_LEFT =   {0,1,12,13};
        MOTORS::MOTOR FORWARD_RIGHT =  {2,3,14,15};
    }
}

void DriverMotor::start()   {
    configMotor(&MOTORS::FORWARD_LEFT);
    configMotor(&MOTORS::FORWARD_RIGHT);
}

void DriverMotor::setDuty(MOTORS::MOTOR* motor, int duty)
{
    // LIMIT DUTY
    if(duty > CONFIG::RESOLUTION_MAX) duty = CONFIG::RESOLUTION_MAX;
    else if (duty < -CONFIG::RESOLUTION_MAX) duty = -CONFIG::RESOLUTION_MAX;

    // WRITE DUTY
	if(duty > 0) {
        ledcWrite(motor->CHANNEL_FORWARD, duty);
        ledcWrite(motor->CHANNEL_BACKWARD, 0);

	} else if(duty == 0) {
		// Frenado Activo
        ledcWrite(motor->CHANNEL_FORWARD, CONFIG::RESOLUTION_MAX);
        ledcWrite(motor->CHANNEL_BACKWARD, CONFIG::RESOLUTION_MAX);
	}
	else {
        ledcWrite(motor->CHANNEL_FORWARD, 0);
        ledcWrite(motor->CHANNEL_BACKWARD, abs(duty));
	}
}

void DriverMotor::configMotor(MOTORS::MOTOR* motor)
{
    ledcSetup(motor->CHANNEL_FORWARD, CONFIG::FREQUENCY, CONFIG::RESOLUTION_BIT);
    ledcSetup(motor->CHANNEL_BACKWARD, CONFIG::FREQUENCY, CONFIG::RESOLUTION_BIT);
    ledcAttachPin(motor->PIN_FORWARD, motor->CHANNEL_FORWARD);
    ledcAttachPin(motor->PIN_BACKWARD, motor->CHANNEL_BACKWARD);

}
