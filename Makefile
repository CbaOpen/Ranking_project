
CFLAGS = -Wall -g -lm
CC = gcc
EXEC = pagerank

compil:
	$(CC) -o $(EXEC) pagerank.c $(CFLAGS)

clean:
	rm pagerank
