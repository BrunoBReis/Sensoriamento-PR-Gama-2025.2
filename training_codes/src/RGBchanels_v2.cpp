#include <filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// ERRADO

int main(void) {

  // defining path
  string path = "../images/espectro.png";
  Mat image = imread(path);

  // creating correct path
  filesystem::path outdir = "../results/RGBcode/test_1/";
  filesystem::create_directories(outdir);

  // error handling //

  if (image.empty()) {
    cerr << "Error: Could not load image\n";
    return -1;
  }

  if (image.channels() != 3) {
    cerr << "Error: Image is not RGB\n";
    return -1;
  }

  // split image into 3 channels
  vector<Mat> channels;
  split(image, channels);
  // [0] = blue; [1] = green; [2] = red;

  // creating images with one channel removed

  // without blue //
  vector<Mat> noBlue = channels;
  noBlue[0] = Mat::zeros(image.size(), CV_8UC1);
  Mat imageNoBlue;
  merge(noBlue, imageNoBlue);

  // without green //
  vector<Mat> noGreen = channels;
  noGreen[1] = Mat::zeros(image.size(), CV_8UC1);
  Mat imageNoGreen;
  merge(noGreen, imageNoGreen);

  // without red //
  vector<Mat> noRed = channels;
  noGreen[2] = Mat::zeros(image.size(), CV_8UC1);
  Mat imageNoRed;
  merge(noRed, imageNoRed);

  // Convert original to grey
  Mat grayImage;
  cvtColor(image, grayImage, COLOR_BGR2GRAY);

  // Display Results

  imwrite("../results/RGBcode/test_1/no_blue.png", imageNoBlue);
  imwrite("../results/RGBcode/test_1/no_green.png", imageNoGreen);
  imwrite("../results/RGBcode/test_1/no_red.png", imageNoRed);
  imwrite("../results/RGBcode/test_1/gray.png", grayImage);

  // imshow("Original", image);
  // imshow("Without Blue", imageNoBlue);
  // imshow("Without Green", imageNoGreen);
  // imshow("Without Red", imageNoRed);
  // imshow("Grayscale Image", grayImage);
  //
  // cout << "Press any key to close the windows...\n";
  // waitKey(0);
  // destroyAllWindows();

  return 0;
}
