#ifndef ANGLEACCUMULATOR_H
#define ANGLEACCUMULATOR_H

#include <opencv2/opencv.hpp>

class AngleAccumulator {
private:
	double phi = 0;
	double teta = 0;
	double psi = 0;
public:
	double getPhi();
	double getTeta();
	double getPsi();
	void updateAngles(double dteta, double dphi, double dpsi);
};

#endif //ANGLEACCUMULATOR_H

