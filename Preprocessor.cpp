#include "Preprocessor.h"

using namespace cv;

Preprocessor::Preprocessor(const std::string& path)
{
    videoPath = path;
}

bool Preprocessor::loadFirstFrame()
{
    Mat firstFrame;

    if (!cap.open(videoPath))
    {
        return false;
    }

    if (!cap.read(firstFrame))
    {
        return false;
    }

    cvtColor(firstFrame, prevGray, COLOR_BGR2GRAY);

    return true;
}

bool Preprocessor::getNextFrame(FramePair& frames)
{
    Mat currFrame, currGray;

    if (!cap.read(currFrame))
    {
        return false;
    }

    frames.currFrame = currFrame;

    cvtColor(currFrame, currGray, COLOR_BGR2GRAY);

    frames.currGrayFrame = currGray;
    frames.prevGrayFrame = prevGray;

    prevGray = currGray.clone();

    return true;
}