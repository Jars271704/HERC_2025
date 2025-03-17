#include "DriverMotor.h"

void PWM_Init(PWM_Config *config) {
    HAL_TIM_PWM_Start(config->htim1, config->channel);
}

void PWM_SetValue(PWM_Config *config, uint8_t duty) {
    uint32_t arr = config->htim->Instance->ARR;
    uint32_t ccr = (duty * arr) / 100;
    __HAL_TIM_SET_COMPARE(config->htim1, config->channel, ccr);
}
