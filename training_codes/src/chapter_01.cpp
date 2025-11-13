#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {

  // definindo uma imagem vazia
  Mat image;
  string filename = "../images/dexter.png";
  cout << image.rows << ' ' << image.cols;

  // fazendo a leitura de imagem
  image = imread(filename, IMREAD_GRAYSCALE);

  if (image.empty()) {
    cerr << "Error: image not found\n";
    return -1;
  }

  // definindo window
  namedWindow("Original Image");
  imshow("Original Image", image);

  // nova imagem
  Mat result;
  flip(image, result, 1);

  namedWindow("Output Image");
  imshow("Output Image", result);

  waitKey(0);
  destroyAllWindows();

  return 0;
}
