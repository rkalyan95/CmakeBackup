#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include "pal_usart.h"


typedef enum
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG

} log_level_t;


void logger_init(board_uart_t uart);

void logger_log(log_level_t level,
                const char *fmt,
                ...);

#define log_info(...) \
    logger_log(LOG_LEVEL_INFO, __VA_ARGS__)

#define log_warn(...) \
    logger_log(LOG_LEVEL_WARN, __VA_ARGS__)

#define log_error(...) \
    logger_log(LOG_LEVEL_ERROR, __VA_ARGS__)

#define log_debug(...) \
    logger_log(LOG_LEVEL_DEBUG, __VA_ARGS__)
    
#endif