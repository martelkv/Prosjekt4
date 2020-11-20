#include "IsingModel.h"



int main(){
    IsingModel im(2, 10000000, 2.1, 2.4, 0.01);
    im.printMatrix();
    im.run();
    return 0;
};