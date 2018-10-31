#!/usr/bin/env python3

import sys
import numpy as np
from spc import SPC
import matplotlib.pyplot as plt


def plot(files, fac=1.0):
    for f in files:
        if f.split('.')[-1] == 'xy':
            td = np.loadtxt(f)
            plt.plot(td[:, 0], np.log(1. / td[:, 1]) * fac, label=f)
        elif f.split('.')[-1] == 'spc':
            td = SPC(f)
            plt.plot(td.xdata, np.log(1. / np.array(td.ydata)), label=f)
    plt.legend()
    plt.show()


if __name__ == '__main__':
    files = sys.argv[2:]
    fac = float(sys.argv[1])
    plot(files, fac)
