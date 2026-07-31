#include "gpio.h"
#include "board.h"
#include "stm32l4xx_hal.h"
#include "pal_gpio.h"
extern const board_gpio_map_t board_gpio_map[];

void pal_gpio_toggle(board_gpio_t gpio)
{
    HAL_GPIO_TogglePin(
        board_gpio_map[gpio].port,
        board_gpio_map[gpio].pin);
}

void pal_gpio_write(board_gpio_t gpio,
                    pal_gpio_level_t level)
{
    HAL_GPIO_WritePin(
        board_gpio_map[gpio].port,
        board_gpio_map[gpio].pin,
        (level == PAL_GPIO_HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

pal_gpio_level_t pal_gpio_read(board_gpio_t gpio)
{
    return (HAL_GPIO_ReadPin(
                board_gpio_map[gpio].port,
                board_gpio_map[gpio].pin) == GPIO_PIN_SET)
            ? PAL_GPIO_HIGH
            : PAL_GPIO_LOW;
}

void pal_blocking_delay(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);
}