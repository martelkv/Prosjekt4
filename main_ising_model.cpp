/* 
   Program to solve the two-dimensional Ising model 
   with zero external field using MPI
   The coupling constant J = 1
   Boltzmann's constant = 1, temperature has thus dimension energy
   metropolis sampling is used. Periodic boundary conditions.
   The code needs an output file on the command line and the variables mcs, nspins,
   initial temp, final temp and temp step.
*/

#include "MPIising.h"  

int main(int argc, char** argv){

    MPIising mpiising(40,1000000);
    mpiising.run(argc, argv);
    
    return 0;
}

<<<<<<< HEAD:main_ising_model.cpp



int main(){
    std::clock_t c_start = std::clock();
    IsingModel im(20, 1000000, 1.0, 1.0, 1.0); //Matrisedimensjoner, MCcycles, initial temp, final temp, tempstep
    im.printMatrix();
    im.run();
    std::clock_t c_end = std::clock();
=======
>>>>>>> 33ee463c5757d0e090fb153bbafe192fa9a44492:main.cpp


