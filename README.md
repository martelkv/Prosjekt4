

# Prosjekt4
Authors: Jelena Jankovic and Marte Lunde Kvam  
Usernames: jelenaj98 and martelkv.  

NB: Read section *Other*.  

### What the directory contains:
- *main.cpp* - The file which everything is run from.  
- *Makefile* - To easily compile and run the code. Run 
- **IsingModel.cpp** - The main source code file used in **main_ising_model.cpp**.  
- **IsingModel.h** - The header file corresponding to **IsingModel.cpp**.  
- **MPIising** - The file used in **main.cpp**.  
- **plott.py** - The file used to plot the results from tasks F, G and E.  
- **plottMPIising.py** - This plot is used for plotting with **main**
- **Lattice\*** - Text files used for plotting.   


### How to run:
- To compile the MPI-code, run the command: ```mpic++ -O3 -o a  *.cpp```.   
- To run the code, run ```mpirun --hostfile my_hostfile -n 8 ./a <textfile name>.txt``` .     


### Dependencies:
- *Armadillo* is used for its data structures.  
The installation process below is from: uio.no/studier/emner/matnat/fys/FYS4411/v13/guides/installing-armadillo/ and is for Linux-users.   
    - ```sudo apt-get install liblapack-dev
    - 
    - ```sudo apt-get install libboost-dev
    Finally, run ```sudo apt-get install libarmadillo-dev```.  
 
On MacOS, this is:  
    - ```brew install armadillo```.  


### Other:
- Task D has been solved by using *main_ising_model.cpp* and *plott_ising_model.py*.      
- The rest of the tasks have been solved using *main.cpp* and *plottMPIising.cpp*.
