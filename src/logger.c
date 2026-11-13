#include "logger.h"

#include <stdarg.h>
#include <time.h>

static FILE *logger_output = NULL;
static int logger_color_enabled = 1;
static int logger_timestamp_enabled = 1;

static const char *logger_level_name(logger_level level)
{
    switch (level) {
    case LOGGER_LEVEL_DEBUG:
        return "DEBUG";
    case LOGGER_LEVEL_LOG:
        return "LOG";
    case LOGGER_LEVEL_INFO:
        return "INFO";
    case LOGGER_LEVEL_WARN:
        return "WARN";
    case LOGGER_LEVEL_ERROR:
        return "ERROR";
    default:
        return NULL;
    }
}

static const char *logger_level_color(logger_level level)
{
    switch (level) {
    case LOGGER_LEVEL_DEBUG:
        return "\033[35m";
    case LOGGER_LEVEL_LOG:
        return "\033[32m";
    case LOGGER_LEVEL_INFO:
        return "\033[36m";
    case LOGGER_LEVEL_WARN:
        return "\033[33m";
    case LOGGER_LEVEL_ERROR:
        return "\033[31m";
    default:
        return "";
    }
}

int logger_set_output(FILE *output)
{
    if (output == NULL) {
        return -1;
    }

    logger_output = output;
    return 0;
}

void logger_set_color(int enabled)
{
    logger_color_enabled = enabled != 0;
}

void logger_set_timestamp(int enabled)
{
    logger_timestamp_enabled = enabled != 0;
}

static int logger_vwrite(logger_level level, const char *format,
                         va_list arguments)
{
    const char *name;
    FILE *output;
    char timestamp[32];
    time_t now;
    struct tm *utc_time;

    name = logger_level_name(level);
    if (name == NULL || format == NULL) {
        return -1;
    }

    output = logger_output != NULL ? logger_output : stdout;
    if (logger_timestamp_enabled) {
        now = time(NULL);
        if (now == (time_t)-1) {
            return -1;
        }
        utc_time = gmtime(&now);
        if (utc_time == NULL ||
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ",
                     utc_time) == 0) {
            return -1;
        }
    }

    if (logger_color_enabled && fputs(logger_level_color(level), output) == EOF) {
        return -1;
    }
    if (fprintf(output, "[%s]", name) < 0) {
        return -1;
    }
    if (logger_color_enabled && fputs("\033[0m", output) == EOF) {
        return -1;
    }
    if (fputc(' ', output) == EOF) {
        return -1;
    }
    if (logger_timestamp_enabled &&
        fprintf(output, "%s ", timestamp) < 0) {
        return -1;
    }
    if (vfprintf(output, format, arguments) < 0 ||
        fputc('\n', output) == EOF || fflush(output) == EOF) {
        return -1;
    }

    return 0;
}

int logger_write(logger_level level, const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(level, format, arguments);
    va_end(arguments);
    return result;
}

int logger_debug(const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(LOGGER_LEVEL_DEBUG, format, arguments);
    va_end(arguments);
    return result;
}

int logger_log(const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(LOGGER_LEVEL_LOG, format, arguments);
    va_end(arguments);
    return result;
}

int logger_info(const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(LOGGER_LEVEL_INFO, format, arguments);
    va_end(arguments);
    return result;
}

int logger_warn(const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(LOGGER_LEVEL_WARN, format, arguments);
    va_end(arguments);
    return result;
}

int logger_error(const char *format, ...)
{
    int result;
    va_list arguments;

    if (format == NULL) {
        return -1;
    }

    va_start(arguments, format);
    result = logger_vwrite(LOGGER_LEVEL_ERROR, format, arguments);
    va_end(arguments);
    return result;
}
