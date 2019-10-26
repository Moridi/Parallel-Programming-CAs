#include <iostream>
#include <cstdlib>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ipp.h"
#include "intrin.h"

#include "Common.h"

using namespace cv;
using namespace std;

IplImage *first, *second;
unsigned char *first_data, *second_data;
int height, width;

void load_images()
{
	first = cvLoadImage("./first.png", CV_LOAD_IMAGE_GRAYSCALE);
	second = cvLoadImage("./second.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!first || !second)
	{
		cout << "could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	first_data = (unsigned char *)first->imageData;
	second_data = (unsigned char *)second->imageData;

	height = first->height;
	width = first->width;
}

Ipp64u diff_serial()
{
	Ipp64u start, end, serial_duration;
	IplImage* diff_img = cvCreateImage(cvGetSize(first), IPL_DEPTH_8U, 1);
	unsigned char* diff_img_char = (unsigned char*)diff_img->imageData;

	start = ippGetCpuClocks();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			diff_img_char[i * width + j] = abs(second_data[i * width + j] -
					first_data[i * width + j]);

	end = ippGetCpuClocks();
	serial_duration = end - start;

	show_image(diff_img, "Problem_4", "Serial Output");
	printf("Serial Run time = %d\n", (Ipp32s)serial_duration);

	return serial_duration;
}

Ipp64u diff_parallel()
{
	Ipp64u start, end, parallel_duration;
	IplImage* diff_img = cvCreateImage(cvGetSize(first), IPL_DEPTH_8U, 1);
	unsigned char* diff_img_char = (unsigned char*)diff_img->imageData;

	start = ippGetCpuClocks();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; j += 16)
		{
			__m128i m1 = _mm_loadu_si128((__m128i*)(first_data + i * width + j));
			__m128i m2 = _mm_loadu_si128((__m128i*)(second_data + i * width + j));
			__m128i diff = _mm_sub_epi8(m1, m2);
			__m128i abs = _mm_abs_epi8(diff);

			_mm_storeu_si128((__m128i*)(diff_img_char + i * width + j), abs);
		}

	end = ippGetCpuClocks();
	parallel_duration = end - start;

	show_image(diff_img, "Problem_4", "Parallel Output");
	printf("Parallel Run time = %d\n", (Ipp32s)parallel_duration);
	return parallel_duration;
}

void problem4()
{
	load_images();

	printf("=== Image Subtraction ===\n");
	Ipp64u serial_duration = diff_serial();
	Ipp64u parallel_duration = diff_parallel();

	printf("Speed-up = %f\n", (float)serial_duration / parallel_duration);
}
