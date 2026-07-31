#include "gpio.h"
#include "board.h"
#include "pal_gpio.h"

int main(void)
{
    board_init();

    while (1)
    {
        pal_gpio_toggle(BOARD_GPIO_LED_STATUS);
        pal_blocking_delay(500);
    }
}