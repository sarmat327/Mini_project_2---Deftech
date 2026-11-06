#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* PUBLIC FUNCTIONS */

void D_ADC_Init(void);

uint16_t D_ADC_Read(uint32_t channel);