#include "wolverin_claw.h"
#include "pal_gpio.h"
#include "logger.h"



#define ACT_CHANNEL             0U

/* Replace with the actual value from your TLE92466 configuration */
#define TLE_MISSION_MODE        1U

/* Replace with the actual 16-bit setpoint encoding for 500 mA */
#define TLE_SETPOINT_500MA      0x0015U

/* Replace with the actual CTRL value required to enable CH1 */
#define TLE_CH1_ENABLE          0x0000U


actuator_status_t actuator_init_and_start(void)
{
    uint32_t base = CHANNEL_BASE_ADDR(0);
    uint8_t pin_lvl = 0;
    uint16_t glb_regs = 0;
    uint32_t command = 0;
    pal_gpio_read(BOARD_GPIO_TLE_RST,&pin_lvl);
    if(!pin_lvl)
    {
        log_info("%s","RESET PIN NOT OKAY");
        return ACT_RESN_NOT_OK;
    }
    pal_gpio_read(BOARD_GPIO_TLE_EN,&pin_lvl);
    if(!pin_lvl)
    {
        log_info("%s","ENABLE PIN NOT OKAY");
        return ACT_EN_NOT_OK;
    }

    update_fault_structure();
    update_fb_status_data();

    if(fb_status_data.init_done_status == 0)
    {
        log_info("%s","INIT FB STATUS NOT OKAY");
        return ACT_INIT_NOT_DONE;
    }

    if(tle_central_faults.vbat_under_volt_flt)
    {
        log_info("%s","CHECK VBAT of Device");
        return ACT_FB_STAT_FAULT;
    }

    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL_MODE %04X",glb_regs);
    glb_regs |= (CH_CTRL_OP_MODE_msk);

    log_info("WRITE->CH_CTRL_MODE %04X",glb_regs);

    command = tle_write_ch_ctrl(glb_regs);

    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL_MODE %04X",glb_regs);

    if (!(glb_regs & CH_CTRL_OP_MODE_msk))
    {
        log_info("%s","FAILED TO UPDATE TO MISSION MODE");
        return ACT_CONFIG_FAILED;
    }
    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL %04X",glb_regs);
    glb_regs |= CH_CTRL_EN_CH0_msk;
    log_info("WRITE->CH_CTRL %04X",glb_regs);
    command = tle_write_ch_ctrl(glb_regs);
    
    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL %04X",glb_regs);
    if(!(glb_regs & CH_CTRL_EN_CH0_msk))
    {
        log_info("%s","FAILED TO ENABLE THE CHANNEL");
        return ACT_ENABLE_FAILED;
    }

    command = tle_write_setpoint(ACT_CHANNEL, TLE_SETPOINT_500MA);
    log_info("WRITE->CH_SETPOINT %04X",TLE_SETPOINT_500MA);

    command = tle_read_setpoint(base ,ACT_CHANNEL, &glb_regs);

    log_info("READ->CH_SETPOINT %04X",glb_regs);

    if (!(glb_regs & SETPOINT_TARGET_msk))
    {
        log_info("%s","FAILED TO CONFIGURE SET POINT");
        return ACT_SETPOINT_FAILED;
    }

     log_info("%s","SUCCESSFULLY CONFIGURED");
    return ACT_OK;
}