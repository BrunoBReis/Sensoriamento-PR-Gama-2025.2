#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {

  // reading
  string filename = "";

  int image_choice = 0;
  cout << "(1) Image with river\n";
  cout << "(2) Image with big squares\n";
  cout << "(3) Image with a lot of clouds\n";
  cout << "(4) Image the most hard image\n";
  cin >> image_choice;
  switch (image_choice) {
  case 1:
    filename = "../images/"
               "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_Highlight_"
               "Optimized_Natural_Color (1).png";
    break;
  case 2:
    filename = "../images/"
               "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_Highlight_"
               "Optimized_Natural_Color (2).png";
    break;
  case 3:
    filename = "../images/"
               "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_Highlight_"
               "Optimized_Natural_Color.png";
    break;
  case 4:
    filename =
        "../images/"
        "2022-05-18-00_00_2022-05-18-23_59_Sentinel-2_L2A_True_Color.png";
    break;
  }

  Mat image = imread(filename);

  // error
  if (image.empty()) {
    cerr << "Error: image not found\n";
    return -1;
  }

  // filters add on image to get more accure data
  // GaussianBlur(image, image, Size(5, 5), 0);
  // bilateralFilter(image, image, 9, 75, 75);

  // playing wiht K //
  Mat data;
  image.convertTo(data, CV_32F);
  data = data.reshape(1, data.total());

  int K = 3;
  Mat labels, centers;
  kmeans(data, K, labels,
         TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 10, 1.0), 3,
         KMEANS_PP_CENTERS, centers);

  centers = centers.reshape(3, centers.rows);
  Mat clustered(image.size(), CV_32FC3);

  for (int i = 0; i < data.rows; ++i) {
    int cluster_idx = labels.at<int>(i);
    Vec3f color = Vec3f(centers.at<float>(cluster_idx, 0),
                        centers.at<float>(cluster_idx, 1),
                        centers.at<float>(cluster_idx, 2));
    clustered.at<Vec3f>(i / image.cols, i % image.cols) = color;
  }

  clustered.convertTo(clustered, CV_8U);
  clustered.convertTo(clustered, CV_8U, 1.0);
  // playing wiht K //

  // convert to hsv and start segmentation
  Mat hsv;
  cvtColor(image, hsv, COLOR_BGR2HSV);

  // HSV COLORS
  // color, color that is selected (0 - 179)
  // saturation, quantity of color added to filter (0 = cinza - 255 = cor pura)
  // value, bright and luminosity (0 = preto - 255 = claro)

  // green (35 - 85)
  Scalar lower_native(35, 0, 0);
  Scalar upper_native(85, 255, 255);
  Mat mask_native;
  Mat image_native = Mat::zeros(image.size(), image.type());
  inRange(hsv, lower_native, upper_native, mask_native);
  // Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
  // morphologyEx(mask_native, mask_native, MORPH_OPEN, kernel);
  // morphologyEx(mask_native, mask_native, MORPH_CLOSE, kernel);

  // brown ()
  Scalar lower_agriculture(10, 0, 0);
  Scalar upper_agriculture(25, 255, 255);
  Mat mask_agriculture;
  Mat image_agriculture = Mat::zeros(image.size(), image.type());
  inRange(hsv, lower_agriculture, upper_agriculture, mask_agriculture);
  // kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
  // morphologyEx(mask_agriculture, mask_agriculture, MORPH_OPEN, kernel);
  // morphologyEx(mask_agriculture, mask_agriculture, MORPH_CLOSE, kernel);

  image.copyTo(image_native, mask_native);
  image.copyTo(image_agriculture, mask_agriculture);

  // percentage
  double native_percentage =
      (countNonZero(mask_native) * 100.0) / (image.rows * image.cols);
  double agriculture_percentage =
      (countNonZero(mask_agriculture) * 100.0) / (image.rows * image.cols);

  // printing
  cout << "Native percentage: " << native_percentage << '\n';
  cout << "Agriculture percentage: " << agriculture_percentage << '\n';

  // adding text to images
  putText(image, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(image_native, "Vegetacao", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(image_agriculture, "Agricultura", Point(10, 30), FONT_HERSHEY_SIMPLEX,
          1, Scalar(255, 255, 255), 2);

  putText(clustered, "Kmeans", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  // organizing images
  Mat result;
  hconcat(image_agriculture, image_native, result);
  resize(result, result, Size(), 0.8, 0.8);
  Mat native_original;
  hconcat(image_native, image, native_original);
  resize(native_original, native_original, Size(), 0.8, 0.8);
  Mat agriculture_original;
  hconcat(image_agriculture, image, agriculture_original);
  resize(agriculture_original, agriculture_original, Size(), 0.8, 0.8);
  Mat clusterd_original;
  hconcat(clustered, image, clusterd_original);
  resize(clusterd_original, clusterd_original, Size(), 0.8, 0.8);

  // choice
  cout << "(1) Agriculture + Native\n";
  cout << "(2) Native + Original\n";
  cout << "(3) Agriculture + Original\n";
  cout << "(4) Clustered + Original\n";
  cout << "(5) Clustered\n";
  cout << "(6) Original\n";

  int choice = 0;
  cin >> choice;
  switch (choice) {
  case 1:
    imshow("Resultado Final", result);
    waitKey(0);
    destroyAllWindows();
    break;
  case 2:
    imshow("Native", native_original);
    waitKey(0);
    destroyAllWindows();
    break;
  case 3:
    imshow("Agriculture", agriculture_original);
    waitKey(0);
    destroyAllWindows();
    break;
  case 4:
    imshow("Clustered + Original", clusterd_original);
    waitKey(0);
    destroyAllWindows();
    break;
  case 5:
    imshow("Clustered", clustered);
    waitKey(0);
    destroyAllWindows();
    break;
  case 6:
    imshow("Original", image);
    waitKey(0);
    destroyAllWindows();
    break;
  }

  return 0;
}
