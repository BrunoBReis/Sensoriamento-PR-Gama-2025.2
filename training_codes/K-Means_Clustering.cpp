#include <opencv2/opencv.hpp>   // Inclui a biblioteca principal do OpenCV (para processamento de imagens)
#include <iostream>             // Inclui a biblioteca padrão de entrada/saída (cout, cin, etc)
#include <vector>               // Inclui o tipo de dado "vector" (vetor dinâmico do C++)

using namespace cv;             // Permite usar as classes do OpenCV sem precisar escrever "cv::"
using namespace std;            // Permite usar elementos padrão do C++ sem "std::"

int main() {

    // Vetor de strings com os caminhos das imagens a serem processadas
    vector<string> imagens = {...};

    int K = 4; // número de clusters (quantas cores/grupos o K-means vai separar)
    int contador = 1; // contador para nomear as janelas e arquivos de saída

    // Loop para processar todas as imagens
    for (const string& caminho : imagens) {
        cout << "Processando: " << caminho << endl;  // Mostra no terminal qual imagem está sendo processada

        Mat img = imread(caminho);   // Lê a imagem do disco
        if (img.empty()) {           // Verifica se a imagem foi carregada corretamente
            cerr << "Erro ao carregar: " << caminho << endl;  // Exibe mensagem de erro
            continue; // Pula para a próxima imagem do vetor
        }

        // ====== Pré-processamento ======

        Mat data;
        img.convertTo(data, CV_32F); // Converte os valores de pixel para float (necessário para o K-means)
        data = data.reshape(1, img.rows * img.cols); // Transforma a matriz 3D (altura x largura x canais)
                                                     // em uma matriz 2D (número_de_pixels x 3 canais)
                                                     // Ou seja, "achatamos" a imagem em uma lista de pixels RGB

        // ====== Aplicação do K-means ======

        Mat labels, centers;  // labels: indica o cluster de cada pixel | centers: cores médias de cada cluster

        kmeans(
            data,             // Dados de entrada (pixels)
            K,                // Número de clusters
            labels,           // Vetor de saída com o índice do cluster de cada pixel
            TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 10, 1.0), // Critério de parada
            3,                // Número de tentativas (para achar o melhor agrupamento)
            KMEANS_PP_CENTERS,// Método de inicialização dos centros
            centers            // Matriz de saída com as cores médias (centros)
        );

        // ====== Reconstrução da imagem segmentada ======

        centers = centers.reshape(3, centers.rows);  // Ajusta os centros para formato 3 canais (RGB)
        Mat new_image(img.size(), CV_32FC3);         // Cria uma nova imagem com mesmo tamanho e tipo float

        // Preenche a nova imagem substituindo cada pixel pela cor do seu cluster correspondente
        for (int i = 0; i < img.rows * img.cols; i++) {
            int cluster_idx = labels.at<int>(i);  // Obtém o índice do cluster do pixel i
            new_image.at<Vec3f>(i / img.cols, i % img.cols) = centers.at<Vec3f>(cluster_idx);
        }

        new_image.convertTo(new_image, CV_8U);   // Converte de volta para 8 bits (0–255) para exibição

        // ====== Exibição e salvamento ======

        string janela = "K-Means " + to_string(contador);  // Nome da janela de exibição
        imshow(janela, new_image);                         // Mostra a imagem segmentada na tela

        // Gera um nome de arquivo de saída com o número da imagem
        string saida = "C:/Users/pedro/Documents/UnB/GAMA/segmentada_" + to_string(contador) + ".png";
        imwrite(saida, new_image);                         // Salva a imagem processada no disco
        cout << "Imagem salva em: " << saida << endl;      // Confirma no terminal

        contador++; // incrementa o número da imagem
    }

    waitKey(0); // Espera o usuário pressionar uma tecla antes de fechar as janelas
    return 0;   // Encerra o programa
}
