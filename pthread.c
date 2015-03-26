#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  // exit, EXIT_SUCCESS, EXIT_FAILURE
#include <time.h>    // clock
#include <unistd.h>  // getopt
#include <stdbool.h> // bool

#define TIMES 10000000

static bool with_mutex = false;
static int global_cnt = 0;
pthread_mutex_t global_cnt_lock;

void timer() {
	static clock_t timer = 0;

	if(timer == 0){
		timer = clock();
	}
	else {
		printf("Took me %f seconds\n", (double) (clock() - timer) / CLOCKS_PER_SEC);
	}
}

/* Increment function */
void *inc_cnt() {
	unsigned int i;

	for(i = 0; i < TIMES; ++i){
		if(with_mutex)
			pthread_mutex_lock(&global_cnt_lock);
		++global_cnt;
		if(with_mutex)
			pthread_mutex_unlock(&global_cnt_lock);
	}

	return NULL;
}

int main(int argc, char **argv){
	int opt;
	unsigned int j;
	pthread_t inc_cnt_thread;

	/* Parse command line arguments */
	while ((opt = getopt(argc, argv, "m")) != -1) {
		switch(opt) {
			case 'm':
				with_mutex = true;
				break;
			default:
				printf("Starting without mutex\n");
				break;
		}
	}

	/* Mutex init */
	if(with_mutex && pthread_mutex_init(&global_cnt_lock, NULL) != 0) {
		fprintf(stderr, "Mutex init failed\n");
		exit(EXIT_FAILURE);
	}

	/* Start timer */
	timer();

	/* Create thread for increment */
	if(pthread_create(&inc_cnt_thread, NULL, inc_cnt, NULL)){
		fprintf(stderr, "Error pthread create\n");
		exit(EXIT_FAILURE);
	}

	/* Current thread for decrement */
	for(j = 0; j < TIMES; ++j){
		if(with_mutex)
			pthread_mutex_lock(&global_cnt_lock);
		--global_cnt;
		if(with_mutex)
			pthread_mutex_unlock(&global_cnt_lock);
	}

	/* Wait for increment thread */
	if(pthread_join(inc_cnt_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		exit(EXIT_FAILURE);
	}

	/* Calue is different without mutex */
	printf("Counter value %d\n", global_cnt);
	
	/* End timer and print elapsed time */
	timer();

	/* Destroy mutex */
	if(with_mutex)
		pthread_mutex_destroy(&global_cnt_lock);

	return EXIT_SUCCESS;
}
