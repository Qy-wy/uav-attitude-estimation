#include "AttitudeEstimator.h"
#include <fstream>

using namespace std;

AttitudeEstimator::AttitudeEstimator(const cv::Mat& k, std::string path) : preprocessor(path), matcher(), estimator(k), accumulator(), visualisator(k)
{
	k.copyTo(K);
	videoPath = path;
}

void AttitudeEstimator::run()
{
	if (!preprocessor.loadFirstFrame())
	{
		cerr << "Error loading the first frame";
		return;
	}

	std::ofstream logFile("angles.csv");
	logFile << "frame,phi,theta,psi\n";
	int frameIndex = 0;

	FramePair frames;

	while (preprocessor.getNextFrame(frames))
	{
		MatchResult matches = matcher.match(frames.prevGrayFrame, frames.currGrayFrame);
		AnglesIncrements incs = estimator.estimate(matches.point1, matches.point2, matches.matches);
		accumulator.updateAngles(incs.dteta, incs.dphi, incs.dpsi);
		double roll = accumulator.getPhi();
		double pitch = accumulator.getTeta();
		double yaw = accumulator.getPsi();

		logFile << frameIndex << ","
			<< roll * 180.0 / CV_PI << ","
			<< pitch * 180.0 / CV_PI << ","
			<< yaw * 180.0 / CV_PI << "\n";

		frameIndex++;

		cv::Mat stabilized = visualisator.stabilize(frames.currColorFrame, incs.dteta, incs.dphi, incs.dpsi);

		string info = "Roll: " + to_string(roll * 180.0 / CV_PI) + " Pitch: " + to_string(pitch * 180.0 / CV_PI) + " Yaw: " + to_string(yaw * 180.0 / CV_PI);
		cv::putText(stabilized, info, cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 255), 2);
		cv::imshow("Стабилизация", stabilized);

		if(cv::waitKey(1) == 27)
		{
			break;
		}
	}

	logFile.close();
} 
