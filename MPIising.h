#ifndef MPIISING_H
#define MPIISING_H

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <armadillo>
#include <string>

using namespace  std;
using namespace arma;

class MPIising{
    mat spin_matrix;
    string outfilename;
    ofstream ofile;

    int n_spins,mcs, my_rank, numprocs;
    double E, M, w[17], total_average[6], initial_temp, final_temp, temp_step;
    long idum;
    bool output_temp;
    bool output_mcs;

  

public:
    MPIising(int argc, char **argv);
    void run();

private:
    void initMPI(int argc, char **argv);
    void initialize();
    void metropolis();
    double ran2();
    void output(double temperature,double E);
    int inputInteger(string name);
    double inputDouble(string name);
    // function for periodic boundary conditions
    int periodic(int i, int limit, int add) {return (i+limit+add) % (limit);}

};

#endif //MPIISING_H 

