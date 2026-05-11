#include "Visualisator.h"

using namespace cv;

Visualisator::Visualisator(const Mat& K)
{
	fx = K.at<double>(0, 0);
	fy = K.at<double>(1, 1);
	px = K.at<double>(0, 2);
	py = K.at<double>(1, 2);
	K.copyTo(k);
}

void Visualisator::drawVirtualHorizon(Mat& frame, double roll, double pitch)
{
	if (frame.empty())
	{
		return;
	}

	double y0 = py + fy * tan(pitch);

	double x1 = -frame.cols;
	double x2 = 2 * frame.cols;

	double y1 = y0 + tan(roll) * (x1 - px);
	double y2 = y0 + tan(roll) * (x2 - px);

	Point p1(static_cast<int>(x1), static_cast<int>(y1));
	Point p2(static_cast<int>(x2), static_cast<int>(y2));

	line(frame, p1, p2, Scalar(0, 0, 255), 2);

	const int cs = 15;

	line(frame, Point(px - cs, py), Point(px + cs, py), Scalar(255, 255, 255), 1);
	line(frame, Point(px, py - cs), Point(px, py + cs), Scalar(255, 255, 255), 1);
}

Mat Visualisator::stabilize(Mat& frame, double dteta, double dphi, double dpsi)
{
	if (frame.empty())
	{
		return frame;
	}

	Mat stabilized;
	Mat omega_x = Mat::zeros(3, 3, CV_64F);
	Mat I = Mat::eye(3, 3, CV_64F);


	omega_x.at<double>(0, 0) = 0.0;
	omega_x.at<double>(0, 1) = -dpsi;
	omega_x.at<double>(0, 2) = dteta;

	omega_x.at<double>(1, 0) = dpsi;
	omega_x.at<double>(1, 1) = 0.0;
	omega_x.at<double>(1, 2) = -dphi;

	omega_x.at<double>(2, 0) = -dteta;
	omega_x.at<double>(2, 1) = dphi;
	omega_x.at<double>(2, 2) = 0.0;

	Mat deltaR = I - omega_x;
	Mat H = k * deltaR * k.inv();

	warpPerspective(frame, stabilized, H, frame.size(), INTER_LINEAR, BORDER_REPLICATE);

	return stabilized;
}
