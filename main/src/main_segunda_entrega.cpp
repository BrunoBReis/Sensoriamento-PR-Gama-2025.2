#include <cmath>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main() {

  // Leitura da imagem
  string filename = "../images/big_clouds.png";
  Mat image = imread(filename);

  // Tratamento de erros
  if (image.empty()) {
    cerr << "Erro ao abrir imagem." << endl;
    return -1;
  }
  if (image.channels() != 3) {
    cerr << "Error: image not BGR\n";
    return -1;
  }

  // Transformando imagem para o padrao CV_32F para ser guardado dentro de um
  // float de 32 bits
  Mat data = image.reshape(1, image.rows * image.cols);
  data.convertTo(data, CV_32F);

  // Definição da quantidade de clusteres
  int K = 5;
  Mat labels, centers;
  TermCriteria criteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0);

  // Chamda do Kmeans para a identificação dos clusteres
  kmeans(data, K, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

  // Definição do padrão de cor da nuvem
  double whiteThreshold = 170.0; // Nuvens/Céu
  double maxDistance = 60.0;     // Precisão da cor

  // Vetores para armazenar propriedades dos clusters
  vector<bool> isClusterValid(K, true);
  vector<float> clusterBrightness(K, 0.0f);
  vector<int> clusterClass(
      K, 0); // 0=Descartado, 1=Nativa(Escuro), 2=Plantacao(Claro)

  float somaBrilhoValidos = 0;
  int qtdValidos = 0;

  for (int i = 0; i < K; i++) {
    float b = centers.at<float>(i, 0);
    float g = centers.at<float>(i, 1);
    float r = centers.at<float>(i, 2);

    // 1. Calcula Brilho/Luminosidade Simples
    float brightness = (b + g + r) / 3.0f;
    clusterBrightness[i] = brightness;

    // 2. Filtro de Nuvens (Branco)
    bool isBright =
        (b > whiteThreshold && g > whiteThreshold && r > whiteThreshold);

    if (isBright) {
      isClusterValid[i] = false;
      clusterClass[i] = 0; // 0 = Descartado
    } else {
      // Acumula para calcular a média depois
      somaBrilhoValidos += brightness;
      qtdValidos++;
    }
  }

  // --- LÓGICA DE CLASSIFICAÇÃO AUTOMÁTICA ---
  // Calculamos a "linha de corte" entre o escuro e o claro
  float mediaBrilho = 0;
  if (qtdValidos > 0)
    mediaBrilho = somaBrilhoValidos / qtdValidos;

  for (int i = 0; i < K; i++) {
    if (!isClusterValid[i])
      continue; // Pula nuvens

    if (clusterBrightness[i] < mediaBrilho) {
      clusterClass[i] = 1; // 1 = VEGETAÇÃO NATIVA (Escuro)
    } else {
      clusterClass[i] = 2; // 2 = PLANTAÇÃO (Claro)
    }
  }

  // --- CONTAGEM DE PIXELS ---
  // Vamos somar tudo direto nas categorias finais
  long pixelsNativa = 0;
  long pixelsPlantacao = 0;
  long pixelsRejeitados = 0;

  for (int i = 0; i < data.rows; i++) {
    int clusterIdx = labels.at<int>(i);

    // Se o cluster base já é nuvem, descarta
    if (clusterClass[clusterIdx] == 0) {
      pixelsRejeitados++;
      continue;
    }

    // Verifica Distância (Filtro de Ruído)
    float b = data.at<float>(i, 0);
    float g = data.at<float>(i, 1);
    float r = data.at<float>(i, 2);
    float cB = centers.at<float>(clusterIdx, 0);
    float cG = centers.at<float>(clusterIdx, 1);
    float cR = centers.at<float>(clusterIdx, 2);
    double dist = sqrt(pow(b - cB, 2) + pow(g - cG, 2) + pow(r - cR, 2));

    if (dist > maxDistance) {
      pixelsRejeitados++; // Cor válida, mas muito distante do centro (ruído)
      continue;
    }

    // Soma nas categorias finais
    if (clusterClass[clusterIdx] == 1) {
      pixelsNativa++;
    } else if (clusterClass[clusterIdx] == 2) {
      pixelsPlantacao++;
    }
  }

  // --- EXIBIÇÃO DOS RESULTADOS ---
  float totalPixels = (float)image.rows * image.cols;
  float percNativa = (pixelsNativa / totalPixels) * 100.0f;
  float percPlantacao = (pixelsPlantacao / totalPixels) * 100.0f;
  float percRejeitados = (pixelsRejeitados / totalPixels) * 100.0f;

  cout << fixed << setprecision(2);
  cout << "Vegetacao Nativa (Tons Escuros): " << percNativa << "%" << endl;
  cout << "Plantacao (Tons Claros):         " << percPlantacao << "%" << endl;
  cout << "Area Irrelevante (Nuvens/Fundo): " << percRejeitados << "%" << endl;
  cout << "Soma Total: " << (percNativa + percPlantacao + percRejeitados) << "%"
       << endl;

  return 0;
}
