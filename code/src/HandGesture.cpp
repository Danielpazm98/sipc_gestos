#include "HandGesture.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

HandGesture::HandGesture() {
}


double HandGesture::getAngle(Point s, Point e, Point f) {

	double v1[2],v2[2];
	v1[0] = s.x - f.x;
	v1[1] = s.y - f.y;

	v2[0] = e.x - f.x;
	v2[1] = e.y - f.y;

	double ang1 = atan2(v1[1],v1[0]);
	double ang2 = atan2(v2[1],v2[0]);

	double angle = ang1 - ang2;
	if (angle > CV_PI) angle -= 2 * CV_PI;
	if (angle < -CV_PI) angle += 2 * CV_PI;
	return (angle * 180.0/CV_PI);
}



void HandGesture::FeaturesDetection(Mat mask, Mat output_img, vector<Point> &trace, Scalar &color, Point &p_p) {

	vector<vector<Point> > contours;
	Mat temp_mask;
	mask.copyTo(temp_mask);
	int index = -1;

			circle(temp_mask, Point(10,10), 5, Scalar(255), 3);
        // CODIGO 3.1
        // detección del contorno de la mano y selección del contorno más largo
        //...
			findContours(temp_mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        // pintar el contorno
        //...
				//vector<Point> biggest = contours[0];

			index = 0;
			vector<Point> max_contour = contours[0];

					//Busca el mayor contorno
			for(int i = 1; i < contours.size(); i++){
				if(contourArea(contours[i]) > contourArea(max_contour)){
					max_contour = contours[i];
					index = i;
				}
			}
						//Escribe en azul el mayor contorno
			drawContours(output_img, contours, index, cv::Scalar(255, 0, 0), 2, 8, vector<Vec4i>(), 0, Point());

				//obtener el convex hull
	vector<int> hull;
	convexHull(contours[index],hull);



		// pintar el convex hull
	Point pt0 = contours[index][hull[hull.size()-1]];
	for (int i = 0; i < hull.size(); i++)
	{
		Point pt = contours[index][hull[i]];
		line(output_img, pt0, pt, Scalar(0, 0, 255), 2, CV_AA);
		pt0 = pt;
	}

      //obtener los defectos de convexidad
	vector<Vec4i> defects;
	convexityDefects(contours[index], hull, defects);
			//Crea un rectángulo con el contorno de la mano
	Rect rect = boundingRect(contours[index]);
				//Pinta el rectángulo
	//rectangle(output_img, rect, Scalar(255,255,255), 1, 8, 0);

			//Punto a insertar en el historial, para dibujar
	Point to_trace;
			//Punto actual, para comparar con el anterior. Es el centro del rectángulo
	Point a_p(((rect.width / 2) + rect.x), (rect.height / 2) + rect.y);

	int cont = 0;

	for (int i = 0; i < defects.size(); i++) {		//Para cada defecto de convexidad

		Point s = contours[index][defects[i][0]];		//Punto inicial
		Point e = contours[index][defects[i][1]];		//Punto final
		Point f = contours[index][defects[i][2]];		//Punto más lejano
		float depth = (float)defects[i][3] / 256.0;	//Profundidad
		double angle = getAngle(s, e, f);						//Ángulo

              // CODIGO 3.2
              // filtrar y mostrar los defectos de convexidad
			        //...

			if((depth > (rect.height*0.2)) && (angle < 110)){			//Si la profundidad es mayor al 20% y el ángulo es menor a 110º
				cont++;			//Sumas "dedo"
				circle(output_img, f, 5, Scalar(0,255,0),3);				//Colocas un círculo en el defecto de convexidad del dedo
				if(cont == 1)
					to_trace = f;			//Si ha habido sólo 1 defecto admitido.
			}
  }

				if((rect.height / rect.width < 1.2) && (cont == 0))		//Comprueba si tienes 0 dedos levantados.
					cont = 0;
				else
					cont++;

					//Escribe la cantidad de dedos levantados
				const string aux = to_string(cont);
				putText(output_img, aux, Point(30,30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0), 1, 8, false);
								//función circle. Buscar en la documentación de opencv


				if(cont == 3){		//Si tienes 3 dedos levantados, te cambia el color (rojo, amarillo, cian)
					Scalar aux1(0,255,255);	//Amarillo
					Scalar aux2(255,255,0); //Cian
					Scalar aux3(0,0,255);		//Rojo

					if(color == aux3){
						color = Scalar(255,255,0);
					}
					else if(color == aux2){
						color = Scalar(0,255,255);
					}
					else if(color == aux1){
						color = Scalar(0,0,255);
					}

				}

				if(cont == 2)				//Si tienes 2 dedos levantados, pones el punto en el historial de dibujo
					trace.push_back(to_trace);
				else if(cont == 5)
					trace.clear();


						//Escribe el número de puntos que tiene la traza
				const string trace_sz = to_string(trace.size());
				putText(output_img, trace_sz, Point(30,100), FONT_HERSHEY_SIMPLEX, 1, color, 1, 8, false);

						//Dibuja la traza
				for(int i = 0; i < trace.size(); i++)
					circle(output_img, trace[i], 5, color,3);

						//Comprueba cuánto se ha movido la mano desde el frame anterior, y si es más de 10px, te avisa de que te estés quieto
				if((abs(p_p.y - a_p.y) > 10) || (abs(p_p.y - a_p.y) > 10))
					putText(output_img, "Estate quieto", Point(100,30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 1, 8, false);

				p_p = a_p;		//Coloca el piel actual como pixel anterior
}
