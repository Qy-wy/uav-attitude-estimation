#ifndef FEATUREMATCHER_H
#define FEATUREMATCHER_H

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

struct MatchResult {
	std::vector<cv::KeyPoint> point1;
	std::vector<cv::KeyPoint> point2;
	std::vector<cv::DMatch> matches;
};

class FeatureMatcher {
private:
	cv::Ptr<cv::ORB> orb;
	cv::Ptr<cv::xfeatures2d::FREAK> freak;
	cv::Ptr<cv::BFMatcher> matcher;
	cv::Mat descriptor1;
	cv::Mat descriptor2;

public:
	FeatureMatcher(int nFeatures = 500);
	MatchResult match(const cv::Mat& prevGrayFrame, const cv::Mat& currGrayFrame);
};

#endif //FEATUREMATCHER_H