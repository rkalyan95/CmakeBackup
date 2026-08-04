#ifndef TLE92446_H
#define TLE92446_H

#include "board.h"

#include "pal_status.h"
#include "pal_assert.h"
#include "pal_spi.h"
#define BASE_ADDR               0x00000000U

#define CH_CTRL_ADDR            (BASE_ADDR + 0x0000)
#define GLOBAL_CONFIG_ADDR      (BASE_ADDR + 0x0002)
#define GLOBAL_DIAG0_ADDR       (BASE_ADDR + 0x0003)
#define GLOBAL_DIAG1_ADDR       (BASE_ADDR + 0x0004)
#define GLOBAL_DIAG2_ADDR       (BASE_ADDR + 0x0005)
#define VBAT_TH_ADDR            (BASE_ADDR + 0x0006)
#define FB_FRZ_ADDR             (BASE_ADDR + 0x0007)
#define FB_UPD_ADDR             (BASE_ADDR + 0x0008)
#define WD_RELOAD_ADDR          (BASE_ADDR + 0x0009)

#define DIAG_ERR_CHGR_ADDR(x)   (BASE_ADDR + 0x000A + (x))
#define DIAG_WARN_CHGR_ADDR(x)  (BASE_ADDR + 0x0010 + (x))

#define FAULT_MASK0_ADDR        (BASE_ADDR + 0x0016)
#define FAULT_MASK1_ADDR        (BASE_ADDR + 0x0017)
#define FAULT_MASK2_ADDR        (BASE_ADDR + 0x0018)

#define CLK_DIV_ADDR            (BASE_ADDR + 0x0019)

#define SFF_BIST_ADDR           (BASE_ADDR + 0x003F)

#define VERSION_ADDR            (BASE_ADDR + 0x0200)
#define PIN_STAT_ADDR           (BASE_ADDR + 0x0201)
#define FB_STAT_ADDR            (BASE_ADDR + 0x0202)
#define FB_VOLTAGE1_ADDR        (BASE_ADDR + 0x0203)
#define FB_VOLTAGE2_ADDR        (BASE_ADDR + 0x0204)

#define CHIPID0_ADDR            (BASE_ADDR + 0x0205)
#define CHIPID1_ADDR            (BASE_ADDR + 0x0206)
#define CHIPID2_ADDR            (BASE_ADDR + 0x0207)


#define CH0_BASE_ADDR               0x00000040U
#define CH1_BASE_ADDR               0x00000050U
#define CH2_BASE_ADDR               0x00000060U
#define CH3_BASE_ADDR               0x00000070U
#define CH4_BASE_ADDR               0x00000020U
#define CH5_BASE_ADDR               0x00000030U

/* Helper macro */
#define CHANNEL_BASE_ADDR(ch)       ((ch) == 0 ? CH0_BASE_ADDR : \
                                     (ch) == 1 ? CH1_BASE_ADDR : \
                                     (ch) == 2 ? CH2_BASE_ADDR : \
                                     (ch) == 3 ? CH3_BASE_ADDR : \
                                     (ch) == 4 ? CH4_BASE_ADDR : \
                                                 CH5_BASE_ADDR)

#define SETPOINT_ADDR(base)             ((base) + 0x0000U)
#define CTRL_ADDR(base)                 ((base) + 0x0001U)
#define PERIOD_ADDR(base)               ((base) + 0x0002U)
#define INTEGRATOR_LIMIT_ADDR(base)     ((base) + 0x0003U)
#define DITHER_CLK_DIV_ADDR(base)       ((base) + 0x0004U)
#define DITHER_STEP_ADDR(base)          ((base) + 0x0005U)
#define DITHER_CTRL_ADDR(base)          ((base) + 0x0006U)
#define CH_CONFIG_ADDR(base)            ((base) + 0x0007U)

#define MODE_ADDR(base)                 ((base) + 0x000CU)
#define TON_ADDR(base)                  ((base) + 0x000DU)
#define CTRL_INT_THRESH_ADDR(base)      ((base) + 0x000EU)

#define FB_DC_ADDR(base)                ((base) + 0x0200U)
#define FB_VBAT_ADDR(base)              ((base) + 0x0201U)
#define FB_I_AVG_ADDR(base)             ((base) + 0x0202U)
#define FB_IMIN_IMAX_ADDR(base)         ((base) + 0x0203U)
#define FB_I_AVG_S16_ADDR(base)         ((base) + 0x0204U)
#define FB_INT_THRESH_ADDR(base)        ((base) + 0x0205U)
#define FB_PERIOD_MIN_MAX_ADDR(base)    ((base) + 0x0206U)

typedef struct reply_16bit_
{
    uint8_t reg_data_lsb:8;
    uint8_t reg_data_msb:8;
    uint8_t rw:1;
    uint8_t status : 5;
    uint8_t reply_mode:2;
    uint8_t calc_crc : 8;
}reply_16bit_t;

typedef union reply_16bit_union_
{
    uint32_t raw;
    reply_16bit_t fields;
}reply_16bit_union_t;

typedef struct reply_22bit_
{
    uint8_t reg_data_lsb:8;
    uint8_t reg_data_msb_low:8;
    uint8_t reg_data_msb_high:6;
    uint8_t replymode : 2;
    uint8_t calc_crc : 8;
}reply_22bit_t;

typedef union reply_22bit_union_
{
    uint32_t raw;
    reply_16bit_t fields;
}reply_22bit_union_t;

typedef struct reply_critical_bit_
{
    uint8_t wd_ref_clk_flt:1;
    uint8_t dig_clk_fast_flt:1;
    uint8_t dig_clk_slow_flt:1;
    uint8_t clk_too_fast_flt:1;
    uint8_t clk_too_slow_flt:1;
    uint8_t adc_bg_flt:1;
    uint8_t supply_2v5_flt:1;
    uint8_t supply_1v5_flt : 1;
    uint8_t reserved1 : 8;
    uint8_t reserved2 : 6;
    uint8_t reply_mode : 2;
    uint8_t reserved3 : 8;
}reply_critical_bit_t;

typedef union reply_criticalbit_union_
{
    uint32_t raw;
    reply_16bit_t fields;
}reply_criticalbit_union_t;


extern reply_16bit_union_t version_value;
extern reply_16bit_union_t chipid0_value;
extern reply_16bit_union_t chipid1_value;
extern reply_16bit_union_t ch_ctrl_value;
extern reply_16bit_union_t global_config_value;
extern reply_16bit_union_t global_diag0_value;
extern reply_16bit_union_t global_diag1_value;
extern reply_16bit_union_t global_diag2_value;
extern reply_16bit_union_t vbat_th_value;
extern reply_16bit_union_t fb_frz_value;
extern reply_16bit_union_t fb_upd_value;
extern reply_16bit_union_t wd_reload_value;
extern reply_16bit_union_t diag_err_chgr_value;
extern reply_16bit_union_t diag_warn_chgr_value;
extern reply_16bit_union_t fault_mask0_value;
extern reply_16bit_union_t fault_mask1_value;
extern reply_16bit_union_t fault_mask2_value;
extern reply_16bit_union_t clk_div_value;
extern reply_16bit_union_t sff_bist_value;
extern reply_16bit_union_t pin_stat_value;

extern reply_22bit_union_t fb_stat_value;
extern reply_22bit_union_t fb_voltage1_value;
extern reply_22bit_union_t fb_voltage2_value;

uint32_t tle_read_version(uint16_t *version);
uint32_t tle_write_glb_cfg(uint16_t glb_cfg);
uint32_t tle_read_glb_cfg(uint16_t *glb_cfg);

#endif