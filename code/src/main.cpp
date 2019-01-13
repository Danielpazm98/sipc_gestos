#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>

#include <stdio.h>
#include <string>
#include <iostream>

#include "MyBGSubtractorColor.hpp"
#include "HandGesture.hpp"

using namespace std;
using namespace cv;



int main(int argc, char** argv)
{

	Mat frame, bgmask, out_frame;



	//Abrimos la webcam

	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened())
	{
		printf("\nNo se puede abrir la c�mara\n");
		return -1;
	}
        int cont = 0;
        while (frame.empty()&& cont < 2000 ) {

                cap >> frame;
                ++cont;
        }
        if (cont >= 2000) {
                printf("No se ha podido leer un frame v�lido\n");
                exit(-1);
        }

	// Creamos las ventanas que vamos a usar en la aplicaci�n

	//namedWindow("Reconocimiento");
	//namedWindow("Fondo");

        // creamos el objeto para la substracci�n de fondo
	MyBGSubtractorColor bg_colorless(cap);
	bg_colorless.LearnModel();

	HandGesture gestures;

	// creamos el objeto para el reconocimiento de gestos

	// iniciamos el proceso de obtenci�n del modelo del fondo


	vector<Point> trace;			//Historial de puntos, para dibujar
	Scalar color(0,255,255);	//Color de la traza
	Point p_p(100,100);				//Punto central previo, para detectar movimiento

	for (;;)
	{
		cap >> frame;
		//flip(frame, frame, 1);
		if (frame.empty())
		{
			printf("Leído frame vacío\n");
			continue;
		}
		int c = cvWaitKey(40);
		if ((char)c == 'q') break;


		//imshow("Reconocimiento", frame);
		bg_colorless.ObtainBGMask(frame, bgmask);
                // CODIGO 2.1
                // limpiar la m�scara del fondo de ruido
                //...
										//Filtro de la media
								medianBlur(bgmask, bgmask, 5);

										//Dilata los puntos blancos
								Mat element = getStructuringElement(MORPH_RECT, Size(2 * 2 + 1, 2 * 2 + 1), Point(2, 2));
								dilate(bgmask, bgmask, element);

										//Erosiona los puntos blancos según el contorno
								element = getStructuringElement(MORPH_RECT, Size(2 * 2 + 1, 2 * 2 + 1), Point(2, 2));
								erode(bgmask, bgmask, element);

										//Dilata los puntos blancos
								element = getStructuringElement(MORPH_RECT, Size(3 * 2 + 1, 3 * 2 + 1), Point(3, 3));
								dilate(bgmask, bgmask, element);


		// deteccion de las caracter�sticas de la mano
								gestures.FeaturesDetection(bgmask, frame, trace, color, p_p);


              // mostramos el resultado de la sobstracci�n de fondo
								imshow("Fondo", bgmask);

                // mostramos el resultado del reconocimento de gestos

								imshow("Gestos", frame);

		// obtenemos la m�scara del fondo con el frame actual


	}

	destroyWindow("Reconocimiento");
	destroyWindow("Fondo");
	cap.release();
	return 0;
}
