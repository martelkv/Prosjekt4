#ifndef ISINGMODEL_H
#define ISINGMODEL_H
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <armadillo>
#include <string>
#include <ctime>
using namespace  std;
using namespace arma;



class IsingModel{
    mat spinMatrix;
    int nSpins, mcCycles;
    vec expectationValues;
    double initialTemp, finalTemp, tempStep, energy, magneticMoment;
    ofstream ofile; // output file

public:
    IsingModel(int nSpins, int mcCycles, double initialTemp, double finalTemp, double tempStep);
    void run();
    void printMatrix();

private:
    void metropolisSampling(double temperature);
    void initializeLattice();
    void writeResultsToFile(double temperature);
    int periodicBoundary(int i, int limit, int add) { return (i+limit+add) % (limit);}
};


#endif // ISINGMODEL_H