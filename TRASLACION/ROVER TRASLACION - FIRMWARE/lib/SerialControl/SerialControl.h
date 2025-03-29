#pragma once
#include <Precompile.h>
#include <DriverControl.h>

namespace SerialControl {

    // CONFIG
    enum CONFIG {
        SERIAL_RATE =   115200,
        BUFFER_SIZE =   64,      // Numero de cifras significativas + "." + "-" + "\0" por 2 datos float
        TIME_QUANTUM =  20,
    };

    // Start Driver
    void start();

    void loop();

    bool waitSerial(float* speed_forward, float* speed_middle, float* speed_backward);

}
