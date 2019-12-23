#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "unistd.h"

#define NUM_THREADS  6
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *idp)
{
	int j,i;
	double result=0.0;
	long my_id = (long)idp;
	for (i=0; i < TCOUNT; i++) {
		pthread_mutex_lock(&count_mutex);
		count++;
		if (count == COUNT_LIMIT) {
			// pthread_cond_signal(&count_threshold_cv);
			pthread_cond_broadcast(&count_threshold_cv);
			printf("inc_count(): thread %ld, count = %d  Threshold reached.\n", my_id, count);
		}
		printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
		pthread_mutex_unlock(&count_mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *watch_count(void *idp)
{
	long my_id = (long)idp;

	printf("Starting watch_count(): thread %ld\n", my_id);
	pthread_mutex_lock(&count_mutex);
	printf("***Before cond_wait: thread %ld\n", my_id);
	pthread_cond_wait(&count_threshold_cv, &count_mutex);
	printf("***Thread %ld Condition signal received.\n", my_id);
	pthread_mutex_unlock(&count_mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i, rc;
	pthread_t threads[6];
	pthread_attr_t attr;

	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init (&count_threshold_cv, NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&threads[2], &attr, watch_count, (void *)2);
	pthread_create(&threads[3], &attr, watch_count, (void *)3);
	pthread_create(&threads[4], &attr, watch_count, (void *)4);
	pthread_create(&threads[5], &attr, watch_count, (void *)5);
	pthread_create(&threads[0], &attr, inc_count, (void *)0);
	pthread_create(&threads[1], &attr, inc_count, (void *)1);

	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
	printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);
	pthread_exit (NULL);
}
