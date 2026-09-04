#include "wolverin_claw.h"
#include "pal_gpio.h"
#include "logger.h"



#define ACT_CHANNEL             0U

/* Replace with the actual value from your TLE92466 configuration */
#define TLE_MISSION_MODE        1U

/* Replace with the actual 16-bit setpoint encoding for 500 mA */
#define TLE_SETPOINT_500MA      0x5000U
#define TLE_SETPOINT_HOLDMA     0x0000U

/* Replace with the actual CTRL value required to enable CH1 */
#define TLE_CH1_ENABLE          0x0000U

#define VIO_VOLT_SEL3V3(x)    (x &= ~(GLOBAL_CONFIG_VIO_SEL_msk))
#define VIO_VOLT_SEL5V0(x)    (x |= (GLOBAL_CONFIG_VIO_SEL_msk))

void start_check_bist(void)
{
    uint16_t glb_regs = 0;
    tle_write_sff_bist(0x0001U);
    do
    {
        tle_read_sff_bist(&glb_regs);

    }while(!((glb_regs & SFF_BIST_SMU_SLF_TST_DONE_msk) >> SFF_BIST_SMU_SLF_TST_DONE_pos));

    if((glb_regs & SFF_BIST_SMU_SLF_TST_FAIL_msk) >> SFF_BIST_SMU_SLF_TST_FAIL_pos)
    {
        log_info("%s","BIST FAILED");
    }
    else
    {
        log_info("%s","BIST PASSED");
    }

    if((glb_regs & SFF_BIST_SMU_SLF_TST_UERR_msk) >> SFF_BIST_SMU_SLF_TST_UERR_pos)
    {
        log_info("%s","BIST UNCORRECTABLE ERROR TESTED");
    }
    if((glb_regs & SFF_BIST_SMU_SLF_TST_CERR_msk) >> SFF_BIST_SMU_SLF_TST_CERR_pos)
    {
        log_info("%s","BIST CORRECTABLE ERROR  TESTED");
    }

}

actuator_status_t actuator_hold(void)
{
    uint32_t base = CHANNEL_BASE_ADDR(0);
    uint16_t glb_regs = 0;
    uint32_t command = 0;
    command = tle_write_setpoint(ACT_CHANNEL, TLE_SETPOINT_HOLDMA);
    log_info("WRITE->CH_SETPOINT %04X",TLE_SETPOINT_HOLDMA);

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

    tle_read_version(&glb_regs);
    if(glb_regs != 0xC1FE)
    {
        log_info("%s","TLE VERSION NOT OKAY");
        return ACT_INIT_NOT_DONE;
    }

    //start_check_bist();
    command = tle_read_glb_cfg(&glb_regs);
    log_info("READ->GLB_CFG %04X",glb_regs);

    VIO_VOLT_SEL3V3(glb_regs);
    log_info("WRITE->GLB_CFG %04X",glb_regs);
    command = tle_write_glb_cfg(glb_regs);
    
    command = tle_read_glb_cfg(&glb_regs);
    log_info("READ->GLB_CFG %04X",glb_regs);

    tle_write_glb_diag0(0x0000U);
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
    command  = tle_write_mode(ACT_CHANNEL, 0x0001U);

    command = tle_read_mode(base ,ACT_CHANNEL, &glb_regs);
    if ((glb_regs & MODE_CH_MODE_msk)!=0x0001U)
    {
        log_info("%s","FAILED TO CONFIGURE CHANNEL MODE");
        return ACT_CHANNEL_FAULT;
    }
    log_info("%s","SUCCESS TO CONFIGURE CHANNEL MODE");

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

void wolverin_log_diagonstics(void)
{
    /* ============================================================
     * CENTRAL FAULTS
     * ============================================================ */
uint8_t channel;
    logger_log(LOG_LEVEL_INFO,
               "TLE FAULTS: "
               "VBAT_UV=%d VBAT_OV=%d "
               "VIO_UV=%d VIO_OV=%d "
               "VDD_UV=%d VDD_OV=%d",
               tle_central_faults.vbat_under_volt_flt,
               tle_central_faults.vbat_over_volt_flt,
               tle_central_faults.vio_under_volt_flt,
               tle_central_faults.vio_over_volt_flt,
               tle_central_faults.vdd_under_volt_flt,
               tle_central_faults.vdd_over_volt_flt);

    logger_log(LOG_LEVEL_INFO,
               "TLE FAULTS: "
               "CLK=%d COT_ERR=%d COT_WARN=%d "
               "RESET=%d POR=%d SPI_WD=%d",
               tle_central_faults.clock_flt,
               tle_central_faults.central_over_temp_err,
               tle_central_faults.central_over_temp_warn,
               tle_central_faults.reset_event,
               tle_central_faults.power_on_reset_event,
               tle_central_faults.spi_watchdog_flt);

    logger_log(LOG_LEVEL_INFO,
               "TLE FAULTS: "
               "VREF_I_UV=%d VREF_I_OV=%d "
               "VDD2V5_UV=%d VDD2V5_OV=%d "
               "REF_UV=%d REF_OV=%d",
               tle_central_faults.vref_i_under_volt_flt,
               tle_central_faults.vref_i_over_volt_flt,
               tle_central_faults.vdd_2v5_under_volt_flt,
               tle_central_faults.vdd_2v5_over_volt_flt,
               tle_central_faults.ref_under_volt_flt,
               tle_central_faults.ref_over_volt_flt);

    logger_log(LOG_LEVEL_INFO,
               "TLE FAULTS: "
               "VPRE_OV=%d HVADC=%d "
               "REG_ECC=%d OTP_ECC=%d OTP_VIRGIN=%d",
               tle_central_faults.vpre_over_volt_flt,
               tle_central_faults.hv_adc_flt,
               tle_central_faults.register_ecc_flt,
               tle_central_faults.otp_ecc_flt,
               tle_central_faults.otp_virgin_flt);


    /* ============================================================
     * PIN STATUS
     * ============================================================ */

    logger_log(LOG_LEVEL_INFO,
               "TLE PIN: "
               "DRV0=%d DRV1=%d EN=%d "
               "FAULTN=%d FAULTN_FB=%d",
               fb_status_data.drv0_status,
               fb_status_data.drv1_status,
               fb_status_data.en_status,
               fb_status_data.faultn_status,
               fb_status_data.faultn_fb_status);


    /* ============================================================
     * FEEDBACK STATUS
     * ============================================================ */

    logger_log(LOG_LEVEL_INFO,
               "TLE FB_STAT: "
               "COT_ERR=%d COT_WARN=%d "
               "RESET=%d POR=%d DATA_ERR=%d",
               fb_status_data.coterr_status,
               fb_status_data.cotwarn_status,
               fb_status_data.reset_event_status,
               fb_status_data.por_event_status,
               fb_status_data.data_err_status);

    logger_log(LOG_LEVEL_INFO,
               "TLE FB_STAT: "
               "SUP_EXT=%d SUP_INT=%d "
               "CHGR0=%d CHGR1=%d CHGR2=%d "
               "SPI_WD=%d INIT_DONE=%d",
               fb_status_data.supply_ext_fault_status,
               fb_status_data.supply_int_fault_status,
               fb_status_data.err_chgr0_status,
               fb_status_data.err_chgr1_status,
               fb_status_data.err_chgr2_status,
               fb_status_data.spi_watchdog_status,
               fb_status_data.init_done_status);


    /* ============================================================
     * FEEDBACK VOLTAGES / TEMPERATURE
     * ============================================================ */

    logger_log(LOG_LEVEL_INFO,
               "TLE FB_VOLTAGE1: "
               "VIO_RAW=%u VIO=%.3fV "
               "VDD_RAW=%u VDD=%.3fV",
               fb_status_data.vio_voltage_raw,
               fb_status_data.vio_voltage,
               fb_status_data.vdd_voltage_raw,
               fb_status_data.vdd_voltage);

    logger_log(LOG_LEVEL_INFO,
               "TLE FB_VOLTAGE2: "
               "TEMP_RAW=%u TEMP=%.2fC "
               "VBAT_RAW=%u VBAT=%.3fV",
               fb_status_data.temp_raw,
               fb_status_data.temperature,
               fb_status_data.vbat_voltage_raw,
               fb_status_data.vbat_voltage);
    /* ============================================================
     * CHANNEL FEEDBACK
     * ============================================================ */

    for (channel = 0U; channel < 1U; channel++)
    {
        tle_read_channel_diag(channel);
        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_DC: "
                   "TP_MANT=%u TO_MANT=%u "
                   "DUTY=%.3f",
                   channel,
                   channel_fb_status_data[channel].tp_mant_raw,
                   channel_fb_status_data[channel].to_mant_raw,
                   channel_fb_status_data[channel].duty_cycle);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_VBAT: "
                   "VBAT_MANT=%u EXP=%u "
                   "VBAT=%.3fV",
                   channel,
                   channel_fb_status_data[channel].vbat_avg_mant_raw,
                   channel_fb_status_data[channel].vbat_exp,
                   channel_fb_status_data[channel].vbat_voltage);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_I_AVG: "
                   "I_AVG_MANT=%d EXP=%u "
                   "I_AVG=%.3fA",
                   channel,
                   channel_fb_status_data[channel].i_avg_mant_raw,
                   channel_fb_status_data[channel].i_avg_exp,
                   channel_fb_status_data[channel].average_current);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_IMIN_IMAX: "
                   "IMIN=%d IMAX=%d "
                   "MIN=%.3fA MAX=%.3fA",
                   channel,
                   channel_fb_status_data[channel].i_min_raw,
                   channel_fb_status_data[channel].i_max_raw,
                   channel_fb_status_data[channel].minimum_current,
                   channel_fb_status_data[channel].maximum_current);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_I_AVG_s16: "
                   "I_AVG_s16=%ld TIMESTAMP=%u "
                   "I_AVG=%.3fA",
                   channel,
                   (long)channel_fb_status_data[channel].i_avg_s16_raw,
                   channel_fb_status_data[channel].timestamp,
                   channel_fb_status_data[channel].average_current_s16);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_INT_THRESH: "
                   "INT_THRESH=%u",
                   channel,
                   channel_fb_status_data[channel].integrator_threshold);

        logger_log(LOG_LEVEL_INFO,
                   "TLE CH%u FB_PERIOD_MIN_MAX: "
                   "PMIN=%u PMAX=%u "
                   "FREQ_MIN=%.2fHz FREQ_MAX=%.2fHz",
                   channel,
                   channel_fb_status_data[channel].pwm_period_min_raw,
                   channel_fb_status_data[channel].pwm_period_max_raw,
                   channel_fb_status_data[channel].pwm_frequency_min,
                   channel_fb_status_data[channel].pwm_frequency_max);
    }
}
