#pragma once

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "config.h"

// Функція зворотного виклику для EXTI
typedef void (*D_GPIO_EXTI_Callback_t)(uint16_t pin, GPIO_PinState state);

// Ініціалізація GPIO
void D_GPIO_Init(void);

// Керування виходами
void D_GPIO_Write(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);
void D_GPIO_Toggle(GPIO_TypeDef *port, uint16_t pin);

// Зчитування стану входу
GPIO_PinState D_GPIO_Read(GPIO_TypeDef *port, uint16_t pin);

// Реєстрація обробника EXTI
void D_GPIO_RegisterEXTICallback(D_GPIO_EXTI_Callback_t callback);