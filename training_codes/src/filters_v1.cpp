// Para compilar: g++ filtros.cpp -o filtros `pkg-config --cflags --libs
// opencv4` Para executar: ./filtros

#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {

  // Carrega a imagem
  Mat original = imread("../images/dexter.png");
  if (original.empty()) {
    printf("Erro ao carregar a imagem 'dexter.png'.\n");
    return -1;
  }

  // Filtros de borda funcionam melhor em tons de cinza.
  Mat cinza;
  cvtColor(original, cinza, COLOR_BGR2GRAY);

  // Filtro Sharpen (Nitidez)
  // Usa um 'kernel' personalizado para realçar as bordas.
  Mat sharpened;
  Mat kernel = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
  filter2D(original, sharpened, -1, kernel);

  // Filtro Laplacian (Borda)
  Mat laplacian_img, laplacian_edges;
  Laplacian(cinza, laplacian_img, CV_16S, 3);
  convertScaleAbs(laplacian_img,
                  laplacian_edges); // Converte de volta para 8-bits

  // Filtro Sobel (Borda)
  Mat grad_x, grad_y, abs_grad_x, abs_grad_y, sobel_edges;
  Sobel(cinza, grad_x, CV_16S, 1, 0, 3);
  Sobel(cinza, grad_y, CV_16S, 0, 1, 3);
  convertScaleAbs(grad_x, abs_grad_x);
  convertScaleAbs(grad_y, abs_grad_y);
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_edges);

  // Filtro Canny (Borda)
  Mat canny_edges;
  Canny(cinza, canny_edges, 100, 200);

  // Filtro GaussianBlur (Desfoque/Suavização)
  // Aplica um desfoque gaussiano.
  Mat gaussiano_blur;
  GaussianBlur(original, gaussiano_blur, Size(7, 7), 0); // Kernel 7x7

  // Converte as imagens de borda (1 canal) para BGR (3 canais) para concatenar.
  Mat laplacian_bgr, sobel_bgr, canny_bgr;
  cvtColor(laplacian_edges, laplacian_bgr, COLOR_GRAY2BGR);
  cvtColor(sobel_edges, sobel_bgr, COLOR_GRAY2BGR);
  cvtColor(canny_edges, canny_bgr, COLOR_GRAY2BGR);

  // Adiciona texto a cada imagem para identificação
  putText(original, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);
  putText(sharpened, "Sharpen", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(255, 255, 255), 2);
  putText(gaussiano_blur, "Blur (Gauss)", Point(10, 30), FONT_HERSHEY_SIMPLEX,
          1, Scalar(255, 255, 255), 2);
  // Texto verde nas imagens de borda para melhor visualização
  putText(laplacian_bgr, "Laplacian", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(0, 255, 0), 2);
  putText(sobel_bgr, "Sobel", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(0, 255, 0), 2);
  putText(canny_bgr, "Canny", Point(10, 30), FONT_HERSHEY_SIMPLEX, 1,
          Scalar(0, 255, 0), 2);

  // Concatena as imagens
  Mat linha1_temp, linha1;
  Mat linha2_temp, linha2;
  Mat resultado;

  // Constrói a Linha 1: (Original) + (Sharpen) -> (linha1_temp) + (Laplacian)
  // -> (linha1)
  hconcat(original, sharpened, linha1_temp);
  hconcat(linha1_temp, laplacian_bgr, linha1);

  // Constrói a Linha 2: (Sobel) + (Canny) -> (linha2_temp) + (Gaussiano Blur)
  // -> (linha2)
  hconcat(sobel_bgr, canny_bgr, linha2_temp);
  hconcat(linha2_temp, gaussiano_blur,
          linha2); // Substituído o preenchimento pelo blur

  // Concatena as duas linhas verticalmente
  vconcat(linha1, linha2, resultado);

  // Exibe o resultado
  imshow("Filtros de Borda, Nitidez e Desfoque", resultado);
  waitKey(0);

  destroyAllWindows();

  return 0;
}
