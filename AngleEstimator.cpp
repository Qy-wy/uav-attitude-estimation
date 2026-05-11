#include "AngleEstimator.h"

using namespace cv;

AngleEstimator::AngleEstimator(const Mat& K)
{
	fx = K.at<double>(0,0);
	fy = K.at<double>(1, 1);
	px = K.at<double>(0, 2);
	py = K.at<double>(1, 2);
}

AnglesIncrements AngleEstimator::estimate(const std::vector<KeyPoint>& p1, const std::vector<KeyPoint>& p2, const std::vector<DMatch>& inliers)
{
	const int N = static_cast<int>(inliers.size());

	std::vector<double> u_tilda(N);
	std::vector<double> v_tilda(N);
	std::vector<double> du(N);
	std::vector<double> dv(N);
	std::vector<double> u2(N);
	std::vector<double> v2(N);

	Mat A = Mat::zeros(2 * N, 3, CV_64F);
	Mat b = Mat::zeros(2 * N, 1, CV_64F);
	Mat dAngles;
	AnglesIncrements result = { 0.0, 0.0, 0.0 };

	if (inliers.size() < 3)
	{
		return result;
	}

	for (int i = 0; i < N; i++)
	{
		const DMatch& m = inliers[i]; 

		double u1 = p1[m.queryIdx].pt.x;
		double v1 = p1[m.queryIdx].pt.y;

		u2[i] = p2[m.trainIdx].pt.x;
		v2[i] = p2[m.trainIdx].pt.y;

		u_tilda[i] = (u1 - px) / fx;
		v_tilda[i] = (v1 - py) / fy;

		du[i] = u2[i] - u1;
		dv[i] = v2[i] - v1;
	}

	for (int i = 0; i < N; i++)
	{
		A.at<double>(2 * i, 0) = fx + u_tilda[i] * (u2[i] - px);
		A.at<double>(2 * i, 1) = v_tilda[i] * (u2[i] + px);
		A.at<double>(2 * i, 2) = -fx * v_tilda[i];

		A.at<double>(2 * i + 1, 0) = u_tilda[i] * (v2[i] - py);
		A.at<double>(2 * i + 1, 1) = -fy + v_tilda[i] * (v2[i] + py);
		A.at<double>(2 * i + 1, 2) = fy * u_tilda[i];

		b.at<double>(2 * i) = -du[i];
		b.at<double>(2 * i + 1) = -dv[i];
	}

	solve(A, b, dAngles, DECOMP_SVD);

	result.dteta = dAngles.at<double>(0);
	result.dphi = dAngles.at<double>(1);
	result.dpsi = dAngles.at<double>(2);

	return result;
}
