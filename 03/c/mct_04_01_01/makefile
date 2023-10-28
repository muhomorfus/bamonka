CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror
LFLAGS :=

unit: unit_tests.exe
	./unit_tests.exe

unit_tests.exe: CFLAGS += -g3 -ggdb
unit_tests.exe: LFLAGS += -g3 -ggdb -lcheck -lpthread -lrt # -lsubunit -lm
unit_tests.exe: out/stack.o out/queue.o out/check_queue.o out/check_main.o
	gcc $(CFLAGS) $^ -o $@ $(LFLAGS)

out/stack.o: src/stack.c inc/stack.h
	[ -f out ] || mkdir -p out
	gcc -c $(CFLAGS) $< -I inc -o $@

out/queue.o: src/queue.c inc/queue.h inc/stack.h
	[ -f out ] || mkdir -p out
	gcc -c $(CFLAGS) $< -I inc -o $@

out/check_queue.o: unit_tests/check_queue.c inc/queue.h
	[ -f out ] || mkdir -p out
	gcc -c $(CFLAGS) $< -I inc -o $@

out/check_main.o: unit_tests/check_main.c
	[ -f out ] || mkdir -p out
	gcc -c $(CFLAGS) $< -I inc -o $@

clean:
	rm -rf *.exe out
