/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu_adapter.h"
#include "ir_receiver_adapter.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 1500

/*****************************************
 * Main Function
 *****************************************/

uint16_t data;

int main(void) {
    mcu_adapter_init();
    ir_receiver_adapter_init();

    for (;;) {
        if (ir_receiver_process() == IR_RECEIVER_ADAPTER_DATA_READY) {
            data = ir_receiver_get_data();
        }
    }
}
