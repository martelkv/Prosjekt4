import numpy as np
import matplotlib.pyplot as plt


L_20 = True
#Histogram = False

if L_20:
    Temp, Etot_av,E_var,Mtot_av,M_var,Mabstot_av = np.loadtxt("output40.txt", usecols=(0,1,2,3,4,5), unpack =True)


    plt.plot(Temp,M_var, label="evariance")
    plt.xlabel("Temperature")
    plt.ylabel("E_varience")
    plt.legend()
    plt.show()
    #plt.plot(M_tot, temp, label="magnetization")
    plt.legend()
    #plt.show()
    #plt.plot(cycles, attempt/accepted)
    #plt.title("Total number of accepted configuratation/attempt")
    #plt.xlabel("Number of cycles")
    #plt.ylabel("Attempt/accepted")
    #plt.show()

"""
if Histogram:
   # temp, E_exp,evariance,M_exp,mvariance,Mabs_Exp, attempt, accepted,cycles,energy = np.loadtxt("Lattice20.txt", usecols=(0,1,2,3,4,5), unpack =True)

    #plt.hist(energy, density=False, bins=30)
    #plt.show()
    """