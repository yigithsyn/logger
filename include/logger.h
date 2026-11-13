#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOGGER_LEVEL_DEBUG = 0,
    LOGGER_LEVEL_LOG,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARN,
    LOGGER_LEVEL_ERROR
} logger_level;

/* The output stream remains owned by the caller. NULL is rejected. */
int logger_set_output(FILE *output);
void logger_set_color(int enabled);
void logger_set_timestamp(int enabled);

int logger_write(logger_level level, const char *format, ...);
int logger_debug(const char *format, ...);
int logger_log(const char *format, ...);
int logger_info(const char *format, ...);
int logger_warn(const char *format, ...);
int logger_error(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
