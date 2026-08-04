#include "pal_gpio.h"
#include "pal_usart.h"
#include "pal_spi.h"
#include "spi.h"
#include "pal_timer.h"
#include "logger.h"
#include "pal_adc.h"
#include <stdio.h>
#include "cli.h"
#include "tle92466.h"
static void cmd_led(uint8_t argc, char *argv[])
{
    (void)argc;
    (void)argv;

    pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

    log_info("LED toggled");
}

static void cmd_adc(uint8_t argc, char *argv[])
{
    (void)argc;
    (void)argv;

    adc_value_t value;

    if(pal_adc_read(BOARD_ADC_1, &value) == PAL_STATUS_OK)
    {
        log_info("ADC = %u", value);
    }
    else
    {
        log_error("ADC Read Failed");
    }
}

void test_blink(void)
{
     pal_gpio_toggle(BOARD_GPIO_LED_STATUS);
     pal_timer_delay_ms(1000);
}

void test_uart_transmit(void)
{
    const uint8_t message[] =
        "\r\nHello from EmbeddedFramework!\r\n";  
     pal_uart_transmit(
            BOARD_UART_DEBUG,
            message,
            sizeof(message) - 1U);
}

void test_spi_tle(void)
{

     pal_gpio_write(BOARD_GPIO_TLE_RST,0);
     pal_timer_delay_ms(200);
     pal_gpio_write(BOARD_GPIO_TLE_RST,1);
     pal_timer_delay_ms(200);
     pal_gpio_write(BOARD_GPIO_TLE_EN,1);
     pal_timer_delay_ms(200);



}

static void led_timer_callback(void *context)
{
    (void)context;
    uint16_t adc_value = 16234;
    pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

   /* const uint8_t message[] = "Timer Interrupt!\r\n";
    pal_uart_transmit(
        BOARD_UART_DEBUG,
        message,
        sizeof(message) - 1);
        */

    logger_log(LOG_LEVEL_INFO,
           "ADC = %u",
           adc_value);
}


void test_timer(void)
{
pal_timer_start(BOARD_TIMER_1);

while (1)
{
    uint32_t count;

    pal_timer_get_counter(BOARD_TIMER_1, &count);

    if (count > 100000)
    {
        pal_gpio_toggle(BOARD_GPIO_LED_STATUS);

        pal_timer_set_counter(BOARD_TIMER_1, 0);
        test_uart_transmit();
    }
}
}

void test_timer_interrupt(void)
{
    /* Configure timer for 1 second */
    pal_timer_stop(BOARD_TIMER_1);

    pal_timer_set_counter(BOARD_TIMER_1, 0);
    pal_timer_set_period(BOARD_TIMER_1, 999999);

    pal_timer_register_callback(
        BOARD_TIMER_1,
        led_timer_callback,
        NULL);

    pal_timer_start_it(BOARD_TIMER_1);

    while (1)
    {
        /* Do other work */
    }
}

void test_adc(void)
{
    adc_value_t value;

    while (1)
    {
        if (pal_adc_read(BOARD_ADC_1, &value) == PAL_STATUS_OK)
        {
            printf("ADC = %u\r\n", value);
        }

        pal_timer_delay_ms(500);
    }
}

void test_cli(void)
{
    cli_init();
cli_register("adc-read",
             1,
             1,
             "adc-read",
             cmd_adc);
#if 0
cli_register("tle-init",
             1,
             1,
             "tle-init",
             cmd_tle_init);

cli_register("tle-read",
             2,
             2,
             "tle-read <register>",
             cmd_tle_read);

cli_register("tle-write",
             3,
             3,
             "tle-write <register> <value>",
             cmd_tle_write);
#endif
cli_register("gpio-toggle",
             1,
             1,
             "gpio-toggle",
             cmd_led);

    while(1)
    {
        uint8_t c;

        if(pal_uart_receive(BOARD_UART_DEBUG,
                            &c,
                            1) == PAL_STATUS_OK)
        {
            cli_receive_char((char)c);
        }

        cli_process();
    }

}






int main(void)
{
     
    uint16_t tleversion = 0;
    uint16_t glb_cfg = 0;
    uint32_t cmd = 0;
    uint16_t data_reg = 0x5005;
     board_init();
     test_spi_tle();
     //test_timer();
     //test_timer_interrupt();
     //test_cli();
     const uint8_t message[] =
        "\r\n --New Data Set--\r\n";
   cli_init();
    while (1)
    {
        
        //test_blink();
        //test_timer();

        cmd = tle_write_glb_cfg(data_reg);
        logger_log(LOG_LEVEL_INFO,
           "CMD_GLBCFG = %x",
           cmd);
        logger_log(LOG_LEVEL_INFO,
           "GLB_DATA = %x",
           data_reg); 


        cmd = tle_read_version(&tleversion);
        logger_log(LOG_LEVEL_INFO,
           "CMD = %x",
           cmd);
        logger_log(LOG_LEVEL_INFO,
           "RESP = %x",
           version_value.raw);
        logger_log(LOG_LEVEL_INFO,
           "Version = %x",
           tleversion);
        cmd = tle_read_glb_cfg(&glb_cfg);
        logger_log(LOG_LEVEL_INFO,
           "READ_GLBCFG = %x",
           cmd);  
        logger_log(LOG_LEVEL_INFO,
           "RESP_GLBRAW = %x",
           global_config_value.raw);      
        logger_log(LOG_LEVEL_INFO,
           "RegVal = %x",
           glb_cfg);

        logger_log(LOG_LEVEL_INFO,
           "%s",
           message);
     /*pal_uart_transmit(
            BOARD_UART_DEBUG,
            (uint8_t *)&version,
            sizeof(version));
        //test_uart_transmit();
*/
        

        pal_timer_delay_ms(1000);
    }
    
}

