#ifndef DRIVER_MOTOR
#define DRIVER_MOTOR

#include "stm32f4xx_hal.h"  // Acceso to HAL_TIM, GPIO, etc.

// Estructura General de Constantes y Variables Globales
typedef struct {
    TIM_HandleTypeDef *htim1;
    uint32_t channel;
} PWM_Config;

// Funciones ligadas a una instancia de PWM_Config
void PWM_Init(PWM_Config *config);
void PWM_SetDutyCycle(PWM_Config *config, uint8_t duty);

#endif
