#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

void salt(Mat image, int n, int color) {
  int i, j;

  for (int k = 0; k < n; ++k) {
    i = rand() % image.cols;
    j = rand() % image.rows;
    if (image.type() == CV_8UC1)
      image.at<uchar>(j, i) = color;
    else if (image.type() == CV_8UC3) {
      image.at<cv::Vec3b>(j, i)[0] = color;
      image.at<cv::Vec3b>(j, i)[1] = color;
      image.at<cv::Vec3b>(j, i)[2] = color;
    }
  }
}

int main(void) {
  string filename = "../images/dexter.png";

  Mat image = imread(filename);
  salt(image, 5000, 125);

  imshow("Image", image);
  waitKey(0);

  return 0;
}
