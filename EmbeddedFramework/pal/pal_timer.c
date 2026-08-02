#include "pal_timer.h"
#include "tim.h"



pal_status_t pal_timer_register_callback(
        board_timer_t timer,
        pal_callback_t callback,
        void *context)
{
    PAL_ASSERT_TIMER(timer);

    timer_callbacks[timer].callback = callback;
    timer_callbacks[timer].context  = context;

    return PAL_STATUS_OK;
}

pal_status_t pal_timer_delay_ms(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);

    return PAL_STATUS_OK;
}

uint32_t pal_timer_get_tick(void)
{
    return HAL_GetTick();
}

pal_status_t pal_timer_start(board_timer_t timer)
{
    PAL_ASSERT_TIMER(timer);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    return pal_status_from_hal(
        HAL_TIM_Base_Start(timer_map->handle));
}

pal_status_t pal_timer_stop(board_timer_t timer)
{
    PAL_ASSERT_TIMER(timer);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    return pal_status_from_hal(
        HAL_TIM_Base_Stop(timer_map->handle));
}

pal_status_t pal_timer_set_period(board_timer_t timer,
                                  uint32_t period)
{
    PAL_ASSERT_TIMER(timer);
    PAL_ASSERT(period > 0U, PAL_STATUS_INVALID_PARAM);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    __HAL_TIM_SET_AUTORELOAD(timer_map->handle, period);

    return PAL_STATUS_OK;
}

pal_status_t pal_timer_get_counter(board_timer_t timer,
                                   uint32_t *counter)
{
    PAL_ASSERT_TIMER(timer);
    PAL_ASSERT_NULL(counter);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    *counter = __HAL_TIM_GET_COUNTER(timer_map->handle);

    return PAL_STATUS_OK;
}

pal_status_t pal_timer_set_counter(board_timer_t timer,
                                   uint32_t counter)
{
    PAL_ASSERT_TIMER(timer);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    __HAL_TIM_SET_COUNTER(timer_map->handle, counter);

    return PAL_STATUS_OK;
}

pal_status_t pal_timer_start_it(board_timer_t timer)
{
    PAL_ASSERT_TIMER(timer);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    HAL_StatusTypeDef status =
        HAL_TIM_Base_Start_IT(timer_map->handle);

    return pal_status_from_hal(status);
}

pal_status_t pal_timer_stop_it(board_timer_t timer)
{
    PAL_ASSERT_TIMER(timer);

    const board_timer_map_t *timer_map = &board_timer_map[timer];

    HAL_StatusTypeDef status =
        HAL_TIM_Base_Stop_IT(timer_map->handle);

    return pal_status_from_hal(status);
}