#include "logger.h"
#include "pal_usart.h"
#include <stdio.h>
#include <string.h>

static board_uart_t logger_uart;

#define LOGGER_BUFFER_SIZE    128

static char logger_buffer[LOGGER_BUFFER_SIZE];


static const char *logger_level_to_string(log_level_t level)
{
    switch(level)
    {
        case LOG_LEVEL_ERROR:
            return "[ERROR] ";

        case LOG_LEVEL_WARN:
            return "[WARN ] ";

        case LOG_LEVEL_INFO:
            return "[INFO ] ";

        case LOG_LEVEL_DEBUG:
            return "[DEBUG] ";

        default:
            return "[UNKWN] ";
    }
}


void logger_init(board_uart_t uart)
{
    logger_uart = uart;
}

void logger_log(log_level_t level,
                const char *fmt,
                ...)
{
    int prefix_length;
    int message_length;

    va_list args;

    prefix_length = snprintf(logger_buffer,
                             LOGGER_BUFFER_SIZE,
                             "%s",
                             logger_level_to_string(level));

    va_start(args, fmt);

    message_length = vsnprintf(&logger_buffer[prefix_length],
                               LOGGER_BUFFER_SIZE - prefix_length,
                               fmt,
                               args);

    va_end(args);

    int total_length = prefix_length + message_length;

    if ((total_length + 2) < LOGGER_BUFFER_SIZE)
    {
        logger_buffer[total_length++] = '\r';
        logger_buffer[total_length++] = '\n';
        logger_buffer[total_length] = '\0';
    }

    pal_uart_transmit(logger_uart,
                      (uint8_t *)logger_buffer,
                      strlen(logger_buffer));
}