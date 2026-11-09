#include "stm32f4xx_hal.h"
#include "drivers/d_timer.h"
#include "drivers/d_gpio.h"
#include "drivers/d_adc.h"
#include "components/c_led.h"
#include "components/c_button.h"
#include "components/c_ldr.h"
#include "config.h"


static C_Button_t button1;
static C_Button_t button2;

static C_LED_t green_led;
static C_LED_t yellow_led;

static C_LDR_t ldr;



int main(void)
{
    HAL_Init();
    TIM2_Init();
    D_GPIO_Init();
    
    D_ADC_Init();
    C_LDR_Init(&ldr, LDR_ADC_CHANNEL);
    C_LED_Init(&green_led,GREEN_LED_PORT,GREEN_LED_PIN);
    C_LED_Init(&yellow_led,YELLOW_LED_PORT,YELLOW_LED_PIN);
    C_Button_Init(&button1, BUTTON1_PORT, BUTTON1_PIN);
    C_Button_Init(&button2, BUTTON2_PORT, BUTTON2_PIN);

    D_GPIO_RegisterEXTICallback(C_Button_EXTI_Handler);
   
    uint32_t last_toggle = D_Timer_GetTick();

while (1)
{
    C_Button_Process(&button2);
    C_LDR_Process(&ldr);

    uint32_t now = D_Timer_GetTick();

    if (now - last_toggle >= 500)
    {
        last_toggle = now;                       // перезапуск відліку
        //D_GPIO_Toggle(green_led.port, green_led.pin);
        C_LED_Toggle(&green_led);
    }

    if(C_Button_GetEvent(&button2) == BUTTON_EVENT_LONG_PRESS)
    {
       // D_GPIO_Toggle(yellow_led.port, yellow_led.pin);
        C_LED_Off(&yellow_led);

    }

    float ldr_current = C_LDR_GetFiltered(&ldr);
    if(ldr_current >= LDR_THRESHOLD_MAX) 
    {
        C_LED_On(&yellow_led);
    }
    if(ldr_current <= LDR_THRESHOLD_MIN) 
    {
        C_LED_Off(&yellow_led);
    }


}
}


void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
