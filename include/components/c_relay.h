#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* RELAY TIMING */
#define RELAY_ON_TIME_MS       100U
#define RELAY_OFF_TIME_MS      250U
#define RELAY_PULSE_COUNT      3U

/* RELAY STATES */
typedef enum
{
    RELAY_IDLE,
    RELAY_PULSE_ON,
    RELAY_PULSE_OFF

} C_Relay_State_t;

/* RELAY COMPONENT */
typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

    GPIO_PinState active_level;

    C_Relay_State_t state;

    uint32_t timestamp;
    uint8_t pulse_count;

} C_Relay_t;

/* PUBLIC FUNCTIONS */

void C_Relay_Init(C_Relay_t *relay,
                  GPIO_TypeDef *port,
                  uint16_t pin,
                  GPIO_PinState active_level);

void C_Relay_StartSequence(C_Relay_t *relay);

void C_Relay_Process(C_Relay_t *relay);

bool C_Relay_IsBusy(const C_Relay_t *relay);