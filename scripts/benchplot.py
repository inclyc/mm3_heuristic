#!/usr/bin/env python
from matplotlib import pyplot as plt
import sys
import numpy as np


def main():
    brute = []
    approx = []
    for line in sys.stdin:
        a, b = map(float, line.split(' '))
        brute.append(a)
        approx.append(b)

    x_axis = np.arange(len(brute))
    brute = np.array(brute)
    approx = np.array(approx)
    plt.plot(x_axis, approx / brute, label='Accuracy')
    plt.xlabel('Test case')
    plt.ylabel('Answer')
    plt.legend()
    plt.show()


if __name__ == '__main__':
    main()
