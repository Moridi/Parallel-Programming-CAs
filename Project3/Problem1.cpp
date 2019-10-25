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
		vector[i] = static_cast <float> (rand() % MAX_NUM);
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

	printf("Serial Run time = %d \n", (Ipp32s)serial_duration);
	return serial_duration;
}

Ipp64u find_min_parallel()
{
	Ipp64u parallel_duration = 0;
	return parallel_duration;
	//assert(size_t(vector) % 1)
}

void problem1()
{
	initialize_min_vector();

	printf("=== Finding Minimum ===\n");
	Ipp64u serial_duration = find_min_serial();
	Ipp64u parallel_duration = find_min_parallel();

	while (1)
		printf("");
	exit(EXIT_SUCCESS);
}