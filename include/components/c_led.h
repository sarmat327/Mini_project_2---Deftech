#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>
//#include <stdbool.h>

typedef enum
{
    LED_OFF,
    LED_ON
} C_LED_State_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    C_LED_State_t state;
} C_LED_t;

/* Initialization */
void C_LED_Init(C_LED_t *led, GPIO_TypeDef *port, uint16_t pin);

/* Control */
void C_LED_On(C_LED_t *led);
void C_LED_Off(C_LED_t *led);
void C_LED_Toggle(C_LED_t *led);
void C_LED_SetState(C_LED_t *led, C_LED_State_t state);

/* Get state */
C_LED_State_t C_LED_GetState(const C_LED_t *led);