#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ipp.h"
#include "intrin.h"

#include "Common.h"

using namespace cv;
using namespace std;

IplImage *in_img;
int img_height, img_width;
unsigned char *in_img_char;

void load_image()
{
	in_img = cvLoadImage("./lena.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	if (!in_img)
	{
		cout << "could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	img_height = in_img->height;
	img_width = in_img->width;

	in_img_char = (unsigned char *)in_img->imageData;
}

Ipp64u smooth_serial()
{
	Ipp64u start, end, serial_duration;
	IplImage* smooth_img = cvCreateImage(cvGetSize(in_img), IPL_DEPTH_8U, 1);
	unsigned char* smooth_img_char = (unsigned char*)smooth_img->imageData;

	start = ippGetCpuClocks();

	for (int i = 0; i < img_height - 2; ++i)
		for (int j = 0; j < img_width - 2; ++j)
		{
			int avg = 0;
			for (int u = 0; u < 3; u++)
				for (int v = 0; v < 3; v++)
					avg += in_img_char[(i + u) * img_width + (j + v)];
			smooth_img_char[i * img_width + j] = avg / 9;
		}

	for (int i = img_height - 2; i < img_height; i++)
		for (int j = img_width - 2; j < img_width; j++)
			smooth_img_char[i * img_width + j] = in_img_char[i * img_width + j];

	end = ippGetCpuClocks();
	serial_duration = end - start;

	printf("Serial Run time = %d\n", (Ipp32s)serial_duration);
	show_image(smooth_img, "Problem_3", "Serial");
	
	return serial_duration;
}

Ipp64u smooth_parallel()
{
	Ipp64u start, end, parallel_duration;
	IplImage* smooth_img = cvCreateImage(cvGetSize(in_img), IPL_DEPTH_8U, 1);
	unsigned char* smooth_img_char = (unsigned char*)smooth_img->imageData;

	start = ippGetCpuClocks();

	__m128i smoothed_data[3][3];
	__m128i avg;

	for (int i = 0; i < img_height - 2; i++)
		for (int j = 0; j < img_width - 2; j += 16)
		{
			for (int u = 0; u < 3; u++)
				for (int v = 0; v < 3; v++)
					smoothed_data[u][v] = _mm_loadu_si128(
							(__m128i*)(in_img_char + (i + u) * img_width + (j + v)));

			smoothed_data[0][0] = _mm_avg_epu8(smoothed_data[0][0], smoothed_data[0][1]);
			smoothed_data[2][0] = _mm_avg_epu8(smoothed_data[2][0], smoothed_data[2][1]);
			smoothed_data[0][0] = _mm_avg_epu8(smoothed_data[0][0], smoothed_data[2][0]);

			smoothed_data[0][2] = _mm_avg_epu8(smoothed_data[0][2], smoothed_data[2][2]);
			smoothed_data[1][0] = _mm_avg_epu8(smoothed_data[1][0], smoothed_data[1][2]);
			smoothed_data[0][2] = _mm_avg_epu8(smoothed_data[0][2], smoothed_data[1][0]);

			smoothed_data[0][0] = _mm_avg_epu8(smoothed_data[0][2], smoothed_data[0][0]);

			avg = _mm_avg_epu8(smoothed_data[0][0], smoothed_data[0][1]);
			avg = _mm_avg_epu8(smoothed_data[0][0], avg);
			avg = _mm_avg_epu8(smoothed_data[0][0], avg);

			_mm_storeu_si128((__m128i*)(smooth_img_char + i * img_width + j), avg);
		}

	for (int i = img_height - 2; i < img_height; i++)
		for (int j = img_width - 2; j < img_width; j++)
			smooth_img_char[i * img_width + j] = in_img_char[i * img_width + j];

	end = ippGetCpuClocks();
	parallel_duration = end - start;

	printf("Parallel Run time = %d\n", (Ipp32s)parallel_duration);
	show_image(smooth_img, "Problem_3", "Parallel");

	return parallel_duration;
}

void problem3()
{
	load_image();

	printf("=== Image Smoothing ===\n");
	Ipp64u serial_duration = smooth_serial();
	Ipp64u parallel_duration = smooth_parallel();

	printf("Speed-up = %f\n", (float)serial_duration / (float)parallel_duration);
}
