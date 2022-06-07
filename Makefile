CFLAGS := -std=c17 -lX11 -pedantic -Wextra -Wall -Werror -Wfloat-equal -fsanitize=address,undefined
HEADERS := $(wildcard src/*.h)

.PHONY: run
run: src/main.o
	./src/main.o

src/main.o: src/main.c $(HEADERS)
	echo $(HEADERS)
	cc $(CFLAGS) -o src/main.o src/main.c

.PHONY: clean
clean:
	rm src/*.o