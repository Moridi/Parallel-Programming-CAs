#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ipp.h"
#include "intrin.h"

using namespace cv;
using namespace std;

IplImage *in_img;
unsigned char *in_img_char;

void load_image()
{
	in_img = cvLoadImage("./lena.png");
	
	if (!in_img)
	{
		cout << "could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	namedWindow("Input image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Input image", in_img);
	waitKey(0);

	in_img_char = (unsigned char *)in_img->imageData;
}

Ipp64u smooth_serial()
{
	Ipp64u start, end, serial_duration;
	IplImage *smooth_img;
	unsigned char *smooth_img_char;

	start = ippGetCpuClocks();



	end = ippGetCpuClocks();
	serial_duration = end - start;

	return serial_duration;
}

Ipp64u smooth_parallel()
{
	Ipp64u start, end, parallel_duration;
	IplImage *smooth_img;
	unsigned char *smooth_img_char;

	start = ippGetCpuClocks();



	end = ippGetCpuClocks();
	parallel_duration = end - start;

	return parallel_duration;
}

void problem3()
{
	load_image();

	printf("=== Image Smoothing ===\n");
	Ipp64u serial_duration = smooth_serial();
	Ipp64u parallel_duration = smooth_parallel();

	printf("Speed-up = %d\n", serial_duration / parallel_duration);
}
