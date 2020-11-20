#include "IsingModel.h"


IsingModel::IsingModel(int nSpins, int mcCycles, double initialTemp, double finalTemp, double tempStep){
    this->nSpins = nSpins;
    this->mcCycles = mcCycles;
    this->initialTemp = initialTemp;
    this->finalTemp = finalTemp;
    this->tempStep = tempStep;
    
    expectationValues = zeros<mat>(5);
    spinMatrix = zeros<mat>(nSpins,nSpins);
    magneticMoment = energy = 0;
    initializeLattice();

}



void IsingModel::run(){
    // Declare new file name and add lattice size to file name
    string fileout = "Lattice";
    fileout.append( to_string(nSpins)+".txt");
    ofile.open(fileout);

    for (double temperature = initialTemp; temperature <= finalTemp; temperature+=tempStep){
        // Start Monte Carlo computation and get expectation values
         metropolisSampling(temperature);
    
        writeResultsToFile(temperature);
  }
  ofile.close();  // close output file
}


void IsingModel::initializeLattice(){
    srand((unsigned) time(0));
    for(int i = 0; i<nSpins; i++){
        for(int j=0; j<nSpins; j++){
            if(rand() % 2 == 0)
                spinMatrix(i,j) = 1;
            else
                spinMatrix(i,j) = -1;
        }
    }

    // initialize magnetic moment and energy
    for(int i = 0; i<nSpins; i++){
        for(int j=0; j<nSpins; j++){
          magneticMoment +=  (double) spinMatrix(i,j);  
          energy -=  (double) spinMatrix(i,j) * (spinMatrix(periodicBoundary(i,nSpins,-1),j) + spinMatrix(i,periodicBoundary(j,nSpins,-1)));
        }
    }
    
}


void IsingModel::printMatrix(){
   spinMatrix.print("Initial Spin Matrix:");
}



void IsingModel::metropolisSampling(double temperature){
     // Initialize the seed and call the Mersienne algo
    random_device rd;
    mt19937_64 gen(rd());
    
    // Set up the uniform distribution for x \in [[0, 1]
    uniform_real_distribution<double> randomNumberGenerator(0.0,1.0);
   

    // setup array for possible energy changes
    vec energyDifference = zeros<mat>(17); 
    for( int de =-8; de <= 8; de+=4) energyDifference(de+8) = exp(-de/temperature);
    
    // Start Monte Carlo cycles
    for (int cycles = 1; cycles <= mcCycles; cycles++){
        // The sweep over the lattice, looping over all spin sites
        for(int x =0; x < nSpins; x++) {
            for (int y= 0; y < nSpins; y++){
	            int ix = (int) (randomNumberGenerator(gen)*(double)nSpins);
	            int iy = (int) (randomNumberGenerator(gen)*(double)nSpins);
	            int deltaE =  2*spinMatrix(ix,iy)*
	            (spinMatrix(ix,periodicBoundary(iy,nSpins,-1))+
	            spinMatrix(periodicBoundary(ix,nSpins,-1),iy) +
	            spinMatrix(ix,periodicBoundary(iy,nSpins,1)) +
	            spinMatrix(periodicBoundary(ix,nSpins,1),iy));
	            if ( randomNumberGenerator(gen) <= energyDifference(deltaE+8) ) {
	                spinMatrix(ix,iy) *= -1.0;  // flip one spin and accept new spin config
	                magneticMoment += (double) 2*spinMatrix(ix,iy);
	                energy += (double) deltaE;
	            }
            }
        }
        // update expectation values  for local node
        expectationValues(0) += energy;
        expectationValues(1) += energy*energy;
        expectationValues(2) += magneticMoment;    
        expectationValues(3) += magneticMoment*magneticMoment; 
        expectationValues(4) += fabs(magneticMoment);
    }   

}



void IsingModel::writeResultsToFile( double temperature)
{
  double norm = 1.0/((double) (mcCycles));  // divided by  number of cycles 
  double E_ExpectationValues = expectationValues(0)*norm;
  double E2_ExpectationValues = expectationValues(1)*norm;
  double M_ExpectationValues = expectationValues(2)*norm;
  double M2_ExpectationValues = expectationValues(3)*norm;
  double Mabs_ExpectationValues = expectationValues(4)*norm;
  // all expectation values are per spin, divide by 1/NSpins/NSpins
  double evariance = (E2_ExpectationValues- E_ExpectationValues*E_ExpectationValues)/nSpins/nSpins;
  double mvariance = (M2_ExpectationValues - Mabs_ExpectationValues*Mabs_ExpectationValues)/nSpins/nSpins;
  ofile << setiosflags(ios::showpoint | ios::uppercase);
  ofile << setw(15) << setprecision(8) << temperature;
  ofile << setw(15) << setprecision(8) << E_ExpectationValues/nSpins/nSpins;
  ofile << setw(15) << setprecision(8) << evariance/temperature/temperature;
  ofile << setw(15) << setprecision(8) << M_ExpectationValues/nSpins/nSpins;
  ofile << setw(15) << setprecision(8) << mvariance/temperature;
  ofile << setw(15) << setprecision(8) << Mabs_ExpectationValues/nSpins/nSpins << endl;
} // end output function

