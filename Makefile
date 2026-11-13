CC ?= cc
CFLAGS ?= -O2
CPPFLAGS ?= -Iinclude

LIBRARY = build/liblogger.a
TEST_BINARY = build/test_logger
OBJECT = build/logger.o

.PHONY: all test clean

all: $(LIBRARY)

$(OBJECT): src/logger.c include/logger.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c89 -pedantic -Wall -Wextra -Werror \
		-c src/logger.c -o $(OBJECT)

$(LIBRARY): $(OBJECT)
	ar rcs $(LIBRARY) $(OBJECT)

$(TEST_BINARY): tests/test_logger.c $(LIBRARY) include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c89 -pedantic -Wall -Wextra -Werror \
		tests/test_logger.c $(LIBRARY) -o $(TEST_BINARY)

test: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	rm -rf build
