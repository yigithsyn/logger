CC ?= cc
CFLAGS ?= -O2
CPPFLAGS ?= -Iinclude

LIBRARY = build/liblogger.a
CLI_BINARY = build/logger
TEST_BINARY = build/test_logger
OBJECT = build/logger.o

.PHONY: all test clean

all: $(LIBRARY) $(CLI_BINARY)

$(OBJECT): src/logger.c include/logger.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c89 -pedantic -Wall -Wextra -Werror \
		-c src/logger.c -o $(OBJECT)

$(LIBRARY): $(OBJECT)
	ar rcs $(LIBRARY) $(OBJECT)

$(CLI_BINARY): src/logger_cli.c include/logger.h $(LIBRARY)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c89 -pedantic -Wall -Wextra -Werror \
		src/logger_cli.c $(LIBRARY) -o $(CLI_BINARY)

$(TEST_BINARY): tests/test_logger.c $(LIBRARY) include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c89 -pedantic -Wall -Wextra -Werror \
		tests/test_logger.c $(LIBRARY) -o $(TEST_BINARY)

test: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	rm -rf build
