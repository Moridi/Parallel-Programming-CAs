#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

using namespace std;
using namespace cv;

void show_image(IplImage* img, string directory, string name)
{
	IplImage* out_img = cvCreateImage(cvSize((int)(img->width),
		(int)(img->height)), img->depth, img->nChannels);

	cvResize(img, out_img);

	namedWindow(name, CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), out_img);
	name = "./images/output/" + directory + "/" + name + ".png";
	cvSaveImage(name.c_str(), out_img);
	waitKey(0);
}