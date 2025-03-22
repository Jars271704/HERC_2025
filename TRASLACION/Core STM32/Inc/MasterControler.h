#ifndef MASTER_CONTROLER
#define MASTER_CONTROLER

/* INCLUDES ------------------------------------------------------------------*/
#include "PreCompile.h"
#include "DriverMotor.h"
#include "DriverEncoder.h"
#include "DriverControl.h"

/* CONFIG ------------------------------------------------------------------*/
typedef struct {
	DM_UNIT* DM_unit;
	DE_UNIT* DE_unit;
	DC_UNIT* DC_unit;
} MC_UNIT;

typedef struct {
	MC_UNIT* MC_forward_left;
	//DE_UNIT* encoder_forward_right;
	//DE_UNIT* encoder_middle_left;
	//DE_UNIT* encoder_middle_right;
	//DE_UNIT* encoder_backward_left;
	//DE_UNIT* encoder_backward_right;
} MC_CONFIG;


#endif
