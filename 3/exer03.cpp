#include "omp.h"
#include "x86intrin.h"
#include "stdio.h"

int N = 512;    // a, b, and c are N x N matrices
int NR_THREADS = 4;

void transpose(int size, float **m)
{
	float fTemp;
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++){
			fTemp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = fTemp;
		}
}

int main (int argc, char *argv[]) {

	int matrix_sizes[] = {128, 256, 512, 1024};
	int number_of_threads[] = {2, 4, 6, 8, 10, 16};
	double result[4][6];

	for (int u = 0; u < 4; u++)
	{
		for (int v = 0; v < 6; v++)
		{
			N = matrix_sizes[u];
			NR_THREADS = number_of_threads[v];

			float **m1, **m2, **m3;
			double start, end, parallel_duration, serial_duration;

			int	tid, nthreads, i, j, k;
			float fTemp;

			m1 = new float*[N];
			m2 = new float*[N];
			m3 = new float*[N];
			for (i = 0; i < N; i++){
				m1[i] = new float[N];
				m2[i] = new float[N];
				m3[i] = new float[N];
			}

			// printf("matrix multiplication started\n");
			// printf("initializing matrices ...\n\n");
			for (i = 0; i < N; i++){
				for (j = 0; j < N; j++) {
					m1[i][j] = (float)rand();
					m2[i][j] = (float)rand();
				}
			}

			// printf("staring serial version ...\n");
			start = omp_get_wtime();
			transpose(N, m2);
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					float fTemp = 0;
					for (k = 0; k < N; k++)
						fTemp += m1[i][k] * m2[j][k];
					m3[i][j] = fTemp;
				}
			}
			transpose(N, m2);
			end = omp_get_wtime();
			serial_duration = (double)(end - start);
			// printf("serial run time = %f\n\n", (double)(end - start));

			// openmp version
			#pragma omp parallel num_threads(NR_THREADS) shared(m1, m2, m3, nthreads) private (i, j, k, tid)
			{
				tid = omp_get_thread_num();
				if (tid == 0) {  // only thread 0 prints
					nthreads = omp_get_num_threads();
					// // printf("staring parallel version with %d threads...\n",nthreads);
					start = omp_get_wtime();  // master thread measures the execution time
				}

				// transpose m2
				#pragma omp for
					for (int i = 0; i < N; i++)
						for (int j = i + 1; j < N; j++){
							fTemp = m2[i][j];
							m2[i][j] = m2[j][i];
							m2[j][i] = fTemp;
						}
				// matrix multiplication
				#pragma omp for
					for (i = 0; i < N; i++) {
						for (j = 0; j < N; j++) {
							float fTemp = 0;
							for (k = 0; k < N; k++)
								fTemp += m1[i][k] * m2[j][k];
							m3[i][j] = fTemp;
						}
					}
				// transpose m2
				#pragma omp for
					for (int i = 0; i < N; i++)
						for (int j = i + 1; j < N; j++){
							fTemp = m2[i][j];
							m2[i][j] = m2[j][i];
							m2[j][i] = fTemp;
						}
				if (tid == 0) {	// only thread 0 prints
					end = omp_get_wtime();
					parallel_duration = (double)(end - start);
					// // printf("parallel run time = %f\n\n", (double)(end - start));
				}
			} // end of parallel region

			printf("Matrix Size = %d, Number of Threads = %d, Speed-up: %f\n",
					matrix_sizes[u], number_of_threads[v], serial_duration / parallel_duration);
			result[u][v] = serial_duration / parallel_duration;
		}
	}

	for (int u = 0; u < 4; u++)
	{
		printf("Matrix Size = %d :\t[", matrix_sizes[u]);
		for (int v = 0; v < 5; v++)
			printf("%f,", result[u][v] / number_of_threads[v]);
		printf("%f]\n", result[u][5] / number_of_threads[5]);
	}

	return 0;
}
