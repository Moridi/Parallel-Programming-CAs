#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
#include "ipp.h"
#include "intrin.h"

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

void show_smoothed_image(IplImage* smooth_img)
{
	IplImage* in_show = cvCreateImage(cvSize((int)(img_width),
		(int)(img_height)),
	in_img->depth,
	in_img->nChannels);

	IplImage* out_smooth = cvCreateImage(cvSize((int)(img_width),
		(int)(img_height)),
		smooth_img->depth,
		smooth_img->nChannels);

	cvResize(in_img, in_show);
	cvResize(smooth_img, out_smooth);

	namedWindow("Input image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Input image", in_show);
	namedWindow("Smoothed image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Smoothed image", out_smooth);
	waitKey(0);
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


	end = ippGetCpuClocks();
	serial_duration = end - start;

	show_smoothed_image(smooth_img);
	printf("Serial Run time = %d\n", (Ipp32s)serial_duration);
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
