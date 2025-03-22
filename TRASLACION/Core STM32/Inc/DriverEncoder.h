#ifndef DRIVER_ENCODER
#define DRIVER_ENCODER

/* INCLUDES ------------------------------------------------------------------*/
#include "PreCompile.h"
#include "stm32f4xx_hal.h"

/* CONFIG ------------------------------------------------------------------*/
typedef struct {
	TIM_HandleTypeDef* htim;
	bool mode_forward;
	float speed;
	uint32_t previous_encoder_value;
	uint32_t previous_time;
	float encoder_divisor;	// 11300 aprox
} DE_UNIT;

void DE_start(DE_UNIT* config);

void DE_read(DE_UNIT* config);

float DE_get_speed(DE_UNIT* config);

#endif
