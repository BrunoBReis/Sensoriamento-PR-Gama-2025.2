#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

void savePercentagesToJson(const string &jsonFilename, const string &imageName,
                           double nativePercentage,
                           double agriculturePercentage,
                           double rejectedPercentage) {
  ofstream file(jsonFilename);
  if (!file.is_open()) {
    cerr << "Erro ao abrir arquivo JSON para escrita: " << jsonFilename << "\n";
    return;
  }

  file << "{\n"
       << "  \"native_percentage\": " << nativePercentage << ",\n"
       << "  \"agriculture_percentage\": " << agriculturePercentage << ",\n"
       << "  \"rejected_percentage\": " << rejectedPercentage << "\n"
       << "}\n";

  file.close();
}

int main() {

  // Leitura manual da imagem
  string filename = "image.png";
  Mat image = imread(filename);

  // Tratamento de erros
  if (image.empty()) {
    cerr << "Erro ao abrir imagem.\n";
    return -1;
  }
  if (image.channels() != 3) {
    cerr << "Error: image not BGR\n";
    return -1;
  }

  // Preparando os dados para o K-means
  Mat data = image.reshape(1, image.rows * image.cols);
  data.convertTo(data, CV_32F);

  // Configuração do K-means
  int K = 5; // Cinco clusters
  Mat labels, centers;
  TermCriteria criteria(
      TermCriteria::EPS + TermCriteria::COUNT, 10,
      1.0); // Número máximo de inerações = 10 OU Mudança menor que = 1.0

  // Chamada do K-means
  kmeans(data, K, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

  // Definição do padrão de cor da nuvem
  double maxColor = 170.0;   // Nuvens e texto
  double maxDistance = 60.0; // Precisão da cor

  // Vetores para armazenar propriedades dos clusters
  vector<bool> isClusterValid(K, true);
  vector<float> clusterBrightness(K, 0.0f);
  vector<int> clusterClass(K, 0); // 0 = Descartado, 1 = Nativa, 2 = Plantacao

  float brightnessSum = 0;
  int valids = 0;

  // Analisando cada cluster
  for (int i = 0; i < K; ++i) {
    float b = centers.at<float>(i, 0);
    float g = centers.at<float>(i, 1);
    float r = centers.at<float>(i, 2);

    // Calcula brilho
    float brightness = (b + g + r) / 3.0f;
    clusterBrightness[i] = brightness;

    // Filtro de nuvens e texto
    if (b > maxColor && g > maxColor && r > maxColor) {
      isClusterValid[i] = false;
      clusterClass[i] = 0; // 0 = Descartado
    } else {
      // Acumula para calcular a média depois
      brightnessSum += brightness;
      valids++;
    }
  }

  // Cálculo da média de brilho
  float averageBrightness = 0;
  if (valids > 0)
    averageBrightness = brightnessSum / valids;

  for (int i = 0; i < K; ++i) {
    if (!isClusterValid[i])
      continue;

    if (clusterBrightness[i] < averageBrightness)
      clusterClass[i] = 1; // 1 = Vegetação nativa
    else
      clusterClass[i] = 2; // 2 = Plantação
  }

  // Contagem de pixels
  long nativePixels = 0;
  long agriculturePixels = 0;
  long rejectedPixels = 0;

  for (int i = 0; i < data.rows; ++i) {
    int clusterIdx = labels.at<int>(i);

    if (clusterClass[clusterIdx] == 0) {
      rejectedPixels++;
      continue;
    }

    // Distância Euclidiana
    // Isso mede o quanto cada pixel é parecido com o centro do cluster
    float b = data.at<float>(i, 0);
    float g = data.at<float>(i, 1);
    float r = data.at<float>(i, 2);
    float cB = centers.at<float>(clusterIdx, 0);
    float cG = centers.at<float>(clusterIdx, 1);
    float cR = centers.at<float>(clusterIdx, 2);
    double dist = sqrt(pow(b - cB, 2) + pow(g - cG, 2) + pow(r - cR, 2));

    if (dist > maxDistance) {
      rejectedPixels++; // Cor válida, mas muito distante do centro (ruído)
      continue;
    }

    // Soma nas categorias finais
    if (clusterClass[clusterIdx] == 1)
      nativePixels++;
    else if (clusterClass[clusterIdx] == 2)
      agriculturePixels++;
  }

  // Mostrando resultados finais
  float totalPixels = (float)image.rows * image.cols;
  float nativePercentage = (nativePixels / totalPixels) * 100.0f;
  float agriculturePercentage = (agriculturePixels / totalPixels) * 100.0f;
  float rejectedPercentage = (rejectedPixels / totalPixels) * 100.0f;

  cout << fixed << setprecision(2);
  cout << left << setw(30) << "Image:" << filename << '\n'
       << left << setw(30) << "Native percentage:" << nativePercentage << " %\n"
       << left << setw(30) << "Agriculture percentage:" << agriculturePercentage
       << " %\n"
       << left << setw(30) << "Rejected percentage:" << rejectedPercentage
       << " %\n"
       << "--------------------------------------------\n"
       << left << setw(30) << "Total:"
       << (nativePercentage + agriculturePercentage + rejectedPercentage)
       << " %\n";

  // Gera arquivo JSON com os resultados
  string jsonFilename = "results.json";
  savePercentagesToJson(jsonFilename, filename, nativePercentage,
                        agriculturePercentage, rejectedPercentage);

  return 0;
}
