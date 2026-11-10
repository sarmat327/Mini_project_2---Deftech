#include "stm32f4xx_hal.h"
#include "business_layer/app.h"

int main(void)
{
    HAL_Init();

    App_Init();

    while (1)
    {
        App_Process();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}