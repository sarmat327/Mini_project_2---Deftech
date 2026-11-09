#pragma once
 
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
 
#define D_TIMER_COUNT    8U
 
typedef void (*D_Timer_Callback_t)(void);
 
/* Ініціалізація TIM2 як системного таймера з тіком 1 мс */
void TIM2_Init(void);
 
/* Час у мілісекундах від старту TIM2 */
uint32_t D_Timer_GetTick(void);
 
/* Обробник фатальної помилки */
void Error_Handler(void);