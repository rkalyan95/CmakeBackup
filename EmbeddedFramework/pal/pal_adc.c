#include "pal_adc.h"

pal_status_t pal_adc_read(board_adc_t adc,
                          adc_value_t *value)
{
    PAL_ASSERT_ADC(adc);
    PAL_ASSERT_NULL(value);

    const board_adc_map_t *adc_map =
        &board_adc_map[adc];

    HAL_StatusTypeDef status;

    status = HAL_ADC_Start(adc_map->handle);

    if (status != HAL_OK)
    {
        return pal_status_from_hal(status);
    }

    status = HAL_ADC_PollForConversion(
                    adc_map->handle,
                    HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        HAL_ADC_Stop(adc_map->handle);
        return pal_status_from_hal(status);
    }

    *value = (adc_value_t)
        HAL_ADC_GetValue(adc_map->handle);

    status = HAL_ADC_Stop(adc_map->handle);

    if (status != HAL_OK)
    {
        return pal_status_from_hal(status);
    }

    return PAL_STATUS_OK;
}