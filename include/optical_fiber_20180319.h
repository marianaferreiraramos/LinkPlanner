#ifndef OPTICAL_FIBER_H
#define OPTICAL_FIBER_H

#include "netxpto_20180118.h"
#include <math.h> 

enum SOPType{DeterministicStatic, DeterministicDynamic, Stocastic};

class OpticalFiber : public Block {
	/*Input Parameters*/
	SOPType sopType{ DeterministicStatic };
	double zDistance{0.001}; //Distance in km
	double tetha{ 0.0 }; // rotation angle in degrees
	int lambda{ 1550 }; // wavelength in nm
	double c{ 3e8 }; // speed of ligth
	double Dp{ 16 }; //Polarization mode dispersion coefficient ps/nm km

public:
	OpticalFiber() {};
	OpticalFiber(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSOPType(SOPType sType) { sopType = sType; };
	SOPType getSOPType() { return sopType; };

	void setFiberLength(double fLength) { zDistance = fLength; };
	double getFiberLength() { return zDistance; };

	void setRotationAngle(double angle) { tetha = angle; };
	double getRotationAngle() { return tetha; };

	void setWaveLength(int wLength) { lambda = wLength; };
	int getWaveLength() { return lambda; };

	void setDispersionCoefficient(double dCoefficient) { Dp = dCoefficient; };
	double getSispersionCoefficient() { return Dp; };
};

#endif // !OPTICAL_FIBER_H

