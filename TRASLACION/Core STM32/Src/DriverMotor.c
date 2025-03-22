#include "DriverMotor.h"

/* GLOBAL VARIABLES ------------------------------------------------------------------*/


void DM_start(DM_UNIT* config) {
	HAL_TIM_PWM_Start(config->htim, config->channel_forward);
	HAL_TIM_PWM_Start(config->htim, config->channel_backward);
}


void DM_set_value(DM_UNIT* config, float value) {
	// Pasando el valor en floats del 0 al 1
	uint32_t value_converted = (uint32_t)(fabsf(value) * (float)config->htim->Instance->ARR);
	if(value > 0) {
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_forward, value_converted);
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_backward, 0);

	} else if(value == 0) {
		// Frenado Activo
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_forward, config->htim->Instance->ARR);
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_backward, config->htim->Instance->ARR);
	}
	else {
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_forward, 0);
		__HAL_TIM_SET_COMPARE(config->htim, config->channel_backward, value_converted);
	}
}


