#ifndef PAL_ADC_H
#define PAL_ADC_H

#include "board.h"
#include "pal_status.h"
#include "pal_assert.h"

#define PAL_ASSERT_ADC(adc) \
    PAL_ASSERT((adc) < BOARD_ADC_COUNT, PAL_STATUS_INVALID_PARAM)

typedef uint16_t adc_value_t;

pal_status_t pal_adc_read(board_adc_t adc,
                          adc_value_t *value);

#endif