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
        
    if (pin == BUTTON1_PIN)
    {
        button1_ptr = button;
    }
    else if (pin == BUTTON2_PIN)
    {
    button2_ptr = button;
    }
}
