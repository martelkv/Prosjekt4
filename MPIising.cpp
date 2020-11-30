#include "MPIising.h"


// function to initialise energy, spin matrix and magnetization
void MPIising::initialize()
{
    // setup spin matrix and intial magnetization
    for(int y =0; y < n_spins; y++) {
        for (int x= 0; x < n_spins; x++){
            spin_matrix(y,x) = 1; // spin orientation for the ground state
            M +=  (double) spin_matrix(y,x);
        }
    }
    
    // setup initial energy
    for(int y =0; y < n_spins; y++) {
        for (int x= 0; x < n_spins; x++){
            E -=  (double) spin_matrix(y,x)*
	        (spin_matrix(periodic(y,n_spins,-1),x) +
	        spin_matrix(y,periodic(x,n_spins,-1)));
        }
    }
}



void MPIising::metropolis()
{
    int count_attempt;
    // loop over all spins
    for(int y =0; y < n_spins; y++) {
        for (int x= 0; x < n_spins; x++){
            int ix = (int) (ran2()*(double)n_spins);
            int iy = (int) (ran2()*(double)n_spins);
            int deltaE =  2*spin_matrix(iy,ix)*
	                    (spin_matrix(iy,periodic(ix,n_spins,-1))+
	                    spin_matrix(periodic(iy,n_spins,-1),ix) +
	                    spin_matrix(iy,periodic(ix,n_spins,1)) +
	                    spin_matrix(periodic(iy,n_spins,1),ix));
            if ( ran2() <= w[deltaE+8] ) {
	            spin_matrix(iy,ix) *= -1;  // flip one spin and accept new spin config
                M += (double) 2*spin_matrix(iy,ix);
                E += (double) deltaE;
            }
        }
    }
}   





void MPIising::output(double temperature, double E)
{
    double norm = 1/((double) (mcs));  // divided by total number of cycles 
    double Etotal_average = total_average[0]*norm;
    double E2total_average = total_average[1]*norm;
    double Mtotal_average = total_average[2]*norm;
    double M2total_average = total_average[3]*norm;
    double Mabstotal_average = total_average[4]*norm;
    
    // all expectation values are per spin, divide by 1/n_spins/n_spins
    double Evariance = (E2total_average- Etotal_average*Etotal_average)/n_spins/n_spins;
    double Mvariance = (M2total_average - Mtotal_average*Mtotal_average)/n_spins/n_spins;

    ofile << setiosflags(ios::showpoint | ios::uppercase);
    ofile << setw(15) << setprecision(8) << temperature;
    ofile << setw(15) << setprecision(8) << Etotal_average/n_spins/n_spins;
    ofile << setw(15) << setprecision(8) << Evariance/temperature/temperature;
    ofile << setw(15) << setprecision(8) << Mtotal_average/n_spins/n_spins;
    ofile << setw(15) << setprecision(8) << Mvariance/temperature;
    ofile << setw(15) << setprecision(8) << E/n_spins/n_spins;
    ofile << setw(15) << setprecision(8) << Mabstotal_average/n_spins/n_spins << endl;
} 

MPIising::MPIising(int n_spins, int mcs)
{
    this->n_spins=n_spins;
    this->mcs=mcs;
    this->outfilename = outfilename;
    spin_matrix = zeros<mat>(n_spins,n_spins);
}


void MPIising::run(int argc, char **argv)
{    
    //  MPI initializations
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
 
    if (my_rank == 0 && argc <= 1) {
        cout << "Bad Usage: " << argv[0] << 
        " read output file" << endl;
        exit(1);
    }
    
    if (my_rank == 0 && argc > 1) {
        outfilename = argv[1];
    }

    if (my_rank == 0) {
        ofile.open(outfilename); 
    }

    double  average[6], initial_temp=2.0, final_temp=2.6, temp_step=0.05;
    /*
    Determine number of intervall which are used by all processes
    myloop_begin gives the starting point on process my_rank
    myloop_end gives the end point for summation on process my_rank
    */
    int no_intervalls = mcs/numprocs;
    int myloop_begin = my_rank*no_intervalls + 1;
    int myloop_end = (my_rank+1)*no_intervalls;
    if ( (my_rank == numprocs-1) &&( myloop_end < mcs) ) 
        myloop_end = mcs;

    // broadcast to all nodes common variables
    MPI_Bcast (&n_spins, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&initial_temp, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&final_temp, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&temp_step, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // every node has its own seed for the random numbers, this is important else
    // if one starts with the same seed, one ends with the same random numbers
    idum = -1-my_rank;  // random starting point
    // Start Monte Carlo sampling by looping over T first

    double  TimeStart, TimeEnd, TotalTime;
    TimeStart = MPI_Wtime();
    for ( double temperature = initial_temp; temperature <= final_temp; temperature+=temp_step){
        //    initialise energy and magnetization 
        E = M = 0.;
        // initialise array for expectation values
        initialize();
        
        // setup array for possible energy changes
        for( int de =-8; de <= 8; de++) 
            w[de+8] = 0;
        
        for( int de =-8; de <= 8; de+=4) 
            w[de+8] = exp(-de/temperature);
        
        for( int i = 0; i < 5; i++) 
            average[i] = 0.;
        
        for( int i = 0; i < 5; i++) 
            total_average[i] = 0.;


         // burn in
        for (int cycles = myloop_begin; cycles <= myloop_end/10; cycles++){
            metropolis();
        }


        // start Monte Carlo computation
        for (int cycles = myloop_begin; cycles <= myloop_end; cycles++){
            metropolis();
    
            // update expectation values  for local node
            average[0] += E;    
            average[1] += E*E;
            average[2] += M;    
            average[3] += M*M; 
            average[4] += fabs(M);
            average[5]=E;
        bool output_temp = true;
        bool output_mcs = false;
         // print results
        if ( output_mcs) {
            output(temperature,E);
        }
        }
    



        // Find total average
        for( int i =0; i < 6; i++){
            MPI_Reduce(&average[i], &total_average[i], 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }
        
        // print results
        if ( output_temp) {
            output(temperature,E);
        }
    }
  
 
    ofile.close();  // close output file
    
    TimeEnd = MPI_Wtime();
    TotalTime = TimeEnd-TimeStart;
    if ( my_rank == 0) {
        cout << "Time = " <<  TotalTime  << " on number of processors: "  << numprocs  << endl;
    }

    // End MPI
    MPI_Finalize (); 
}



/*
** The function ran2()
** is a long periode (> 2 x 10^18) random number generator of 
** L'Ecuyer and Bays-Durham shuffle and added safeguards.
** Call with idum a negative integer to initialize; thereafter,
** do not alter idum between sucessive deviates in a
** sequence. RNMX should approximate the largest floating point value
** that is less than 1.
** The function returns a uniform deviate between 0.0 and 1.0
** (exclusive of end-point values).
*/

#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

double MPIising::ran2()
{
    int j;
    long k;
    static long idum2 = 123456789;
    static long iy=0;
    static long iv[NTAB];
    double temp;

    if(idum <= 0) {
        if(-(idum) < 1) 
            idum = 1;
        else
            idum = -(idum);
        
        idum2 = (idum);
        for(j = NTAB + 7; j >= 0; j--) {
            k = (idum)/IQ1;
            idum = IA1*(idum - k*IQ1) - k*IR1;
            if(idum < 0)
                idum +=  IM1;
            if(j < NTAB)
                iv[j]  = idum;
        }

        iy=iv[0];
    }

    k= (idum)/IQ1;
    idum = IA1*(idum - k*IQ1) - k*IR1;
    
    if(idum < 0) 
        idum += IM1;
    
    k = idum2/IQ2;
    idum2 = IA2*(idum2 - k*IQ2) - k*IR2;
    
    if(idum2 < 0)
        idum2 += IM2;
    j = iy/NDIV;
    iy = iv[j] - idum2;
    iv[j] = idum;
    
    if(iy < 1) 
        iy += IMM1;
    
    if((temp = AM*iy) > RNMX)
        return RNMX;
    else
        return temp;
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX


