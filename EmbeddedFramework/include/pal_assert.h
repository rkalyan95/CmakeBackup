#ifndef PAL_ASSERT_H
#define PAL_ASSERT_H

#include "pal_status.h"

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

#endif