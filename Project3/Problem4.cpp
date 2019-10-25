#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ipp.h"
#include "intrin.h"

using namespace cv;
using namespace std;

IplImage *in_img1, *in_img2;
unsigned char *in_img_char1, *in_img_char2;

void load_images()
{
	in_img1 = cvLoadImage("./lena.png");
	in_img2 = cvLoadImage("./lena.png");

	if (!in_img1 || !in_img2)
	{
		cout << "could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	namedWindow("Input 1 image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Input 1 image", in_img1);
	namedWindow("Input 2 image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Input 2 image", in_img2);
	waitKey(0);

	in_img_char1 = (unsigned char *)in_img1->imageData;
	in_img_char2 = (unsigned char *)in_img2->imageData;
}

Ipp64u diff_serial()
{
	Ipp64u start, end, serial_duration;
	IplImage *smooth_img;
	unsigned char *smooth_img_char;

	start = ippGetCpuClocks();



	end = ippGetCpuClocks();
	serial_duration = end - start;

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
