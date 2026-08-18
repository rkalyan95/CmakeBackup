#ifndef TLE92446_H
#define TLE92446_H

#include "board.h"

#include "pal_status.h"
#include "pal_assert.h"
#include "pal_spi.h"
#include <stdbool.h>
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


/* ============================================================
 * CH_CTRL
 * ============================================================ */

#define CH_CTRL_EN_CH0_pos                     0U
#define CH_CTRL_EN_CH0_msk                     (1U << CH_CTRL_EN_CH0_pos)

#define CH_CTRL_EN_CH1_pos                     1U
#define CH_CTRL_EN_CH1_msk                     (1U << CH_CTRL_EN_CH1_pos)

#define CH_CTRL_EN_CH2_pos                     2U
#define CH_CTRL_EN_CH2_msk                     (1U << CH_CTRL_EN_CH2_pos)

#define CH_CTRL_EN_CH3_pos                     3U
#define CH_CTRL_EN_CH3_msk                     (1U << CH_CTRL_EN_CH3_pos)

#define CH_CTRL_EN_CH4_pos                     4U
#define CH_CTRL_EN_CH4_msk                     (1U << CH_CTRL_EN_CH4_pos)

#define CH_CTRL_EN_CH5_pos                     5U
#define CH_CTRL_EN_CH5_msk                     (1U << CH_CTRL_EN_CH5_pos)

#define CH_CTRL_CH_PAR_4_5_pos                12U
#define CH_CTRL_CH_PAR_4_5_msk                (1U << CH_CTRL_CH_PAR_4_5_pos)

#define CH_CTRL_CH_PAR_0_3_pos                13U
#define CH_CTRL_CH_PAR_0_3_msk                (1U << CH_CTRL_CH_PAR_0_3_pos)

#define CH_CTRL_CH_PAR_1_2_pos                14U
#define CH_CTRL_CH_PAR_1_2_msk                (1U << CH_CTRL_CH_PAR_1_2_pos)

#define CH_CTRL_OP_MODE_pos                   15U
#define CH_CTRL_OP_MODE_msk                   (1U << CH_CTRL_OP_MODE_pos)


/* ============================================================
 * GLOBAL_CONFIG
 * ============================================================ */

#define GLOBAL_CONFIG_CLK_WD_EN_pos            0U
#define GLOBAL_CONFIG_CLK_WD_EN_msk            (1U << GLOBAL_CONFIG_CLK_WD_EN_pos)

#define GLOBAL_CONFIG_SPI_WD_EN_pos            1U
#define GLOBAL_CONFIG_SPI_WD_EN_msk            (1U << GLOBAL_CONFIG_SPI_WD_EN_pos)

#define GLOBAL_CONFIG_CRC_EN_pos               2U
#define GLOBAL_CONFIG_CRC_EN_msk               (1U << GLOBAL_CONFIG_CRC_EN_pos)

#define GLOBAL_CONFIG_V1V5_UV_TEST_pos         4U
#define GLOBAL_CONFIG_V1V5_UV_TEST_msk         (1U << GLOBAL_CONFIG_V1V5_UV_TEST_pos)

#define GLOBAL_CONFIG_V1V5_OV_TEST_pos         5U
#define GLOBAL_CONFIG_V1V5_OV_TEST_msk         (1U << GLOBAL_CONFIG_V1V5_OV_TEST_pos)

#define GLOBAL_CONFIG_OT_TEST_pos             12U
#define GLOBAL_CONFIG_OT_TEST_msk             (1U << GLOBAL_CONFIG_OT_TEST_pos)

#define GLOBAL_CONFIG_UV_OV_SWAP_pos          13U
#define GLOBAL_CONFIG_UV_OV_SWAP_msk          (1U << GLOBAL_CONFIG_UV_OV_SWAP_pos)

#define GLOBAL_CONFIG_VIO_SEL_pos              14U
#define GLOBAL_CONFIG_VIO_SEL_msk              (1U << GLOBAL_CONFIG_VIO_SEL_pos)


/* ============================================================
 * GLOBAL_DIAG0
 * ============================================================ */

#define GLOBAL_DIAG0_VBAT_UV_pos               0U
#define GLOBAL_DIAG0_VBAT_UV_msk               (1U << GLOBAL_DIAG0_VBAT_UV_pos)

#define GLOBAL_DIAG0_VBAT_OV_pos               1U
#define GLOBAL_DIAG0_VBAT_OV_msk               (1U << GLOBAL_DIAG0_VBAT_OV_pos)

#define GLOBAL_DIAG0_VIO_UV_pos                2U
#define GLOBAL_DIAG0_VIO_UV_msk                (1U << GLOBAL_DIAG0_VIO_UV_pos)

#define GLOBAL_DIAG0_VIO_OV_pos                3U
#define GLOBAL_DIAG0_VIO_OV_msk                (1U << GLOBAL_DIAG0_VIO_OV_pos)

#define GLOBAL_DIAG0_VDD_UV_pos                4U
#define GLOBAL_DIAG0_VDD_UV_msk                (1U << GLOBAL_DIAG0_VDD_UV_pos)

#define GLOBAL_DIAG0_VDD_OV_pos                5U
#define GLOBAL_DIAG0_VDD_OV_msk                (1U << GLOBAL_DIAG0_VDD_OV_pos)

#define GLOBAL_DIAG0_CLK_NOK_pos               6U
#define GLOBAL_DIAG0_CLK_NOK_msk               (1U << GLOBAL_DIAG0_CLK_NOK_pos)

#define GLOBAL_DIAG0_COTERR_pos                7U
#define GLOBAL_DIAG0_COTERR_msk                (1U << GLOBAL_DIAG0_COTERR_pos)

#define GLOBAL_DIAG0_COTWARN_pos               8U
#define GLOBAL_DIAG0_COTWARN_msk               (1U << GLOBAL_DIAG0_COTWARN_pos)

#define GLOBAL_DIAG0_RES_EVENT_pos             9U
#define GLOBAL_DIAG0_RES_EVENT_msk             (1U << GLOBAL_DIAG0_RES_EVENT_pos)

#define GLOBAL_DIAG0_POR_EVENT_pos            10U
#define GLOBAL_DIAG0_POR_EVENT_msk            (1U << GLOBAL_DIAG0_POR_EVENT_pos)

#define GLOBAL_DIAG0_SPI_WD_ERR_pos           14U
#define GLOBAL_DIAG0_SPI_WD_ERR_msk           (1U << GLOBAL_DIAG0_SPI_WD_ERR_pos)


/* ============================================================
 * GLOBAL_DIAG1
 * ============================================================ */

#define GLOBAL_DIAG1_VR_IREF_UV_pos            0U
#define GLOBAL_DIAG1_VR_IREF_UV_msk            (1U << GLOBAL_DIAG1_VR_IREF_UV_pos)

#define GLOBAL_DIAG1_VR_IREF_OV_pos            1U
#define GLOBAL_DIAG1_VR_IREF_OV_msk            (1U << GLOBAL_DIAG1_VR_IREF_OV_pos)

#define GLOBAL_DIAG1_VDD2V5_UV_pos             2U
#define GLOBAL_DIAG1_VDD2V5_UV_msk             (1U << GLOBAL_DIAG1_VDD2V5_UV_pos)

#define GLOBAL_DIAG1_VDD2V5_OV_pos             3U
#define GLOBAL_DIAG1_VDD2V5_OV_msk             (1U << GLOBAL_DIAG1_VDD2V5_OV_pos)

#define GLOBAL_DIAG1_REF_UV_pos                4U
#define GLOBAL_DIAG1_REF_UV_msk                (1U << GLOBAL_DIAG1_REF_UV_pos)

#define GLOBAL_DIAG1_REF_OV_pos                5U
#define GLOBAL_DIAG1_REF_OV_msk                (1U << GLOBAL_DIAG1_REF_OV_pos)

#define GLOBAL_DIAG1_VPRE_OV_pos               6U
#define GLOBAL_DIAG1_VPRE_OV_msk               (1U << GLOBAL_DIAG1_VPRE_OV_pos)

#define GLOBAL_DIAG1_HVADC_ERR_pos            15U
#define GLOBAL_DIAG1_HVADC_ERR_msk            (1U << GLOBAL_DIAG1_HVADC_ERR_pos)


/* ============================================================
 * GLOBAL_DIAG2
 * ============================================================ */

#define GLOBAL_DIAG2_REG_ECC_ERR_pos            1U
#define GLOBAL_DIAG2_REG_ECC_ERR_msk            (1U << GLOBAL_DIAG2_REG_ECC_ERR_pos)

#define GLOBAL_DIAG2_OTP_ECC_ERR_pos            3U
#define GLOBAL_DIAG2_OTP_ECC_ERR_msk            (1U << GLOBAL_DIAG2_OTP_ECC_ERR_pos)

#define GLOBAL_DIAG2_OTP_VIRGIN_pos             4U
#define GLOBAL_DIAG2_OTP_VIRGIN_msk             (1U << GLOBAL_DIAG2_OTP_VIRGIN_pos)


/* ============================================================
 * VBAT_TH
 * ============================================================ */

#define VBAT_TH_VBAT_UV_TH_pos                 0U
#define VBAT_TH_VBAT_UV_TH_msk                 (0xFFU << VBAT_TH_VBAT_UV_TH_pos)

#define VBAT_TH_VBAT_OV_TH_pos                 8U
#define VBAT_TH_VBAT_OV_TH_msk                 (0xFFU << VBAT_TH_VBAT_OV_TH_pos)


/* ============================================================
 * FB_FRZ
 * ============================================================ */

#define FB_FRZ_FR_CH0_pos                       0U
#define FB_FRZ_FR_CH0_msk                       (1U << FB_FRZ_FR_CH0_pos)

#define FB_FRZ_FR_CH1_pos                       1U
#define FB_FRZ_FR_CH1_msk                       (1U << FB_FRZ_FR_CH1_pos)

#define FB_FRZ_FR_CH2_pos                       2U
#define FB_FRZ_FR_CH2_msk                       (1U << FB_FRZ_FR_CH2_pos)

#define FB_FRZ_FR_CH3_pos                       3U
#define FB_FRZ_FR_CH3_msk                       (1U << FB_FRZ_FR_CH3_pos)

#define FB_FRZ_FR_CH4_pos                       4U
#define FB_FRZ_FR_CH4_msk                       (1U << FB_FRZ_FR_CH4_pos)

#define FB_FRZ_FR_CH5_pos                       5U
#define FB_FRZ_FR_CH5_msk                       (1U << FB_FRZ_FR_CH5_pos)


/* ============================================================
 * FB_UPD
 * ============================================================ */

#define FB_UPD_UD_CH0_pos                       0U
#define FB_UPD_UD_CH0_msk                       (1U << FB_UPD_UD_CH0_pos)

#define FB_UPD_UD_CH1_pos                       1U
#define FB_UPD_UD_CH1_msk                       (1U << FB_UPD_UD_CH1_pos)

#define FB_UPD_UD_CH2_pos                       2U
#define FB_UPD_UD_CH2_msk                       (1U << FB_UPD_UD_CH2_pos)

#define FB_UPD_UD_CH3_pos                       3U
#define FB_UPD_UD_CH3_msk                       (1U << FB_UPD_UD_CH3_pos)

#define FB_UPD_UD_CH4_pos                       4U
#define FB_UPD_UD_CH4_msk                       (1U << FB_UPD_UD_CH4_pos)

#define FB_UPD_UD_CH5_pos                       5U
#define FB_UPD_UD_CH5_msk                       (1U << FB_UPD_UD_CH5_pos)


/* ============================================================
 * WD_RELOAD
 * ============================================================ */

#define WD_RELOAD_WD_TIME_pos                   0U
#define WD_RELOAD_WD_TIME_msk                   (0x7FFU << WD_RELOAD_WD_TIME_pos)


/* ============================================================
 * DIAG_ERR_CHGR0
 * ============================================================ */

#define DIAG_ERR_CHGR0_OLSG_CH0_pos             0U
#define DIAG_ERR_CHGR0_OLSG_CH0_msk             (1U << DIAG_ERR_CHGR0_OLSG_CH0_pos)

#define DIAG_ERR_CHGR0_OL_CH0_pos               1U
#define DIAG_ERR_CHGR0_OL_CH0_msk               (1U << DIAG_ERR_CHGR0_OL_CH0_pos)

#define DIAG_ERR_CHGR0_OC_CH0_pos               2U
#define DIAG_ERR_CHGR0_OC_CH0_msk               (1U << DIAG_ERR_CHGR0_OC_CH0_pos)

#define DIAG_ERR_CHGR0_SG_CH0_pos               3U
#define DIAG_ERR_CHGR0_SG_CH0_msk               (1U << DIAG_ERR_CHGR0_SG_CH0_pos)

#define DIAG_ERR_CHGR0_OTE_CH0_pos              4U
#define DIAG_ERR_CHGR0_OTE_CH0_msk              (1U << DIAG_ERR_CHGR0_OTE_CH0_pos)

#define DIAG_ERR_CHGR0_OLSG_CH1_pos             8U
#define DIAG_ERR_CHGR0_OLSG_CH1_msk             (1U << DIAG_ERR_CHGR0_OLSG_CH1_pos)

#define DIAG_ERR_CHGR0_OL_CH1_pos               9U
#define DIAG_ERR_CHGR0_OL_CH1_msk               (1U << DIAG_ERR_CHGR0_OL_CH1_pos)

#define DIAG_ERR_CHGR0_OC_CH1_pos              10U
#define DIAG_ERR_CHGR0_OC_CH1_msk              (1U << DIAG_ERR_CHGR0_OC_CH1_pos)

#define DIAG_ERR_CHGR0_SG_CH1_pos              11U
#define DIAG_ERR_CHGR0_SG_CH1_msk              (1U << DIAG_ERR_CHGR0_SG_CH1_pos)

#define DIAG_ERR_CHGR0_OTE_CH1_pos             12U
#define DIAG_ERR_CHGR0_OTE_CH1_msk             (1U << DIAG_ERR_CHGR0_OTE_CH1_pos)


/* ============================================================
 * DIAG_ERR_CHGR1
 * ============================================================ */

#define DIAG_ERR_CHGR1_OLSG_CH2_pos             0U
#define DIAG_ERR_CHGR1_OLSG_CH2_msk             (1U << DIAG_ERR_CHGR1_OLSG_CH2_pos)

#define DIAG_ERR_CHGR1_OL_CH2_pos               1U
#define DIAG_ERR_CHGR1_OL_CH2_msk               (1U << DIAG_ERR_CHGR1_OL_CH2_pos)

#define DIAG_ERR_CHGR1_OC_CH2_pos               2U
#define DIAG_ERR_CHGR1_OC_CH2_msk               (1U << DIAG_ERR_CHGR1_OC_CH2_pos)

#define DIAG_ERR_CHGR1_SG_CH2_pos               3U
#define DIAG_ERR_CHGR1_SG_CH2_msk               (1U << DIAG_ERR_CHGR1_SG_CH2_pos)

#define DIAG_ERR_CHGR1_OTE_CH2_pos              4U
#define DIAG_ERR_CHGR1_OTE_CH2_msk              (1U << DIAG_ERR_CHGR1_OTE_CH2_pos)

#define DIAG_ERR_CHGR1_OLSG_CH3_pos             8U
#define DIAG_ERR_CHGR1_OLSG_CH3_msk             (1U << DIAG_ERR_CHGR1_OLSG_CH3_pos)

#define DIAG_ERR_CHGR1_OL_CH3_pos               9U
#define DIAG_ERR_CHGR1_OL_CH3_msk               (1U << DIAG_ERR_CHGR1_OL_CH3_pos)

#define DIAG_ERR_CHGR1_OC_CH3_pos              10U
#define DIAG_ERR_CHGR1_OC_CH3_msk              (1U << DIAG_ERR_CHGR1_OC_CH3_pos)

#define DIAG_ERR_CHGR1_SG_CH3_pos              11U
#define DIAG_ERR_CHGR1_SG_CH3_msk              (1U << DIAG_ERR_CHGR1_SG_CH3_pos)

#define DIAG_ERR_CHGR1_OTE_CH3_pos             12U
#define DIAG_ERR_CHGR1_OTE_CH3_msk             (1U << DIAG_ERR_CHGR1_OTE_CH3_pos)


/* ============================================================
 * DIAG_ERR_CHGR2
 * ============================================================ */

#define DIAG_ERR_CHGR2_OLSG_CH4_pos             0U
#define DIAG_ERR_CHGR2_OLSG_CH4_msk             (1U << DIAG_ERR_CHGR2_OLSG_CH4_pos)

#define DIAG_ERR_CHGR2_OL_CH4_pos               1U
#define DIAG_ERR_CHGR2_OL_CH4_msk               (1U << DIAG_ERR_CHGR2_OL_CH4_pos)

#define DIAG_ERR_CHGR2_OC_CH4_pos               2U
#define DIAG_ERR_CHGR2_OC_CH4_msk               (1U << DIAG_ERR_CHGR2_OC_CH4_pos)

#define DIAG_ERR_CHGR2_SG_CH4_pos               3U
#define DIAG_ERR_CHGR2_SG_CH4_msk               (1U << DIAG_ERR_CHGR2_SG_CH4_pos)

#define DIAG_ERR_CHGR2_OTE_CH4_pos              4U
#define DIAG_ERR_CHGR2_OTE_CH4_msk              (1U << DIAG_ERR_CHGR2_OTE_CH4_pos)

#define DIAG_ERR_CHGR2_OLSG_CH5_pos             8U
#define DIAG_ERR_CHGR2_OLSG_CH5_msk             (1U << DIAG_ERR_CHGR2_OLSG_CH5_pos)

#define DIAG_ERR_CHGR2_OL_CH5_pos               9U
#define DIAG_ERR_CHGR2_OL_CH5_msk               (1U << DIAG_ERR_CHGR2_OL_CH5_pos)

#define DIAG_ERR_CHGR2_OC_CH5_pos              10U
#define DIAG_ERR_CHGR2_OC_CH5_msk              (1U << DIAG_ERR_CHGR2_OC_CH5_pos)

#define DIAG_ERR_CHGR2_SG_CH5_pos              11U
#define DIAG_ERR_CHGR2_SG_CH5_msk              (1U << DIAG_ERR_CHGR2_SG_CH5_pos)

#define DIAG_ERR_CHGR2_OTE_CH5_pos             12U
#define DIAG_ERR_CHGR2_OTE_CH5_msk             (1U << DIAG_ERR_CHGR2_OTE_CH5_pos)


/* ============================================================
 * DIAG_WARN_CHGR0
 * ============================================================ */

#define DIAG_WARN_CHGR0_PWM_REG_WARN_CH0_pos              0U
#define DIAG_WARN_CHGR0_PWM_REG_WARN_CH0_msk              (1U << DIAG_WARN_CHGR0_PWM_REG_WARN_CH0_pos)

#define DIAG_WARN_CHGR0_I_REG_WARN_CH0_pos                1U
#define DIAG_WARN_CHGR0_I_REG_WARN_CH0_msk                (1U << DIAG_WARN_CHGR0_I_REG_WARN_CH0_pos)

#define DIAG_WARN_CHGR0_OTW_CH0_pos                       2U
#define DIAG_WARN_CHGR0_OTW_CH0_msk                       (1U << DIAG_WARN_CHGR0_OTW_CH0_pos)

#define DIAG_WARN_CHGR0_OLSG_WARN_CH0_pos                 3U
#define DIAG_WARN_CHGR0_OLSG_WARN_CH0_msk                 (1U << DIAG_WARN_CHGR0_OLSG_WARN_CH0_pos)

#define DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH0_pos         4U
#define DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH0_msk         (1U << DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH0_pos)

#define DIAG_WARN_CHGR0_PWM_REG_WARN_CH1_pos              8U
#define DIAG_WARN_CHGR0_PWM_REG_WARN_CH1_msk              (1U << DIAG_WARN_CHGR0_PWM_REG_WARN_CH1_pos)

#define DIAG_WARN_CHGR0_I_REG_WARN_CH1_pos                9U
#define DIAG_WARN_CHGR0_I_REG_WARN_CH1_msk                (1U << DIAG_WARN_CHGR0_I_REG_WARN_CH1_pos)

#define DIAG_WARN_CHGR0_OTW_CH1_pos                      10U
#define DIAG_WARN_CHGR0_OTW_CH1_msk                      (1U << DIAG_WARN_CHGR0_OTW_CH1_pos)

#define DIAG_WARN_CHGR0_OLSG_WARN_CH1_pos                11U
#define DIAG_WARN_CHGR0_OLSG_WARN_CH1_msk                (1U << DIAG_WARN_CHGR0_OLSG_WARN_CH1_pos)

#define DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH1_pos        12U
#define DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH1_msk        (1U << DIAG_WARN_CHGR0_OLSG_WARN_CHK_NOK_CH1_pos)


/* ============================================================
 * DIAG_WARN_CHGR1
 * ============================================================ */

#define DIAG_WARN_CHGR1_PWM_REG_WARN_CH2_pos              0U
#define DIAG_WARN_CHGR1_PWM_REG_WARN_CH2_msk              (1U << DIAG_WARN_CHGR1_PWM_REG_WARN_CH2_pos)

#define DIAG_WARN_CHGR1_I_REG_WARN_CH2_pos                1U
#define DIAG_WARN_CHGR1_I_REG_WARN_CH2_msk                (1U << DIAG_WARN_CHGR1_I_REG_WARN_CH2_pos)

#define DIAG_WARN_CHGR1_OTW_CH2_pos                       2U
#define DIAG_WARN_CHGR1_OTW_CH2_msk                       (1U << DIAG_WARN_CHGR1_OTW_CH2_pos)

#define DIAG_WARN_CHGR1_OLSG_WARN_CH2_pos                 3U
#define DIAG_WARN_CHGR1_OLSG_WARN_CH2_msk                 (1U << DIAG_WARN_CHGR1_OLSG_WARN_CH2_pos)

#define DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH2_pos         4U
#define DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH2_msk         (1U << DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH2_pos)

#define DIAG_WARN_CHGR1_PWM_REG_WARN_CH3_pos              8U
#define DIAG_WARN_CHGR1_PWM_REG_WARN_CH3_msk              (1U << DIAG_WARN_CHGR1_PWM_REG_WARN_CH3_pos)

#define DIAG_WARN_CHGR1_I_REG_WARN_CH3_pos                9U
#define DIAG_WARN_CHGR1_I_REG_WARN_CH3_msk                (1U << DIAG_WARN_CHGR1_I_REG_WARN_CH3_pos)

#define DIAG_WARN_CHGR1_OTW_CH3_pos                      10U
#define DIAG_WARN_CHGR1_OTW_CH3_msk                      (1U << DIAG_WARN_CHGR1_OTW_CH3_pos)

#define DIAG_WARN_CHGR1_OLSG_WARN_CH3_pos                11U
#define DIAG_WARN_CHGR1_OLSG_WARN_CH3_msk                (1U << DIAG_WARN_CHGR1_OLSG_WARN_CH3_pos)

#define DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH3_pos        12U
#define DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH3_msk        (1U << DIAG_WARN_CHGR1_OLSG_WARN_CHK_NOK_CH3_pos)


/* ============================================================
 * DIAG_WARN_CHGR2
 * ============================================================ */

#define DIAG_WARN_CHGR2_PWM_REG_WARN_CH4_pos              0U
#define DIAG_WARN_CHGR2_PWM_REG_WARN_CH4_msk              (1U << DIAG_WARN_CHGR2_PWM_REG_WARN_CH4_pos)

#define DIAG_WARN_CHGR2_I_REG_WARN_CH4_pos                1U
#define DIAG_WARN_CHGR2_I_REG_WARN_CH4_msk                (1U << DIAG_WARN_CHGR2_I_REG_WARN_CH4_pos)

#define DIAG_WARN_CHGR2_OTW_CH4_pos                       2U
#define DIAG_WARN_CHGR2_OTW_CH4_msk                       (1U << DIAG_WARN_CHGR2_OTW_CH4_pos)

#define DIAG_WARN_CHGR2_OLSG_WARN_CH4_pos                 3U
#define DIAG_WARN_CHGR2_OLSG_WARN_CH4_msk                 (1U << DIAG_WARN_CHGR2_OLSG_WARN_CH4_pos)

#define DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH4_pos         4U
#define DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH4_msk         (1U << DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH4_pos)

#define DIAG_WARN_CHGR2_PWM_REG_WARN_CH5_pos              8U
#define DIAG_WARN_CHGR2_PWM_REG_WARN_CH5_msk              (1U << DIAG_WARN_CHGR2_PWM_REG_WARN_CH5_pos)

#define DIAG_WARN_CHGR2_I_REG_WARN_CH5_pos                9U
#define DIAG_WARN_CHGR2_I_REG_WARN_CH5_msk                (1U << DIAG_WARN_CHGR2_I_REG_WARN_CH5_pos)

#define DIAG_WARN_CHGR2_OTW_CH5_pos                      10U
#define DIAG_WARN_CHGR2_OTW_CH5_msk                      (1U << DIAG_WARN_CHGR2_OTW_CH5_pos)

#define DIAG_WARN_CHGR2_OLSG_WARN_CH5_pos                11U
#define DIAG_WARN_CHGR2_OLSG_WARN_CH5_msk                (1U << DIAG_WARN_CHGR2_OLSG_WARN_CH5_pos)

#define DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH5_pos        12U
#define DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH5_msk        (1U << DIAG_WARN_CHGR2_OLSG_WARN_CHK_NOK_CH5_pos)


/* ============================================================
 * FAULT_MASK0
 * ============================================================ */

#define FAULT_MASK0_CH0_ERR_MASK_pos        0U
#define FAULT_MASK0_CH0_ERR_MASK_msk        (1U << FAULT_MASK0_CH0_ERR_MASK_pos)

#define FAULT_MASK0_CH1_ERR_MASK_pos        1U
#define FAULT_MASK0_CH1_ERR_MASK_msk        (1U << FAULT_MASK0_CH1_ERR_MASK_pos)

#define FAULT_MASK0_CH2_ERR_MASK_pos        2U
#define FAULT_MASK0_CH2_ERR_MASK_msk        (1U << FAULT_MASK0_CH2_ERR_MASK_pos)

#define FAULT_MASK0_CH3_ERR_MASK_pos        3U
#define FAULT_MASK0_CH3_ERR_MASK_msk        (1U << FAULT_MASK0_CH3_ERR_MASK_pos)

#define FAULT_MASK0_CH4_ERR_MASK_pos        4U
#define FAULT_MASK0_CH4_ERR_MASK_msk        (1U << FAULT_MASK0_CH4_ERR_MASK_pos)

#define FAULT_MASK0_CH5_ERR_MASK_pos        5U
#define FAULT_MASK0_CH5_ERR_MASK_msk        (1U << FAULT_MASK0_CH5_ERR_MASK_pos)

#define FAULT_MASK0_EN_PIN_MASK_pos        13U
#define FAULT_MASK0_EN_PIN_MASK_msk        (1U << FAULT_MASK0_EN_PIN_MASK_pos)

#define FAULT_MASK0_SUP_NOK_INT_MASK_pos   14U
#define FAULT_MASK0_SUP_NOK_INT_MASK_msk   (1U << FAULT_MASK0_SUP_NOK_INT_MASK_pos)

#define FAULT_MASK0_SUP_NOK_EXT_MASK_pos   15U
#define FAULT_MASK0_SUP_NOK_EXT_MASK_msk   (1U << FAULT_MASK0_SUP_NOK_EXT_MASK_pos)


/* ============================================================
 * FAULT_MASK1
 * ============================================================ */

#define FAULT_MASK1_CH0_WARN_MASK_pos       0U
#define FAULT_MASK1_CH0_WARN_MASK_msk       (1U << FAULT_MASK1_CH0_WARN_MASK_pos)

#define FAULT_MASK1_CH1_WARN_MASK_pos       1U
#define FAULT_MASK1_CH1_WARN_MASK_msk       (1U << FAULT_MASK1_CH1_WARN_MASK_pos)

#define FAULT_MASK1_CH2_WARN_MASK_pos       2U
#define FAULT_MASK1_CH2_WARN_MASK_msk       (1U << FAULT_MASK1_CH2_WARN_MASK_pos)

#define FAULT_MASK1_CH3_WARN_MASK_pos       3U
#define FAULT_MASK1_CH3_WARN_MASK_msk       (1U << FAULT_MASK1_CH3_WARN_MASK_pos)

#define FAULT_MASK1_CH4_WARN_MASK_pos       4U
#define FAULT_MASK1_CH4_WARN_MASK_msk       (1U << FAULT_MASK1_CH4_WARN_MASK_pos)

#define FAULT_MASK1_CH5_WARN_MASK_pos       5U
#define FAULT_MASK1_CH5_WARN_MASK_msk       (1U << FAULT_MASK1_CH5_WARN_MASK_pos)

#define FAULT_MASK1_COTWARN_MASK_pos       12U
#define FAULT_MASK1_COTWARN_MASK_msk       (1U << FAULT_MASK1_COTWARN_MASK_pos)

#define FAULT_MASK1_COTERR_MASK_pos        13U
#define FAULT_MASK1_COTERR_MASK_msk        (1U << FAULT_MASK1_COTERR_MASK_pos)

#define FAULT_MASK1_CLK_LOW_MASK_pos       14U
#define FAULT_MASK1_CLK_LOW_MASK_msk       (1U << FAULT_MASK1_CLK_LOW_MASK_pos)


/* ============================================================
 * FAULT_MASK2
 * ============================================================ */

#define FAULT_MASK2_DATA_ERR_MASK_pos       14U
#define FAULT_MASK2_DATA_ERR_MASK_msk       (1U << FAULT_MASK2_DATA_ERR_MASK_pos)

#define FAULT_MASK2_SPI_WD_MASK_pos         15U
#define FAULT_MASK2_SPI_WD_MASK_msk         (1U << FAULT_MASK2_SPI_WD_MASK_pos)


/* ============================================================
 * CLK_DIV
 * ============================================================ */

#define CLK_DIV_PLL_FBDIV_pos               0U
#define CLK_DIV_PLL_FBDIV_msk               (0x1FFU << CLK_DIV_PLL_FBDIV_pos)

#define CLK_DIV_PLL_REFDIV_pos              9U
#define CLK_DIV_PLL_REFDIV_msk              (0x3FU << CLK_DIV_PLL_REFDIV_pos)

#define CLK_DIV_EXT_CLK_pos                15U
#define CLK_DIV_EXT_CLK_msk                (1U << CLK_DIV_EXT_CLK_pos)


/* ============================================================
 * SFF_BIST
 * ============================================================ */

#define SFF_BIST_SMU_SLF_TST_EN_pos        0U
#define SFF_BIST_SMU_SLF_TST_EN_msk        (1U << SFF_BIST_SMU_SLF_TST_EN_pos)

#define SFF_BIST_SMU_SLF_TST_DONE_pos      1U
#define SFF_BIST_SMU_SLF_TST_DONE_msk      (1U << SFF_BIST_SMU_SLF_TST_DONE_pos)

#define SFF_BIST_SMU_SLF_TST_FAIL_pos      2U
#define SFF_BIST_SMU_SLF_TST_FAIL_msk      (1U << SFF_BIST_SMU_SLF_TST_FAIL_pos)

#define SFF_BIST_SMU_SLF_TST_UERR_pos      3U
#define SFF_BIST_SMU_SLF_TST_UERR_msk      (1U << SFF_BIST_SMU_SLF_TST_UERR_pos)

#define SFF_BIST_SMU_SLF_TST_CERR_pos      4U
#define SFF_BIST_SMU_SLF_TST_CERR_msk      (1U << SFF_BIST_SMU_SLF_TST_CERR_pos)


/* ============================================================
 * ICVID
 * ============================================================ */

#define ICVID_VERSION_pos                   0U
#define ICVID_VERSION_msk                   (0xFFU << ICVID_VERSION_pos)

#define ICVID_MANUFACTURER_pos              8U
#define ICVID_MANUFACTURER_msk              (0xFFU << ICVID_MANUFACTURER_pos)


/* ============================================================
 * PIN_STAT
 * ============================================================ */

#define PIN_STAT_DRV0_pos                   0U
#define PIN_STAT_DRV0_msk                   (1U << PIN_STAT_DRV0_pos)

#define PIN_STAT_DRV1_pos                   1U
#define PIN_STAT_DRV1_msk                   (1U << PIN_STAT_DRV1_pos)

#define PIN_STAT_EN_pos                     4U
#define PIN_STAT_EN_msk                     (1U << PIN_STAT_EN_pos)

#define PIN_STAT_FAULTN_pos                 5U
#define PIN_STAT_FAULTN_msk                 (1U << PIN_STAT_FAULTN_pos)

#define PIN_STAT_FAULTN_FB_pos              6U
#define PIN_STAT_FAULTN_FB_msk              (1U << PIN_STAT_FAULTN_FB_pos)


/* ============================================================
 * FB_STAT
 * ============================================================ */

#define FB_STAT_DIAG_WARN_CHGR0_pos          0U
#define FB_STAT_DIAG_WARN_CHGR0_msk          (1U << FB_STAT_DIAG_WARN_CHGR0_pos)

#define FB_STAT_DIAG_WARN_CHGR1_pos          1U
#define FB_STAT_DIAG_WARN_CHGR1_msk          (1U << FB_STAT_DIAG_WARN_CHGR1_pos)

#define FB_STAT_DIAG_WARN_CHGR2_pos          2U
#define FB_STAT_DIAG_WARN_CHGR2_msk          (1U << FB_STAT_DIAG_WARN_CHGR2_pos)

#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR0_pos  3U
#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR0_msk  (1U << FB_STAT_OLSG_WARN_CHK_NOK_CHGR0_pos)

#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR1_pos  4U
#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR1_msk  (1U << FB_STAT_OLSG_WARN_CHK_NOK_CHGR1_pos)

#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR2_pos  5U
#define FB_STAT_OLSG_WARN_CHK_NOK_CHGR2_msk  (1U << FB_STAT_OLSG_WARN_CHK_NOK_CHGR2_pos)

#define FB_STAT_CLK_NOK_pos                  6U
#define FB_STAT_CLK_NOK_msk                  (1U << FB_STAT_CLK_NOK_pos)

#define FB_STAT_COTERR_pos                   7U
#define FB_STAT_COTERR_msk                   (1U << FB_STAT_COTERR_pos)

#define FB_STAT_COTWARN_pos                  8U
#define FB_STAT_COTWARN_msk                  (1U << FB_STAT_COTWARN_pos)

#define FB_STAT_RES_EVENT_pos                9U
#define FB_STAT_RES_EVENT_msk                (1U << FB_STAT_RES_EVENT_pos)

#define FB_STAT_POR_EVENT_pos               10U
#define FB_STAT_POR_EVENT_msk               (1U << FB_STAT_POR_EVENT_pos)

#define FB_STAT_DATA_ERR_pos                11U
#define FB_STAT_DATA_ERR_msk                (1U << FB_STAT_DATA_ERR_pos)

#define FB_STAT_SUP_NOK_EXT_pos             12U
#define FB_STAT_SUP_NOK_EXT_msk             (1U << FB_STAT_SUP_NOK_EXT_pos)

#define FB_STAT_SUP_NOK_INT_pos             13U
#define FB_STAT_SUP_NOK_INT_msk             (1U << FB_STAT_SUP_NOK_INT_pos)

#define FB_STAT_ERR_CHGR0_pos               14U
#define FB_STAT_ERR_CHGR0_msk               (1U << FB_STAT_ERR_CHGR0_pos)

#define FB_STAT_ERR_CHGR1_pos               15U
#define FB_STAT_ERR_CHGR1_msk               (1U << FB_STAT_ERR_CHGR1_pos)

#define FB_STAT_ERR_CHGR2_pos               16U
#define FB_STAT_ERR_CHGR2_msk               (1U << FB_STAT_ERR_CHGR2_pos)

#define FB_STAT_SPI_WD_ERR_pos              20U
#define FB_STAT_SPI_WD_ERR_msk              (1U << FB_STAT_SPI_WD_ERR_pos)

#define FB_STAT_INIT_DONE_pos               21U
#define FB_STAT_INIT_DONE_msk               (1U << FB_STAT_INIT_DONE_pos)


/* ============================================================
 * FB_VOLTAGE1
 * ============================================================ */

#define FB_VOLTAGE1_VIO_pos                  0U
#define FB_VOLTAGE1_VIO_msk                  (0x7FFU << FB_VOLTAGE1_VIO_pos)

#define FB_VOLTAGE1_VDD_pos                 11U
#define FB_VOLTAGE1_VDD_msk                 (0x7FFU << FB_VOLTAGE1_VDD_pos)


/* ============================================================
 * FB_VOLTAGE2
 * ============================================================ */

#define FB_VOLTAGE2_TEMP_VALUE_pos           0U
#define FB_VOLTAGE2_TEMP_VALUE_msk           (0x7FFU << FB_VOLTAGE2_TEMP_VALUE_pos)

#define FB_VOLTAGE2_VBAT_pos                11U
#define FB_VOLTAGE2_VBAT_msk                (0x7FFU << FB_VOLTAGE2_VBAT_pos)


/* ============================================================
 * CHIPID0
 * ============================================================ */

#define CHIPID0_ID0_pos                      0U
#define CHIPID0_ID0_msk                      (0xFFFFU << CHIPID0_ID0_pos)


/* ============================================================
 * CHIPID1
 * ============================================================ */

#define CHIPID1_ID1_pos                      0U
#define CHIPID1_ID1_msk                      (0xFFFFU << CHIPID1_ID1_pos)


/* ============================================================
 * CHIPID2
 * ============================================================ */

#define CHIPID2_ID2_pos                      0U
#define CHIPID2_ID2_msk                      (0xFFFFU << CHIPID2_ID2_pos)

/* ============================================================
 * CHANNEL SPECIFIC MASKS
 * ============================================================ */
/* ============================================================
 * SETPOINT
 * ============================================================ */

#define SETPOINT_TARGET_pos                 0U
#define SETPOINT_TARGET_msk                 (0x7FFFU << SETPOINT_TARGET_pos)

#define SETPOINT_AUTO_LIMIT_DIS_pos         15U
#define SETPOINT_AUTO_LIMIT_DIS_msk         (1U << SETPOINT_AUTO_LIMIT_DIS_pos)


/* ============================================================
 * CTRL
 * ============================================================ */

#define CTRL_MIN_INT_THRESH_pos             0U
#define CTRL_MIN_INT_THRESH_msk             (0xFFU << CTRL_MIN_INT_THRESH_pos)

#define CTRL_PWM_PERIOD_CALC_MODE_pos       8U
#define CTRL_PWM_PERIOD_CALC_MODE_msk       (1U << CTRL_PWM_PERIOD_CALC_MODE_pos)

#define CTRL_OLSG_WARN_WINDOW_pos           9U
#define CTRL_OLSG_WARN_WINDOW_msk           (0x1FU << CTRL_OLSG_WARN_WINDOW_pos)

#define CTRL_OLSG_WARN_EN_pos               14U
#define CTRL_OLSG_WARN_EN_msk               (1U << CTRL_OLSG_WARN_EN_pos)


/* ============================================================
 * PERIOD
 * ============================================================ */

#define PERIOD_PERIOD_MANT_pos              0U
#define PERIOD_PERIOD_MANT_msk              (0xFFU << PERIOD_PERIOD_MANT_pos)

#define PERIOD_PERIOD_EXP_pos               8U
#define PERIOD_PERIOD_EXP_msk               (0x07U << PERIOD_PERIOD_EXP_pos)

#define PERIOD_LOW_FREQ_RANGE_EN_pos        11U
#define PERIOD_LOW_FREQ_RANGE_EN_msk        (1U << PERIOD_LOW_FREQ_RANGE_EN_pos)

#define PERIOD_PWM_CTRL_PARAM_pos           12U
#define PERIOD_PWM_CTRL_PARAM_msk           (0x0FU << PERIOD_PWM_CTRL_PARAM_pos)


/* ============================================================
 * INTEGRATOR_LIMIT
 * ============================================================ */

#define INTEGRATOR_LIMIT_LIM_VALUE_ABS_pos      0U
#define INTEGRATOR_LIMIT_LIM_VALUE_ABS_msk      (0x03FFU << INTEGRATOR_LIMIT_LIM_VALUE_ABS_pos)

#define INTEGRATOR_LIMIT_AUTO_LIM_VALUE_ABS_pos 10U
#define INTEGRATOR_LIMIT_AUTO_LIM_VALUE_ABS_msk (0x1FU << INTEGRATOR_LIMIT_AUTO_LIM_VALUE_ABS_pos)


/* ============================================================
 * DITHER_CLK_DIV
 * ============================================================ */

#define DITHER_CLK_DIV_MANT_pos             0U
#define DITHER_CLK_DIV_MANT_msk             (0x03FFU << DITHER_CLK_DIV_MANT_pos)

#define DITHER_CLK_DIV_EXP_pos              10U
#define DITHER_CLK_DIV_EXP_msk              (0x0FU << DITHER_CLK_DIV_EXP_pos)

#define DITHER_CLK_DIV_DITHER_PWM_SYNC_EN_pos   14U
#define DITHER_CLK_DIV_DITHER_PWM_SYNC_EN_msk   (1U << DITHER_CLK_DIV_DITHER_PWM_SYNC_EN_pos)

#define DITHER_CLK_DIV_DITHER_SETPOINT_SYNC_EN_pos 15U
#define DITHER_CLK_DIV_DITHER_SETPOINT_SYNC_EN_msk (1U << DITHER_CLK_DIV_DITHER_SETPOINT_SYNC_EN_pos)


/* ============================================================
 * DITHER_STEP
 * ============================================================ */

#define DITHER_STEP_FLAT_pos                0U
#define DITHER_STEP_FLAT_msk                (0xFFU << DITHER_STEP_FLAT_pos)

#define DITHER_STEP_STEPS_pos               8U
#define DITHER_STEP_STEPS_msk               (0xFFU << DITHER_STEP_STEPS_pos)


/* ============================================================
 * DITHER_CTRL
 * ============================================================ */

#define DITHER_CTRL_STEP_SIZE_pos           0U
#define DITHER_CTRL_STEP_SIZE_msk           (0x0FFFU << DITHER_CTRL_STEP_SIZE_pos)

#define DITHER_CTRL_DEEP_DITHER_pos         13U
#define DITHER_CTRL_DEEP_DITHER_msk         (1U << DITHER_CTRL_DEEP_DITHER_pos)

#define DITHER_CTRL_FAST_MEAS_pos           14U
#define DITHER_CTRL_FAST_MEAS_msk           (0x03U << DITHER_CTRL_FAST_MEAS_pos)


/* ============================================================
 * CH_CONFIG
 * ============================================================ */

#define CH_CONFIG_SLEWR_pos                 0U
#define CH_CONFIG_SLEWR_msk                 (0x03U << CH_CONFIG_SLEWR_pos)

#define CH_CONFIG_I_DIAG_pos                2U
#define CH_CONFIG_I_DIAG_msk                (0x03U << CH_CONFIG_I_DIAG_pos)

#define CH_CONFIG_OL_TH_pos                 4U
#define CH_CONFIG_OL_TH_msk                 (0x07U << CH_CONFIG_OL_TH_pos)

#define CH_CONFIG_OL_TH_FIXED_pos           7U
#define CH_CONFIG_OL_TH_FIXED_msk           (0x3FU << CH_CONFIG_OL_TH_FIXED_pos)

#define CH_CONFIG_OC_DIAG_EN_pos            13U
#define CH_CONFIG_OC_DIAG_EN_msk            (1U << CH_CONFIG_OC_DIAG_EN_pos)

#define CH_CONFIG_OFF_DIAG_CH_pos            14U
#define CH_CONFIG_OFF_DIAG_CH_msk            (0x03U << CH_CONFIG_OFF_DIAG_CH_pos)


/* ============================================================
 * MODE
 * ============================================================ */

#define MODE_CH_MODE_pos                    0U
#define MODE_CH_MODE_msk                    (0x0FU << MODE_CH_MODE_pos)


/* ============================================================
 * TON
 * ============================================================ */

#define TON_TON_MANT_pos                    0U
#define TON_TON_MANT_msk                    (0x03FFU << TON_TON_MANT_pos)

#define TON_OLSG_TIMEOUT_pos                10U
#define TON_OLSG_TIMEOUT_msk                (0x3FU << TON_OLSG_TIMEOUT_pos)


/* ============================================================
 * CTRL_INT_THRESH
 * ============================================================ */

#define CTRL_INT_THRESH_INT_THRESH_pos      0U
#define CTRL_INT_THRESH_INT_THRESH_msk      (0x01FFU << CTRL_INT_THRESH_pos)

/* ============================================================
 * FB_DC
 * ============================================================ */

#define FB_DC_TP_MANT_pos                  0U
#define FB_DC_TP_MANT_msk                  (0x7FFU << FB_DC_TP_MANT_pos)

#define FB_DC_TO_MANT_pos                  11U
#define FB_DC_TO_MANT_msk                  (0x7FFU << FB_DC_TO_MANT_pos)


/* ============================================================
 * FB_VBAT
 * ============================================================ */

#define FB_VBAT_VBAT_AVG_MANT_pos          0U
#define FB_VBAT_VBAT_AVG_MANT_msk          (0x7FFU << FB_VBAT_VBAT_AVG_MANT_pos)

#define FB_VBAT_EXP_pos                    12U
#define FB_VBAT_EXP_msk                    (0x0FU << FB_VBAT_EXP_pos)

#define FB_VBAT_DITHER_QUAD_CNT_pos        16U
#define FB_VBAT_DITHER_QUAD_CNT_msk        (0x03U << FB_VBAT_DITHER_QUAD_CNT_pos)

#define FB_VBAT_DITHER_PERIOD_CNT_pos      18U
#define FB_VBAT_DITHER_PERIOD_CNT_msk      (0x03U << FB_VBAT_DITHER_PERIOD_CNT_pos)


/* ============================================================
 * FB_I_AVG
 * ============================================================ */

#define FB_I_AVG_I_AVG_MANT_pos            0U
#define FB_I_AVG_I_AVG_MANT_msk            (0x0FFFU << FB_I_AVG_I_AVG_MANT_pos)

#define FB_I_AVG_EXP_pos                   12U
#define FB_I_AVG_EXP_msk                   (0x0FU << FB_I_AVG_EXP_pos)


/* ============================================================
 * FB_IMIN_IMAX
 * ============================================================ */

#define FB_IMIN_IMAX_IMIN_pos              0U
#define FB_IMIN_IMAX_IMIN_msk              (0x01FFU << FB_IMIN_IMAX_IMIN_pos)

#define FB_IMIN_IMAX_IMAX_pos              10U
#define FB_IMIN_IMAX_IMAX_msk              (0x01FFU << FB_IMIN_IMAX_IMAX_pos)


/* ============================================================
 * FB_I_AVG_s16
 * ============================================================ */

#define FB_I_AVG_s16_I_AVG_s16_pos         0U
#define FB_I_AVG_s16_I_AVG_s16_msk         (0x1FFFFU << FB_I_AVG_s16_I_AVG_s16_pos)

#define FB_I_AVG_s16_TIME_STAMP_pos        20U
#define FB_I_AVG_s16_TIME_STAMP_msk        (0x03U << FB_I_AVG_s16_TIME_STAMP_pos)


/* ============================================================
 * FB_INT_THRESH
 * ============================================================ */

#define FB_INT_THRESH_INT_THRESH_VAL_pos   0U
#define FB_INT_THRESH_INT_THRESH_VAL_msk   (0xFFFFU << FB_INT_THRESH_INT_THRESH_VAL_pos)


/* ============================================================
 * FB_PERIOD_MIN_MAX
 * ============================================================ */

#define FB_PERIOD_MIN_MAX_PMIN_pos         0U
#define FB_PERIOD_MIN_MAX_PMIN_msk         (0x03FFU << FB_PERIOD_MIN_MAX_PMIN_pos)

#define FB_PERIOD_MIN_MAX_PMAX_pos         10U
#define FB_PERIOD_MIN_MAX_PMAX_msk         (0x03FFU << FB_PERIOD_MIN_MAX_PMAX_pos)


#define F_SYS_HZ    28000000UL

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
    reply_22bit_t fields;
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

typedef struct faults_tle_
{
    /* GLOBAL_DIAG0 */

    bool vbat_under_volt_flt;
    bool vbat_over_volt_flt;

    bool vio_under_volt_flt;
    bool vio_over_volt_flt;

    bool vdd_under_volt_flt;
    bool vdd_over_volt_flt;

    bool clock_flt;

    bool central_over_temp_err;
    bool central_over_temp_warn;

    bool reset_event;
    bool power_on_reset_event;

    bool spi_watchdog_flt;


    /* GLOBAL_DIAG1 */

    bool vref_i_under_volt_flt;
    bool vref_i_over_volt_flt;

    bool vdd_2v5_under_volt_flt;
    bool vdd_2v5_over_volt_flt;

    bool ref_under_volt_flt;
    bool ref_over_volt_flt;

    bool vpre_over_volt_flt;

    bool hv_adc_flt;


    /* GLOBAL_DIAG2 */

    bool register_ecc_flt;
    bool otp_ecc_flt;
    bool otp_virgin_flt;

} faults_tle_t;

typedef struct fb_status_data_
{
    /* PIN_STAT */

    bool drv0_status;
    bool drv1_status;
    bool en_status;
    bool faultn_status;
    bool faultn_fb_status;


    /* FB_STAT */

    bool coterr_status;
    bool cotwarn_status;
    bool reset_event_status;
    bool por_event_status;
    bool data_err_status;
    bool supply_ext_fault_status;
    bool supply_int_fault_status;

    bool err_chgr0_status;
    bool err_chgr1_status;
    bool err_chgr2_status;

    bool spi_watchdog_status;
    bool init_done_status;


    /* FB_VOLTAGE1 */

    uint16_t vio_voltage_raw;
    uint16_t vdd_voltage_raw;

    float vio_voltage;
    float vdd_voltage;


    /* FB_VOLTAGE2 */

    uint16_t temp_raw;
    uint16_t vbat_voltage_raw;

    float temperature;
    float vbat_voltage;

} fb_status_data_t;

typedef struct channel_fb_status_data_
{
    /* ============================================================
     * FB_DC
     * ============================================================ */

    uint16_t tp_mant_raw;
    uint16_t to_mant_raw;

    float duty_cycle;


    /* ============================================================
     * FB_VBAT
     * ============================================================ */

    uint16_t vbat_avg_mant_raw;
    uint8_t  vbat_exp;

    float vbat_voltage;


    /* ============================================================
     * FB_I_AVG
     * ============================================================ */

    int16_t i_avg_mant_raw;
    uint8_t i_avg_exp;

    float average_current;


    /* ============================================================
     * FB_IMIN_IMAX
     * ============================================================ */

    int16_t i_min_raw;
    int16_t i_max_raw;

    float minimum_current;
    float maximum_current;


    /* ============================================================
     * FB_I_AVG_s16
     * ============================================================ */

    int16_t i_avg_s16_raw;
    uint8_t timestamp;

    float average_current_s16;


    /* ============================================================
     * FB_INT_THRESH
     * ============================================================ */

    uint16_t integrator_threshold;


    /* ============================================================
     * FB_PERIOD_MIN_MAX
     * ============================================================ */

    uint16_t pwm_period_min_raw;
    uint16_t pwm_period_max_raw;

    float pwm_frequency_min;
    float pwm_frequency_max;

} channel_fb_status_data_t;


extern fb_status_data_t fb_status_data;
extern faults_tle_t tle_central_faults;
extern channel_fb_status_data_t channel_fb_status_data[6];
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


extern reply_16bit_union_t set_point_value[6];

extern reply_16bit_union_t ctrl_value[6];

extern reply_16bit_union_t period_value[6];

extern reply_16bit_union_t integrator_limit_value[6];

extern reply_16bit_union_t dither_clk_div_value[6];

extern reply_16bit_union_t dither_step_value[6];

extern reply_16bit_union_t dither_ctrl_value[6];

extern reply_16bit_union_t ch_config_value[6];

extern reply_16bit_union_t mode_value[6];

extern reply_16bit_union_t ton_value[6];

extern reply_16bit_union_t ctrl_int_thresh_value[6];

extern reply_22bit_union_t fb_dc_value[6];

extern reply_22bit_union_t fb_vbat_value[6];

extern reply_22bit_union_t fb_i_avg_value[6];

extern reply_22bit_union_t fb_imin_imax_value[6];

extern reply_22bit_union_t fb_i_avg_s16_value[6];

extern reply_22bit_union_t fb_int_thresh_value[6];

extern reply_22bit_union_t fb_period_min_max_value[6];


uint32_t tle_read_glb_cfg(uint16_t *glb_cfg);

uint32_t tle_read_version(uint16_t *version);

uint32_t tle_read_ch_ctrl(uint16_t *ch_ctrl);

uint32_t tle_read_glb_diag0(uint16_t *glb_diag0);

uint32_t tle_read_glb_diag1(uint16_t *glb_diag1);

uint32_t tle_read_glb_diag2(uint16_t *glb_diag2);

uint32_t tle_read_vbat_th(uint16_t *vbat_th);

uint32_t tle_read_fb_frz(uint16_t *fb_frz);

uint32_t tle_read_fb_upd(uint16_t *fb_upd);

uint32_t tle_read_wd_reload(uint16_t *wd_reload);

uint32_t tle_read_diag_err_chgr(uint8_t channel,
                                 uint16_t *diag_err_chgr);

uint32_t tle_read_diag_warn_chgr(uint8_t channel,
                                  uint16_t *diag_warn_chgr);

uint32_t tle_read_fault_mask0(uint16_t *fault_mask0);

uint32_t tle_read_fault_mask1(uint16_t *fault_mask1);

uint32_t tle_read_fault_mask2(uint16_t *fault_mask2);

uint32_t tle_read_clk_div(uint16_t *clk_div);

uint32_t tle_read_sff_bist(uint16_t *sff_bist);

uint32_t tle_read_pin_stat(uint16_t *pin_stat);

uint32_t tle_read_fb_stat(uint32_t *fb_stat);

uint32_t tle_read_fb_voltage1(uint32_t *fb_voltage1);

uint32_t tle_read_fb_voltage2(uint32_t *fb_voltage2);


uint32_t tle_write_ch_ctrl(uint16_t value);

uint32_t tle_write_glb_cfg(uint16_t value);

uint32_t tle_write_glb_diag0(uint16_t value);

uint32_t tle_write_glb_diag1(uint16_t value);

uint32_t tle_write_glb_diag2(uint16_t value);

uint32_t tle_write_vbat_th(uint16_t value);

uint32_t tle_write_fb_frz(uint16_t value);

uint32_t tle_write_fb_upd(uint16_t value);

uint32_t tle_write_wd_reload(uint16_t value);

uint32_t tle_write_diag_err_chgr(uint8_t channel, uint16_t value);

uint32_t tle_write_diag_warn_chgr(uint8_t channel, uint16_t value);

uint32_t tle_write_fault_mask0(uint16_t value);

uint32_t tle_write_fault_mask1(uint16_t value);

uint32_t tle_write_fault_mask2(uint16_t value);

uint32_t tle_write_clk_div(uint16_t value);

uint32_t tle_write_sff_bist(uint16_t value);

/*Channel regs*/

uint32_t tle_read_setpoint(uint32_t base,
                           uint8_t channel,
                           uint16_t *setpoint);

uint32_t tle_read_ctrl(uint32_t base,
                       uint8_t channel,
                       uint16_t *ctrl);

uint32_t tle_read_period(uint32_t base,
                         uint8_t channel,
                         uint16_t *period);

uint32_t tle_read_integrator_limit(uint32_t base,
                                   uint8_t channel,
                                   uint16_t *integrator_limit);

uint32_t tle_read_dither_clk_div(uint32_t base,
                                 uint8_t channel,
                                 uint16_t *dither_clk_div);

uint32_t tle_read_dither_step(uint32_t base,
                              uint8_t channel,
                              uint16_t *dither_step);

uint32_t tle_read_dither_ctrl(uint32_t base,
                              uint8_t channel,
                              uint16_t *dither_ctrl);

uint32_t tle_read_ch_config(uint32_t base,
                            uint8_t channel,
                            uint16_t *ch_config);

uint32_t tle_read_mode(uint32_t base,
                       uint8_t channel,
                       uint16_t *mode);

uint32_t tle_read_ton(uint32_t base,
                      uint8_t channel,
                      uint16_t *ton);

uint32_t tle_read_ctrl_int_thresh(uint32_t base,
                                  uint8_t channel,
                                  uint16_t *ctrl_int_thresh);


uint32_t tle_read_fb_dc(uint32_t base,
                        uint8_t channel,
                        uint32_t *fb_dc);

uint32_t tle_read_fb_vbat(uint32_t base,
                          uint8_t channel,
                          uint32_t *fb_vbat);

uint32_t tle_read_fb_i_avg(uint32_t base,
                           uint8_t channel,
                           uint32_t *fb_i_avg);

uint32_t tle_read_fb_imin_imax(uint32_t base,
                               uint8_t channel,
                               uint32_t *fb_imin_imax);

uint32_t tle_read_fb_i_avg_s16(uint32_t base,
                               uint8_t channel,
                               uint32_t *fb_i_avg_s16);

uint32_t tle_read_fb_int_thresh(uint32_t base,
                                uint8_t channel,
                                uint32_t *fb_int_thresh);

uint32_t tle_read_fb_period_min_max(uint32_t base,
                                    uint8_t channel,
                                    uint32_t *fb_period_min_max);

uint32_t tle_write_setpoint(uint8_t channel, uint16_t value);
uint32_t tle_write_ctrl(uint8_t channel, uint16_t value);
uint32_t tle_write_period(uint8_t channel, uint16_t value);
uint32_t tle_write_integrator_limit(uint8_t channel, uint16_t value);
uint32_t tle_write_dither_clk_div(uint8_t channel, uint16_t value);
uint32_t tle_write_dither_step(uint8_t channel, uint16_t value);
uint32_t tle_write_dither_ctrl(uint8_t channel, uint16_t value);
uint32_t tle_write_ch_config(uint8_t channel, uint16_t value);

uint32_t tle_write_mode(uint8_t channel, uint16_t value);
uint32_t tle_write_ton(uint8_t channel, uint16_t value);
uint32_t tle_write_ctrl_int_thresh(uint8_t channel, uint16_t value);

uint8_t tle_read_central_diag(void);
uint8_t tle_read_channel_diag(uint8_t channel);
void update_fault_structure(void);
void update_fb_status_data(void);
uint8_t tle_read_channel_diag(uint8_t channel);
#endif