CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g

CLI_OBJS = main.o factor.o prime.o
GUI_OBJS = gui_main.o factor.o prime.o
BENCH_OBJS = bench.o factor.o prime.o


all: rsalite

rsalite: $(CLI_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLI_OBJS)

rsalite-bench: $(BENCH_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BENCH_OBJS)


main.o: main.c factor.h prime.h
	$(CC) $(CFLAGS) -c $<

factor.o: factor.c factor.h prime.h
	$(CC) $(CFLAGS) -c $<

prime.o: prime.c prime.h
	$(CC) $(CFLAGS) -c $<

test_factor.o: test_factor.c factor.h prime.h
	$(CC) $(CFLAGS) -c test_factor.c

GTK_CFLAGS := $(shell pkg-config --cflags gtk+-3.0 2>/dev/null)
GTK_LIBS   := $(shell pkg-config --libs gtk+-3.0 2>/dev/null)

rsalite-gui: $(GUI_OBJS)
	$(CC) $(CFLAGS) -o $@ $(GUI_OBJS) $(GTK_LIBS)

gui_main.o: gui_main.c factor.h prime.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $<

test: test_factor

test_factor: test_factor.o factor.o prime.o
	$(CC) $(CFLAGS) -o test_factor test_factor.o factor.o prime.o

bench.o: bench.c factor.h prime.h
	$(CC) $(CFLAGS) -c bench.c


clean:
	rm -f *.o rsalite rsalite-gui rsalite-bench test_factor


.PHONY: all clean
