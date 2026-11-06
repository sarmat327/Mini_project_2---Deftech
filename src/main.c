#include "stm32f4xx_hal.h"
#include "drivers/d_timer.h"
#include "drivers/d_gpio.h"
#include "config.h"

// Callback для обробки подій кнопок
void Test_Button_Callback(uint16_t pin, GPIO_PinState state)
{
    if (pin == BUTTON1_PIN)
    {
        // Кнопка 1 керує зеленим LED
        D_GPIO_Write(GREEN_LED_PORT, GREEN_LED_PIN, state);
    }

    if (pin == BUTTON2_PIN)
    {
        // Кнопка 2 керує жовтим LED
        D_GPIO_Write(YELLOW_LED_PORT, YELLOW_LED_PIN, state);
    }
}

int main(void)
{

    HAL_Init();
    TIM2_Init();
    D_GPIO_Init();
    // Реєструємо callback
    D_GPIO_RegisterEXTICallback(Test_Button_Callback);
 
   while (1)
    {
        __WFI();   // усе робиться в перериваннях
    }
}