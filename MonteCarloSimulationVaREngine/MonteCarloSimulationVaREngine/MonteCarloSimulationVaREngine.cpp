// MonteCarloSimulationVaREngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
#include <algorithm>
#include <iomanip>
//including helper resources
#include "ReadCSV.h"
#include "ReturnType.h"
#include "TSHandler.h"
//including Valuation Resources
#include "ValuationFunction.h"
#include "OptionCall.h"
#include "Bond.h"
#include "FXForward.h"
#include "ValuationFunctionCombiner.h"


int main()
{
    unsigned long riskFactorDaysUsed = 250;

    //Read file to get the values of risk factors, make sure the file is located in same folder as the project
    vector<vector<double>> RiskFactorTS = parse2DCsvFile("RiskFactorsHistory.csv");

    //Specifiy if the relative returns or absolute returns of the input risk factors
    vector<AbsOrRel> ReturnTypes{ AbsOrRel::rel, AbsOrRel::rel,AbsOrRel::abs, AbsOrRel::rel };

    //TSHandler - time-series handler generate returns, covariance matrix and spor Rates
    TSHandler TSHandler(RiskFactorTS, ReturnTypes, riskFactorDaysUsed);
    TSHandler.createCovarianceMatrix(0);
    vector<vector<double>> myCovMatrix = TSHandler.GetCovarianceMatrix();
    vector<double>  currentValues = TSHandler.GetMostRecentValues();

    for (unsigned long i = 0; i < currentValues.size(); i++)
    {
        std::cout << "Current values of the risk factors: " << currentValues[i] << "\n";
    }


    for (unsigned long i = 0; i < myCovMatrix.size(); i++)
    {
        for (unsigned long j = 0; j < myCovMatrix[i].size(); j++)
        {
            std::cout << myCovMatrix[i][j] << ", ";
        }
        std::cout << "\n";
    }

 

    //Creating the positions we would like to calculate VaR for
    vector<double> spotRates = TSHandler.GetMostRecentValues();
    //CallOptions
    double zeroDrift = 0.0;  double nominal = 1000; double d = 0; double r = 0; double S0 = spotRates[0]; double Strike = spotRates[0]; double frontImpvol = 0.5; double TTM = 3.0 / 12.0;
    std::shared_ptr<valuationFunction>CallOptionEquity = std::make_shared<BSCall>("3 month ATM European call", nominal, S0, r, d, frontImpvol, TTM, Strike);

    double INDEX0 = spotRates[1]; double IndexStrike = spotRates[1]; double IndexfrontImpvol = 0.25; TTM = 12.0;
    std::shared_ptr<valuationFunction> CallOptionIndex = std::make_shared<BSCall>("1 year ATM European call", nominal, INDEX0, r, d, IndexfrontImpvol, TTM, IndexStrike);
    //Combining portfolio of options as the same simulated scenarios will be used for Valuations
    std::shared_ptr<valuationFunction> CallOptions = std::make_shared<FunctionCombiner>(vector<std::shared_ptr<valuationFunction>>{ CallOptionIndex, CallOptionEquity});
    
    //Bond
    nominal = 1000; double yield = spotRates[2] / 100.0;  double facevalue = 100; double couponFreq = 2; double couponRate = 0.01; TTM = 2;
    std::shared_ptr<valuationFunction> USTreasuryBond = std::make_shared<BondFunction>("10 year UST bond", nominal, yield, facevalue, couponRate, couponFreq, TTM);
    //FX Forward
    nominal = 1000; double F0 = 1.15; double r_foreign = -0.003; double FXrate = (spotRates[3]); TTM = 1; double r_domestic = -0.02;
    double FXDrift = r_domestic - r_foreign;
    std::shared_ptr<valuationFunction> EURUSDForward = std::make_shared<FXForwardFunction>("EUR/USD FX forward", nominal, FXrate, r, r_foreign, TTM, F0);

}
