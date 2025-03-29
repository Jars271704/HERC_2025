#pragma once
#include "Precompile.h"
#include <driver/pcnt.h>

namespace DriverEncoder {

    // CONFIG
    enum CONFIG {
        MAX_COUNTER = 32767,
    };
    // PINOUT ENCODERS
    namespace ENCODERS {
        struct ENCODER {
            pcnt_unit_t PCNT_UNIT;
            uint8_t PIN_A;
            uint8_t PIN_B;
            bool MODE_FORWARD;
            float encoder_factor;	// 0.24 aprox
            int16_t previous_encoder_value;
            uint32_t previous_time;
            float speed;
            ENCODER(pcnt_unit_t p, uint8_t pin_A, uint8_t pin_B, float encoder_fac) {
                this->PCNT_UNIT = p; this->PIN_A = pin_A, this->PIN_B = pin_B, this->encoder_factor = encoder_fac;};
        };
        //extern ENCODER FORWARD_LEFT;
        extern ENCODER FORWARD;
        extern ENCODER MIDDLE;
        extern ENCODER BACKWARD;
    }

    // Start Driver
    void start();

    // Read and Save Velocity RPS
    float getVelocity(ENCODERS::ENCODER* encoder);

    void configEncoder(ENCODERS::ENCODER* encoder);

}
