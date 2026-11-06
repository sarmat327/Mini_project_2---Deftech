#include "drivers/d_timer.h"
#include "config.h"

static TIM_HandleTypeDef htim2;

volatile uint32_t system_tick = 0;


/* Timer initialization */
void TIM2_Init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 16 - 1;      /* 16 MHz / 16 = 1 MHz (1 us per count) */
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 1000 - 1;    /* 1000 counts * 1 us = 1 ms */
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&htim2);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


void SysTick_Handler(void)
{
    HAL_IncTick();
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2)
{
    if (htim2->Instance == TIM2)
    {
        system_tick++;
    }
}

uint32_t D_Timer_GetTick(void)
{
    return system_tick;
}