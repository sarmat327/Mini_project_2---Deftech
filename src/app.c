#include "business_layer/app.h"

#include "stm32f4xx_hal.h"
#include "drivers/d_timer.h"
#include "drivers/d_gpio.h"
#include "drivers/d_adc.h"
#include "components/c_led.h"
#include "components/c_button.h"
#include "components/c_ldr.h"
#include "components/c_relay.h"
#include "config.h"

#define GREEN_LED_BLINK_PERIOD_MS    300U
#define YELLOW_LED_BLINK_PERIOD_MS   300U
#define GREEN_LED_MAX_COUNT          3
#define LDR_AIMING_TIME_MS           1000U
#define LDR_ALARM_TIME_MS            2000U


static C_Button_t btn_flare; // кнопка теплових пасток
static C_Button_t btn_combat; // кнопка бойового режиму
static C_LED_t green_led;
static C_LED_t yellow_led;
static C_LDR_t ldr;
static uint32_t ldr_time_impact = 0;
static bool ldr_lost = false;   // чи падав ldr нижче порогу під час BLINKING

static C_Relay_t relay;

static App_State_t app_state_combat = STATE_NONE;
static App_State_t app_state = STATE_NONE;

static uint32_t last_toggle;

static uint32_t yellow_led_LTC = 0;


static uint32_t green_led_LTC = 0; // LTC - last time change
static uint16_t green_led_counter = 0; // counter для blinking

void App_Init(void)
{
    TIM2_Init();
    D_GPIO_Init();
    D_ADC_Init();

    C_LDR_Init(&ldr, LDR_ADC_CHANNEL);
    C_LED_Init(&green_led, GREEN_LED_PORT, GREEN_LED_PIN);
    C_LED_Init(&yellow_led, YELLOW_LED_PORT, YELLOW_LED_PIN);
    C_Button_Init(&btn_flare, BUTTON1_PORT, BUTTON1_PIN);
    C_Button_Init(&btn_combat, BUTTON2_PORT, BUTTON2_PIN);
    C_Relay_Init(&relay, RELAY_PORT, RELAY_PIN, RELAY_IDLE);

    D_GPIO_RegisterEXTICallback(C_Button_EXTI_Handler);


    last_toggle = D_Timer_GetTick();
    
}

void App_Process(void)
{
    /* --- Обробка компонентів --- */
    C_Button_Process(&btn_combat);
    C_Button_Process(&btn_flare);
    C_LDR_Process(&ldr);
    C_Relay_Process(&relay);

    /* 
    ОБРОБКА 1 пункту через кнопку пасток btn_flare
    та РЕЛЕ
    */
    
    if (C_Button_GetEvent(&btn_flare) == BUTTON_EVENT_SHORT_PRESS)
    {
        C_Relay_StartSequence(&relay);
        app_state = STATE_NONE;
    }
   

    /* 
    ОБРОБКА 2 пункту через кнопку бойового режиму
    та ЗЕЛЕНИЙ лед
    // */
    C_Button_Event_t btn_event = C_Button_GetEvent(&btn_combat);
    if (btn_event == BUTTON_EVENT_LONG_PRESS &&
        app_state_combat == STATE_NONE)
    {
        app_state_combat = TARGET_FIX;
    }
    if (btn_event == BUTTON_EVENT_SHORT_PRESS && 
        app_state_combat == TARGET_FIX)
    {
        app_state_combat = TARGET_DESTROY;
        C_LED_Off(&green_led);
        
    }

    switch (app_state_combat)
    {
    case STATE_NONE:
        /* code */
        break;
    
    case TARGET_FIX:
        C_LED_On(&green_led);
        break;
    
    case TARGET_DESTROY:
        App_LED_StartSequence(&green_led);
        break;
    
    default:
        break;
    }

    
    App_Yellow_Process();
}

void App_Yellow_Process(void)
{
    float ldr_current = C_LDR_GetFiltered(&ldr);
    uint32_t now = D_Timer_GetTick();

    switch (app_state)
    {
    case STATE_NONE:
        C_LED_Off(&yellow_led);
        if (ldr_current >= LDR_THRESHOLD_MAX)
        {
            ldr_time_impact = now;      // початок засвічення
            app_state = AIMED;
        }
        break;

    case AIMED:
        if (ldr_current <= LDR_THRESHOLD_MIN)
        {
            app_state = STATE_NONE;     // засвічення було коротшим за 1000 мс
        }
        else if (now - ldr_time_impact >= LDR_AIMING_TIME_MS)
        {
            ldr_lost = false;
            yellow_led_LTC = now;
            app_state = BLINKING;
        }
        break;

    case BLINKING:
    App_LED_Blinking(&yellow_led);

    if (now - ldr_time_impact >= LDR_ALARM_TIME_MS &&
        ldr_current >= LDR_THRESHOLD_MAX)
    {
        app_state = ALARM;
    }
    break;

    case ALARM:
        C_LED_On(&yellow_led);          // горить постійно, поки кнопка не скине стан
        break;

    default:
        break;
    }
}

void App_LED_Blinking(C_LED_t *led)
{
    if (led == NULL)
    {
        return;
    }
    uint32_t now = D_Timer_GetTick();
    if (now - yellow_led_LTC >= YELLOW_LED_BLINK_PERIOD_MS)
    {
        C_LED_Toggle(led);
        yellow_led_LTC = now;           
    }
}

/* ФУНКЦІЯ ПРАЦЮЄ З GREEN LED*/
void App_LED_StartSequence(C_LED_t *led) 
{
    if (led == NULL)
    {
        return;
    }
    uint32_t now = D_Timer_GetTick();
    if(now - green_led_LTC >= GREEN_LED_BLINK_PERIOD_MS &&
        C_LED_GetState(led) == LED_OFF)
    {
        C_LED_On(led);
        green_led_LTC = now;
    }
    if( now - green_led_LTC>= GREEN_LED_BLINK_PERIOD_MS &&
        C_LED_GetState(led) == LED_ON)
    {
        C_LED_Off(led);
        green_led_LTC = now;
        green_led_counter++;
    }

    if(green_led_counter >= GREEN_LED_MAX_COUNT + 1)
    {
        app_state_combat = STATE_NONE;
        green_led_counter = 0;
    }



}