#include "AngleAccumulator.h"

double AngleAccumulator::getPhi()
{
	return phi;
}

double AngleAccumulator::getTeta()
{
	return teta;
}

double AngleAccumulator::getPsi()
{
	return psi;
}

void AngleAccumulator::updateAngles(double dteta, double dphi, double dpsi)
{
	phi += dphi;
	teta += dteta;
	psi += dpsi;
}
