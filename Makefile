TARGET = run
CC = gcc

LIBS = 

DEPS = fifo_queue.h test.h
OBJS = fifo_queue.o test.o main.o

CFLAGS = -Wall -pthread


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean

clean: 
	rm -f *.o run
