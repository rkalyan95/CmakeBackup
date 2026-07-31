#ifndef BOARD_H
#define BOARD_H

#include "stm32l4xx_hal.h"

typedef enum
{
    BOARD_GPIO_LED_STATUS,
    BOARD_GPIO_BUTTON_USER,

    BOARD_GPIO_COUNT

} board_gpio_t;

typedef enum
{
    BOARD_UART_DEBUG = 0,
    BOARD_UART_GPS,
    BOARD_UART_BLE,

    BOARD_UART_COUNT

} board_uart_t;


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


void board_init(void);

#endif