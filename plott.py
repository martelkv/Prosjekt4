import numpy as np
import matplotlib.pyplot as plt


L_20 = False
Histogram = True

if L_20:
    temp, E_exp,evariance,M_exp,mvariance,Mabs_Exp, attempt, accepted,cycles,energy = np.loadtxt("Lattice20.txt", usecols=(0,1,2,3,4,5,6,7,8,9), unpack =True)




    plt.plot(cycles, E_exp, label="energy")
    plt.xlabel("cycles")
    plt.ylabel("E_exp")
    plt.legend()
    plt.show()
    plt.plot(cycles, Mabs_Exp, label="magnetization")
    plt.legend()
    plt.show()
    plt.plot(cycles, attempt/accepted)
    plt.title("Total number of accepted configuratation/attempt")
    plt.xlabel("Number of cycles")
    plt.ylabel("Attempt/accepted")
    plt.show()


if Histogram:
    temp, E_exp,evariance,M_exp,mvariance,Mabs_Exp, attempt, accepted,cycles,energy = np.loadtxt("Lattice20.txt", usecols=(0,1,2,3,4,5,6,7,8,9), unpack =True)

    plt.hist(energy, density=False, bins=30)
    plt.show()
