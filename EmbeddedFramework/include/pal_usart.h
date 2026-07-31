#ifndef PAL_UART_H
#define PAL_UART_H


#include "board.h"
#include "pal_status.h"
#include "pal_assert.h"

typedef size_t buffer_len_t;

#define PAL_ASSERT_UART(uart) \
    PAL_ASSERT((uart) < BOARD_UART_COUNT, PAL_STATUS_INVALID_PARAM)

pal_status_t pal_uart_transmit(board_uart_t uart,
                               const uint8_t *tx_buffer,
                               buffer_len_t length);

pal_status_t pal_uart_receive(board_uart_t uart,
                              uint8_t *rx_buffer,
                              buffer_len_t length);

#endif /* PAL_UART_H */