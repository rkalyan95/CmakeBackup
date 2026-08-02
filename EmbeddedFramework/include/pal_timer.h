#ifndef PAL_TIMER_H
#define PAL_TIMER_H

#include "board.h"
#include "pal_status.h"
#include "pal_assert.h"
#include "pal_callback.h"

typedef void (*pal_callback_t)(void *context);

pal_status_t pal_timer_delay_ms(uint32_t milliseconds);

uint32_t pal_timer_get_tick(void);


#define PAL_ASSERT_TIMER(timer) \
    PAL_ASSERT((timer) < BOARD_TIMER_COUNT, PAL_STATUS_INVALID_PARAM)

pal_status_t pal_timer_register_callback(
    board_timer_t timer,
    pal_callback_t callback,
    void *context);

pal_status_t pal_timer_start(board_timer_t timer);

pal_status_t pal_timer_stop(board_timer_t timer);

pal_status_t pal_timer_set_period(board_timer_t timer,
                                  uint32_t period);

pal_status_t pal_timer_get_counter(board_timer_t timer,
                                   uint32_t *counter);
pal_status_t pal_timer_set_counter(board_timer_t timer,
                                   uint32_t counter);

pal_status_t pal_timer_start_it(board_timer_t timer);
pal_status_t pal_timer_stop_it(board_timer_t timer);
#endif