#ifndef BOARD_H
#define BOARD_H

#include "stm32l4xx_hal.h"

typedef enum
{
    BOARD_GPIO_LED_STATUS,
    BOARD_GPIO_SPI_CS,
    BOARD_GPIO_TLE_RST,
    BOARD_GPIO_TLE_EN,
    BOARD_GPIO_COUNT

} board_gpio_t;

typedef enum
{
    BOARD_UART_DEBUG = 0,
    BOARD_UART_GPS,
    BOARD_UART_BLE,

    BOARD_UART_COUNT

} board_uart_t;

typedef enum
{
    BOARD_SPI_1 = 0,
    BOARD_SPI_COUNT
} board_spi_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

} board_gpio_map_t;

extern const board_gpio_map_t board_gpio_map[];

typedef struct
{
    UART_HandleTypeDef *handle;

} board_uart_map_t;

extern const board_uart_map_t board_uart_map[];


typedef struct
{
    SPI_HandleTypeDef *handle;
} board_spi_map_t;

extern const board_spi_map_t board_spi_map[BOARD_SPI_COUNT];

void board_init(void);

#endif