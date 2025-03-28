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
        constexpr MOTOR FORWARD_LEFT =   {0,1,12,13};
        constexpr MOTOR FORWARD_RIGHT =  {2,3,14,15};
    }

    // Start Driver
    void start();

    // Control directo de motores
    void setDuty(MOTORS::MOTOR motor, int duty);

    void configMotor(MOTORS::MOTOR motor);

}
