#ifndef BOARD_H
#define BOARD_H

#include "stm32l4xx_hal.h"

typedef enum
{
    BOARD_GPIO_LED_STATUS,
    BOARD_GPIO_BUTTON_USER,

    BOARD_GPIO_COUNT

} board_gpio_t;

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;

} board_gpio_map_t;

extern const board_gpio_map_t board_gpio_map[];

void board_init(void);

#endif