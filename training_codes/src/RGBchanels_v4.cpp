#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// this needs to be done because asssign channels to a vector
// does not copy the matrix, instead copies it reference
vector<Mat> cloneChannels(const vector<Mat> &src) {
  // run for every channel clone method
  // similar to : cloned = {channel[0].clone(), channel[1].clone(), ...}
  vector<Mat> cloned;
  for (const auto &c : src)
    cloned.emplace_back(c.clone());
  return cloned;
}

int main(void) {

  // defining path
  string path = "../images/espectro.png";
  Mat image = imread(path, IMREAD_COLOR_BGR);

  // error handling //

  if (image.empty()) {
    cerr << "Error: Could not load image\n";
    return -1;
  }

  if (image.channels() != 3) {
    cerr << "Error: Image is not BGR\n";
    return -1;
  }

  // split image into 3 channels
  vector<Mat> channels;
  split(image, channels);
  // [0] = blue; [1] = green; [2] = red;

  // getting the same reference
  vector<Mat> noBlue = cloneChannels(channels);
  vector<Mat> noGreen = cloneChannels(channels);
  vector<Mat> noRed = cloneChannels(channels);

  // removing specfic color for each

  noBlue[0] = Mat::zeros(image.size(), CV_8UC1);
  noGreen[1] = Mat::zeros(image.size(), CV_8UC1);
  noRed[2] = Mat::zeros(image.size(), CV_8UC1);

  Mat imageNoBlue, imageNoGreen, imageNoRed;
  merge(noBlue, imageNoBlue);
  merge(noGreen, imageNoGreen);
  merge(noRed, imageNoRed);

  // Convert original to grey
  Mat grayImage;
  cvtColor(image, grayImage, COLOR_BGR2GRAY);

  // Display Results

  imwrite("../results/RGBcode/test_3/no_blue.png", imageNoBlue);
  imwrite("../results/RGBcode/test_3/no_green.png", imageNoGreen);
  imwrite("../results/RGBcode/test_3/no_red.png", imageNoRed);
  imwrite("../results/RGBcode/test_3/gray.png", grayImage);

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
