#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "unistd.h"

#define NTHREADS	32

void *Hello(void *threadid)
{
	int i;
	double result=0.0;
	sleep(3);
	for (i=0; i<10000; i++)
		result = result + sin(i) * tan(i);
	printf("%ld: Hello World!\n", threadid);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NTHREADS];
	int rc;
	long t;
	for(t=0;t<NTHREADS;t++){
		rc = pthread_create(&threads[t], NULL, Hello, (void *)t);
		if (rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	 }
	printf("main(): Created %ld threads.\n", t);
	pthread_exit(NULL);
}
