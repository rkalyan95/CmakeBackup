#include "pal_gpio.h"
#include "pal_usart.h"
#include "pal_spi.h"
#include "spi.h"
#include "pal_timer.h"
#include "logger.h"
#include "pal_adc.h"
#include <stdio.h>
#include "cli.h"
#include "tle92466.h"
static void cmd_led(uint8_t argc, char *argv[])
{
    (void)argc;
    (void)argv;

    pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

    log_info("LED toggled");
}

static void cmd_adc(uint8_t argc, char *argv[])
{
    (void)argc;
    (void)argv;

    adc_value_t value;

    if(pal_adc_read(BOARD_ADC_1, &value) == PAL_STATUS_OK)
    {
        log_info("ADC = %u", value);
    }
    else
    {
        log_error("ADC Read Failed");
    }
}

void test_blink(void)
{
     pal_gpio_toggle(BOARD_GPIO_LED_STATUS);
     pal_timer_delay_ms(1000);
}

void test_uart_transmit(void)
{
    const uint8_t message[] =
        "\r\nHello from EmbeddedFramework!\r\n";  
     pal_uart_transmit(
            BOARD_UART_DEBUG,
            message,
            sizeof(message) - 1U);
}

void test_spi_tle(void)
{

     pal_gpio_write(BOARD_GPIO_TLE_RST,0);
     pal_timer_delay_ms(200);
     pal_gpio_write(BOARD_GPIO_TLE_RST,1);
     pal_timer_delay_ms(200);
     pal_gpio_write(BOARD_GPIO_TLE_EN,1);
     pal_timer_delay_ms(200);



}

static void led_timer_callback(void *context)
{
    (void)context;
    uint16_t adc_value = 16234;
    pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

   /* const uint8_t message[] = "Timer Interrupt!\r\n";
    pal_uart_transmit(
        BOARD_UART_DEBUG,
        message,
        sizeof(message) - 1);
        */

    logger_log(LOG_LEVEL_INFO,
           "ADC = %u",
           adc_value);
}


void test_timer(void)
{
pal_timer_start(BOARD_TIMER_1);

while (1)
{
    uint32_t count;

    pal_timer_get_counter(BOARD_TIMER_1, &count);

    if (count > 100000)
    {
        pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

        pal_timer_set_counter(BOARD_TIMER_1, 0);
        test_uart_transmit();
    }
}
}

void test_timer_interrupt(void)
{
    /* Configure timer for 1 second */
    pal_timer_stop(BOARD_TIMER_1);

    pal_timer_set_counter(BOARD_TIMER_1, 0);
    pal_timer_set_period(BOARD_TIMER_1, 999999);

    pal_timer_register_callback(
        BOARD_TIMER_1,
        led_timer_callback,
        NULL);

    pal_timer_start_it(BOARD_TIMER_1);

    while (1)
    {
        /* Do other work */
    }
}

void test_adc(void)
{
    adc_value_t value;

    while (1)
    {
        if (pal_adc_read(BOARD_ADC_1, &value) == PAL_STATUS_OK)
        {
            printf("ADC = %u\r\n", value);
        }

        pal_timer_delay_ms(500);
    }
}

void test_cli(void)
{
    cli_init();
cli_register("adc-read",
             1,
             1,
             "adc-read",
             cmd_adc);
#if 0
cli_register("tle-init",
             1,
             1,
             "tle-init",
             cmd_tle_init);

cli_register("tle-read",
             2,
             2,
             "tle-read <register>",
             cmd_tle_read);

cli_register("tle-write",
             3,
             3,
             "tle-write <register> <value>",
             cmd_tle_write);
#endif
cli_register("gpio-toggle",
             1,
             1,
             "gpio-toggle",
             cmd_led);

    while(1)
    {
        uint8_t c;

        if(pal_uart_receive(BOARD_UART_DEBUG,
                            &c,
                            1) == PAL_STATUS_OK)
        {
            cli_receive_char((char)c);
        }

        cli_process();
    }

}

void tle_sm(void)
{
    uint8_t rst_lvl = 1;
    pal_gpio_read(BOARD_GPIO_TLE_RST,&rst_lvl);
    if(rst_lvl!= 0)
    {
        return;
    }

    #if 0
    switch(sm)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    uint32_t cmd;
    uint16_t value;
    uint16_t test_data = 0x5005;  
    
#endif
}

void test_tle_comm(void)
{
    uint32_t cmd;
    uint16_t value;
    uint32_t fb_stat_reg;
    uint32_t fb_voltage1_reg;
    uint32_t fb_voltage2_reg;
    uint16_t test_data = 0x5005;

    /*-------------------- Write Registers --------------------*/

    cmd = tle_write_ch_ctrl(test_data);
    logger_log(LOG_LEVEL_INFO, "WR CH_CTRL      CMD = %08X", cmd);

    cmd = tle_write_glb_cfg(test_data);
    logger_log(LOG_LEVEL_INFO, "WR GLB_CFG      CMD = %08X", cmd);

    cmd = tle_write_glb_diag0(test_data);
    logger_log(LOG_LEVEL_INFO, "WR GLB_DIAG0    CMD = %08X", cmd);

    cmd = tle_write_glb_diag1(test_data);
    logger_log(LOG_LEVEL_INFO, "WR GLB_DIAG1    CMD = %08X", cmd);

    cmd = tle_write_glb_diag2(test_data);
    logger_log(LOG_LEVEL_INFO, "WR GLB_DIAG2    CMD = %08X", cmd);

    cmd = tle_write_vbat_th(test_data);
    logger_log(LOG_LEVEL_INFO, "WR VBAT_TH      CMD = %08X", cmd);

    cmd = tle_write_fb_frz(test_data);
    logger_log(LOG_LEVEL_INFO, "WR FB_FRZ       CMD = %08X", cmd);

    cmd = tle_write_fb_upd(test_data);
    logger_log(LOG_LEVEL_INFO, "WR FB_UPD       CMD = %08X", cmd);

    cmd = tle_write_wd_reload(test_data);
    logger_log(LOG_LEVEL_INFO, "WR WD_RELOAD    CMD = %08X", cmd);

    cmd = tle_write_diag_err_chgr(0, test_data);
    logger_log(LOG_LEVEL_INFO, "WR DIAG_ERR0    CMD = %08X", cmd);

    cmd = tle_write_diag_warn_chgr(0, test_data);
    logger_log(LOG_LEVEL_INFO, "WR DIAG_WARN0   CMD = %08X", cmd);

    cmd = tle_write_fault_mask0(test_data);
    logger_log(LOG_LEVEL_INFO, "WR FAULT_MASK0  CMD = %08X", cmd);

    cmd = tle_write_fault_mask1(test_data);
    logger_log(LOG_LEVEL_INFO, "WR FAULT_MASK1  CMD = %08X", cmd);

    cmd = tle_write_fault_mask2(test_data);
    logger_log(LOG_LEVEL_INFO, "WR FAULT_MASK2  CMD = %08X", cmd);

    cmd = tle_write_clk_div(test_data);
    logger_log(LOG_LEVEL_INFO, "WR CLK_DIV      CMD = %08X", cmd);

    cmd = tle_write_sff_bist(test_data);
    logger_log(LOG_LEVEL_INFO, "WR SFF_BIST     CMD = %08X", cmd);

    /*-------------------- Read Registers --------------------*/


    cmd = tle_read_version(&value);
    logger_log(LOG_LEVEL_INFO, "VERSION   CMD=%08X RAW=%08X DATA=%04X",
               cmd, version_value.raw, value);

    cmd = tle_read_ch_ctrl(&value);
    logger_log(LOG_LEVEL_INFO, "CH_CTRL   CMD=%08X RAW=%08X DATA=%04X",
               cmd, ch_ctrl_value.raw, value);

    cmd = tle_read_glb_cfg(&value);
    logger_log(LOG_LEVEL_INFO, "GLB_CFG   CMD=%08X RAW=%08X DATA=%04X",
               cmd, global_config_value.raw, value);

    cmd = tle_read_glb_diag0(&value);
    logger_log(LOG_LEVEL_INFO, "GLB_DG0   CMD=%08X RAW=%08X DATA=%04X",
               cmd, global_diag0_value.raw, value);

    cmd = tle_read_glb_diag1(&value);
    logger_log(LOG_LEVEL_INFO, "GLB_DG1   CMD=%08X RAW=%08X DATA=%04X",
               cmd, global_diag1_value.raw, value);

    cmd = tle_read_glb_diag2(&value);
    logger_log(LOG_LEVEL_INFO, "GLB_DG2   CMD=%08X RAW=%08X DATA=%04X",
               cmd, global_diag2_value.raw, value);

    cmd = tle_read_vbat_th(&value);
    logger_log(LOG_LEVEL_INFO, "VBAT_TH   CMD=%08X RAW=%08X DATA=%04X",
               cmd, vbat_th_value.raw, value);

    cmd = tle_read_fb_frz(&value);
    logger_log(LOG_LEVEL_INFO, "FB_FRZ    CMD=%08X RAW=%08X DATA=%04X",
               cmd, fb_frz_value.raw, value);

    cmd = tle_read_fb_upd(&value);
    logger_log(LOG_LEVEL_INFO, "FB_UPD    CMD=%08X RAW=%08X DATA=%04X",
               cmd, fb_upd_value.raw, value);

    cmd = tle_read_wd_reload(&value);
    logger_log(LOG_LEVEL_INFO, "WD_RELD   CMD=%08X RAW=%08X DATA=%04X",
               cmd, wd_reload_value.raw, value);

    cmd = tle_read_diag_err_chgr(0, &value);
    logger_log(LOG_LEVEL_INFO, "ERR_CH0   CMD=%08X RAW=%08X DATA=%04X",
               cmd, diag_err_chgr_value.raw, value);

    cmd = tle_read_diag_warn_chgr(0, &value);
    logger_log(LOG_LEVEL_INFO, "WARN_CH0  CMD=%08X RAW=%08X DATA=%04X",
               cmd, diag_warn_chgr_value.raw, value);

    cmd = tle_read_fault_mask0(&value);
    logger_log(LOG_LEVEL_INFO, "FLT_MSK0  CMD=%08X RAW=%08X DATA=%04X",
               cmd, fault_mask0_value.raw, value);

    cmd = tle_read_fault_mask1(&value);
    logger_log(LOG_LEVEL_INFO, "FLT_MSK1  CMD=%08X RAW=%08X DATA=%04X",
               cmd, fault_mask1_value.raw, value);

    cmd = tle_read_fault_mask2(&value);
    logger_log(LOG_LEVEL_INFO, "FLT_MSK2  CMD=%08X RAW=%08X DATA=%04X",
               cmd, fault_mask2_value.raw, value);

    cmd = tle_read_clk_div(&value);
    logger_log(LOG_LEVEL_INFO, "CLK_DIV   CMD=%08X RAW=%08X DATA=%04X",
               cmd, clk_div_value.raw, value);

    cmd = tle_read_sff_bist(&value);
    logger_log(LOG_LEVEL_INFO, "SFF_BIST  CMD=%08X RAW=%08X DATA=%04X",
               cmd, sff_bist_value.raw, value);

    cmd = tle_read_pin_stat(&value);
    logger_log(LOG_LEVEL_INFO, "PIN_STAT  CMD=%08X RAW=%08X DATA=%04X",
               cmd, pin_stat_value.raw, value);

    cmd = tle_read_fb_stat(&fb_stat_reg);
    logger_log(LOG_LEVEL_INFO, "FB_STAT      CMD=%08X RAW=%08X DATA=%04X",
            cmd, fb_stat_value.raw, fb_stat_reg);

    cmd = tle_read_fb_voltage1(&fb_voltage1_reg);
    logger_log(LOG_LEVEL_INFO, "FB_VOLTAGE1  CMD=%08X RAW=%08X DATA=%04X",
            cmd, fb_voltage1_value.raw, fb_voltage1_reg);

    cmd = tle_read_fb_voltage2(&fb_voltage2_reg);
    logger_log(LOG_LEVEL_INFO, "FB_VOLTAGE2  CMD=%08X RAW=%08X DATA=%04X",
            cmd, fb_voltage2_value.raw, fb_voltage2_reg);

/*-------------------- Read Channel Registers --------------------*/

/*-------------------- Read Channel Registers --------------------*/

uint16_t channel_value;
uint32_t channel_fb_value;
    cmd = tle_write_integrator_limit(0, 0x43FE);
    logger_log(LOG_LEVEL_INFO, "WR INT_LIMIT    CMD = %08X", cmd);
for (uint8_t channel = 0U; channel < 6U; channel++)
{
    uint32_t base = CHANNEL_BASE_ADDR(channel);

    /*-------------------- 16-bit Registers --------------------*/

    cmd = tle_read_setpoint(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_SETPOINT       CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               set_point_value[channel].raw,
               channel_value);

    cmd = tle_read_ctrl(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_CTRL           CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               ctrl_value[channel].raw,
               channel_value);

    cmd = tle_read_period(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_PERIOD         CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               period_value[channel].raw,
               channel_value);

    cmd = tle_read_integrator_limit(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_INT_LIMIT      CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               integrator_limit_value[channel].raw,
               channel_value);

    cmd = tle_read_dither_clk_div(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_DITHER_CLK_DIV CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               dither_clk_div_value[channel].raw,
               channel_value);

    cmd = tle_read_dither_step(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_DITHER_STEP    CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               dither_step_value[channel].raw,
               channel_value);

    cmd = tle_read_dither_ctrl(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_DITHER_CTRL    CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               dither_ctrl_value[channel].raw,
               channel_value);

    cmd = tle_read_ch_config(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_CH_CONFIG      CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               ch_config_value[channel].raw,
               channel_value);

    cmd = tle_read_mode(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_MODE           CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               mode_value[channel].raw,
               channel_value);

    cmd = tle_read_ton(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_TON            CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               ton_value[channel].raw,
               channel_value);

    cmd = tle_read_ctrl_int_thresh(base, channel, &channel_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_CTRL_INT_THRESH CMD=%08X RAW=%08X DATA=%04X",
               channel,
               cmd,
               ctrl_int_thresh_value[channel].raw,
               channel_value);


    /*-------------------- 22-bit Feedback Registers --------------------*/

    cmd = tle_read_fb_dc(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_DC          CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_dc_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_vbat(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_VBAT        CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_vbat_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_i_avg(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_I_AVG       CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_i_avg_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_imin_imax(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_IMIN_IMAX   CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_imin_imax_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_i_avg_s16(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_I_AVG_S16   CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_i_avg_s16_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_int_thresh(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_INT_THRESH  CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_int_thresh_value[channel].raw,
               channel_fb_value);

    cmd = tle_read_fb_period_min_max(base, channel, &channel_fb_value);
    logger_log(LOG_LEVEL_INFO,
               "CH%u_FB_PERIOD_MINMX CMD=%08X RAW=%08X DATA=%06X",
               channel,
               cmd,
               fb_period_min_max_value[channel].raw,
               channel_fb_value);
}
}




int main(void)
{
     
    uint16_t tleversion = 0;
    uint16_t glb_cfg = 0;
    uint32_t cmd = 0;
    uint16_t data_reg = 0x5005;
     board_init();
     test_spi_tle();
     //test_timer();
     //test_timer_interrupt();
     //test_cli();
     const uint8_t message[] =
        "\r\n --New Data Set--\r\n";
   cli_init();
    while (1)
    {
        
        //test_blink();
        //test_timer();
        test_tle_comm();

        pal_timer_delay_ms(1000);
    }
    
}

