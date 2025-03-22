#include "DriverEncoder.h"

/* GLOBAL VARIABLES ------------------------------------------------------------------*/

void DE_start(DE_UNIT* config) {
	HAL_TIM_Encoder_Start(config->htim, TIM_CHANNEL_ALL);
}


void DE_read(DE_UNIT* config) {
	uint32_t encoder_value = __HAL_TIM_GET_COUNTER(config->htim);
	// Obtener Dif Encoder considerando cuando se reinicia el contador CNT
	float dif_encoder_value;
	if(config->mode_forward) {
		dif_encoder_value = (float)(encoder_value - config->previous_encoder_value);
		if(encoder_value < config->previous_encoder_value) {
			dif_encoder_value = (float)(encoder_value + (config->htim->Instance->ARR - config->previous_encoder_value));
		}
	}
	else {
		dif_encoder_value = (float)(config->previous_encoder_value - encoder_value);
		if(encoder_value > config->previous_encoder_value) {
			dif_encoder_value = (float)(config->previous_encoder_value + (config->htim->Instance->ARR - encoder_value));
		}
	}
	config->previous_encoder_value = encoder_value;
	uint32_t time = HAL_GetTick();
	// Obtener Dif Time en segundos evitando ceros
	float dif_time = (float)(time - config->previous_time) / 1000.0;
	if(time <= config->previous_time) dif_time = 0.001;
	config->previous_time = time;
	config->speed = (dif_encoder_value / dif_time) / config->encoder_divisor;
	printf("Encoder Value: %lu, Encoder Previous Value: %lu, Speed Value: %f\n", encoder_value, config->previous_encoder_value, config->speed);
	HAL_Delay(1000);
}

float DE_get_speed(DE_UNIT* config) {
	return config->speed;
}
