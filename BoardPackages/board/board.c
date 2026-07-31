#include "board.h"
//#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "usart.h"
#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
extern UART_HandleTypeDef huart1;
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

const board_uart_map_t board_uart_map[BOARD_UART_COUNT] =
{
    [BOARD_UART_DEBUG] =
    {
        .handle = &huart1
    }
};

void board_init(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_USART1_UART_Init();
}