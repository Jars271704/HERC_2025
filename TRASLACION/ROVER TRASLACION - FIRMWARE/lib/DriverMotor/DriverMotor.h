#pragma once
#include <Precompile.h>

namespace DriverMotor {

    // CONFIG
    enum CONFIG {
        FREQUENCY =         18000,
        RESOLUTION_BIT =    8,
        RESOLUTION_MAX =    255,
    };

    // PINOUT MOTORS
    namespace MOTORS {
        struct MOTOR {
            uint8_t CHANNEL_FORWARD;
            uint8_t CHANNEL_BACKWARD;
            uint8_t PIN_FORWARD;
            uint8_t PIN_BACKWARD;
        };
        extern MOTOR FORWARD_LEFT;
        extern MOTOR FORWARD_RIGHT;
    }

    // Start Driver
    void start();

    // Control directo de motores
    void setDuty(MOTORS::MOTOR* motor, int duty);

    void configMotor(MOTORS::MOTOR* motor);

}
