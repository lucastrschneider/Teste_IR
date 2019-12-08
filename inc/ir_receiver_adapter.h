/**
 * @file ir_receiver_adapter.h
 *
 * @brief IR receiver adapter reading and initialization.
 *
 * @author Lucas Schneider <lucas.schneider@thunderatz.org>
 *
 * @date 12/2019
 */

#ifndef __IR_RECEIVER_ADAPTER_H__
#define __IR_RECEIVER_ADAPTER_H__

#include <stdint.h>
#include <stdbool.h>

#include "tsop48_handler.h"

/*****************************************
 * Public Types
 *****************************************/

/**
 * @brief Reciever states type.
 */
typedef enum ir_receiver_states {
    IR_RECEIVER_ADAPTER_IDLE = 0,
    IR_RECEIVER_ADAPTER_START,
    IR_RECEIVER_ADAPTER_WAIT_NEXT_BIT,
    IR_RECEIVER_ADAPTER_WAIT_READING,
    IR_RECEIVER_ADAPTER_CHECK_INTEGRITY,
    IR_RECEIVER_ADAPTER_ERRO,
} ir_receiver_states_t;

/**
 * @brief Reciever status type.
 */
typedef enum ir_receiver_status {
    IR_RECEIVER_ADAPTER_DATA_READY = 0,
    IR_RECEIVER_ADAPTER_NO_UPDATE,
    IR_RECEIVER_ADAPTER_READING_ERRO,
} ir_receiver_status_t;

/*****************************************
 * Public Functions Prototypes
 *****************************************/

/**
 * @brief Initializes the IR receiver.
 */
void ir_receiver_adapter_init(void);

ir_receiver_status_t ir_receiver_process(void);

uint16_t ir_receiver_get_data(void);

#endif // __IR_RECEIVER_ADAPTER_H__
