#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <opencv2/opencv.hpp>

struct FramePair {
	cv::Mat prevGrayFrame;
	cv::Mat currGrayFrame;
	cv::Mat currColorFrame;
};

class Preprocessor {
private:
	cv::VideoCapture cap;
	cv::Mat prevGray;
	std::string videoPath;
public:
	Preprocessor(const std::string& videoPath);
	bool loadFirstFrame();
	bool getNextFrame(FramePair& frames);
};

#endif //PREPROCESSOR_H
