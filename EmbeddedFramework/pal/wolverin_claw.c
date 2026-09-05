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


static inline bool pin_lvl_check_en(void)
{
    uint8_t pin_lvl = 0;
    pal_gpio_read(BOARD_GPIO_TLE_EN,&pin_lvl);
    return ((!pin_lvl)?(false):(true));
}

static inline bool pin_lvl_check_rst(void)
{
    uint8_t pin_lvl = 0;
    pal_gpio_read(BOARD_GPIO_TLE_RST,&pin_lvl);
    return ((!pin_lvl)?(false):(true));
}

static inline bool is_device_present(void)
{
    uint16_t glb_regs = 0;
    tle_read_version(&glb_regs);
    return ((glb_regs == 0xC1FE)?(true):(false));
}

static inline bool set_device_vio(void)
{
    uint16_t glb_regs = 0;
    uint32_t command = 0;

    command = tle_read_glb_cfg(&glb_regs);
    log_info("READ->GLB_CFG %04X",glb_regs);

    VIO_VOLT_SEL3V3(glb_regs);

    log_info("WRITE->GLB_CFG %04X",glb_regs);
    command = tle_write_glb_cfg(glb_regs);
    
    command = tle_read_glb_cfg(&glb_regs);
    log_info("READ->GLB_CFG %04X",glb_regs);   

    return true;
}

static inline bool clear_faults(void)
{
     tle_write_glb_diag0(0x0000U);
}

static inline bool is_device_initialised(void)
{
    return ((fb_status_data.init_done_status == 0)?(false):(true));
}

static inline bool is_vbat_good(void)
{
    return ((tle_central_faults.vbat_under_volt_flt || tle_central_faults.vbat_over_volt_flt)?(false):(true));
}

static inline bool is_vio_good(void)
{
    return ((tle_central_faults.vio_under_volt_flt || tle_central_faults.vio_over_volt_flt)?(false):(true));
}

static inline bool is_vdd_good(void)
{
    return ((tle_central_faults.vdd_under_volt_flt || tle_central_faults.vdd_over_volt_flt)?(false):(true));
}
static inline bool is_clock_good(void)
{
    return ((tle_central_faults.clock_flt)?(false):(true));
}

static inline bool is_central_temp_good(void)
{
    return ((tle_central_faults.central_over_temp_err || tle_central_faults.central_over_temp_warn)?(false):(true));
}

static inline bool is_reset_event(void)
{
    return ((tle_central_faults.reset_event)?(true):(false));
}

static inline bool is_power_on_reset_event(void)
{
    return ((tle_central_faults.power_on_reset_event)?(true):(false));
}

static inline bool is_spi_watchdog_fault(void)
{
    return ((tle_central_faults.spi_watchdog_flt)?(true):(false));
}

static inline bool is_vref_good(void)
{
    return ((tle_central_faults.vref_i_under_volt_flt || tle_central_faults.vref_i_over_volt_flt)?(false):(true));
}

static inline bool is_vdd_2v5_good(void)
{
    return ((tle_central_faults.vdd_2v5_under_volt_flt || tle_central_faults.vdd_2v5_over_volt_flt)?(false):(true));
}

static inline bool is_ref_good(void)
{
    return ((tle_central_faults.ref_under_volt_flt || tle_central_faults.ref_over_volt_flt)?(false):(true));
}

static inline bool is_vpre_good(void)
{
    return ((tle_central_faults.vpre_over_volt_flt)?(false):(true));
}

static inline bool is_hv_adc_good(void)
{
    return ((tle_central_faults.hv_adc_flt)?(false):(true));
}

static inline bool is_register_ecc_good(void)
{
    return ((tle_central_faults.register_ecc_flt)?(false):(true));
}

static inline bool is_otp_ecc_good(void)
{
    return ((tle_central_faults.otp_ecc_flt)?(false):(true));
}

static inline bool is_otp_virgin_good(void)
{
    return ((tle_central_faults.otp_virgin_flt)?(false):(true));
}

static actuator_status_t enable_mission_mode(void)
{   
    uint16_t glb_regs = 0;
    uint32_t command = 0;
    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL_MODE %04X",glb_regs);

    glb_regs |= (CH_CTRL_OP_MODE_msk);
    log_info("WRITE->CH_CTRL_MODE %04X",glb_regs);
    command = tle_write_ch_ctrl(glb_regs);

    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL_MODE %04X",glb_regs);

    return (!(glb_regs & CH_CTRL_OP_MODE_msk)?(ACT_CONFIG_FAILED):(ACT_OK));
}

static actuator_status_t configure_channel_mode(uint8_t solenoid_num)
{
    uint16_t glb_regs = 0;
    uint32_t command = 0;

    uint32_t base = CHANNEL_BASE_ADDR(solenoid_num);
    command  = tle_write_mode(ACT_CHANNEL, 0x0001U);
    command = tle_read_mode(base ,ACT_CHANNEL, &glb_regs);

    return (((glb_regs & MODE_CH_MODE_msk)!=0x0001U)?(ACT_CONFIG_FAILED):(ACT_OK));
}

static actuator_status_t enable_solenoid_channel(uint8_t solenoid_num)
{
    uint16_t glb_regs = 0;
    uint32_t command = 0;

    command = tle_read_ch_ctrl(&glb_regs);
    log_info("READ->CH_CTRL %04X",glb_regs);

    glb_regs |= CH_CTRL_EN_CH0_msk;
    log_info("WRITE->CH_CTRL %04X",glb_regs);
    command = tle_write_ch_ctrl(glb_regs);
    
    command = tle_read_ch_ctrl(&glb_regs);

    return (!(glb_regs & CH_CTRL_EN_CH0_msk)?(ACT_ENABLE_FAILED):(ACT_OK));
}

static actuator_status_t configure_setpoint(uint8_t solenoid_num, uint16_t setpoint)
{
    uint16_t glb_regs = 0;
    uint32_t command = 0;

    uint32_t base = CHANNEL_BASE_ADDR(solenoid_num);
    command = tle_write_setpoint(ACT_CHANNEL, TLE_SETPOINT_500MA);
    log_info("WRITE->CH_SETPOINT %04X",TLE_SETPOINT_500MA);

    command = tle_read_setpoint(base ,ACT_CHANNEL, &glb_regs);

    return (!(glb_regs & SETPOINT_TARGET_msk)?(ACT_SETPOINT_FAILED):(ACT_OK));
}

actuator_status_t actuator_hold(void)
{
    uint16_t glb_regs = 0;
    uint32_t command = 0;

    uint32_t base = CHANNEL_BASE_ADDR(0);
    command = tle_write_setpoint(ACT_CHANNEL, TLE_SETPOINT_HOLDMA);
    log_info("WRITE->CH_SETPOINT %04X",TLE_SETPOINT_HOLDMA);

    command = tle_read_setpoint(base ,ACT_CHANNEL, &glb_regs);

    return (!(glb_regs & SETPOINT_TARGET_msk)?(ACT_SETPOINT_FAILED):(ACT_OK));
}

static inline void handle_fault_state(bool actual_state , bool expected_state , uint8_t fault_num)
{
    if(actual_state != expected_state)
    {
        log_error("ERROR , FAULT_NUM->%d",fault_num);
    }
}

static inline void handle_actuator_status(actuator_status_t actual_status , actuator_status_t expected_status,uint8_t fault_num)
{
    if(actual_status != expected_status)
    {
        log_error("ERROR , ACT_STATUS->%d , FIELD-> %d",actual_status,fault_num);
    }
}

actuator_status_t actuator_init_and_start(void)
{

    actuator_status_t err = ACT_OK;
    bool fault_state = true;
    fault_state = pin_lvl_check_en();
    handle_fault_state(fault_state,true,1);
    fault_state = pin_lvl_check_rst();
    handle_fault_state(fault_state,true,2);
    fault_state = is_device_present();
    handle_fault_state(fault_state,true,3);
    fault_state = set_device_vio();
    handle_fault_state(fault_state,true,4);
    clear_faults();
    update_fault_structure();
    update_fb_status_data();
    fault_state = is_device_initialised();
    handle_fault_state(fault_state,true,5);
    fault_state = is_vbat_good();
    handle_fault_state(fault_state,true,6);
    fault_state = is_vio_good();
    handle_fault_state(fault_state,true,7);
    fault_state = is_vdd_good();
    handle_fault_state(fault_state,true,8);
    fault_state = is_clock_good();
    handle_fault_state(fault_state,true,9);
    fault_state = is_central_temp_good();
    handle_fault_state(fault_state,true,10);
    fault_state = is_reset_event();
    handle_fault_state(fault_state,false,11);
    fault_state = is_power_on_reset_event();
    handle_fault_state(fault_state,false,12);
    fault_state = is_spi_watchdog_fault();
    handle_fault_state(fault_state,false,13);
    fault_state = is_vref_good();
    handle_fault_state(fault_state,true,14);
    fault_state = is_vdd_2v5_good();
    handle_fault_state(fault_state,true,15);
    fault_state = is_ref_good();
    handle_fault_state(fault_state,true,16);
    fault_state = is_vpre_good();
    handle_fault_state(fault_state,true,17);
    fault_state = is_hv_adc_good();
    handle_fault_state(fault_state,true,18);
    fault_state = is_register_ecc_good();
    handle_fault_state(fault_state,true,19);
    fault_state = is_otp_ecc_good();
    handle_fault_state(fault_state,true,20);

    fault_state = is_otp_virgin_good();
    handle_fault_state(fault_state,true,21);

    err = configure_channel_mode(0);
    handle_actuator_status(err,ACT_OK,1);
    err = enable_mission_mode();
    handle_actuator_status(err,ACT_OK,2);
    err = enable_solenoid_channel(0);
    handle_actuator_status(err,ACT_OK,3);
    err = configure_setpoint(0, TLE_SETPOINT_500MA);
    handle_actuator_status(err,ACT_OK,4);
    return (fault_state==false || err != ACT_OK)?(err):(ACT_OK);

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
