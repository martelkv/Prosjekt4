import numpy as np
import matplotlib.pyplot as plt


L_20 = False
Histogram = True

if L_20:
    Temp20, Etot_av20,E_var20,Mtot_av20,M_var20,Mabstot_av20= np.loadtxt("output20ny.txt", usecols=(0,1,2,3,4,5), unpack =True)
    Temp40, Etot_av40,E_var40,Mtot_av40,M_var40,Mabstot_av40= np.loadtxt("output40ny.txt", usecols=(0,1,2,3,4,5), unpack =True)
    Temp60, Etot_av60,E_var60,Mtot_av60,M_var60,Mabstot_av60 = np.loadtxt("output60ny.txt", usecols=(0,1,2,3,4,5), unpack =True)
    Temp80, Etot_av80,E_var80,Mtot_av80,M_var80,Mabstot_av80 = np.loadtxt("output80ny.txt", usecols=(0,1,2,3,4,5), unpack =True)
    Temp100, Etot_av100,E_var100,Mtot_av100,M_var100,Mabstot_av100 = np.loadtxt("output100ny.txt", usecols=(0,1,2,3,4,5), unpack =True)


    plt.plot(Temp20,M_var20,  label='20 x 20 ')
    plt.plot(Temp40,M_var40,  label='40 x 40')
    plt.plot(Temp60,M_var60,  label='60 x 60')
    plt.plot(Temp80,M_var80, label='80 x 80')
    plt.plot(Temp100,M_var100, label='100 x 100')
    plt.xlabel("T")
    plt.ylabel("χ")
    plt.legend()
    plt.title("Susceptibility vs Temperature")
    plt.savefig("x.png")
    plt.show()


if Histogram:
    Temp, Etot_av,E_var,Mtot_av,M_var,E,Mabstot_av = np.loadtxt("output200.txt", usecols=(0,1,2,3,4,5,6), unpack =True)
    plt.hist(E, density=False, bins=10)
    plt.ylabel("Energy")
    plt.xlabel("Probability")
    plt.legend()
    plt.title("Probability P(E)")
    plt.savefig("histogram1.png")
    plt.show()
