#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "unistd.h"

#define 	NUMTHRDS 	4
#define 	VECLEN 		100000

int 	*a, *b;
long 	sum = 0;
pthread_mutex_t 	mutexsum;

void *dotprod(void *arg)
{
	int i, start, end, offset, len;
	long tid = (long)arg;
	offset = tid;
	len = VECLEN;
	start = offset*len;
	end   = start + len;
	long my_sum = 0;

	printf("thread: %ld starting. start=%d end=%d\n",tid,start,end-1);
	for (i=start; i<end ; i++)
		my_sum += (a[i] * b[i]);

	pthread_mutex_lock (&mutexsum);
	sum += my_sum;
	pthread_mutex_unlock (&mutexsum);
	
	printf("thread: %ld done. Global sum now is=%li\n",tid,sum);

	pthread_exit((void*) 0);
}

int main (int argc, char *argv[])
{
	long i;
	void *status;
	pthread_t threads[NUMTHRDS];
	pthread_attr_t attr;

	a = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	b = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));

	for (i=0; i<VECLEN*NUMTHRDS; i++)
		a[i]= b[i]=1;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(i=0; i<NUMTHRDS; i++)
		pthread_create(&threads[i], &attr, dotprod, (void *)i);

	pthread_attr_destroy(&attr);

	for(i=0; i<NUMTHRDS; i++)
		pthread_join(threads[i], &status);

	printf ("Final Global Sum=%li\n",sum);
	free (a);
	free (b);
	pthread_exit(NULL);
}
