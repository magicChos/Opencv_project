import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

beforeArray = np.loadtxt("../build/beforeVertical.txt")[:200]
afterArrray = np.loadtxt("../build/afterVertical.txt")[:200]

x = np.arange(len(beforeArray))
plt.figure('before & after')
l1 = plt.plot(x , beforeArray , 'g' , lw = 3)
l2 = plt.plot(x , afterArrray , 'b' , lw = 3)

plt.legend([l1 , l2] , labels = ['before' , 'after'] , loc = 'best')
plt.show()