#include "BlackSholesStep.h"
#include <iostream>
#include <algorithm>

BSstepEngine::BSstepEngine(double drift_, std::shared_ptr<valuationFunction>& theFunction_, RiskFactor simulatedRiskFactor_) : SimulationEngine(theFunction_, simulatedRiskFactor_), drift(drift_)
{
}

void BSstepEngine::DoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());  //If original TTM is lower than the VaR time horizion then only simulate up to that point
        double variance = vol * vol * adjustedHorizon;
        double rootVariance = sqrt(variance);
        double itoCorrection = -0.5 * variance;
        double factor = exp(drift * adjustedHorizon + itoCorrection + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorMultiply(factor, simulatedRiskFactor);
    }
    return;
}

void BSstepEngine::UnDoOnePath(double horizon, double vol, double normvariate)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        double adjustedHorizon = std::min(horizon, innerReferences[i].get().GetOrigTTM());  //If original TTM is lower than the VaR time horizion then only simulate up to that point
        double variance = vol * vol * adjustedHorizon;
        double rootVariance = sqrt(variance);
        double itoCorrection = -0.5 * variance;
        double factor = exp(drift * adjustedHorizon + itoCorrection + rootVariance * normvariate);
        innerReferences[i].get().RiskFactorMultiply(1 / factor, simulatedRiskFactor);
    }
}

void BSstepEngine::ApplyReturns(double returns)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        innerReferences[i].get().RiskFactorMultiply(exp(returns), simulatedRiskFactor);
    }
    return;
}

void BSstepEngine::ApplyInverseReturns(double returns)
{
    auto innerReferences = theFunction->GetInnerReference();
    for (unsigned long i = 0; i < innerReferences.size(); ++i)
    {
        innerReferences[i].get().RiskFactorMultiply(1 / exp(returns), simulatedRiskFactor);
    }
    return;
}


SimulationEngine* BSstepEngine::clone() const
{
    return new BSstepEngine(*this);
}