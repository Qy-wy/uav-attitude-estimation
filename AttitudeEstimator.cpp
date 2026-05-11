#include "AttitudeEstimator.h"

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

	FramePair frames;

	while (preprocessor.getNextFrame(frames))
	{
		MatchResult matches = matcher.match(frames.prevGrayFrame, frames.currGrayFrame);
		AnglesIncrements incs = estimator.estimate(matches.point1, matches.point2, matches.matches);
		accumulator.updateAngles(incs.dteta, incs.dphi, incs.dpsi);
		double roll = accumulator.getPhi();
		double pitch = accumulator.getTeta();
		double yaw = accumulator.getPsi();

		cv::Mat stabilized = visualisator.stabilize(frames.currColorFrame, incs.dteta, incs.dphi, incs.dpsi);
		visualisator.drawVirtualHorizon(stabilized, roll, pitch);
		cv::imshow("Стабилизация", stabilized);

		if(cv::waitKey(1) == 27)
		{
			break;
		}
	}
}
