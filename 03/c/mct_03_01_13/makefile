CFLAGS := -Werror -Wall -Wextra -Wpedantic -std=c99
COMPILER := gcc
DEBUG_FLAGS := -g3 --coverage
LINK_FLAGS :=

release: app.exe

debug: CFLAGS += $(DEBUG_FLAGS)
debug: LINK_FLAGS += $(DEBUG_FLAGS)
debug: app.exe

out:
	[ -f out ] || mkdir -p out

clean:
	rm -rf *.exe out

out/matrix.o: out | src/matrix.c inc/matrix.h
	$(COMPILER) $(CFLAGS) -c src/matrix.c -o out/matrix.o -I inc

out/main.o: out | inc/matrix.h
	$(COMPILER) $(CFLAGS) -c src/main.c -o out/main.o -I inc

app.exe: out/main.o out/matrix.o
	gcc $(LINK_FLAGS) -o app.exe out/main.o out/matrix.o

