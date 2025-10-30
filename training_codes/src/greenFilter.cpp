
#include <opencv2/opencv.hpp>
#include <cstdio>

int main() {

    using namespace cv;
	using namespace std;

    Mat luz = imread("C:/Users/pedro/Documents/UnB/GAMA/img2.png");
    if (luz.empty()) {
        printf("Erro ao carregar a imagem!\n");
        return -1;
    }

    // Converter a imagem para o espaço de cores HSV
    Mat hsv;
    cvtColor(luz, hsv, COLOR_BGR2HSV);

    // Definir os limites da cor azul em HSV
    Scalar menorVerde(25, 30, 30);
    Scalar maiorVerde(95, 255, 255);

    // Criar máscaras para as cores azul, branco e roxo
    Mat mascara1, mascara2, mascara3;
    inRange(hsv, menorVerde, maiorVerde, mascara1);
    

    // Combinar as três máscaras
    Mat mascara = mascara1;// | mascara2 | mascara3;

    // Aplicar a máscara à imagem original
    Mat resultado;
    bitwise_and(luz, luz, resultado, mascara);

    resize(luz, luz, Size(), 0.5, 0.5);
    resize(mascara, mascara, Size(), 0.5, 0.5);

    // Mostrar as imagens
    imshow("Imagem Original", luz);
    imshow("Máscara", mascara);


    waitKey(0);
    return 0;
}
