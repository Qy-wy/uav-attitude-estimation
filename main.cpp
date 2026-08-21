#include "AttitudeEstimator.h"

int main() {
    cv::Mat K = (cv::Mat_<double>(3, 3) << 5714.285714285715, 0, 960.0, 0, 5714.285714285715, 540, 0, 0, 1);
    AttitudeEstimator ae(K, "D:\\Курсовая код\\uav-attitude-estimation\\videos\\1_1,5.mp4");
    ae.run();
    return 0;
}