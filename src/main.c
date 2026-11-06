#include "stm32f4xx_hal.h"
#include "drivers/d_timer.h"

#define LED_PORT GPIOB
#define LED_PIN  GPIO_PIN_0

#define LED_INTERVAL_MS 500
uint32_t lastTimeChange = 0;

static void GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

int main(void)
{
    HAL_Init();
    GPIO_Init();
    TIM2_Init();

    while (1)
    {
        uint32_t now = D_Timer_GetTick();

        if (now - lastTimeChange >= 500)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            lastTimeChange = now;
        }
    }
}

