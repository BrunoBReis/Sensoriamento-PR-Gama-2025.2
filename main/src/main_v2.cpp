#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {

  // get image
  string filename = "../images/"
                    "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_"
                    "Highlight_Optimized_Natural_Color (1).png";
  Mat original = imread(filename);

  // error
  if (original.empty()) {
    cerr << "Error: Could load image\n";
    return -1;
  }

  // sharpening image
  Mat sharpened;
  Mat kernel = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
  filter2D(original, sharpened, -1, kernel);

  // adding text
  putText(original, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(sharpened, "Tratada", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  // adding images side by side
  Mat result;
  hconcat(original, sharpened, result);

  // printing images
  imshow("Result", result);
  waitKey(0);

  return 0;
}
