// Para compilar: g++ canaisRGB.cpp -o main.o `pkg-config --cflags --libs
// opencv4` Para executar: ./main

#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  Mat espectro = imread("../images/espectro.png");

  if (espectro.empty()) { // verificar se a imagem foi carregada corretamente ou
                          // se o caminho está errado
    printf("Erro ao carregar a imagem.\n");
    return -1; // erro
  }

  // cor azul

  vector<Mat> canal_azul;      // declarando um vetor chamado canais que irá
                               // armazenar múltiplas matrizes (Mat).
  split(espectro, canal_azul); // divide os canais de cores em B, G e R.

  canal_azul[0] = Mat::zeros(
      espectro.size(),
      CV_8UC1); // Iguala o canal de cor azul ao Mat::zeros que cria uma matriz
                // ou imagem de zeros com as mesmas dimensões que a matriz
                // espectro, CV_8UC1 indica que a matriz é de 8 bits por canal e
                // possui apenas um canal (ou seja, é uma imagem em escala de
                // cinza).

  Mat imagem_s_azul; // declarando variável imagem sem azul
  merge(canal_azul,
        imagem_s_azul); // combinar múltiplos canais de cor em uma única imagem

  // cor verde

  vector<Mat> canal_verde;
  split(espectro, canal_verde);

  canal_verde[1] = Mat::zeros(espectro.size(), CV_8UC1); // canal azul

  Mat imagem_s_verde;
  merge(canal_verde, imagem_s_verde);

  // cor vermelha

  vector<Mat> canal_vermelho;
  split(espectro, canal_vermelho);

  canal_vermelho[2] = Mat::zeros(espectro.size(), CV_8UC1); // canal azul

  Mat imagem_s_vermelho;
  merge(canal_vermelho, imagem_s_vermelho);

  // preto e branco

  Mat cinza; // converter a imagem para cinza
  cvtColor(espectro, cinza, COLOR_BGR2GRAY);

  ///////////////////////////////////////////////////////////////////////////////////////////

  imshow("Imagem Original", espectro);        // mostrar imagem og
  imshow("Imagem sem azul", imagem_s_azul);   // mostrar imagem sem a cor azul
  imshow("Imagem sem verde", imagem_s_verde); // mostrar imagem sem a cor verde
  imshow("Imagem sem vermelho",
         imagem_s_vermelho); // mostrar imagem sem a cor vermelha
  imshow("Imagem em preto e branco", cinza); // mostrar imagem em preto e branco

  waitKey(0);

  destroyAllWindows();

  return 0;
}
