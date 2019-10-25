#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <ipp.h>
#include <intrin.h>
#include <float.h>

#include "Problem1.h"

float* vector;

void initialize_min_vector()
{
	vector = new float[ARRAY_SIZE];

	if (!vector)
	{
		printf("Memory allocation error!!\n");
		exit(EXIT_FAILURE);
	}

	for (long i = 0; i < ARRAY_SIZE; i++)
		vector[i] = static_cast <float> (rand() % MAX_NUM + 4);
}

Ipp64u find_min_serial()
{
	Ipp64u start, end, serial_duration;
	float min_value = FLT_MAX;

	start = ippGetCpuClocks();

	for (long i = 0; i < ARRAY_SIZE; i++)
	if (vector[i] < min_value)
		min_value = vector[i];

	end = ippGetCpuClocks();
	serial_duration = end - start;

	printf("Serial Run time = %d , Min Value = %f\n", (Ipp32s)serial_duration, min_value);
	return serial_duration;
}

float min(float a, float b)
{
	return a < b ? a : b;
}

Ipp64u find_min_parallel()
{
	Ipp64u start, end, parallel_duration;
	start = ippGetCpuClocks();

	__m128 min_values = _mm_set1_ps(MAX_FLOAT);

	for (long i = 0; i < ARRAY_SIZE; i += 4)
		min_values = _mm_min_ps(min_values, _mm_loadu_ps(&vector[i]));

	float min_value = MAX_FLOAT;

	for (int i = 0; i < 4; i++)
		if (min_values.m128_f32[i] < min_value)
			min_value = min_values.m128_f32[i];

	end = ippGetCpuClocks();
	parallel_duration = end - start;
	printf("Parallel Run time = %d , Min Value = %f\n", (Ipp32s)parallel_duration, min_value);
	return parallel_duration;
}

void problem1()
{
	initialize_min_vector();

	printf("=== Finding Minimum ===\n");
	Ipp64u serial_duration = find_min_serial();
	Ipp64u parallel_duration = find_min_parallel();

	printf("Speed-up = %f\n", (float)serial_duration / (float)parallel_duration);
}