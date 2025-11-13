#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

Mat function() {
  Mat ima(500, 500, CV_8U, 50);
  return ima;
}

int main(void) {

  string filename = "../images/dexter.png";

  namedWindow("Image 1");
  namedWindow("Image 2");
  namedWindow("Image 3");
  namedWindow("Image 4");
  namedWindow("Image 5");
  namedWindow("Image");

  // creating an image 240 rows and 320 cols
  Mat image1(240, 320, CV_8U, 100);

  imshow("Image", image1);
  waitKey(0);

  // re-allocate a new image
  image1.create(200, 200, CV_8U);
  image1 = 200;

  imshow("Image", image1);
  waitKey(0);

  // create a red color image
  Mat image2(240, 320, CV_8UC3, Scalar(0, 0, 255));

  imshow("Image", image2);
  waitKey(0);

  Mat image3 = imread(filename, IMREAD_GRAYSCALE);

  return 0;
}
