#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>

void TIM2_Init(void);

uint32_t D_Timer_GetTick(void);