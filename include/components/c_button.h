#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "components/c_button.h"

/* BUTTON STATES */
typedef enum
{
    BUTTON_IDLE,
    BUTTON_DEBOUNCE_PRESS,
    BUTTON_PRESSED,
    BUTTON_LONG_PRESSED,
    BUTTON_DEBOUNCE_RELEASE

} C_Button_State_t;


/* BUTTON EVENTS */
typedef enum
{
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS

} C_Button_Event_t;


/* BUTTON об'єкт */
typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

    C_Button_State_t state;
    C_Button_Event_t event;

    uint32_t timestamp;
    uint32_t press_timestamp;

    GPIO_PinState pin_state;

} C_Button_t;


/* PUBLIC FUNCTIONS */

void C_Button_Init(C_Button_t *button, GPIO_TypeDef *port, uint16_t pin);

void C_Button_EXTI_Handler(uint16_t pin, GPIO_PinState state);

void C_Button_Process(C_Button_t *button);

C_Button_Event_t C_Button_GetEvent(C_Button_t *button);