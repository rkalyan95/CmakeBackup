#include "wolverin_claw.h"
#include "pal_gpio.h"

void wolv_sm(void)
{
    uint8_t lvl_resn = 0;
    uint32_t cmd_wolv = 0;
    uint16_t resp_wolv = 0;
    pal_gpio_read(BOARD_GPIO_TLE_RST,lvl_resn);
    if(!lvl_resn)
    {
        return;
    }

    update_fb_status_data();


}