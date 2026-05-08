#ifndef ANGLEESTIMATOR_H
#define ANGLEESTIMATOR_H

#include <opencv2/opencv.hpp>

struct AnglesIncrements {
	double dphi;
	double dteta;
	double dpsi;
};

class AngleEstimator {
private:
	double fx;
	double fy;
	double px;
	double py;
public:
	AngleEstimator(const cv::Mat& K);
	AnglesIncrements estimate(const std::vector<cv::KeyPoint>& p1 , const std::vector<cv::KeyPoint>& p2 ,const std::vector<cv::DMatch>& inliers);
};

#endif //ANGLEESTIMATOR_H