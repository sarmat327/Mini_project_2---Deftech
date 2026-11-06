#pragma once
 
#include "stm32f4xx_hal.h"
#include <stdint.h>

/* TIMER */
#define D_TIMER_IRQ_PRIORITY   1

/* GPIO */
#define RELAY_PORT             GPIOC            //СИСТЕМА "ПАСТОК"
#define RELAY_PIN              GPIO_PIN_13

#define BUTTON1_PORT           GPIOB            //КНОПКА ЯКА ЗАПУСКАЄ РЕЛЕ
#define BUTTON1_PIN            GPIO_PIN_12

#define BUTTON2_PORT           GPIOB            // КНОПКА "БОЙОВОГО" РЕЖИМУ
#define BUTTON2_PIN            GPIO_PIN_13

#define GREEN_LED_PORT         GPIOB            // ЛЕД ІНДИКАЦІЇ "БОЙОВОГО" РЕЖИМУ
#define GREEN_LED_PIN          GPIO_PIN_0

#define YELLOW_LED_PORT        GPIOB            // ІНДИКАТОР ОПРОМІНЕННЯ
#define YELLOW_LED_PIN         GPIO_PIN_1

/* ADC */
#define LDR_PORT               GPIOA
#define LDR_PIN                GPIO_PIN_0
#define LDR_ADC_CHANNEL        ADC_CHANNEL_0
