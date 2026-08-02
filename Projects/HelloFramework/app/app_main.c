#include "pal_gpio.h"
#include "pal_usart.h"
#include "pal_spi.h"
#include "spi.h"
#include "pal_timer.h"
#include "logger.h"
#include "pal_adc.h"
#include <stdio.h>
#include "cli.h"

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

uint8_t crc8_sae_j1850(const uint8_t *data, size_t length) {
    // Initial value is 0xFF
    uint8_t crc = 0xFF; 

    for (size_t i = 0; i < length; i++) {
        // XOR the input byte into the CRC register
        crc ^= data[i]; 

        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                // If the MSB is 1, shift and XOR with polynomial 0x1D
                crc = (crc << 1) ^ 0x1D;
            } else {
                // Otherwise, just shift left
                crc <<= 1;
            }
        }
    }

    // Final XOR value is 0xFF
    return crc ^ 0xFF; 
}


uint32_t spi_write_frame(uint16_t addr, uint16_t data) {

    uint32_t payload = (((uint32_t)(addr & 0x7F) << 17) | (1U << 16) | (uint32_t)data) & 0x00FFFFFF;

    // Pack bytes explicitly in Big-Endian order (MSB first)
    uint8_t frame_bytes[3];
    frame_bytes[0] = (uint8_t)(payload >> 16); // Byte 0 (Header/Addr)
    frame_bytes[1] = (uint8_t)(payload >> 8);  // Byte 1 (Data High)
    frame_bytes[2] = (uint8_t)(payload & 0xFF); // Byte 2 (Data Low)

    // Calculate CRC over the 3-byte payload
    uint8_t crc = crc8_sae_j1850(frame_bytes, 3);

    // Append CRC into the top byte (Bits 31..24)
    uint32_t frame = ((uint32_t)crc << 24) | payload;

    return frame;
}

uint32_t spi_read_frame(uint16_t addr) {
    uint8_t rw_bit = 0;
    
    uint32_t payload = ((0x00000000U << 17) | ((uint32_t)rw_bit << 16) | addr) & 0x00FFFFFF;

    // Calculate CRC over the big-endian byte stream
    uint8_t crc = crc8_sae_j1850((uint8_t *)&payload, 3);

    // Combine CRC (MSB) with 24-bit payload to form 32-bit frame
    uint32_t frame = ((uint32_t)crc << 24) | payload;

    return frame;
}


int main(void)
{
     
     uint32_t cmd = 0x69000200;
     
     uint8_t command[4] = {((cmd & 0xff000000)>>24),((cmd & 0x00ff0000)>>16),((cmd & 0x0000ff00)>>8),(cmd & 0x000000ff)};
     uint8_t response[4];
     board_init();
     test_spi_tle();
     //test_timer();
     //test_timer_interrupt();
     test_cli();
    #if 0
    while (1)
    {
        
        test_blink();
        //test_timer();
     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
    // pal_timer_delay_ms(1);
     pal_spi_transfer(BOARD_SPI_1 , command , response , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
    // pal_timer_delay_ms(1);
     response[0] = 0;
     response[1] = 0;
     response[2] = 0;
     response[3] = 0;
     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
    // pal_blocking_delay(1);
     pal_spi_transfer(BOARD_SPI_1 , command , response , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
   // pal_timer_delay_ms(1);


     pal_uart_transmit(
            BOARD_UART_DEBUG,
            response,
            sizeof(response));
        //test_uart_transmit();


        pal_timer_delay_ms(1000);
    }
    #endif
}

