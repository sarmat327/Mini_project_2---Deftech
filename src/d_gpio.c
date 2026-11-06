#include "drivers/d_gpio.h"
#include "config.h"

static D_GPIO_EXTI_Callback_t exti_callback = NULL;

void D_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    // 1. Увімкнення тактування портів
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 2. Початковий стан виходів — LOW (до налаштування пінів, без глітчів)
    HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YELLOW_LED_PORT, YELLOW_LED_PIN, GPIO_PIN_RESET);

    // 3. Виходи: реле (PC13), зелений (PB0), жовтий (PB1)
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    gpio.Pin = RELAY_PIN;
    HAL_GPIO_Init(RELAY_PORT, &gpio);

    gpio.Pin = GREEN_LED_PIN | YELLOW_LED_PIN;
    HAL_GPIO_Init(GREEN_LED_PORT, &gpio);

    // 4. Кнопки (PB12, PB13): EXTI по обох фронтах, кнопка іде на 3.3 В
    gpio.Pin  = BUTTON1_PIN | BUTTON2_PIN;
    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON1_PORT, &gpio);

    // 5. Переривання EXTI12 і EXTI13 (спільний вектор EXTI15_10)
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Керування виходом
void D_GPIO_Write(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(port, pin, state);
}

// Перемикання виходу
void D_GPIO_Toggle(GPIO_TypeDef *port, uint16_t pin)
{
    HAL_GPIO_TogglePin(port, pin);
}

// Зчитування входу
GPIO_PinState D_GPIO_Read(GPIO_TypeDef *port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

// Реєстрація callback
void D_GPIO_RegisterEXTICallback(D_GPIO_EXTI_Callback_t callback)
{
    exti_callback = callback;
}

// Обробник спільного переривання EXTI10–15
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BUTTON1_PIN);
    HAL_GPIO_EXTI_IRQHandler(BUTTON2_PIN);
}

// Callback HAL після обробки EXTI
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (exti_callback != NULL)
    {
        GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON1_PORT, GPIO_Pin);
        exti_callback(GPIO_Pin, state);
    }
}