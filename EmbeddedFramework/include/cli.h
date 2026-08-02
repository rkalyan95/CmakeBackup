#ifndef CLI_H
#define CLI_H

#include <stdint.h>

#define CLI_MAX_COMMANDS        16
#define CLI_MAX_INPUT_LENGTH    64
#define CLI_MAX_ARGUMENTS       8

typedef void (*cli_callback_t)(uint8_t argc,
                               char *argv[]);

typedef struct
{
    const char      *command;
    uint8_t          min_args;
    uint8_t          max_args;
    const char      *usage;
    cli_callback_t   callback;

} cli_command_t;


void cli_init(void);

void cli_register(const char *command,
                  uint8_t min_args,
                  uint8_t max_args,
                  const char *usage,
                  cli_callback_t callback);

void cli_receive_char(char c);

void cli_process(void);

#endif