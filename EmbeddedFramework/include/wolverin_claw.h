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


actuator_status_t actuator_init_and_start(void);

#endif