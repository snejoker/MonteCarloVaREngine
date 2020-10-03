#include "OptionCall.h"
#include <cmath>
#include <iostream>
#include <iomanip>
BSCall::BSCall(std::string uniqueIdentifier_, int nominal_, double S0_, double r_, double d_, double impvol_, double TTM_, double strike_) : r(r_), S(S0_), d(d_), impvol(impvol_), valuationFunction(uniqueIdentifier_, TTM_, nominal_), strike(strike_)
{
}

void BSCall::ValueInstrument()
{
	double d1 = (log(S / strike) + (r - d + 0.5 * impvol * impvol) * TTM) / (impvol * sqrt(TTM));
	double d2 = d1 - impvol * sqrt(TTM);
	double CumulativeNormal_d1 = std::erfc(-d1 / std::sqrt(2)) / 2;
	double CumulativeNormal_d2 = std::erfc(-d2 / std::sqrt(2)) / 2;
	f = nominal * (S * exp(-d * TTM) * CumulativeNormal_d1 - strike * exp(-r * TTM) * CumulativeNormal_d2);
	return;
}

void BSCall::RiskFactorAdd(double increment, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r += increment;
		break;
	case RiskFactor::equity:
		S += increment;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

void BSCall::RiskFactorMultiply(double factor, RiskFactor simulatedRiskFactor)
{
	switch (simulatedRiskFactor)
	{
	case RiskFactor::interest_rate:
		r *= factor;
		break;
	case RiskFactor::equity:
		S *= factor;
		break;
	default:
		throw std::runtime_error("Unsupported or unknown risk factor found in " + GetuniqueIdentifier()[0]);
		break;
	}
	return;
}

std::vector<std::reference_wrapper<valuationFunction>> BSCall::GetInnerReference()
{
	std::vector<std::reference_wrapper<valuationFunction>> innerVector;
	innerVector.push_back(std::ref(*this));
	return innerVector;
}

valuationFunction* BSCall::clone() const
{
	return new BSCall(*this);
}
