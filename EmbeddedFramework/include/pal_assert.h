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

#endif