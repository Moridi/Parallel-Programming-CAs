// Example Chapter 3 OpenMP Program
#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <omp.h>

double timeGetTime() 
{     
	struct timeval time;     
	struct timezone zone;     
	gettimeofday(&time, &zone);     
	return time.tv_sec + time.tv_usec*1e-6; 
}  


const long int VERYBIG = 100000;
// ***********************************************************************
int main( void )
{
  int i;
  long int j, k, sum;
  double sumx, sumy, total, z;
  double starttime, elapsedtime;
  double thread_starttime;
  double thread_time;
  // ---------------------------------------------------------------------
  // Output a start message
  printf( "OpenMP Parallel Timings for %ld iterations \n\n", VERYBIG );

  // repeat experiment several times
  for( i=0; i<6; i++ )
  {
    // get starting time
    starttime = timeGetTime();
    // reset check sum and total
    sum = 0;
    total = 0.0;
    thread_time = 0;
    
    
    // Work loop, do some work by looping VERYBIG times
    #pragma omp parallel num_threads (4) private( sumx, sumy, k )
    {
      thread_starttime = timeGetTime();

    #pragma omp for reduction( +: sum, total ) schedule( dynamic, 1000 ) nowait
    
      for( int j=0; j<VERYBIG; j++ )
      {
        // increment check sum
        sum += 1;
       
        // Calculate first arithmetic series
        sumx = 0.0;
        for( k=0; k<j; k++ )
         sumx = sumx + (double)k;

        // Calculate second arithmetic series
        sumy = 0.0;
        for( k=j; k>0; k-- )
         sumy = sumy + (double)k;

        if( sumx > 0.0 )total = total + 1.0 / sqrt( sumx );
        if( sumy > 0.0 )total = total + 1.0 / sqrt( sumy );

      }

      thread_time = timeGetTime() - thread_starttime;
      printf("Thread %d Time Elapsed: %10d mSecs\n",omp_get_thread_num(), (int)(thread_time * 1000));

    }
    // get ending time and use it to determine elapsed time
    elapsedtime = timeGetTime() - starttime;
  
    // report elapsed time
    printf("\n\nTime Elapsed %10d mSecs Total=%lf Check Sum = %ld\n\n",
                   (int)(elapsedtime * 1000), total, sum );
  }

  // return integer as required by function header
  return 0;
}
// **********************************************************************
