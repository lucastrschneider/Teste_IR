/**
 * @file mcu_adapter.c
 *
 * @brief MCU related
 */

#include <stdint.h>

#include "mcu_adapter.h"

#include "gpio.h"
#include "main.h"
#include "stm32f3xx_hal.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/

void mcu_adapter_init(void) {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
}

void mcu_adapter_sleep(uint32_t ms) {
    HAL_Delay(ms);
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
