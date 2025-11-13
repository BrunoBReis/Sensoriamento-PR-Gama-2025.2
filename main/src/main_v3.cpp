#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  string filename =
      "../images/"
      "2022-05-18-00_00_2022-05-18-23_59_Sentinel-2_L2A_True_Color.png";

  Mat image = imread(filename);

  if (image.empty()) {
    cerr << "Error: Could load image\n";
    return -1;
  }

  if (image.channels() != 3) {
    cerr << "Error: Image not BGR\n";
    return -1;
  }

  // Converter a imagem para o espaço de cores HSV
  Mat hsv;
  cvtColor(image, hsv, COLOR_BGR2HSV);

  // Definir os limites da cor verde em HSV
  Scalar lower_green(35, 0, 0);
  Scalar upper_green(85, 255, 255);
  Scalar lower_brown(10, 0, 0);
  Scalar upper_brown(30, 255, 255);

  // Criar máscaras para as cores azul, branco e roxo
  Mat mask_green, mask_brown;
  inRange(hsv, lower_green, upper_green, mask_green);
  inRange(hsv, lower_brown, upper_brown, mask_brown);

  // adding range
  Mat masks = mask_brown;

  // Aplicar a máscara à imagem original
  Mat result;
  bitwise_and(image, image, result, masks);

  // calculating percentage
  double green_pixels = countNonZero(mask_green);
  double total_pixels = image.rows * image.cols;
  double percentage = (green_pixels / total_pixels) * 100.0;

  // adding text

  putText(image, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  putText(result, "Tratada", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);

  Mat concat_image;
  hconcat(image, result, concat_image);

  resize(concat_image, concat_image, Size(), 0.8, 0.8);

  cout << percentage << '\n';

  // Mostrar as imagens
  imshow("Imagem Original", concat_image);
  waitKey(0);

  return 0;
}
