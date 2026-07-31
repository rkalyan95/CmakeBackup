#include "board.h"
#include "stm32l4xx_hal.h"
#include "gpio.h"
#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

const board_gpio_map_t board_gpio_map[BOARD_GPIO_COUNT] =
{
    [BOARD_GPIO_LED_STATUS] =
    {
        .port = GPIOB,
        .pin  = GPIO_PIN_13
    },

    [BOARD_GPIO_BUTTON_USER] =
    {
        .port = GPIOC,
        .pin  = GPIO_PIN_13
    }
};

void board_init(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
}