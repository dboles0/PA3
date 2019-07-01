TARGET = run
CC = gcc

LIBS = 

DEPS = fifo_queue.h multi-lookup.h test.h
OBJS = fifo_queue.o multi-lookup.o test.o main.o

CFLAGS = -Wall -Wextra -pthread -g


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean

clean: 
	rm -f *.o run
