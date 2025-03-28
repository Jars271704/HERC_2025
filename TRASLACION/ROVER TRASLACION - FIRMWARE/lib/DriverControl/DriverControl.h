#pragma once
#include <Precompile.h>

namespace DriverControl {

    // CONFIG
    enum CONFIG {
        FREQUENCY =         18000,
        RESOLUTION_BIT =    8,
        RESOLUTION_MAX =    255,
    };

    // PINOUT MOTORS
    namespace CONTROLERS {
        struct CONTROL {
            float KP;
            float KI;
            float previous_error;
            float integral;
            float control_output;
        };
        constexpr CONTROL FORWARD_LEFT =   {160.0f, -147.0601f, 0, 0, 0};
    }

    // Start Driver
    void start();

    float update(CONTROLERS::CONTROL* control, float error);

}
