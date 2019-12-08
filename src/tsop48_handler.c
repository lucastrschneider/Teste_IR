/**
 * @file tsop48_handler.c
 *
 * @brief RGB handler control and initialization.
 *
 * @author Lucas Schneider <lucas.schneider@thunderatz.org>
 *
 * @date 12/2019
 */

#include "tsop48_handler.h"

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

void tsop48_handler_init(tsop48_handler_t* p_tsop48) {
    (void) p_tsop48;
}

void tsop48_handler_reset_data(tsop48_handler_t* p_tsop48) {
    p_tsop48->aux_data = 0;
    p_tsop48->bit_counter = 0;
}

void tsop48_handler_reset_flag(tsop48_handler_t* p_tsop48) {
    p_tsop48->data_ready = false;
}

uint8_t tsop48_handler_read_bit(tsop48_handler_t* p_tsop48) {
    if (HAL_GPIO_ReadPin(p_tsop48->gpio.port, p_tsop48->gpio.pin) == GPIO_PIN_SET) {
        p_tsop48->aux_data |= 1 << p_tsop48->bit_counter;
    }

    p_tsop48->bit_counter++;

    return p_tsop48->bit_counter;
}

void tsop48_handler_set_data(tsop48_handler_t* p_tsop48) {
    p_tsop48->data = p_tsop48->aux_data;
    p_tsop48->data_ready = true;
}
