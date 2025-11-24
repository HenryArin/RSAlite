CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   = $(shell pkg-config --libs gtk+-3.0)

CLI_OBJS = main.o factor.o prime.o
GUI_OBJS = gui_main.o factor.o prime.o

all: rsalite rsalite-gui

rsalite: $(CLI_OBJS)
	$(CC) $(CFLAGS) -o rsalite $(CLI_OBJS)

rsalite-gui: $(GUI_OBJS)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -o rsalite-gui $(GUI_OBJS) $(GTK_LIBS)

main.o: main.c factor.h prime.h
	$(CC) $(CFLAGS) -c main.c

gui_main.o: gui_main.c factor.h prime.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c gui_main.c

factor.o: factor.c factor.h prime.h
	$(CC) $(CFLAGS) -c factor.c

prime.o: prime.c prime.h
	$(CC) $(CFLAGS) -c prime.c

clean:
	rm -f *.o rsalite rsalite-gui
