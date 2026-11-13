#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void logger_cli_usage(FILE *output)
{
    fprintf(output,
            "Usage: logger [OPTIONS] [MESSAGE ...]\n"
            "Write MESSAGE to stdout, or log lines read from stdin when no "
            "message is given.\n\n"
            "Options:\n"
            "  -l, --level LEVEL  Set level: debug, log, info, warn, error "
            "(default: info)\n"
            "      --no-color     Disable ANSI color\n"
            "      --no-timestamp Disable timestamps\n"
            "  -h, --help         Show this help\n");
}

static int logger_cli_parse_level(const char *name, logger_level *level)
{
    if (strcmp(name, "debug") == 0) {
        *level = LOGGER_LEVEL_DEBUG;
    } else if (strcmp(name, "log") == 0) {
        *level = LOGGER_LEVEL_LOG;
    } else if (strcmp(name, "info") == 0) {
        *level = LOGGER_LEVEL_INFO;
    } else if (strcmp(name, "warn") == 0) {
        *level = LOGGER_LEVEL_WARN;
    } else if (strcmp(name, "error") == 0) {
        *level = LOGGER_LEVEL_ERROR;
    } else {
        return -1;
    }

    return 0;
}

static int logger_cli_write_stdin(logger_level level)
{
    size_t capacity;
    size_t length;
    char *line;
    int character;

    capacity = 128U;
    length = 0U;
    line = (char *)malloc(capacity);
    if (line == NULL) {
        fprintf(stderr, "logger: unable to allocate input buffer\n");
        return 1;
    }

    while ((character = getchar()) != EOF) {
        if (character == '\n') {
            if (length > 0U && line[length - 1U] == '\r') {
                --length;
            }
            line[length] = '\0';
            if (logger_write(level, "%s", line) != 0) {
                fprintf(stderr, "logger: failed to write log message\n");
                free(line);
                return 1;
            }
            length = 0U;
        } else {
            if (length == capacity - 1U) {
                size_t new_capacity;
                char *new_line;

                if (capacity > (size_t)-1 / 2U) {
                    fprintf(stderr, "logger: input line is too long\n");
                    free(line);
                    return 1;
                }
                new_capacity = capacity * 2U;
                new_line = (char *)realloc(line, new_capacity);
                if (new_line == NULL) {
                    fprintf(stderr, "logger: unable to grow input buffer\n");
                    free(line);
                    return 1;
                }
                line = new_line;
                capacity = new_capacity;
            }
            line[length] = (char)character;
            ++length;
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "logger: failed to read stdin\n");
        free(line);
        return 1;
    }

    if (length > 0U) {
        if (length > 0U && line[length - 1U] == '\r') {
            --length;
        }
        line[length] = '\0';
        if (logger_write(level, "%s", line) != 0) {
            fprintf(stderr, "logger: failed to write log message\n");
            free(line);
            return 1;
        }
    }

    free(line);
    return 0;
}

static int logger_cli_write_arguments(logger_level level, int argc,
                                      char **argv, int first_argument)
{
    size_t length;
    size_t argument_length;
    size_t offset;
    int index;
    char *message;

    length = 1U;
    for (index = first_argument; index < argc; ++index) {
        argument_length = strlen(argv[index]);
        if (argument_length > (size_t)-1 - length ||
            (index > first_argument && length == (size_t)-1)) {
            fprintf(stderr, "logger: message is too long\n");
            return 1;
        }
        length += argument_length;
        if (index + 1 < argc) {
            if (length == (size_t)-1) {
                fprintf(stderr, "logger: message is too long\n");
                return 1;
            }
            ++length;
        }
    }

    message = (char *)malloc(length);
    if (message == NULL) {
        fprintf(stderr, "logger: unable to allocate message buffer\n");
        return 1;
    }

    offset = 0U;
    for (index = first_argument; index < argc; ++index) {
        argument_length = strlen(argv[index]);
        if (index > first_argument) {
            message[offset] = ' ';
            ++offset;
        }
        memcpy(message + offset, argv[index], argument_length);
        offset += argument_length;
    }
    message[offset] = '\0';

    if (logger_write(level, "%s", message) != 0) {
        fprintf(stderr, "logger: failed to write log message\n");
        free(message);
        return 1;
    }

    free(message);
    return 0;
}

int main(int argc, char **argv)
{
    int argument;
    int color_enabled;
    int timestamp_enabled;
    logger_level level;

    argument = 1;
    color_enabled = 1;
    timestamp_enabled = 1;
    level = LOGGER_LEVEL_INFO;

    while (argument < argc && argv[argument][0] == '-') {
        if (strcmp(argv[argument], "--") == 0) {
            ++argument;
            break;
        } else if (strcmp(argv[argument], "-h") == 0 ||
                   strcmp(argv[argument], "--help") == 0) {
            logger_cli_usage(stdout);
            return 0;
        } else if (strcmp(argv[argument], "--no-color") == 0) {
            color_enabled = 0;
        } else if (strcmp(argv[argument], "--no-timestamp") == 0) {
            timestamp_enabled = 0;
        } else if (strcmp(argv[argument], "-l") == 0 ||
                   strcmp(argv[argument], "--level") == 0) {
            if (argument + 1 >= argc ||
                logger_cli_parse_level(argv[argument + 1], &level) != 0) {
                fprintf(stderr, "logger: missing or invalid level\n");
                logger_cli_usage(stderr);
                return 2;
            }
            ++argument;
        } else {
            fprintf(stderr, "logger: unknown option: %s\n", argv[argument]);
            logger_cli_usage(stderr);
            return 2;
        }
        ++argument;
    }

    logger_set_color(color_enabled);
    logger_set_timestamp(timestamp_enabled);

    if (argument == argc) {
        return logger_cli_write_stdin(level);
    }
    return logger_cli_write_arguments(level, argc, argv, argument);
}
