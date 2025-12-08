#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {
  Mat frame, hsv, mask;
  string filename = "../images/"
                    "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_"
                    "Highlight_Optimized_Natural_Color (1).png";

  int h_min = 35, s_min = 40, v_min = 40;
  int h_max = 85, s_max = 255, v_max = 255;

  frame = imread(filename);

  if (frame.empty()) {
    cerr << "Error: Could load image\n";
    return -1;
  }

  namedWindow("Ajuste de verde", WINDOW_AUTOSIZE);

  createTrackbar("H min", "Ajuste de Verde", &h_min, 179);
  createTrackbar("H max", "Ajuste de Verde", &h_max, 179);
  createTrackbar("S min", "Ajuste de Verde", &s_min, 255);
  createTrackbar("S max", "Ajuste de Verde", &s_max, 255);
  createTrackbar("V min", "Ajuste de Verde", &v_min, 255);
  createTrackbar("V max", "Ajuste de Verde", &v_max, 255);

  while (true) {
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    Scalar lower(h_min, s_min, v_min);
    Scalar upper(h_max, s_max, v_max);
    inRange(hsv, lower, upper, mask);

    // imshow("Original", frame);
    imshow("Mascara", frame);

    int key = waitKey(1);
    if (key == 27)
      break;
  }

  return 0;
}
