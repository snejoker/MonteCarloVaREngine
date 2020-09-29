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

    //
}
