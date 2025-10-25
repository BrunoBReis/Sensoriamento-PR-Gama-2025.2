// Para compilar: g++ canaisRGB.cpp -o main.o `pkg-config --cflags --libs
// opencv4` Para executar: ./main

// #include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  Mat espectro = imread("../images/espectro.png");

  // verifica se a imagem foi carregada corretamente
  if (espectro.empty()) {
    // printf("Erro ao carregar a imagem.\n");
    cout << "Erro ao carregar a imagem" << '\n';
    return -1; // erro
  }

  // cor azul //

  // declarando um vetor chamado canais que irá
  // armazenar múltiplas matrizes (Mat).
  // divide os canais de cores em B, G e R.

  vector<Mat> canal_azul;

  split(espectro, canal_azul);

  // Iguala o canal de cor azul ao Mat::zeros que cria uma matriz
  // ou imagem de zeros com as mesmas dimensões que a matriz
  // espectro, CV_8UC1 indica que a matriz é de 8 bits por canal e
  // possui apenas um canal (ou seja, é uma imagem em escala de
  // cinza).

  canal_azul[0] = Mat::zeros(espectro.size(), CV_8UC1);

  Mat imagem_s_azul; // declarando variável imagem sem azul

  // combinar múltiplos canais de cor em uma única imagem
  merge(canal_azul, imagem_s_azul);

  // cor verde//

  // declarando um vetor chamado canais que irá
  // armazenar múltiplas matrizes (Mat).
  // divide os canais de cores em B, G e R.

  vector<Mat> canal_verde;
  split(espectro, canal_verde);

  canal_verde[1] = Mat::zeros(espectro.size(), CV_8UC1); // canal azul

  Mat imagem_s_verde;
  merge(canal_verde, imagem_s_verde);

  // cor vermelha //

  // declarando um vetor chamado canais que irá
  // armazenar múltiplas matrizes (Mat).
  // divide os canais de cores em B, G e R.

  vector<Mat> canal_vermelho;
  split(espectro, canal_vermelho);

  canal_vermelho[2] = Mat::zeros(espectro.size(), CV_8UC1); // canal azul

  Mat imagem_s_vermelho;
  merge(canal_vermelho, imagem_s_vermelho);

  // preto e branco

  // declarando um vetor chamado canais que irá
  // armazenar múltiplas matrizes (Mat).
  // divide os canais de cores em B, G e R.

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
