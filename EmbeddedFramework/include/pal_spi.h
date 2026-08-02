#ifndef PAL_SPI_H
#define PAL_SPI_H

#include "board.h"

#include "pal_status.h"
#include "pal_assert.h"

typedef size_t buffer_len_t;

#define PAL_ASSERT_SPI(spi) \
    PAL_ASSERT((spi) < BOARD_SPI_COUNT, PAL_STATUS_INVALID_PARAM)

pal_status_t pal_spi_transmit(board_spi_t spi,
                              const uint8_t *buffer,
                              buffer_len_t length);

pal_status_t pal_spi_receive(board_spi_t spi,
                             uint8_t *buffer,
                             buffer_len_t length);

pal_status_t pal_spi_transfer(board_spi_t spi,
                              const uint8_t *tx_buffer,
                              uint8_t *rx_buffer,
                              buffer_len_t length);

#endif