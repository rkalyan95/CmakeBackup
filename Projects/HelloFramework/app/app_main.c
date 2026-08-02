#include "pal_gpio.h"
#include "pal_usart.h"
#include "pal_spi.h"
#include "spi.h"
void test_blink(void)
{
     pal_gpio_toggle(BOARD_GPIO_LED_STATUS);
     pal_blocking_delay(1000);
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
     pal_blocking_delay(200);
     pal_gpio_write(BOARD_GPIO_TLE_RST,1);
     pal_blocking_delay(200);
     pal_gpio_write(BOARD_GPIO_TLE_EN,1);
     pal_blocking_delay(200);



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
    while (1)
    {
        
        test_blink();

     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
    // pal_blocking_delay(1);
     pal_spi_transfer(BOARD_SPI_1 , command , response , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
    // pal_blocking_delay(1);
     response[0] = 0;
     response[1] = 0;
     response[2] = 0;
     response[3] = 0;
     pal_gpio_write(BOARD_GPIO_SPI_CS,0);
    // pal_blocking_delay(1);
     pal_spi_transfer(BOARD_SPI_1 , command , response , 4);
     pal_gpio_write(BOARD_GPIO_SPI_CS,1);
   // pal_blocking_delay(1);


     pal_uart_transmit(
            BOARD_UART_DEBUG,
            response,
            sizeof(response));
        //test_uart_transmit();


        pal_blocking_delay(1000);
    }
}

