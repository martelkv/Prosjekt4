#include "IsingModel.h"


IsingModel::IsingModel(int nSpins, int mcCycles, double initialTemp, double finalTemp, double tempStep){
    this->nSpins = nSpins;
    this->mcCycles = mcCycles;
    this->initialTemp = initialTemp;
    this->finalTemp = finalTemp;
    this->tempStep = tempStep;

    sum = zeros<mat>(5);
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
        std::cout << "hei" << '\n';

        //writeResultsToFile(temperature, 0,0,0);
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

        //counting every attempt
        int count_attempt = 0;
        int count_accepted = 0;


        for(int x =0; x < nSpins; x++) {
            for (int y= 0; y < nSpins; y++){

	            int ix = (int) (randomNumberGenerator(gen)*(double)nSpins);
	            int iy = (int) (randomNumberGenerator(gen)*(double)nSpins);
	            int deltaE =  2*spinMatrix(ix,iy)*
	            (spinMatrix(ix,periodicBoundary(iy,nSpins,-1))+
	            spinMatrix(periodicBoundary(ix,nSpins,-1),iy) +
	            spinMatrix(ix,periodicBoundary(iy,nSpins,1)) +
	            spinMatrix(periodicBoundary(ix,nSpins,1),iy));

              count_attempt += 1;

              if (deltaE < 0){
                count_accepted += 1;
                spinMatrix(ix,iy) *= -1.0;  // flip one spin and accept new spin config
                magneticMoment += (double) 2*spinMatrix(ix,iy);
                energy += (double) deltaE;
              }

	            else if ( randomNumberGenerator(gen) <= energyDifference(deltaE+8) ) {
                  count_accepted += 1;
	                spinMatrix(ix,iy) *= -1.0;  // flip one spin and accept new spin config
	                magneticMoment += (double) 2*spinMatrix(ix,iy);
	                energy += (double) deltaE;

	            }
            }
        }
        // update expectation values  for local node


        //std::cout << "attempt " <<cycles <<":"<< count_attempt<<'\n';
        //std::cout << "accepted " <<cycles <<":"<< count_accepted<<'\n';

        sum(0) += energy;
        sum(1) += energy*energy;
        sum(2) += magneticMoment;
        sum(3) += magneticMoment*magneticMoment;
        sum(4) += fabs(magneticMoment);

        expectationValues(0) = sum(0)/cycles;
        expectationValues(1) = sum(1)/cycles;
        expectationValues(2) = sum(2)/cycles;
        expectationValues(3) = sum(3)/cycles;
        expectationValues(4) = sum(4)/cycles;

        writeResultsToFile(temperature,count_attempt, count_accepted,cycles, energy);

    }

}


void IsingModel::WriteToFileCycle(){


}

void IsingModel::writeResultsToFile(double temperature,int count_attempt, int count_accepted,int cycles, double energy)
{

  double norm = 1.0/((double) (mcCycles));  // divided by  number of cycles
  double E_ExpectationValues = expectationValues(0);//*norm;
  double E2_ExpectationValues = expectationValues(1);//*norm;
  double M_ExpectationValues = expectationValues(2);//*norm;
  double M2_ExpectationValues = expectationValues(3);//*norm;
  double Mabs_ExpectationValues = expectationValues(4);//*norm;
  // all expectation values are per spin, divide by 1/NSpins/NSpins
  double evariance = (E2_ExpectationValues- E_ExpectationValues*E_ExpectationValues)/nSpins/nSpins;
  double mvariance = (M2_ExpectationValues - Mabs_ExpectationValues*Mabs_ExpectationValues)/nSpins/nSpins;
  ofile << setiosflags(ios::showpoint | ios::uppercase);
  ofile << setw(15) << setprecision(8) << temperature;
  ofile << setw(15) << setprecision(8) << E_ExpectationValues/nSpins/nSpins;
  ofile << setw(15) << setprecision(8) << evariance/temperature/temperature;
  ofile << setw(15) << setprecision(8) << M_ExpectationValues/nSpins/nSpins;
  ofile << setw(15) << setprecision(8) << mvariance/temperature;
  ofile << setw(15) << setprecision(8) << Mabs_ExpectationValues/nSpins/nSpins;
  ofile << setw(15) << setprecision(8) << count_attempt;
  ofile << setw(15) << setprecision(8) << count_accepted;
  ofile << setw(15) << setprecision(8) << cycles;
  ofile << setw(15) << setprecision(8) << energy<< endl;


} // end output function
