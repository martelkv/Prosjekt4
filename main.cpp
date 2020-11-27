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

    MPIising mpiising(80,1000000);
    mpiising.run(argc, argv);
    
    return 0;
}



