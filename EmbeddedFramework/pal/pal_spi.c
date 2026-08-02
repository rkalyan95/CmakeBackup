#include "pal_spi.h"

pal_status_t pal_spi_transmit(board_spi_t spi,
                              const uint8_t *buffer,
                              buffer_len_t length)
{
    PAL_ASSERT_SPI(spi);
    PAL_ASSERT_NULL(buffer);
    PAL_ASSERT(length > 0U, PAL_STATUS_INVALID_PARAM);

    const board_spi_map_t *spi_map = &board_spi_map[spi];

    HAL_StatusTypeDef status =
        HAL_SPI_Transmit(
            spi_map->handle,
            (uint8_t *)buffer,
            length,
            HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return PAL_STATUS_ERROR;
    }

    return PAL_STATUS_OK;
}

pal_status_t pal_spi_receive(board_spi_t spi,
                             uint8_t *buffer,
                             buffer_len_t length)
{
    PAL_ASSERT_SPI(spi);
    PAL_ASSERT_NULL(buffer);
    PAL_ASSERT(length > 0U, PAL_STATUS_INVALID_PARAM);

    const board_spi_map_t *spi_map = &board_spi_map[spi];

    HAL_StatusTypeDef status =
        HAL_SPI_Receive(
            spi_map->handle,
            buffer,
            length,
            HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return PAL_STATUS_ERROR;
    }

    return PAL_STATUS_OK;
}

pal_status_t pal_spi_transfer(board_spi_t spi,
                              const uint8_t *tx_buffer,
                              uint8_t *rx_buffer,
                              buffer_len_t length)
{
    PAL_ASSERT_SPI(spi);
    PAL_ASSERT_NULL(tx_buffer);
    PAL_ASSERT_NULL(rx_buffer);
    PAL_ASSERT(length > 0U, PAL_STATUS_INVALID_PARAM);

    const board_spi_map_t *spi_map = &board_spi_map[spi];

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(
            spi_map->handle,
            (uint8_t *)tx_buffer,
            rx_buffer,
            (uint16_t)length,
            HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return PAL_STATUS_ERROR;
    }

    return PAL_STATUS_OK;
}