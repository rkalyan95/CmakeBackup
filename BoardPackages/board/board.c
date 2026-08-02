#include "board.h"
//#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "spi.h"
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

    [BOARD_GPIO_SPI_CS] =
    {
        .port = GPIOB,
        .pin  = GPIO_PIN_6
    },
    [BOARD_GPIO_TLE_RST] =
    {
        .port = GPIOA,
        .pin  = GPIO_PIN_12
    },
    [BOARD_GPIO_TLE_EN] =
    {
        .port = GPIOC,
        .pin  = GPIO_PIN_7
    },
};

const board_uart_map_t board_uart_map[BOARD_UART_COUNT] =
{
    [BOARD_UART_DEBUG] =
    {
        .handle = &huart1
    }
};

const board_spi_map_t board_spi_map[BOARD_SPI_COUNT] =
{
    [BOARD_SPI_1] =
    {
        .handle = &hspi2
    }
};

void board_init(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_USART1_UART_Init();

    MX_SPI2_Init();
}