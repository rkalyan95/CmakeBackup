#ifndef PAL_STATUS_H
#define PAL_STATUS_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    PAL_STATUS_OK,

    PAL_STATUS_ERROR,

    PAL_STATUS_TIMEOUT,

    PAL_STATUS_INVALID_PARAM

} pal_status_t;

#define PAL_ASSERT(condition, status)      \
    do                                     \
    {                                      \
        if (!(condition))                  \
        {                                  \
            return (status);               \
        }                                  \
    } while (0)

#endif