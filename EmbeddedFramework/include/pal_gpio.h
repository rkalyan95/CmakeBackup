#ifndef PAL_GPIO_H
#define PAL_GPIO_H

#include "board.h"

typedef enum
{
    PAL_GPIO_LOW,
    PAL_GPIO_HIGH

} pal_gpio_level_t;

void pal_gpio_write(board_gpio_t gpio,
                    pal_gpio_level_t level);

void pal_gpio_toggle(board_gpio_t gpio);

pal_gpio_level_t pal_gpio_read(board_gpio_t gpio);
void pal_blocking_delay(uint32_t milliseconds);
#endif