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