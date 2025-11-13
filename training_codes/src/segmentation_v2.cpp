// Para compilar: g++ segmentacao.cpp -o main.exe `pkg-config --cflags --libs
// opencv4` Para executar: ./main.exe

#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  Mat luz = imread("../images/luz.png"); // carregar a imagem da luz visível

  filesystem::path outdir = "../results/segmentation/test_1/";
  filesystem::create_directories(outdir);

  if (luz.empty()) {
    cerr << "Error: Could load image\n";
    return -1;
  }

  if (luz.channels() != 3) {
    cerr << "Error: Image not BGR\n";
    return -1;
  }

  // Converter a imagem para o espaço de cores HSV
  Mat hsv;
  cvtColor(luz, hsv, COLOR_BGR2HSV);

  // Definir os limites da cor azul em HSV
  Scalar menorazul(126, 100, 100);
  Scalar maiorazul(150, 255, 255);
  Scalar branco(0, 0, 240);
  Scalar branco2(179, 55, 255);
  Scalar roxo(151, 100, 100);
  Scalar roxo2(169, 255, 255);

  // Criar máscaras para as cores azul, branco e roxo
  Mat mascara1, mascara2, mascara3;
  inRange(hsv, menorazul, maiorazul, mascara1);
  inRange(hsv, branco, branco2, mascara2);
  inRange(hsv, roxo, roxo2, mascara3);

  // Combinar as três máscaras
  Mat mascara = mascara1 | mascara2 | mascara3;

  // Aplicar a máscara à imagem original
  Mat resultado;
  bitwise_and(luz, luz, resultado, mascara);

  // resize(luz, luz, Size(), 0.5, 0.5);

  // Mostrar as imagens
  imshow("Imagem Original", luz);
  imshow("Máscara", mascara);
  imwrite("../results/segmentation/test_1/process.png", resultado);

  waitKey(0);
  return 0;
}
