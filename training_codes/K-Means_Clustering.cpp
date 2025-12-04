#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace cv;
using namespace std;

int main() {
    string path = "C:/Users/pedro/Documents/UnB/GAMA/img4.png";
    Mat img = imread(path);

    if (img.empty()) {
        cout << "Erro ao abrir imagem." << endl;
        return -1;
    }

    Mat data = img.reshape(1, img.rows * img.cols);
    data.convertTo(data, CV_32F);

    // 
    int K = 5;
    Mat labels, centers;
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0);

    kmeans(data, K, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

    // --- CONFIGURAÇÕES DE FILTRO ---
    double whiteThreshold = 170.0; // Nuvens/Céu
    double maxDistance = 60.0;     // Precisão da cor

    // Vetores para armazenar propriedades dos clusters
    vector<bool> isClusterValid(K, true);
    vector<float> clusterBrightness(K, 0.0f);
    vector<int> clusterClass(K, 0); // 0=Descartado, 1=Nativa(Escuro), 2=Plantacao(Claro)

    float somaBrilhoValidos = 0;
    int qtdValidos = 0;

    cout << "--- Analise Inicial (K=" << K << ") ---" << endl;
    for (int i = 0; i < K; i++) {
        float b = centers.at<float>(i, 0);
        float g = centers.at<float>(i, 1);
        float r = centers.at<float>(i, 2);

        // 1. Calcula Brilho/Luminosidade Simples
        float brightness = (b + g + r) / 3.0f;
        clusterBrightness[i] = brightness;

        // 2. Filtro de Nuvens (Branco)
        bool isBright = (b > whiteThreshold && g > whiteThreshold && r > whiteThreshold);

        if (isBright) {
            isClusterValid[i] = false;
            clusterClass[i] = 0; // 0 = Descartado
            cout << "Cluster " << i << " [REJEITADO - NUVEM]: Brilho " << (int)brightness << endl;
        }
        else {
            // Acumula para calcular a média depois
            somaBrilhoValidos += brightness;
            qtdValidos++;
            cout << "Cluster " << i << " [VALIDO]: Brilho " << (int)brightness << endl;
        }
    }

    // --- LÓGICA DE CLASSIFICAÇÃO AUTOMÁTICA ---
    // Calculamos a "linha de corte" entre o escuro e o claro
    float mediaBrilho = 0;
    if (qtdValidos > 0) mediaBrilho = somaBrilhoValidos / qtdValidos;

    cout << "\n--- Classificacao Automatica (Media de Corte: " << (int)mediaBrilho << ") ---" << endl;

    for (int i = 0; i < K; i++) {
        if (!isClusterValid[i]) continue; // Pula nuvens

        if (clusterBrightness[i] < mediaBrilho) {
            clusterClass[i] = 1; // 1 = VEGETAÇÃO NATIVA (Escuro)
            cout << "Cluster " << i << " -> Classificado como: VEGETACAO NATIVA (Escuro)" << endl;
        }
        else {
            clusterClass[i] = 2; // 2 = PLANTAÇÃO (Claro)
            cout << "Cluster " << i << " -> Classificado como: PLANTACAO (Claro)" << endl;
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
        }
        else if (clusterClass[clusterIdx] == 2) {
            pixelsPlantacao++;
        }
    }

    // --- EXIBIÇÃO DOS RESULTADOS ---
    float totalPixels = (float)img.rows * img.cols;
    float percNativa = (pixelsNativa / totalPixels) * 100.0f;
    float percPlantacao = (pixelsPlantacao / totalPixels) * 100.0f;
    float percRejeitados = (pixelsRejeitados / totalPixels) * 100.0f;

    cout << fixed << setprecision(2);
    cout << "\n========================================" << endl;
    cout << " RELATORIO FINAL DE COBERTURA DE SOLO " << endl;
    cout << "========================================" << endl;
    cout << "Vegetacao Nativa (Tons Escuros): " << percNativa << "%" << endl;
    cout << "Plantacao (Tons Claros):         " << percPlantacao << "%" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Area Irrelevante (Nuvens/Fundo): " << percRejeitados << "%" << endl;
    cout << "Soma Total: " << (percNativa + percPlantacao + percRejeitados) << "%" << endl;

    // --- VISUALIZAÇÃO INTELIGENTE ---
    // Agora pintamos a imagem com cores de MAPA para facilitar sua validação
    Mat resultImage(img.size(), img.type());

    for (int i = 0; i < img.rows * img.cols; i++) {
        int r = i / img.cols;
        int c = i % img.cols;
        int clusterIdx = labels.at<int>(i);

        float b = data.at<float>(i, 0);
        float g = data.at<float>(i, 1);
        float r_val = data.at<float>(i, 2);
        float cB = centers.at<float>(clusterIdx, 0);
        float cG = centers.at<float>(clusterIdx, 1);
        float cR = centers.at<float>(clusterIdx, 2);
        double dist = sqrt(pow(b - cB, 2) + pow(g - cG, 2) + pow(r_val - cR, 2));

        // Se for ruído ou nuvem, pinta de Rosa
        if (clusterClass[clusterIdx] == 0 || dist > maxDistance) {
            resultImage.at<Vec3b>(r, c) = Vec3b(255, 0, 255); // Rosa Choque
        }
        else if (clusterClass[clusterIdx] == 1) {
            // Nativa -> Verde Escuro Puro
            resultImage.at<Vec3b>(r, c) = Vec3b(0, 100, 0);
        }
        else if (clusterClass[clusterIdx] == 2) {
            // Plantação -> Verde Claro Limão
            resultImage.at<Vec3b>(r, c) = Vec3b(0, 255, 100);
        }
    }

    // Janelas
    // Redimensiona para caber na tela se a imagem for 4k
    namedWindow("Classificacao (Verde Escuro=Nativa, Claro=Plantacao)", WINDOW_NORMAL);
    resizeWindow("Classificacao (Verde Escuro=Nativa, Claro=Plantacao)", 800, 600);

    imshow("Classificacao (Verde Escuro=Nativa, Claro=Plantacao)", resultImage);
    imshow("Original", img);
    waitKey(0);

    return 0;
}
