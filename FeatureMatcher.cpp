#include "FeatureMatcher.h"

using namespace cv;

FeatureMatcher::FeatureMatcher(int nFeatures)
{
	orb = ORB::create(nFeatures);
	freak = xfeatures2d::FREAK::create();
	matcher = BFMatcher::create(NORM_HAMMING, true);
}

MatchResult FeatureMatcher::match(const cv::Mat& prevGrayFrame, const cv::Mat& currGrayFrame)
{
	MatchResult result;

	orb->detect(prevGrayFrame, result.point1);
	orb->detect(currGrayFrame, result.point2);

    if (result.point1.empty() || result.point2.empty())
    {
        return result;
    }

	freak->compute(prevGrayFrame, result.point1, descriptor1);
	freak->compute(currGrayFrame, result.point2, descriptor2);

	std::vector<DMatch> rawMatches;
	if(!descriptor1.empty() && !descriptor2.empty())
	{
		matcher->match(descriptor1, descriptor2, rawMatches);
	}

    if (rawMatches.size() < 8)
    {
        result.matches = rawMatches;
        return result;
    }

    std::vector<Point2f> pts1, pts2;
    pts1.reserve(rawMatches.size());
    pts2.reserve(rawMatches.size());

    for (const DMatch& m : rawMatches)
    {
        pts1.push_back(result.point1[m.queryIdx].pt);
        pts2.push_back(result.point2[m.trainIdx].pt);
    }

    std::vector<uchar> mask;
    Mat F;

    // add try catch, because at the end of the video findFundamentalMat throws exception
    try
    {
        F = findFundamentalMat(pts1, pts2, FM_RANSAC, 1.0, 0.99, mask);
    }
    catch (Exception e)
    {
        result.matches = rawMatches;
        return result;        
    }

    if (F.empty() || mask.empty())
    {
        result.matches = rawMatches;
        return result;
    }

    std::vector<DMatch> inliers;
    inliers.reserve(rawMatches.size());

    for (size_t i = 0; i < rawMatches.size(); i++)
    {
        if (mask[i])
        {
            inliers.push_back(rawMatches[i]);
        }
    }

	result.matches = inliers;

	return result;
}
