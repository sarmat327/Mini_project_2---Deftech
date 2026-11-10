#pragma once
#include "components/c_led.h"

typedef enum
{
    /*бойовий режим*/
    STATE_NONE = 0, 
    TARGET_FIX,
    TARGET_DESTROY,

    /* наведення та режим тривоги */

    AIMED,
    ALARM,
    BLINKING

} App_State_t;

/** Ініціалізація драйверів, компонентів та callback'ів */
void App_Init(void);

/** Один прохід головного циклу застосунку (викликати в while(1)) */
void App_Process(void);

void App_Yellow_Process(void);

void App_LED_Blinking(C_LED_t *led);

void App_LED_StartSequence(C_LED_t *led);