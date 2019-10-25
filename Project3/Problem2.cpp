#include <stdlib.h>
#include "stdio.h"

#include "ipp.h"
#include "intrin.h"

#include "Problem2.h"

float *vector1, *vector2;

void initialize_two_vectors()
{
	vector1 = new float[ARRAY_SIZE];
	vector2 = new float[ARRAY_SIZE];

	if (!vector1 || !vector2)
	{
		printf("Memory allocation error!!\n");
		exit(EXIT_FAILURE);
	}

	for (long i = 0; i < ARRAY_SIZE; i++)
	{
		vector1[i] = static_cast <float> (rand() % MAX_NUM);
		vector2[i] = static_cast <float> (rand() % MAX_NUM);
	}
}

Ipp64u find_matrix_value_serial()
{
	Ipp64u start, end, serial_duration;
	float result = 0, sub;

	start = ippGetCpuClocks();
	for (long i = 0; i < ARRAY_SIZE; i++)
	{
		sub = vector1[i] - vector2[i];
		sub *= sub;
		result += sub;
	}

	end = ippGetCpuClocks();
	serial_duration = end - start;

	printf("Serial Run time = %d , Result = %f\n", (Ipp32s)serial_duration, result);
	return serial_duration;
}

Ipp64u find_matrix_value_parallel()
{
	Ipp64u start, end, parallel_duration;
	start = ippGetCpuClocks();

	__m128 sum = _mm_set1_ps(0.0f);

	for (long i = 0; i < ARRAY_SIZE; i += 4)
		sum = _mm_add_ps(sum, _mm_mul_ps(
				_mm_sub_ps(_mm_loadu_ps(&vector1[i]),
				_mm_loadu_ps(&vector2[i])),
				_mm_sub_ps(_mm_loadu_ps(&vector1[i]),
				_mm_loadu_ps(&vector2[i]))));

	sum = _mm_hadd_ps(sum, sum);
	sum = _mm_hadd_ps(sum, sum);
	float result = _mm_cvtss_f32(sum);

	end = ippGetCpuClocks();
	parallel_duration = end - start;
	printf("Parallel Run time = %d , Result = %f\n", (Ipp32s)parallel_duration, result);
	return parallel_duration;
}

void problem2()
{
	Ipp64u serial_duration, parallel_duration;
	initialize_two_vectors();

	printf("=== Matrix Operations ===\n");
	serial_duration = find_matrix_value_serial();
	parallel_duration = find_matrix_value_parallel();

	printf("Speed-up = %f\n", (float)serial_duration / (float)parallel_duration);

	//// Inner product, Vector edition
	//start = ippGetCpuClocks();
	//__m128 sum = _mm_set1_ps(0.0f);
	//for (long i = 0; i < VECTOR_SIZE; i += 4)
	//	sum = _mm_add_ps(sum, _mm_mul_ps(_mm_loadu_ps(&v1[i]), _mm_loadu_ps(&v2[i])));
	//sum = _mm_hadd_ps(sum, sum);
	//sum = _mm_hadd_ps(sum, sum);
	//fVRes = _mm_cvtss_f32(sum);
	//end = ippGetCpuClocks();
	//time2 = end - start;
	//printf("Serial Run time = %d \n", (Ipp32s)time2);
	//printf("Speedup = %f\n", (float)(time1) / (float)time2);

	//exit(EXIT_SUCCESS);
}
