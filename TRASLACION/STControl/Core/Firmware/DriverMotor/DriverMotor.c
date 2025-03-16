#include "DriverMotor.h"

void PWM_Init(PWM_Config *config) {
    HAL_TIM_PWM_Start(config->htim, config->channel);
}

void PWM_SetDutyCycle(PWM_Config *config, uint8_t duty) {
    uint32_t arr = config->htim->Instance->ARR;
    config->htim->Instance->CCR1 = (duty * arr) / 100;
}
