#ifndef DRIVER_MOTOR
#define DRIVER_MOTOR

/* INCLUDES ------------------------------------------------------------------*/
#include "PreCompile.h"

/* CONFIG ------------------------------------------------------------------*/
typedef struct {
	TIM_HandleTypeDef* htim;
	uint32_t channel_forward;
	uint32_t channel_backward;
} DM_UNIT;

void DM_start(DM_UNIT* config);

void DM_set_value(DM_UNIT* config, float value);


#endif
