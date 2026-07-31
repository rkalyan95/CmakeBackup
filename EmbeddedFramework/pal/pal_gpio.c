#include "gpio.h"
#include "board.h"
#include "stm32l4xx_hal.h"
#include "pal_gpio.h"
#include "pal_assert.h"

pal_status_t pal_gpio_toggle(board_gpio_t gpio)
{
    PAL_ASSERT_GPIO(gpio);
    const board_gpio_map_t *gpio_map = &board_gpio_map[gpio];
    HAL_GPIO_TogglePin(
        gpio_map->port,
        gpio_map->pin);
    
    return PAL_STATUS_OK;
}

pal_status_t pal_gpio_write(board_gpio_t gpio,
                    pal_gpio_level_t level)
{
    PAL_ASSERT_GPIO(gpio);
    const board_gpio_map_t *gpio_map = &board_gpio_map[gpio];

    HAL_GPIO_WritePin(
        gpio_map->port,
        gpio_map->pin,
        (level == PAL_GPIO_HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return PAL_STATUS_OK;
}

pal_status_t pal_gpio_read(board_gpio_t gpio , pal_gpio_level_t *level)
{
    PAL_ASSERT_NULL(level);
    PAL_ASSERT_GPIO(gpio);
    const board_gpio_map_t *gpio_map = &board_gpio_map[gpio];
    *level =  (HAL_GPIO_ReadPin(
                gpio_map->port,
                gpio_map->pin) == GPIO_PIN_SET)
            ? PAL_GPIO_HIGH
            : PAL_GPIO_LOW;

    return PAL_STATUS_OK;    
    
}

pal_status_t pal_blocking_delay(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);
    return PAL_STATUS_OK;
}