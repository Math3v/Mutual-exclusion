CC=gcc

pthread:
	$(CC) -pthread -Wall pthread.c -o pthread

clean:
	rm -rf pthread