CFLAGS := -std=c17 -lm -lX11 -pedantic -Wextra -Wall -Werror -fsanitize=address,undefined -g 
HEADERS := $(wildcard src/*.h)

.PHONY: run
run: src/main.o
	@ ./src/main.o

src/main.o: src/main.c $(HEADERS)
	@ cc $(CFLAGS) src/main.c  -o src/main.o 

.PHONY: env_check
env_check:
	cc $(CFLAGS) -o src/env_check.o src/env_check.c
	./src/env_check.o; rm ./src/env_check.o

.PHONY: clean
clean:
	rm src/*.o
