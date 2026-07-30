#include "dummy.h"
#include "stm32l4xx_hal.h"
#include "gpio.h" // If you generated GPIO init

// Tell the compiler that SystemClock_Config exists in CubeMX's main.c
#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif


void dummy(void)
{
    // 1. Initialize HAL
    HAL_Init();

    // 2. Call the CubeMX clock config directly from main.c
    SystemClock_Config();

    // 3. Call peripheral inits (MX_GPIO_Init, etc.)
    MX_GPIO_Init();

}

void toggle(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,1);
    HAL_Delay(500); // 500 ms delay 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0);
     HAL_Delay(500); // 500 ms delay 
}