#ifndef PAL_ASSERT_H
#define PAL_ASSERT_H

#include "pal_status.h"
#include "board.h"
#define PAL_ASSERT(condition, status)      \
    do                                     \
    {                                      \
        if (!(condition))                  \
        {                                  \
            return (status);               \
        }                                  \
    } while (0)



#define PAL_ASSERT_NULL(ptr) \
    PAL_ASSERT((ptr) != NULL, PAL_STATUS_INVALID_PARAM)


inline static pal_status_t pal_status_from_hal(HAL_StatusTypeDef status)
{
    switch (status)
    {
        case HAL_OK:      return PAL_STATUS_OK;
        case HAL_TIMEOUT: return PAL_STATUS_TIMEOUT;
        default:          return PAL_STATUS_ERROR;
    }
}
#endif