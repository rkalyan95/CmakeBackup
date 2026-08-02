#include "cli.h"

#include <string.h>

#include "logger.h"

static cli_command_t cli_commands[CLI_MAX_COMMANDS];

static uint8_t cli_command_count = 0;

static char cli_input_buffer[CLI_MAX_INPUT_LENGTH];

static uint8_t cli_input_index = 0;

static uint8_t cli_command_ready = 0;

static void cli_execute_command(void);

static uint8_t cli_parse_arguments(char *input,
                                   char *argv[]);



void cli_init(void)
{
    cli_command_count = 0;

    cli_input_index = 0;

    cli_command_ready = 0;

    memset(cli_input_buffer,
           0,
           sizeof(cli_input_buffer));
}



void cli_register(const char *command,
                  uint8_t min_args,
                  uint8_t max_args,
                  const char *usage,
                  cli_callback_t callback)
{
    if(cli_command_count >= CLI_MAX_COMMANDS)
    {
        return;
    }

    cli_commands[cli_command_count].command  = command;
    cli_commands[cli_command_count].min_args = min_args;
    cli_commands[cli_command_count].max_args = max_args;
    cli_commands[cli_command_count].usage    = usage;
    cli_commands[cli_command_count].callback = callback;

    cli_command_count++;
}


void cli_receive_char(char c)
{
    if((c == '\r') || (c == '\n'))
    {
        cli_input_buffer[cli_input_index] = '\0';

        cli_command_ready = 1;

        cli_input_index = 0;

        return;
    }

    if(cli_input_index >= (CLI_MAX_INPUT_LENGTH - 1))
    {
        return;
    }

    cli_input_buffer[cli_input_index++] = c;
}



void cli_process(void)
{
    if(cli_command_ready == 0)
    {
        return;
    }

    cli_command_ready = 0;

    cli_execute_command();

    memset(cli_input_buffer,
           0,
           sizeof(cli_input_buffer));
}



static void cli_execute_command(void)
{
    char *argv[CLI_MAX_ARGUMENTS];

    uint8_t argc;

    argc = cli_parse_arguments(cli_input_buffer,
                               argv);

    if(argc == 0)
    {
        return;
    }

    for(uint8_t i = 0; i < cli_command_count; i++)
    {
        if(strcmp(argv[0],
                  cli_commands[i].command) == 0)
        {
            if((argc < cli_commands[i].min_args) ||
               (argc > cli_commands[i].max_args))
            {
                log_error("Usage: %s",
                          cli_commands[i].usage);
                return;
            }

            cli_commands[i].callback(argc,
                                     argv);

            return;
        }
    }

    log_error("Unknown command");
}

static uint8_t cli_parse_arguments(char *input,
                                   char *argv[])
{
    uint8_t argc = 0;

    char *token;

    token = strtok(input, " ");

    while(token != NULL)
    {
        if(argc >= CLI_MAX_ARGUMENTS)
        {
            break;
        }

        argv[argc++] = token;

        token = strtok(NULL, " ");
    }

    return argc;
}