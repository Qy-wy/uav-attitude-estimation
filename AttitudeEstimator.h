#ifndef ATTITUDEESTIMATOR_H
#define ATTITUDEESTIMATOR_H

#include <opencv2/opencv.hpp>
#include "AngleAccumulator.h"
#include "AngleEstimator.h"
#include "FeatureMatcher.h"
#include "Preprocessor.h"
#include "Visualisator.h"

class AttitudeEstimator {
private:
	Preprocessor preprocessor;
	FeatureMatcher matcher;
	AngleEstimator estimator;
	AngleAccumulator accumulator;
	Visualisator visualisator;
	std::string videoPath;
	cv::Mat K;
public:
	AttitudeEstimator(const cv::Mat& k, std::string path);
	void run();
};

#endif //ATTITUDEESTIMATOR_H