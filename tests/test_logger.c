#include "logger.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *output;
    char buffer[256];
    size_t length;

#if defined(_MSC_VER)
    if (tmpfile_s(&output) != 0) {
        output = NULL;
    }
#else
    output = tmpfile();
#endif
    if (output == NULL) {
        fprintf(stderr, "failed to create test output file\n");
        return 1;
    }
    if (logger_set_output(output) != 0) {
        fprintf(stderr, "failed to configure output stream\n");
        fclose(output);
        return 1;
    }

    logger_set_color(0);
    logger_set_timestamp(0);

    if (logger_debug("debug") != 0 ||
        logger_log("log") != 0 ||
        logger_info("user=%s count=%d", "Ada", 3) != 0 ||
        logger_warn("warning") != 0 ||
        logger_error("error") != 0) {
        fprintf(stderr, "logging call failed\n");
        fclose(output);
        return 1;
    }

    if (logger_write((logger_level)99, "invalid") != -1 ||
        logger_info(NULL) != -1 ||
        logger_set_output(NULL) != -1) {
        fprintf(stderr, "invalid arguments were not rejected\n");
        fclose(output);
        return 1;
    }

    if (fseek(output, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "failed to rewind test output\n");
        fclose(output);
        return 1;
    }
    length = fread(buffer, 1U, sizeof(buffer) - 1U, output);
    if (ferror(output)) {
        fprintf(stderr, "failed to read test output\n");
        fclose(output);
        return 1;
    }
    buffer[length] = '\0';

    if (strcmp(buffer,
               "[DEBUG] debug\n"
               "[LOG] log\n"
               "[INFO] user=Ada count=3\n"
               "[WARN] warning\n"
               "[ERROR] error\n") != 0) {
        fprintf(stderr, "unexpected log output:\n%s", buffer);
        fclose(output);
        return 1;
    }

    logger_set_color(1);
    logger_set_timestamp(1);
    if (fseek(output, 0L, SEEK_END) != 0 ||
        logger_info("timestamp") != 0 ||
        fseek(output, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "failed to test timestamped output\n");
        fclose(output);
        return 1;
    }
    length = fread(buffer, 1U, sizeof(buffer) - 1U, output);
    if (ferror(output)) {
        fprintf(stderr, "failed to read timestamped output\n");
        fclose(output);
        return 1;
    }
    buffer[length] = '\0';
    if (strstr(buffer, "\033[36m[INFO]\033[0m ") == NULL ||
        strstr(buffer, "Z timestamp\n") == NULL) {
        fprintf(stderr, "timestamp or color was missing from log output\n");
        fclose(output);
        return 1;
    }

    fclose(output);
    return 0;
}
