#include "gpio.h"
#include "board.h"
#include "pal_gpio.h"
#include "pal_usart.h"
int main(void)
{
    board_init();
    const uint8_t message[] =
        "Hello from EmbeddedFramework!\r\n";    
    while (1)
    {
        pal_uart_transmit(
            BOARD_UART_DEBUG,
            message,
            sizeof(message) - 1U);
        pal_gpio_toggle(BOARD_GPIO_LED_STATUS);
        pal_blocking_delay(1000);
    }
}