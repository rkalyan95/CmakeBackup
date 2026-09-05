#ifndef WOLV_CLAW_H
#define WOLV_CLAW_H

#include "tle92466.h"

typedef enum
{
    ACT_OK = 0,
    ACT_RESN_NOT_OK,
    ACT_EN_NOT_OK,
    ACT_CENTRAL_FAULT,
    ACT_FB_STAT_FAULT,
    ACT_INIT_NOT_DONE,
    ACT_CONFIG_FAILED,
    ACT_SETPOINT_FAILED,
    ACT_ENABLE_FAILED,
    ACT_CHANNEL_FAULT
} actuator_status_t;


/* Replace with the actual 16-bit setpoint encoding for 500 mA */
#define TLE_SETPOINT_500MA      0x5000U
#define TLE_SETPOINT_HOLDMA     0x0000U

actuator_status_t actuator_run(uint8_t load_ch , uint16_t setpoint);
void wolverin_log_diagonstics(void);
actuator_status_t actuator_hold(void);
#endif