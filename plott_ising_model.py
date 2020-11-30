import numpy as np
import matplotlib.pyplot as plt


L_20 = True
Histogram = False

if L_20:
    temp, E_exp,evariance,M_exp,mvariance,Mabs_Exp, attempt, accepted,cycles,energy = np.loadtxt("Lattice20.txt", usecols=(0,1,2,3,4,5,6,7,8,9), unpack =True)




    plt.plot(cycles, E_exp)
    plt.xlabel("Monte Carlo cycles")
    plt.ylabel(r"$\langle E \rangle$")
    plt.xscale("log")
    plt.title("Mean energy as function of the number of Monte Carlo cycles")
    plt.legend()
    plt.show()
    plt.plot(cycles, Mabs_Exp)
    plt.legend()
    plt.xlabel("Monte Carlo cycles")
    plt.ylabel(r"$\langle M \rangle$")
    plt.xscale("log")
    plt.title("Mean magnetisation (absolute value) as function of the number of Monte Carlo cycles")
    plt.show()
    plt.plot(cycles, accepted/attempt)
    plt.title(r"Total number of accepted configuratation as function of the total number of Monte carlo cycles")
    plt.xlabel("Number of cycles")
    plt.ylabel(r" $\frac{Attempt}{accepted}$")
    plt.xscale("log")
    plt.show()


if Histogram:
    temp, E_exp,evariance,M_exp,mvariance,Mabs_Exp, attempt, accepted,cycles,energy = np.loadtxt("Lattice20.txt", usecols=(0,1,2,3,4,5,6,7,8,9), unpack =True)

    plt.hist(energy, density=False, bins=30)
    plt.xlabel("Energy")
    plt.ylabel("P(E)")
    plt.title("The probability distribution for the sysyem of L=20")
    plt.show()
