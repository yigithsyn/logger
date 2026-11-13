# logger

A small, dependency-free C logging library. It writes timestamped, colored
messages with the `DEBUG`, `LOG`, `INFO`, `WARN`, and `ERROR` levels.
It also provides a `logger` command-line tool.

## Build and test

With CMake:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build -C Debug --output-on-failure
```

Or with the Makefile:

```sh
make
make test
```

The CMake build produces the `logger` command-line executable and installs it
alongside the library. It accepts a message as arguments or reads one message
per line from standard input:

```sh
logger --level warn "Retrying connection"
printf 'Started\nReady\n' | logger --no-color
```

Use `logger --help` to see the available levels and options. By default, the
CLI uses the `info` level and the library's default timestamp and color
settings.

The CMake build provides the `logger` library target and the
`logger::logger` alias. Link your CMake target with `logger::logger`. To
install the library and public header:

```sh
cmake --install build --prefix <install-directory>
```

## Usage

```c
#include <logger.h>

int main(void)
{
    logger_info("Listening on port %d", 8080);
    logger_warn("Retrying %s", "connection");
    logger_error("Request %lu failed", 42UL);
    return 0;
}
```

Link the program against `logger` and include the installed `logger.h`
header. The API also provides `logger_debug`, `logger_log`, and
`logger_write` for selecting a level dynamically.

Messages are written to `stdout` by default. Timestamps (UTC, in ISO 8601
format) and ANSI color are enabled by default. Each line contains the level,
timestamp (when enabled), and formatted message. For example, with color
disabled:

```text
[INFO] 2026-01-02T03:04:05Z Listening on port 8080
```

Use `logger_set_output`, `logger_set_timestamp`, and `logger_set_color` to
configure the process-wide logger. Pass a nonzero value to enable timestamps
or color, or `0` to disable them. `logger_set_output` rejects `NULL`; the
selected stream remains owned by the caller and must remain open while in use.

The level-specific functions (`logger_debug`, `logger_log`, `logger_info`,
`logger_warn`, and `logger_error`) accept `printf`-style format strings.
`logger_write` takes a `logger_level` as its first argument to select a level
dynamically. These logging functions return `0` on success and `-1` for an
invalid level, a `NULL` format, or a time-generation or output error. Each
successful call appends a newline and flushes the output stream.
