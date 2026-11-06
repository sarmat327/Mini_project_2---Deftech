#include "components/c_button.h"
#include "drivers/d_gpio.h"
#include "drivers/d_timer.h"

static C_Button_t *button1_ptr = NULL;
static C_Button_t *button2_ptr = NULL;


void C_Button_Init(C_Button_t *button, GPIO_TypeDef *port, uint16_t pin)
{
    if (button == NULL)
    {
        return;
    }
        button->port = port;
        button->pin = pin;
        button->state = BUTTON_IDLE;
        button->event = BUTTON_EVENT_NONE;
        button->timestamp = D_Timer_GetTick();
        button->press_timestamp = 0;
        button->pin_state = D_GPIO_Read(port, pin);
        button->long_press_reported = false;
        
    if (pin == BUTTON1_PIN)
    {
        button1_ptr = button;
    }
    else if (pin == BUTTON2_PIN)
    {
    button2_ptr = button;
    }
}

void C_Button_EXTI_Handler(uint16_t pin, GPIO_PinState state)
{
    if (button1_ptr != NULL && button1_ptr->pin == pin)
    {
        button1_ptr->pin_state = state;
        button1_ptr->timestamp = D_Timer_GetTick();
    }
    else if (button2_ptr != NULL && button2_ptr->pin == pin)
    {
        button2_ptr->pin_state = state;
        button2_ptr->timestamp = D_Timer_GetTick();
    }
}

void C_Button_Process(C_Button_t *button)
{
    if (button == NULL)
    {
        return;
    }

    uint32_t now = D_Timer_GetTick();

    switch (button->state)
    {
        /* BUTTON IS IDLE */
        case BUTTON_IDLE:

            if (button->pin_state == GPIO_PIN_SET)
            {
                button->state = BUTTON_DEBOUNCE_PRESS;
                button->timestamp = now;
            }

            break;


        /* DEBOUNCE BUTTON PRESS */
        case BUTTON_DEBOUNCE_PRESS:

            if ((uint32_t)(now - button->timestamp) >= BUTTON_DEBOUNCE_MS)
            {
                button->pin_state = D_GPIO_Read(button->port, button->pin);

                if (button->pin_state == GPIO_PIN_SET)
                {
                    button->state = BUTTON_PRESSED;
                    button->press_timestamp = now;
                    button->long_press_reported = false;
                }
                else
                {
                    button->state = BUTTON_IDLE;
                }
            }

            break;


        /* BUTTON IS PRESSED */
        case BUTTON_PRESSED:

            if (button->pin_state == GPIO_PIN_RESET)
            {
                if ((uint32_t)(now - button->press_timestamp) >= BUTTON_LONG_PRESS_MS)
                {
                    button->event = BUTTON_EVENT_LONG_PRESS;
                    button->long_press_reported = true;
                }

                button->state = BUTTON_DEBOUNCE_RELEASE;
                button->timestamp = now;
            }
            else if ((uint32_t)(now - button->press_timestamp) >= BUTTON_LONG_PRESS_MS)
            {
                button->state = BUTTON_LONG_PRESSED;
                button->event = BUTTON_EVENT_LONG_PRESS;
                button->long_press_reported = true;
            }

            break;


        /* BUTTON LONG PRESSED */
        case BUTTON_LONG_PRESSED:

            if (button->pin_state == GPIO_PIN_RESET)
            {
                button->state = BUTTON_DEBOUNCE_RELEASE;
                button->timestamp = now;
            }

            break;


        /* DEBOUNCE BUTTON RELEASE */
        case BUTTON_DEBOUNCE_RELEASE:

            if ((uint32_t)(now - button->timestamp) >= BUTTON_DEBOUNCE_MS)
            {
                button->pin_state = D_GPIO_Read(button->port, button->pin);

                if (button->pin_state == GPIO_PIN_RESET)
                {
                    if (!button->long_press_reported)
                    {
                        button->event = BUTTON_EVENT_SHORT_PRESS;
                    }

                    button->long_press_reported = false;
                    button->state = BUTTON_IDLE;
                    button->press_timestamp = 0;
                }
                else
                {
                    button->state = button->long_press_reported
                                    ? BUTTON_LONG_PRESSED
                                    : BUTTON_PRESSED;
                }
            }

            break;


        /* UNEXPECTED STATE */
        default:

            button->state = BUTTON_IDLE;
            button->event = BUTTON_EVENT_NONE;
            button->long_press_reported = false;
            button->press_timestamp = 0;

            break;
    }
}

C_Button_Event_t C_Button_GetEvent(C_Button_t *button)
{
    if (button == NULL)
    {
        return BUTTON_EVENT_NONE;
    }

    C_Button_Event_t event = button->event;

    button->event = BUTTON_EVENT_NONE;

    return event;
}