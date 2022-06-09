CFLAGS := -std=c17 -lX11 -pedantic -Wextra -Wall -Werror -Wfloat-equal -fsanitize=address,undefined
HEADERS := $(wildcard src/*.h)

.PHONY: run
run: src/main.o
	@ ./src/main.o

src/main.o: src/main.c $(HEADERS)
	@ cc $(CFLAGS) -o src/main.o src/main.c

.PHONY: env_check
env_check:
	cc $(CFLAGS) -o src/env_check.o src/env_check.c
	./src/env_check.o; rm ./src/env_check.o

.PHONY: clean
clean:
	rm src/*.o
