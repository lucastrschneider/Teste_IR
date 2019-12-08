/**
 * @file tsop48_handler.c
 *
 * @brief RGB handler control and initialization.
 *
 * @author Lucas Schneider <lucas.schneider@thunderatz.org>
 *
 * @date 12/2019
 */

#ifndef __TSOP48_HANDLER_H__
#define __TSOP48_HANDLER_H__

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "tim.h"

/*****************************************
 * Public Types
 *****************************************/

/**
 * @brief GPIO type.
 */
typedef struct __attribute__((packed)) gpio {
    GPIO_TypeDef* port;
    uint16_t      pin;
} gpio_t;

/**
 * @brief TSOP48 type.
 */
typedef struct __attribute__((packed)) tsop48_handler {
    gpio_t             gpio;
    TIM_HandleTypeDef* htim;

    uint16_t           data;
    uint16_t           aux_data;
    uint8_t            bit_counter;
    bool               data_ready;
} tsop48_handler_t;

/*****************************************
 * Public Functions Prototypes
 *****************************************/

/**
 * @brief Initializes the TSOP48 handler.
 *
 * @param p_tsop48 Pointer to TSOP48 handler.
 */
void tsop48_handler_init(tsop48_handler_t* p_tsop48);

void tsop48_handler_reset_data(tsop48_handler_t* p_tsop48);

void tsop48_handler_reset_flag(tsop48_handler_t* p_tsop48);

uint8_t tsop48_handler_read_bit(tsop48_handler_t* p_tsop48);

void tsop48_handler_set_data(tsop48_handler_t* p_tsop48);

#endif // __TSOP48_HANDLER_H__
