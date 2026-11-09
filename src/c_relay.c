#include "components/c_relay.h"

#include "drivers/d_gpio.h"
#include "drivers/d_timer.h"

//#include <stddef.h>


/* PRIVATE FUNCTIONS */

static void C_Relay_SetOutput(C_Relay_t *relay, bool active)
{
    if (relay == NULL)
        return;

    GPIO_PinState output_state;

    if (active)
    {
        output_state = relay->active_level;
    }
    else
    {
        output_state = (relay->active_level == GPIO_PIN_SET)
                       ? GPIO_PIN_RESET
                       : GPIO_PIN_SET;
    }

    D_GPIO_Write(relay->port, relay->pin, output_state);
}


/* PUBLIC FUNCTIONS */

void C_Relay_Init(C_Relay_t *relay,
                  GPIO_TypeDef *port,
                  uint16_t pin,
                  GPIO_PinState active_level)
{
    if (relay == NULL || port == NULL)
        return;

    relay->port = port;
    relay->pin = pin;

    relay->active_level = active_level;

    relay->state = RELAY_IDLE;

    relay->timestamp = D_Timer_GetTick();

    relay->pulse_count = 0;

    /* перше значення RELAY OFF */
    C_Relay_SetOutput(relay, RELAY_IDLE);
}


void C_Relay_StartSequence(C_Relay_t *relay)
{
    if (relay == NULL)
        return;

    /* Ignore command if sequence is already running */
    if (relay->state != RELAY_IDLE)
        return;

    relay->pulse_count = 0;

    relay->timestamp = D_Timer_GetTick();

    relay->state = RELAY_PULSE_ON;

    C_Relay_SetOutput(relay, true);
}


void C_Relay_Process(C_Relay_t *relay)
{
    if (relay == NULL)
        return;

    if (relay->state == RELAY_IDLE)
        return;

    uint32_t now = D_Timer_GetTick();

    switch (relay->state)
    {
        case RELAY_PULSE_ON:

            if ((uint32_t)(now - relay->timestamp) >= RELAY_ON_TIME_MS)
            {
                C_Relay_SetOutput(relay, false);

                relay->pulse_count++;

                relay->timestamp = now;

                relay->state = RELAY_PULSE_OFF;
            }

            break;


        case RELAY_PULSE_OFF:

            if ((uint32_t)(now - relay->timestamp) >= RELAY_OFF_TIME_MS)
            {
                if (relay->pulse_count >= RELAY_PULSE_COUNT)
                {
                    relay->state = RELAY_IDLE;
                }
                else
                {
                    C_Relay_SetOutput(relay, true);

                    relay->timestamp = now;

                    relay->state = RELAY_PULSE_ON;
                }
            }

            break;


        case RELAY_IDLE:
        default:
            break;
    }
}


bool C_Relay_IsBusy(const C_Relay_t *relay)
{
    if (relay == NULL)
        return false;

    return (relay->state != RELAY_IDLE);
}