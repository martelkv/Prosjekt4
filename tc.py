import numpy as np
import matplotlib.pyplot as plt


x = [0.025,0.016667,0.0125,0.01]
y = [2.21,2.24,2.27,2.28]

plt.plot(x,y)
plt.xlabel("1/L")
plt.ylabel("Tc")
plt.savefig("tc.png")
plt.show()