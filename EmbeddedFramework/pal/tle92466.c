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