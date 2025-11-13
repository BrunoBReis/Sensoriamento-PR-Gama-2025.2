#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {

  string filename = "../images/"
                    "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_"
                    "Highlight_Optimized_Natural_Color (2).png";
  Mat image = imread(filename);

  if (image.empty()) {
    cerr << "Error: image not found\n";
    return -1;
  }

  Mat hsv;
  cvtColor(image, hsv, COLOR_BGR2HSV);

  // faixas

  // 35-85 = verde
  Scalar lower_native(35, 40, 40);
  Scalar upper_native(85, 255, 255);
  Mat mask_native;
  Mat image_native = Mat::zeros(image.size(), image.type());
  inRange(hsv, lower_native, upper_native, mask_native);

  // 25-40 = marrom
  Scalar lower_agriculture(25, 20, 80);
  Scalar upper_agriculture(40, 255, 255);
  Mat mask_agriculture;
  Mat image_agriculture = Mat::zeros(image.size(), image.type());
  inRange(hsv, lower_agriculture, upper_agriculture, mask_agriculture);

  image.copyTo(image_native, mask_native);
  image.copyTo(image_agriculture, mask_agriculture);

  // percentage
  double native_percentage =
      (countNonZero(mask_native) * 100.0) / (image.rows * image.cols);
  double agriculture_percentage =
      (countNonZero(mask_agriculture) * 100.0) / (image.rows * image.cols);

  cout << "Native percentage: " << native_percentage << '\n';
  cout << "Agriculture percentage: " << agriculture_percentage << '\n';

  putText(image, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(mask_native, "Vegetação", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(mask_agriculture, "Agricultura", Point(10, 30), FONT_HERSHEY_SIMPLEX,
          1, Scalar(255, 255, 255), 2);

  Mat line, result;
  hconcat(image_agriculture, image_native, result);

  resize(result, result, Size(), 0.8, 0.8);

  imshow("Resultado Final", result);
  waitKey(0);
  destroyAllWindows();

  return 0;
}
