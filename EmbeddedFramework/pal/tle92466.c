#include "tle92466.h"
#include "pal_gpio.h"
#include "pal_spi.h"
#include "pal_timer.h"

reply_16bit_union_t version_value;
reply_16bit_union_t chipid0_value;
reply_16bit_union_t chipid1_value;
reply_16bit_union_t ch_ctrl_value;
reply_16bit_union_t global_config_value;
reply_16bit_union_t global_diag0_value;
reply_16bit_union_t global_diag1_value;
reply_16bit_union_t global_diag2_value;
reply_16bit_union_t vbat_th_value;
reply_16bit_union_t fb_frz_value;
reply_16bit_union_t fb_upd_value;
reply_16bit_union_t wd_reload_value;
reply_16bit_union_t diag_err_chgr_value;
reply_16bit_union_t diag_warn_chgr_value;
reply_16bit_union_t fault_mask0_value;
reply_16bit_union_t fault_mask1_value;
reply_16bit_union_t fault_mask2_value;
reply_16bit_union_t clk_div_value;
reply_16bit_union_t sff_bist_value;
reply_16bit_union_t pin_stat_value;

reply_22bit_union_t fb_stat_value;
reply_22bit_union_t fb_voltage1_value;
reply_22bit_union_t fb_voltage2_value;

  reply_16bit_union_t set_point_value[6];

  reply_16bit_union_t ctrl_value[6];

  reply_16bit_union_t period_value[6];

  reply_16bit_union_t integrator_limit_value[6];

  reply_16bit_union_t dither_clk_div_value[6];

  reply_16bit_union_t dither_step_value[6];

  reply_16bit_union_t dither_ctrl_value[6];

  reply_16bit_union_t ch_config_value[6];

  reply_16bit_union_t mode_value[6];

  reply_16bit_union_t ton_value[6];

  reply_16bit_union_t ctrl_int_thresh_value[6];

  reply_22bit_union_t fb_dc_value[6];

  reply_22bit_union_t fb_vbat_value[6];

  reply_22bit_union_t fb_i_avg_value[6];

  reply_22bit_union_t fb_imin_imax_value[6];

  reply_22bit_union_t fb_i_avg_s16_value[6];

  reply_22bit_union_t fb_int_thresh_value[6];

  reply_22bit_union_t fb_period_min_max_value[6];

faults_tle_t tle_central_faults;
fb_status_data_t fb_status_data;
channel_fb_status_data_t channel_fb_status_data[6];

static uint8_t crc8_sae_j1850(const uint8_t *data, size_t length) {
    // Initial value is 0xFF
    uint8_t crc = 0xFF; 

    for (size_t i = 0; i < length; i++) {
        // XOR the input byte into the CRC register
        crc ^= data[i]; 

        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                // If the MSB is 1, shift and XOR with polynomial 0x1D
                crc = (crc << 1) ^ 0x1D;
            } else {
                // Otherwise, just shift left
                crc <<= 1;
            }
        }
    }

    // Final XOR value is 0xFF
    return crc ^ 0xFF; 
}


static uint32_t spi_write_frame(uint16_t addr, uint16_t data) {

    uint32_t payload = (((uint32_t)(addr & 0x7F) << 17) | (1U << 16) | (uint32_t)data) & 0x00FFFFFF;

    // Calculate CRC over the 3-byte payload
   uint8_t crc = crc8_sae_j1850((uint8_t *)&payload, 3);

    // Append CRC into the top byte (Bits 31..24)
    uint32_t frame = ((uint32_t)crc << 24) | payload;

    return frame;
}

static uint32_t spi_read_frame(uint16_t addr) {
    uint8_t rw_bit = 0;
    
    uint32_t payload = ((0x00000000U << 17) | ((uint32_t)rw_bit << 16) | addr) & 0x00FFFFFF;

    // Calculate CRC over the big-endian byte stream
    uint8_t crc = crc8_sae_j1850((uint8_t *)&payload, 3);

    // Combine CRC (MSB) with 24-bit payload to form 32-bit frame
    uint32_t frame = ((uint32_t)crc << 24) | payload;

    return frame;
}

static uint32_t tle_read_register(uint16_t reg_addr,uint32_t *regdata)
{

    uint32_t cmd = spi_read_frame(reg_addr);
     
    uint8_t tx_cmd[4] = {((cmd & 0xff000000)>>24),((cmd & 0x00ff0000)>>16),((cmd & 0x0000ff00)>>8),(cmd & 0x000000ff)};
    uint8_t rx_data[4];

     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
     pal_timer_delay_ms(4);
     pal_spi_transfer(BOARD_SPI_1 , tx_cmd , rx_data , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
     pal_timer_delay_ms(4);

     rx_data[0] = 0;
     rx_data[1] = 0;
     rx_data[2] = 0;
     rx_data[3] = 0;
     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
     pal_timer_delay_ms(4);
     tx_cmd[0] = 0;
     tx_cmd[1] = 0;
     tx_cmd[2] = 0;
     tx_cmd[3] = 0;
     pal_spi_transfer(BOARD_SPI_1 , tx_cmd , rx_data , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
    pal_timer_delay_ms(4);

    *regdata = ((rx_data[0] << 24) | (rx_data[1]<< 16) | (rx_data[2] << 8) | (rx_data[3]));

    return cmd;
    
}

static uint32_t tle_write_register(uint16_t reg_addr,uint16_t regdata)
{
    uint32_t cmd = spi_write_frame(reg_addr,regdata);
    uint8_t tx_cmd[4] = {((cmd & 0xff000000)>>24),((cmd & 0x00ff0000)>>16),((cmd & 0x0000ff00)>>8),(cmd & 0x000000ff)};
    uint8_t rx_data[4];
     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
     pal_timer_delay_ms(4);
     pal_spi_transfer(BOARD_SPI_1 , tx_cmd , rx_data,4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
     pal_timer_delay_ms(4);


    return cmd;
}

uint32_t tle_write_glb_cfg(uint16_t glb_cfg)
{
    uint32_t cmd = tle_write_register(GLOBAL_CONFIG_ADDR,glb_cfg);
    return cmd;
}

uint32_t tle_write_ch_ctrl(uint16_t value)
{
    return tle_write_register(CH_CTRL_ADDR, value);
}


uint32_t tle_write_glb_diag0(uint16_t value)
{
    return tle_write_register(GLOBAL_DIAG0_ADDR, value);
}

uint32_t tle_write_glb_diag1(uint16_t value)
{
    return tle_write_register(GLOBAL_DIAG1_ADDR, value);
}

uint32_t tle_write_glb_diag2(uint16_t value)
{
    return tle_write_register(GLOBAL_DIAG2_ADDR, value);
}

uint32_t tle_write_vbat_th(uint16_t value)
{
    return tle_write_register(VBAT_TH_ADDR, value);
}

uint32_t tle_write_fb_frz(uint16_t value)
{
    return tle_write_register(FB_FRZ_ADDR, value);
}

uint32_t tle_write_fb_upd(uint16_t value)
{
    return tle_write_register(FB_UPD_ADDR, value);
}

uint32_t tle_write_wd_reload(uint16_t value)
{
    return tle_write_register(WD_RELOAD_ADDR, value);
}

uint32_t tle_write_diag_err_chgr(uint8_t channel, uint16_t value)
{
    return tle_write_register(DIAG_ERR_CHGR_ADDR(channel), value);
}

uint32_t tle_write_diag_warn_chgr(uint8_t channel, uint16_t value)
{
    return tle_write_register(DIAG_WARN_CHGR_ADDR(channel), value);
}

uint32_t tle_write_fault_mask0(uint16_t value)
{
    return tle_write_register(FAULT_MASK0_ADDR, value);
}

uint32_t tle_write_fault_mask1(uint16_t value)
{
    return tle_write_register(FAULT_MASK1_ADDR, value);
}

uint32_t tle_write_fault_mask2(uint16_t value)
{
    return tle_write_register(FAULT_MASK2_ADDR, value);
}

uint32_t tle_write_clk_div(uint16_t value)
{
    return tle_write_register(CLK_DIV_ADDR, value);
}

uint32_t tle_write_sff_bist(uint16_t value)
{
    return tle_write_register(SFF_BIST_ADDR, value);
}


uint32_t tle_read_glb_cfg(uint16_t *glb_cfg)
{
    uint32_t cmd = tle_read_register(GLOBAL_CONFIG_ADDR,&global_config_value.raw);

    *glb_cfg = ((global_config_value.fields.reg_data_msb << 8) | (global_config_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_version(uint16_t *version)
{
    uint32_t cmd = tle_read_register(VERSION_ADDR,&version_value.raw);
   
    *version = ((version_value.fields.reg_data_msb << 8) | (version_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_ch_ctrl(uint16_t *ch_ctrl)
{
    uint32_t cmd = tle_read_register(CH_CTRL_ADDR, &ch_ctrl_value.raw);

    *ch_ctrl = ((ch_ctrl_value.fields.reg_data_msb << 8) | (ch_ctrl_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_glb_diag0(uint16_t *glb_diag0)
{
    uint32_t cmd = tle_read_register(GLOBAL_DIAG0_ADDR, &global_diag0_value.raw);

    *glb_diag0 = ((global_diag0_value.fields.reg_data_msb << 8) | (global_diag0_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_glb_diag1(uint16_t *glb_diag1)
{
    uint32_t cmd = tle_read_register(GLOBAL_DIAG1_ADDR, &global_diag1_value.raw);

    *glb_diag1 = ((global_diag1_value.fields.reg_data_msb << 8) | (global_diag1_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_glb_diag2(uint16_t *glb_diag2)
{
    uint32_t cmd = tle_read_register(GLOBAL_DIAG2_ADDR, &global_diag2_value.raw);

    *glb_diag2 = ((global_diag2_value.fields.reg_data_msb << 8) | (global_diag2_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_vbat_th(uint16_t *vbat_th)
{
    uint32_t cmd = tle_read_register(VBAT_TH_ADDR, &vbat_th_value.raw);

    *vbat_th = ((vbat_th_value.fields.reg_data_msb << 8) | (vbat_th_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fb_frz(uint16_t *fb_frz)
{
    uint32_t cmd = tle_read_register(FB_FRZ_ADDR, &fb_frz_value.raw);

    *fb_frz = ((fb_frz_value.fields.reg_data_msb << 8) | (fb_frz_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fb_upd(uint16_t *fb_upd)
{
    uint32_t cmd = tle_read_register(FB_UPD_ADDR, &fb_upd_value.raw);

    *fb_upd = ((fb_upd_value.fields.reg_data_msb << 8) | (fb_upd_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_wd_reload(uint16_t *wd_reload)
{
    uint32_t cmd = tle_read_register(WD_RELOAD_ADDR, &wd_reload_value.raw);

    *wd_reload = ((wd_reload_value.fields.reg_data_msb << 8) | (wd_reload_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_diag_err_chgr(uint8_t channel, uint16_t *diag_err_chgr)
{
    uint32_t cmd = tle_read_register(DIAG_ERR_CHGR_ADDR(channel), &diag_err_chgr_value.raw);

    *diag_err_chgr = ((diag_err_chgr_value.fields.reg_data_msb << 8) | (diag_err_chgr_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_diag_warn_chgr(uint8_t channel, uint16_t *diag_warn_chgr)
{
    uint32_t cmd = tle_read_register(DIAG_WARN_CHGR_ADDR(channel), &diag_warn_chgr_value.raw);

    *diag_warn_chgr = ((diag_warn_chgr_value.fields.reg_data_msb << 8) | (diag_warn_chgr_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fault_mask0(uint16_t *fault_mask0)
{
    uint32_t cmd = tle_read_register(FAULT_MASK0_ADDR, &fault_mask0_value.raw);

    *fault_mask0 = ((fault_mask0_value.fields.reg_data_msb << 8) | (fault_mask0_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fault_mask1(uint16_t *fault_mask1)
{
    uint32_t cmd = tle_read_register(FAULT_MASK1_ADDR, &fault_mask1_value.raw);

    *fault_mask1 = ((fault_mask1_value.fields.reg_data_msb << 8) | (fault_mask1_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fault_mask2(uint16_t *fault_mask2)
{
    uint32_t cmd = tle_read_register(FAULT_MASK2_ADDR, &fault_mask2_value.raw);

    *fault_mask2 = ((fault_mask2_value.fields.reg_data_msb << 8) | (fault_mask2_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_clk_div(uint16_t *clk_div)
{
    uint32_t cmd = tle_read_register(CLK_DIV_ADDR, &clk_div_value.raw);

    *clk_div = ((clk_div_value.fields.reg_data_msb << 8) | (clk_div_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_sff_bist(uint16_t *sff_bist)
{
    uint32_t cmd = tle_read_register(SFF_BIST_ADDR, &sff_bist_value.raw);

    *sff_bist = ((sff_bist_value.fields.reg_data_msb << 8) | (sff_bist_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_pin_stat(uint16_t *pin_stat)
{
    uint32_t cmd = tle_read_register(PIN_STAT_ADDR, &pin_stat_value.raw);

    *pin_stat = ((pin_stat_value.fields.reg_data_msb << 8) | (pin_stat_value.fields.reg_data_lsb));
    return cmd;
}

uint32_t tle_read_fb_stat(uint32_t *fb_stat)
{
    uint32_t cmd = tle_read_register(FB_STAT_ADDR, &fb_stat_value.raw);

    *fb_stat = ((uint32_t)fb_stat_value.fields.reg_data_msb_high << 16) |
               ((uint32_t)fb_stat_value.fields.reg_data_msb_low  << 8)  |
               ((uint32_t)fb_stat_value.fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_voltage1(uint32_t *fb_voltage1)
{
    uint32_t cmd = tle_read_register(FB_VOLTAGE1_ADDR, &fb_voltage1_value.raw);

    *fb_voltage1 = ((uint32_t)fb_voltage1_value.fields.reg_data_msb_high << 16) |
                   ((uint32_t)fb_voltage1_value.fields.reg_data_msb_low  << 8)  |
                   ((uint32_t)fb_voltage1_value.fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_voltage2(uint32_t *fb_voltage2)
{
    uint32_t cmd = tle_read_register(FB_VOLTAGE2_ADDR, &fb_voltage2_value.raw);

    *fb_voltage2 = ((uint32_t)fb_voltage2_value.fields.reg_data_msb_high << 16) |
                   ((uint32_t)fb_voltage2_value.fields.reg_data_msb_low  << 8)  |
                   ((uint32_t)fb_voltage2_value.fields.reg_data_lsb);

    return cmd;
}

/*Channel registers funcitons*/
uint32_t tle_read_setpoint(uint32_t base,
                           uint8_t channel,
                           uint16_t *setpoint)
{
    uint32_t cmd = tle_read_register(
        SETPOINT_ADDR(base),
        &set_point_value[channel].raw);

    *setpoint =
        ((set_point_value[channel].fields.reg_data_msb << 8) |
         (set_point_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_ctrl(uint32_t base,
                       uint8_t channel,
                       uint16_t *ctrl)
{
    uint32_t cmd = tle_read_register(
        CTRL_ADDR(base),
        &ctrl_value[channel].raw);

    *ctrl =
        ((ctrl_value[channel].fields.reg_data_msb << 8) |
         (ctrl_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_period(uint32_t base,
                         uint8_t channel,
                         uint16_t *period)
{
    uint32_t cmd = tle_read_register(
        PERIOD_ADDR(base),
        &period_value[channel].raw);

    *period =
        ((period_value[channel].fields.reg_data_msb << 8) |
         (period_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_integrator_limit(uint32_t base,
                                   uint8_t channel,
                                   uint16_t *integrator_limit)
{
    uint32_t cmd = tle_read_register(
        INTEGRATOR_LIMIT_ADDR(base),
        &integrator_limit_value[channel].raw);

    *integrator_limit =
        ((integrator_limit_value[channel].fields.reg_data_msb << 8) |
         (integrator_limit_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_dither_clk_div(uint32_t base,
                                 uint8_t channel,
                                 uint16_t *dither_clk_div)
{
    uint32_t cmd = tle_read_register(
        DITHER_CLK_DIV_ADDR(base),
        &dither_clk_div_value[channel].raw);

    *dither_clk_div =
        ((dither_clk_div_value[channel].fields.reg_data_msb << 8) |
         (dither_clk_div_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_dither_step(uint32_t base,
                              uint8_t channel,
                              uint16_t *dither_step)
{
    uint32_t cmd = tle_read_register(
        DITHER_STEP_ADDR(base),
        &dither_step_value[channel].raw);

    *dither_step =
        ((dither_step_value[channel].fields.reg_data_msb << 8) |
         (dither_step_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_dither_ctrl(uint32_t base,
                              uint8_t channel,
                              uint16_t *dither_ctrl)
{
    uint32_t cmd = tle_read_register(
        DITHER_CTRL_ADDR(base),
        &dither_ctrl_value[channel].raw);

    *dither_ctrl =
        ((dither_ctrl_value[channel].fields.reg_data_msb << 8) |
         (dither_ctrl_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_ch_config(uint32_t base,
                            uint8_t channel,
                            uint16_t *ch_config)
{
    uint32_t cmd = tle_read_register(
        CH_CONFIG_ADDR(base),
        &ch_config_value[channel].raw);

    *ch_config =
        ((ch_config_value[channel].fields.reg_data_msb << 8) |
         (ch_config_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_mode(uint32_t base,
                       uint8_t channel,
                       uint16_t *mode)
{
    uint32_t cmd = tle_read_register(
        MODE_ADDR(base),
        &mode_value[channel].raw);

    *mode =
        ((mode_value[channel].fields.reg_data_msb << 8) |
         (mode_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_ton(uint32_t base,
                      uint8_t channel,
                      uint16_t *ton)
{
    uint32_t cmd = tle_read_register(
        TON_ADDR(base),
        &ton_value[channel].raw);

    *ton =
        ((ton_value[channel].fields.reg_data_msb << 8) |
         (ton_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_ctrl_int_thresh(uint32_t base,
                                  uint8_t channel,
                                  uint16_t *ctrl_int_thresh)
{
    uint32_t cmd = tle_read_register(
        CTRL_INT_THRESH_ADDR(base),
        &ctrl_int_thresh_value[channel].raw);

    *ctrl_int_thresh =
        ((ctrl_int_thresh_value[channel].fields.reg_data_msb << 8) |
         (ctrl_int_thresh_value[channel].fields.reg_data_lsb));

    return cmd;
}


uint32_t tle_read_fb_dc(uint32_t base,
                        uint8_t channel,
                        uint32_t *fb_dc)
{
    uint32_t cmd = tle_read_register(
        FB_DC_ADDR(base),
        &fb_dc_value[channel].raw);

    *fb_dc = ((uint32_t)fb_dc_value[channel].fields.reg_data_msb_high << 16) |
             ((uint32_t)fb_dc_value[channel].fields.reg_data_msb_low  << 8)  |
             ((uint32_t)fb_dc_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_vbat(uint32_t base,
                          uint8_t channel,
                          uint32_t *fb_vbat)
{
    uint32_t cmd = tle_read_register(
        FB_VBAT_ADDR(base),
        &fb_vbat_value[channel].raw);

    *fb_vbat = ((uint32_t)fb_vbat_value[channel].fields.reg_data_msb_high << 16) |
               ((uint32_t)fb_vbat_value[channel].fields.reg_data_msb_low  << 8)  |
               ((uint32_t)fb_vbat_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_i_avg(uint32_t base,
                           uint8_t channel,
                           uint32_t *fb_i_avg)
{
    uint32_t cmd = tle_read_register(
        FB_I_AVG_ADDR(base),
        &fb_i_avg_value[channel].raw);

    *fb_i_avg = ((uint32_t)fb_i_avg_value[channel].fields.reg_data_msb_high << 16) |
                ((uint32_t)fb_i_avg_value[channel].fields.reg_data_msb_low  << 8)  |
                ((uint32_t)fb_i_avg_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_imin_imax(uint32_t base,
                               uint8_t channel,
                               uint32_t *fb_imin_imax)
{
    uint32_t cmd = tle_read_register(
        FB_IMIN_IMAX_ADDR(base),
        &fb_imin_imax_value[channel].raw);

    *fb_imin_imax =
        ((uint32_t)fb_imin_imax_value[channel].fields.reg_data_msb_high << 16) |
        ((uint32_t)fb_imin_imax_value[channel].fields.reg_data_msb_low  << 8)  |
        ((uint32_t)fb_imin_imax_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_i_avg_s16(uint32_t base,
                               uint8_t channel,
                               uint32_t *fb_i_avg_s16)
{
    uint32_t cmd = tle_read_register(
        FB_I_AVG_S16_ADDR(base),
        &fb_i_avg_s16_value[channel].raw);

    *fb_i_avg_s16 =
        ((uint32_t)fb_i_avg_s16_value[channel].fields.reg_data_msb_high << 16) |
        ((uint32_t)fb_i_avg_s16_value[channel].fields.reg_data_msb_low  << 8)  |
        ((uint32_t)fb_i_avg_s16_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_int_thresh(uint32_t base,
                                uint8_t channel,
                                uint32_t *fb_int_thresh)
{
    uint32_t cmd = tle_read_register(
        FB_INT_THRESH_ADDR(base),
        &fb_int_thresh_value[channel].raw);

    *fb_int_thresh =
        ((uint32_t)fb_int_thresh_value[channel].fields.reg_data_msb_high << 16) |
        ((uint32_t)fb_int_thresh_value[channel].fields.reg_data_msb_low  << 8)  |
        ((uint32_t)fb_int_thresh_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_read_fb_period_min_max(uint32_t base,
                                    uint8_t channel,
                                    uint32_t *fb_period_min_max)
{
    uint32_t cmd = tle_read_register(
        FB_PERIOD_MIN_MAX_ADDR(base),
        &fb_period_min_max_value[channel].raw);

    *fb_period_min_max =
        ((uint32_t)fb_period_min_max_value[channel].fields.reg_data_msb_high << 16) |
        ((uint32_t)fb_period_min_max_value[channel].fields.reg_data_msb_low  << 8)  |
        ((uint32_t)fb_period_min_max_value[channel].fields.reg_data_lsb);

    return cmd;
}


uint32_t tle_write_setpoint(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        SETPOINT_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_ctrl(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        CTRL_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_period(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        PERIOD_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_integrator_limit(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        INTEGRATOR_LIMIT_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_dither_clk_div(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        DITHER_CLK_DIV_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_dither_step(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        DITHER_STEP_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_dither_ctrl(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        DITHER_CTRL_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_ch_config(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        CH_CONFIG_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_mode(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        MODE_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_ton(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        TON_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

uint32_t tle_write_ctrl_int_thresh(uint8_t channel, uint16_t value)
{
    return tle_write_register(
        CTRL_INT_THRESH_ADDR(CHANNEL_BASE_ADDR(channel)),
        value);
}

void update_fault_structure(void)
{
    uint16_t reg_value = 0;

    /* ============================================================
     * GLOBAL_DIAG0
     * ============================================================ */

    tle_read_glb_diag0(&reg_value);

    tle_central_faults.vbat_under_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VBAT_UV_msk) >>
         GLOBAL_DIAG0_VBAT_UV_pos);

    tle_central_faults.vbat_over_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VBAT_OV_msk) >>
         GLOBAL_DIAG0_VBAT_OV_pos);

    tle_central_faults.vio_under_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VIO_UV_msk) >>
         GLOBAL_DIAG0_VIO_UV_pos);

    tle_central_faults.vio_over_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VIO_OV_msk) >>
         GLOBAL_DIAG0_VIO_OV_pos);

    tle_central_faults.vdd_under_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VDD_UV_msk) >>
         GLOBAL_DIAG0_VDD_UV_pos);

    tle_central_faults.vdd_over_volt_flt =
        ((reg_value & GLOBAL_DIAG0_VDD_OV_msk) >>
         GLOBAL_DIAG0_VDD_OV_pos);

    tle_central_faults.clock_flt =
        ((reg_value & GLOBAL_DIAG0_CLK_NOK_msk) >>
         GLOBAL_DIAG0_CLK_NOK_pos);

    tle_central_faults.central_over_temp_err =
        ((reg_value & GLOBAL_DIAG0_COTERR_msk) >>
         GLOBAL_DIAG0_COTERR_pos);

    tle_central_faults.central_over_temp_warn =
        ((reg_value & GLOBAL_DIAG0_COTWARN_msk) >>
         GLOBAL_DIAG0_COTWARN_pos);

    tle_central_faults.reset_event =
        ((reg_value & GLOBAL_DIAG0_RES_EVENT_msk) >>
         GLOBAL_DIAG0_RES_EVENT_pos);

    tle_central_faults.power_on_reset_event =
        ((reg_value & GLOBAL_DIAG0_POR_EVENT_msk) >>
         GLOBAL_DIAG0_POR_EVENT_pos);

    tle_central_faults.spi_watchdog_flt =
        ((reg_value & GLOBAL_DIAG0_SPI_WD_ERR_msk) >>
         GLOBAL_DIAG0_SPI_WD_ERR_pos);


    /* ============================================================
     * GLOBAL_DIAG1
     * ============================================================ */
    reg_value = 0;
    tle_read_glb_diag1(&reg_value);

    tle_central_faults.vref_i_under_volt_flt =
        ((reg_value & GLOBAL_DIAG1_VR_IREF_UV_msk) >>
         GLOBAL_DIAG1_VR_IREF_UV_pos);

    tle_central_faults.vref_i_over_volt_flt =
        ((reg_value & GLOBAL_DIAG1_VR_IREF_OV_msk) >>
         GLOBAL_DIAG1_VR_IREF_OV_pos);

    tle_central_faults.vdd_2v5_under_volt_flt =
        ((reg_value & GLOBAL_DIAG1_VDD2V5_UV_msk) >>
         GLOBAL_DIAG1_VDD2V5_UV_pos);

    tle_central_faults.vdd_2v5_over_volt_flt =
        ((reg_value & GLOBAL_DIAG1_VDD2V5_OV_msk) >>
         GLOBAL_DIAG1_VDD2V5_OV_pos);

    tle_central_faults.ref_under_volt_flt =
        ((reg_value & GLOBAL_DIAG1_REF_UV_msk) >>
         GLOBAL_DIAG1_REF_UV_pos);

    tle_central_faults.ref_over_volt_flt =
        ((reg_value & GLOBAL_DIAG1_REF_OV_msk) >>
         GLOBAL_DIAG1_REF_OV_pos);

    tle_central_faults.vpre_over_volt_flt =
        ((reg_value & GLOBAL_DIAG1_VPRE_OV_msk) >>
         GLOBAL_DIAG1_VPRE_OV_pos);

    tle_central_faults.hv_adc_flt =
        ((reg_value & GLOBAL_DIAG1_HVADC_ERR_msk) >>
         GLOBAL_DIAG1_HVADC_ERR_pos);


    /* ============================================================
     * GLOBAL_DIAG2
     * ============================================================ */
    reg_value = 0;
    tle_read_glb_diag2(&reg_value);

    tle_central_faults.register_ecc_flt =
        ((reg_value & GLOBAL_DIAG2_REG_ECC_ERR_msk) >>
         GLOBAL_DIAG2_REG_ECC_ERR_pos);

    tle_central_faults.otp_ecc_flt =
        ((reg_value & GLOBAL_DIAG2_OTP_ECC_ERR_msk) >>
         GLOBAL_DIAG2_OTP_ECC_ERR_pos);

    tle_central_faults.otp_virgin_flt =
        ((reg_value & GLOBAL_DIAG2_OTP_VIRGIN_msk) >>
         GLOBAL_DIAG2_OTP_VIRGIN_pos);
}

void update_fb_status_data(void)
{
    uint16_t reg_value = 0;
    uint32_t reg_value_24bit = 0;
    /* ============================================================
     * PIN_STAT
     * ============================================================ */

    tle_read_pin_stat(&reg_value);

    fb_status_data.drv0_status =
        ((reg_value & PIN_STAT_DRV0_msk) >>
         PIN_STAT_DRV0_pos);

    fb_status_data.drv1_status =
        ((reg_value & PIN_STAT_DRV1_msk) >>
         PIN_STAT_DRV1_pos);

    fb_status_data.en_status =
        ((reg_value & PIN_STAT_EN_msk) >>
         PIN_STAT_EN_pos);

    fb_status_data.faultn_status =
        ((reg_value & PIN_STAT_FAULTN_msk) >>
         PIN_STAT_FAULTN_pos);

    fb_status_data.faultn_fb_status =
        ((reg_value & PIN_STAT_FAULTN_FB_msk) >>
         PIN_STAT_FAULTN_FB_pos);


    /* ============================================================
     * FB_STAT
     * ============================================================ */

    tle_read_fb_stat(&reg_value_24bit);

    fb_status_data.coterr_status =
        ((reg_value_24bit & FB_STAT_COTERR_msk) >>
         FB_STAT_COTERR_pos);

    fb_status_data.cotwarn_status =
        ((reg_value_24bit & FB_STAT_COTWARN_msk) >>
         FB_STAT_COTWARN_pos);

    fb_status_data.reset_event_status =
        ((reg_value_24bit & FB_STAT_RES_EVENT_msk) >>
         FB_STAT_RES_EVENT_pos);

    fb_status_data.por_event_status =
        ((reg_value_24bit & FB_STAT_POR_EVENT_msk) >>
         FB_STAT_POR_EVENT_pos);

    fb_status_data.data_err_status =
        ((reg_value_24bit & FB_STAT_DATA_ERR_msk) >>
         FB_STAT_DATA_ERR_pos);

    fb_status_data.supply_ext_fault_status =
        ((reg_value_24bit & FB_STAT_SUP_NOK_EXT_msk) >>
         FB_STAT_SUP_NOK_EXT_pos);

    fb_status_data.supply_int_fault_status =
        ((reg_value_24bit & FB_STAT_SUP_NOK_INT_msk) >>
         FB_STAT_SUP_NOK_INT_pos);

    fb_status_data.err_chgr0_status =
        ((reg_value_24bit & FB_STAT_ERR_CHGR0_msk) >>
         FB_STAT_ERR_CHGR0_pos);

    fb_status_data.err_chgr1_status =
        ((reg_value_24bit & FB_STAT_ERR_CHGR1_msk) >>
         FB_STAT_ERR_CHGR1_pos);    

    fb_status_data.err_chgr2_status =
        ((reg_value_24bit & FB_STAT_ERR_CHGR2_msk) >>
         FB_STAT_ERR_CHGR2_pos);

    fb_status_data.spi_watchdog_status =
        ((reg_value_24bit & FB_STAT_SPI_WD_ERR_msk) >>
         FB_STAT_SPI_WD_ERR_pos);

    fb_status_data.init_done_status =
        ((reg_value_24bit & FB_STAT_INIT_DONE_msk) >>
         FB_STAT_INIT_DONE_pos);


    /* ============================================================
     * FB_VOLTAGE1
     * ============================================================ */
    reg_value_24bit = 0;
    tle_read_fb_voltage1(&reg_value_24bit);

    fb_status_data.vio_voltage_raw =
        (reg_value_24bit & FB_VOLTAGE1_VIO_msk) >>
        FB_VOLTAGE1_VIO_pos;

    fb_status_data.vdd_voltage_raw =
        (reg_value_24bit & FB_VOLTAGE1_VDD_msk) >>
        FB_VOLTAGE1_VDD_pos;

    fb_status_data.vio_voltage =
        0.0034534f * fb_status_data.vio_voltage_raw;

    fb_status_data.vdd_voltage =
        0.0034534f * fb_status_data.vdd_voltage_raw;


    /* ============================================================
     * FB_VOLTAGE2
     * ============================================================ */
    reg_value_24bit = 0;
    tle_read_fb_voltage2(&reg_value_24bit);

    fb_status_data.temp_raw =
        (reg_value_24bit & FB_VOLTAGE2_TEMP_VALUE_msk) >>
        FB_VOLTAGE2_TEMP_VALUE_pos;

    fb_status_data.vbat_voltage_raw =
        (reg_value_24bit & FB_VOLTAGE2_VBAT_msk) >>
        FB_VOLTAGE2_VBAT_pos;

    fb_status_data.temperature =
        ((fb_status_data.temp_raw * 0.000593f) - 0.819f) /
        (-0.0016f);

    fb_status_data.vbat_voltage =
        (41.47f * fb_status_data.vbat_voltage_raw) /
        2047.0f;
}




uint8_t tle_read_channel_diag(uint8_t channel)
{
    uint32_t base = 0U;

    if (channel >= 6U)
    {
        return 1U;
    }

    base = CHANNEL_BASE_ADDR(channel);


    /* ============================================================
     * FB_DC
     * ============================================================ */

    tle_read_fb_dc(base,
                   channel,
                   &fb_dc_value[channel].raw);

    channel_fb_status_data[channel].tp_mant_raw =
        (fb_dc_value[channel].raw & FB_DC_TP_MANT_msk) >>
        FB_DC_TP_MANT_pos;

    channel_fb_status_data[channel].to_mant_raw =
        (fb_dc_value[channel].raw & FB_DC_TO_MANT_msk) >>
        FB_DC_TO_MANT_pos;

    if (channel_fb_status_data[channel].tp_mant_raw != 0U)
    {
        channel_fb_status_data[channel].duty_cycle =
            (float)channel_fb_status_data[channel].to_mant_raw /
            (float)channel_fb_status_data[channel].tp_mant_raw;
    }
    else
    {
        channel_fb_status_data[channel].duty_cycle = 0.0f;
    }


    /* ============================================================
     * FB_VBAT
     * ============================================================ */

    tle_read_fb_vbat(base,
                     channel,
                     &fb_vbat_value[channel].raw);

    channel_fb_status_data[channel].vbat_avg_mant_raw =
        (fb_vbat_value[channel].raw & FB_VBAT_VBAT_AVG_MANT_msk) >>
        FB_VBAT_VBAT_AVG_MANT_pos;

    channel_fb_status_data[channel].vbat_exp =
        (fb_vbat_value[channel].raw & FB_VBAT_EXP_msk) >>
        FB_VBAT_EXP_pos;

    if (channel_fb_status_data[channel].tp_mant_raw != 0U)
    {
        channel_fb_status_data[channel].vbat_voltage =
            (41.47f *
             (float)channel_fb_status_data[channel].vbat_avg_mant_raw) /
            (float)channel_fb_status_data[channel].tp_mant_raw;
    }
    else
    {
        channel_fb_status_data[channel].vbat_voltage = 0.0f;
    }


    /* ============================================================
     * FB_I_AVG
     * ============================================================ */

    tle_read_fb_i_avg(base,
                      channel,
                      &fb_i_avg_value[channel].raw);

    channel_fb_status_data[channel].i_avg_mant_raw =
        (int16_t)((fb_i_avg_value[channel].raw &
                   FB_I_AVG_I_AVG_MANT_msk) >>
                  FB_I_AVG_I_AVG_MANT_pos);

    channel_fb_status_data[channel].i_avg_exp =
        (fb_i_avg_value[channel].raw & FB_I_AVG_EXP_msk) >>
        FB_I_AVG_EXP_pos;

    /* 12-bit signed two's complement */
    if (channel_fb_status_data[channel].i_avg_mant_raw & 0x0800)
    {
        channel_fb_status_data[channel].i_avg_mant_raw |= 0xF000;
    }

    if (channel_fb_status_data[channel].tp_mant_raw != 0U)
    {
        channel_fb_status_data[channel].average_current =
            (4.0f *
             (float)channel_fb_status_data[channel].i_avg_mant_raw) /
            (float)channel_fb_status_data[channel].tp_mant_raw;
    }
    else
    {
        channel_fb_status_data[channel].average_current = 0.0f;
    }


    /* ============================================================
     * FB_IMIN_IMAX
     * ============================================================ */

    tle_read_fb_imin_imax(base,
                          channel,
                          &fb_imin_imax_value[channel].raw);

    channel_fb_status_data[channel].i_min_raw =
        (int16_t)((fb_imin_imax_value[channel].raw &
                   FB_IMIN_IMAX_IMIN_msk) >>
                  FB_IMIN_IMAX_IMIN_pos);

    channel_fb_status_data[channel].i_max_raw =
        (int16_t)((fb_imin_imax_value[channel].raw &
                   FB_IMIN_IMAX_IMAX_msk) >>
                  FB_IMIN_IMAX_IMAX_pos);

    /* 9-bit signed two's complement */
    if (channel_fb_status_data[channel].i_min_raw & 0x0100)
    {
        channel_fb_status_data[channel].i_min_raw |= 0xFE00;
    }

    if (channel_fb_status_data[channel].i_max_raw & 0x0100)
    {
        channel_fb_status_data[channel].i_max_raw |= 0xFE00;
    }

    channel_fb_status_data[channel].minimum_current =
        (4.0f *
         (float)channel_fb_status_data[channel].i_min_raw) /
        511.0f;

    channel_fb_status_data[channel].maximum_current =
        (4.0f *
         (float)channel_fb_status_data[channel].i_max_raw) /
        511.0f;


    /* ============================================================
     * FB_I_AVG_s16
     * ============================================================ */

    tle_read_fb_i_avg_s16(base,
                          channel,
                          &fb_i_avg_s16_value[channel].raw);

    channel_fb_status_data[channel].i_avg_s16_raw =
        (int32_t)((fb_i_avg_s16_value[channel].raw &
                   FB_I_AVG_s16_I_AVG_s16_msk) >>
                  FB_I_AVG_s16_I_AVG_s16_pos);

    channel_fb_status_data[channel].timestamp =
        (fb_i_avg_s16_value[channel].raw &
         FB_I_AVG_s16_TIME_STAMP_msk) >>
        FB_I_AVG_s16_TIME_STAMP_pos;

    /*
     * Signed 17-bit two's complement
     */
    if (channel_fb_status_data[channel].i_avg_s16_raw & 0x10000)
    {
        channel_fb_status_data[channel].i_avg_s16_raw |= 0xFFFE0000;
    }

    channel_fb_status_data[channel].average_current_s16 =
        (4.0f *
         (float)channel_fb_status_data[channel].i_avg_s16_raw) /
        65535.0f;


    /* ============================================================
     * FB_INT_THRESH
     * ============================================================ */

    tle_read_fb_int_thresh(base,
                           channel,
                           &fb_int_thresh_value[channel].raw);

    channel_fb_status_data[channel].integrator_threshold =
        (fb_int_thresh_value[channel].raw &
         FB_INT_THRESH_INT_THRESH_VAL_msk) >>
        FB_INT_THRESH_INT_THRESH_VAL_pos;


    /* ============================================================
     * FB_PERIOD_MIN_MAX
     * ============================================================ */

    tle_read_fb_period_min_max(
        base,
        channel,
        &fb_period_min_max_value[channel].raw);

    channel_fb_status_data[channel].pwm_period_min_raw =
        (fb_period_min_max_value[channel].raw &
         FB_PERIOD_MIN_MAX_PMIN_msk) >>
        FB_PERIOD_MIN_MAX_PMIN_pos;

    channel_fb_status_data[channel].pwm_period_max_raw =
        (fb_period_min_max_value[channel].raw &
         FB_PERIOD_MIN_MAX_PMAX_msk) >>
        FB_PERIOD_MIN_MAX_PMAX_pos;

    /*
     * fPWM = fSYS / (period * 256)
     *
     * Replace F_SYS_HZ with your actual system clock.
     */
    if (channel_fb_status_data[channel].pwm_period_max_raw != 0U)
    {
        channel_fb_status_data[channel].pwm_frequency_min =
            F_SYS_HZ /
            ((float)channel_fb_status_data[channel].pwm_period_max_raw *
             256.0f);
    }
    else
    {
        channel_fb_status_data[channel].pwm_frequency_min = 0.0f;
    }

    if (channel_fb_status_data[channel].pwm_period_min_raw != 0U)
    {
        channel_fb_status_data[channel].pwm_frequency_max =
            F_SYS_HZ /
            ((float)channel_fb_status_data[channel].pwm_period_min_raw *
             256.0f);
    }
    else
    {
        channel_fb_status_data[channel].pwm_frequency_max = 0.0f;
    }


    return 0U;
}