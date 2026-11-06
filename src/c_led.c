#include "config.h"
#include "components/c_led.h"
#include "drivers/d_gpio.h"

void C_LED_Init(C_LED_t *led, GPIO_TypeDef *port, uint16_t pin)
{
    if (led == NULL)
    {
        return;
    }

    led->pin = pin;
    led->port = port;
    led->state = LED_OFF;
}

void C_LED_On(C_LED_t *led)
{
    if (led == NULL)
    {
        return;
    }

    D_GPIO_Write(led->port, led->pin, GPIO_PIN_SET);
    led->state = LED_ON;                                // LED_ON = GPIO_PIN_SET
}

void C_LED_Off(C_LED_t *led)
{
    if (led == NULL)
    {
        return;
    }

    D_GPIO_Write(led->port, led->pin, GPIO_PIN_RESET);
    led->state = LED_OFF;                                // LED_OFF = GPIO_PIN_RESET
}

void C_LED_Toggle(C_LED_t *led)
{
    if (led == NULL)
    {
        return;
    }

    D_GPIO_Toggle(led->port, led->pin);

    if (led->state == LED_ON)
    {
        led->state = LED_OFF;
    }
    else
    {
        led->state = LED_ON;
    }
}

void C_LED_SetState(C_LED_t *led, C_LED_State_t state)
{
    if (led == NULL)
    {
        return;
    }

    switch (state)
    {
        case LED_ON:
            D_GPIO_Write(led->port, led->pin, GPIO_PIN_SET);
            led->state = LED_ON;
            break;

        case LED_OFF:
            D_GPIO_Write(led->port, led->pin, GPIO_PIN_RESET);
            led->state = LED_OFF;
            break;
    }
}

C_LED_State_t C_LED_GetState(const C_LED_t *led) // const - захист від помилок програміста
{
     if (led == NULL)
    {
        return LED_OFF;
    }
    return led->state;
}