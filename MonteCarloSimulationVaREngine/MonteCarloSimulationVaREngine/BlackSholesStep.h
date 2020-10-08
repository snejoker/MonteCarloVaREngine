#pragma once
#include "ValuationFunction.h"
#include "SimulationEngine.h"
class BSstepEngine : public SimulationEngine
{
public:
	BSstepEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor);
	void ApplyReturns(double returns);
	void ApplyInverseReturns(double returns);
	void DoOnePath(double horizon, double vol, double normvariate);
	void UnDoOnePath(double horizon, double vol, double normvariate);
	virtual SimulationEngine* clone() const;
private:
	double drift;
};