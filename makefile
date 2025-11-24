CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g

# object files we actually need 
OBJS    = main.o prime.o factor.o

TARGET  = rsalite

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c prime.h
	$(CC) $(CFLAGS) -c main.c

prime.o: prime.c prime.h
	$(CC) $(CFLAGS) -c prime.c

factor.o: factor.c factor.h prime.h
	$(CC) $(CFLAGS) -c factor.c


clean:
	rm -f $(OBJS) $(TARGET)
