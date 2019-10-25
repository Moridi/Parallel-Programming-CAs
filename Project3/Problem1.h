#include "ipp.h"

#define ARRAY_SIZE 100000
#define MAX_NUM 1000
const float MAX_FLOAT = 1001;

float min(float a, float b);
void initialize_min_vector();
Ipp64u find_min_serial();
Ipp64u find_min_parallel();
void problem1();