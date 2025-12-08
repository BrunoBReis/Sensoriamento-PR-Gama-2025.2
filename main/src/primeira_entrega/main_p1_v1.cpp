#include <filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(void) {

  // Defining image source
  string filename = "../images/"
                    "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_"
                    "Highlight_Optimized_Natural_Color (1).png";
  Mat image = imread(filename, IMREAD_COLOR_BGR);
  auto image_size = image.size();

  // creating correct path
  filesystem::path outdir = "../results/main/test_1/";
  filesystem::create_directories(outdir);

  if (image.empty()) {
    cerr << "Error: Could load image\n";
    return -1;
  }

  if (image.channels() != 3) {
    cerr << "Error: Image not BGR\n";
    return -1;
  }

  // Declarations

  vector<Mat> noBlue;
  vector<Mat> noGreen;
  vector<Mat> noRed;

  Mat imageNoBlue;
  Mat imageNoGreen;
  Mat imageNoRed;
  Mat grayImage;

  // Blue //

  split(image, noBlue);
  noBlue[0] = Mat::zeros(image_size, CV_8UC1);
  merge(noBlue, imageNoBlue);

  // Green //

  split(image, noGreen);
  noGreen[1] = Mat::zeros(image_size, CV_8UC1);
  merge(noGreen, imageNoGreen);

  // Red //

  split(image, noRed);
  noRed[2] = Mat::zeros(image_size, CV_8UC1);
  merge(noRed, imageNoRed);

  // Gray //

  cvtColor(image, grayImage, COLOR_BGR2GRAY);

  // Display Results //

  // imwrite("../results/RGBcode/test_4/no_blue.png", imageNoBlue);
  // imwrite("../results/RGBcode/test_4/no_green.png", imageNoGreen);
  // imwrite("../results/RGBcode/test_4/no_red.png", imageNoRed);
  // imwrite("../results/RGBcode/test_4/gray.png", grayImage);

  imshow("Original", image);
  waitKey(0);
  imshow("Without Blue", imageNoBlue);
  waitKey(0);
  imshow("Without Green", imageNoGreen);
  waitKey(0);
  imshow("Without Red", imageNoRed);
  waitKey(0);
  imshow("Grayscale Image", grayImage);
  waitKey(0);

  // cout << "Press any key to close the windows...\n";
  // waitKey(0);
  // destroyAllWindows();

  return 0;
}
