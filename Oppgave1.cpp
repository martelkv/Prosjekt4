
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <armadillo>
#include <string>
using namespace  std;
using namespace arma;
// output file
ofstream ofile;

// function to initialise energy, spin matrix and magnetization
void InitializeLattice(int NSpins, mat &SpinMatrix,  double& Energy, double& MagneticMoment)
{
  // setup spin matrix and initial magnetization
  for(int x =0; x < NSpins; x++) {
    for (int y= 0; y < NSpins; y++){
      SpinMatrix(x,y) = 1.0; // spin orientation for the ground state
      MagneticMoment +=  (double) SpinMatrix(x,y);
    }
  }
  // setup initial energy
  for(int x =0; x < NSpins; x++) {
    for (int y= 0; y < NSpins; y++){
      Energy -=  (double) SpinMatrix(x,y)*
	(SpinMatrix(PeriodicBoundary(x,NSpins,-1),y) +
	 SpinMatrix(x,PeriodicBoundary(y,NSpins,-1)));
    }
  }
}// end function initialise


 //heihei
