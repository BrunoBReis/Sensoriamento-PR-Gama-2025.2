#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void) {

  // Fazendo a leitura manual
  string filename = "../images/"
                    "2021-08-26-00_00_2021-08-26-23_59_Sentinel-2_L2A_"
                    "Highlight_Optimized_Natural_Color (1).png";
  Mat image = imread(filename);

  // Tratamento de Erros
  if (image.empty()) {
    cerr << "Error: image not found\n";
    return -1;
  }
  if (image.channels() != 3) {
    cerr << "Error: image not BGR\n";
    return -1;
  }

  // Convertendo imagem para HSV
  Mat hsv;
  cvtColor(image, hsv, COLOR_BGR2HSV);

  // HSV COLORS
  // Color, escolha da cor (0 - 179)
  // Saturation, quantidade de cor adicionada (0 = cinza - 255 = cor pura)
  // Value, brilho e luminosidade (0 = preto - 255 = claro)

  // Verde (35 - 85) -> Vegetação Nativa
  Scalar lower_native(35, 0, 0);
  Scalar upper_native(85, 255, 255);
  Mat mask_native;
  inRange(hsv, lower_native, upper_native, mask_native);

  // Marrom (10-25) -> Vegetação Agícola
  Scalar lower_agriculture(10, 0, 0);
  Scalar upper_agriculture(25, 255, 255);
  Mat mask_agriculture;
  inRange(hsv, lower_agriculture, upper_agriculture, mask_agriculture);

  // Cálculo
  double native_percentage =
      (countNonZero(mask_native) * 100.0) / (image.rows * image.cols);
  double agriculture_percentage =
      (countNonZero(mask_agriculture) * 100.0) / (image.rows * image.cols);

  // Mostra resultado
  cout << "Native percentage: " << native_percentage << '\n';
  cout << "Agriculture percentage: " << agriculture_percentage << '\n';

  return 0;
}
