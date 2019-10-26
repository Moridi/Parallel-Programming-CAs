#include <iostream>
#include <cstdlib>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ipp.h"
#include "intrin.h"

using namespace cv;
using namespace std;

IplImage *first, *second;
unsigned char *first_data, *second_data;
int height, width;

void show_image(IplImage* img)
{
	IplImage* out_img = cvCreateImage(cvSize((int)(img->width / 2),
		(int)(img->height / 2)), img->depth, img->nChannels);

	cvResize(img, out_img);

	namedWindow("Difference", CV_WINDOW_AUTOSIZE);
	cvShowImage("Difference", out_img);
	waitKey(0);
}

void load_images()
{
	first = cvLoadImage("./first.png", CV_LOAD_IMAGE_GRAYSCALE);
	second = cvLoadImage("./second.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!first || !second)
	{
		cout << "could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	show_image(first);
	show_image(second);
	
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

	show_image(diff_img);
	return serial_duration;
}

Ipp64u diff_parallel()
{
	Ipp64u start, end, parallel_duration;
	IplImage *smooth_img;
	unsigned char *smooth_img_char;

	start = ippGetCpuClocks();



	end = ippGetCpuClocks();
	parallel_duration = end - start;

	return parallel_duration;
}

void problem4()
{
	load_images();

	printf("=== Image Subtraction ===\n");
	Ipp64u serial_duration = diff_serial();
	Ipp64u parallel_duration = diff_parallel();

	printf("Speed-up = %d\n", serial_duration / parallel_duration);
}
