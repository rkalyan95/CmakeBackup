#include "pal_usart.h"
#include "pal_assert.h"

pal_status_t pal_uart_transmit(board_uart_t uart,
                               const uint8_t *buffer,
                               buffer_len_t length)
{
    PAL_ASSERT_UART(uart);
    PAL_ASSERT_NULL(buffer);
    PAL_ASSERT(length > 0U, PAL_STATUS_INVALID_PARAM);

    const board_uart_map_t *uart_map = &board_uart_map[uart];

    /* STM32 HAL API is not const-correct. */
    HAL_StatusTypeDef status = HAL_UART_Transmit(
        uart_map->handle,
        (uint8_t *)buffer,
        length,
        HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return PAL_STATUS_ERROR;
    }

    return PAL_STATUS_OK;
}

pal_status_t pal_uart_receive(board_uart_t uart,
                              uint8_t *buffer,
                              buffer_len_t length)
{
    PAL_ASSERT_UART(uart);
    PAL_ASSERT_NULL(buffer);
    PAL_ASSERT(length > 0U, PAL_STATUS_INVALID_PARAM);

    const board_uart_map_t *uart_map = &board_uart_map[uart];

    HAL_StatusTypeDef status = HAL_UART_Receive(
        uart_map->handle,
        buffer,
        length,
        HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return PAL_STATUS_ERROR;
    }

    return PAL_STATUS_OK;
}