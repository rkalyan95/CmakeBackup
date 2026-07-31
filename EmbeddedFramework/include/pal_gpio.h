#ifndef PAL_GPIO_H
#define PAL_GPIO_H

#include "board.h"
#include "pal_status.h"

#define PAL_ASSERT_NULL(ptr) \
    PAL_ASSERT((ptr) != NULL, PAL_STATUS_INVALID_PARAM)

#define PAL_ASSERT_GPIO(gpio) \
    PAL_ASSERT((gpio) < BOARD_GPIO_COUNT, PAL_STATUS_INVALID_PARAM)

typedef enum
{
    PAL_GPIO_LOW,
    PAL_GPIO_HIGH

} pal_gpio_level_t;

pal_status_t pal_gpio_write(board_gpio_t gpio,
                    pal_gpio_level_t level);

pal_status_t pal_gpio_toggle(board_gpio_t gpio);

pal_status_t pal_gpio_read(board_gpio_t gpio, pal_gpio_level_t *level);
pal_status_t pal_blocking_delay(uint32_t milliseconds);


#endif