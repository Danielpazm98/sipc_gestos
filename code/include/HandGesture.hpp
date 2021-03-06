#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

class HandGesture {

public:
	HandGesture();
	void FeaturesDetection(cv::Mat mask, cv::Mat output_img, /*std::vector<cv::Point> &trace*/ std::vector<std::pair<cv::Point, cv::Scalar> > &trace, cv::Scalar &color, cv::Point &p_p, int &i, int &j, int &k, bool &opt);


private:
	double getAngle(cv::Point s, cv::Point e, cv::Point f);

};
