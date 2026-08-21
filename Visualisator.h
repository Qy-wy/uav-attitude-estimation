#ifndef VISUALISATOR_H
#define VISUALISATOR_H

#include <opencv2/opencv.hpp>

class Visualisator {
private:
	double fx;
	double fy;
	double px;
	double py;
	cv::Mat k;
public:
	Visualisator(const cv::Mat& K);
	void drawVirtualHorizon(cv::Mat& frame, double roll, double pitch, double yaw);
	cv::Mat stabilize(cv::Mat& frame, double dteta, double dphi, double dpsi);
};

#endif //VISUALISATOR_H
