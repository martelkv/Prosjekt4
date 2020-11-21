#include "IsingModel.h"
#include "time.h"
#include <ctime>



int main(){
    std::clock_t c_start = std::clock();
    IsingModel im(20, 10000, 2.4, 2.4, 1.0); //Matrisedimensjoner, MCcycles, initial temp, final temp, tempstep
    im.printMatrix();
    im.run();
    std::clock_t c_end = std::clock();

    double time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    std::cout << "CPU time used: " << time_elapsed_ms /1000.0 << " s\n";

    return 0;
};
