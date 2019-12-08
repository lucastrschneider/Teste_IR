/**
 * @file ir_receiver_adapter.c
 *
 * @brief IR receiver adapter reading and initialization.
 *
 * @author Lucas Schneider <lucas.schneider@thunderatz.org>
 *
 * @date 12/2019
 */

#include "ir_receiver_adapter.h"

#include "gpio.h"

/*****************************************
 * Private Variables
 *****************************************/

static tsop48_handler_t m_dev = {
    .gpio = {
        .port = GPIOA,
        .pin = GPIO_PIN_8,
    },
    .htim = &htim6,
    .data = 0,
    .aux_data = 0,
    .bit_counter = 0,
    .data_ready = false,
};

static ir_receiver_states_t m_state;

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

void ir_receiver_adapter_init(void) {
    MX_TIM6_Init();
    m_state = IR_RECEIVER_ADAPTER_IDLE;
    tsop48_handler_init(&m_dev);
}

ir_receiver_status_t ir_receiver_process(void) {
    if (m_state == IR_RECEIVER_ADAPTER_ERRO) {
        tsop48_handler_reset_data(&m_dev);
        m_state = IR_RECEIVER_ADAPTER_IDLE;
        return IR_RECEIVER_ADAPTER_READING_ERRO;
    }

    if (m_dev.data_ready) {
        return IR_RECEIVER_ADAPTER_DATA_READY;
    }

    return IR_RECEIVER_ADAPTER_NO_UPDATE;
}

uint16_t ir_receiver_get_data(void) {
    tsop48_handler_reset_flag(&m_dev);

    return m_dev.data;
}

/*****************************************
 * Callbacks
 *****************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == m_dev.gpio.pin) {
        switch (m_state) {
            case IR_RECEIVER_ADAPTER_IDLE:

                // Set timer 2500us
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                __HAL_TIM_SET_AUTORELOAD(m_dev.htim, 2500);
                __HAL_TIM_SET_COUNTER(m_dev.htim, 0);
                HAL_TIM_Base_Start_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_START;
                break;

            case IR_RECEIVER_ADAPTER_START:
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_ERRO;
                break;

            case IR_RECEIVER_ADAPTER_WAIT_NEXT_BIT:

                // Set timer 900us
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                __HAL_TIM_SET_AUTORELOAD(m_dev.htim, 900);
                __HAL_TIM_SET_COUNTER(m_dev.htim, 0);
                HAL_TIM_Base_Start_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_WAIT_READING;
                break;

            case IR_RECEIVER_ADAPTER_WAIT_READING:
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_ERRO;
                break;

            case IR_RECEIVER_ADAPTER_CHECK_INTEGRITY:
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_ERRO;
                break;

            case IR_RECEIVER_ADAPTER_ERRO:
                break;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim == m_dev.htim) {
        switch (m_state) {
            case IR_RECEIVER_ADAPTER_IDLE:
                break;

            case IR_RECEIVER_ADAPTER_START:
                tsop48_handler_reset_data(&m_dev);

                HAL_TIM_Base_Stop_IT(m_dev.htim);
                __HAL_TIM_SET_AUTORELOAD(m_dev.htim, 1200);
                __HAL_TIM_SET_COUNTER(m_dev.htim, 0);
                HAL_TIM_Base_Start_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_WAIT_NEXT_BIT;
                break;

            case IR_RECEIVER_ADAPTER_WAIT_NEXT_BIT:
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_ERRO;
                break;

            case IR_RECEIVER_ADAPTER_WAIT_READING:

                if (tsop48_handler_read_bit(&m_dev) >= 11) {
                    HAL_TIM_Base_Stop_IT(m_dev.htim);
                    __HAL_TIM_SET_AUTORELOAD(m_dev.htim, 10000);
                    __HAL_TIM_SET_COUNTER(m_dev.htim, 0);
                    HAL_TIM_Base_Start_IT(m_dev.htim);
                    m_state = IR_RECEIVER_ADAPTER_CHECK_INTEGRITY;
                } else {
                    HAL_TIM_Base_Stop_IT(m_dev.htim);
                    __HAL_TIM_SET_AUTORELOAD(m_dev.htim, 1200);
                    __HAL_TIM_SET_COUNTER(m_dev.htim, 0);
                    HAL_TIM_Base_Start_IT(m_dev.htim);
                    m_state = IR_RECEIVER_ADAPTER_WAIT_NEXT_BIT;
                }

                break;

            case IR_RECEIVER_ADAPTER_CHECK_INTEGRITY:
                tsop48_handler_set_data(&m_dev);
                HAL_TIM_Base_Stop_IT(m_dev.htim);
                m_state = IR_RECEIVER_ADAPTER_IDLE;
                break;

            case IR_RECEIVER_ADAPTER_ERRO:
                break;
        }
    }
}
