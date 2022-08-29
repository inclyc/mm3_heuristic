#!/usr/bin/env python
from matplotlib import pyplot as plt
import sys
import numpy as np


def main():
    brute = []
    approx = []
    approx2 = []
    for line in sys.stdin:
        a, b, c = map(float, line.split(' '))
        brute.append(a)
        approx.append(b)
        approx2.append(c)

    x_axis = np.arange(15, 15 + len(brute))
    brute = np.array(brute)
    approx = np.array(approx)
    approx2 = np.array(approx2)
    plt.plot(x_axis, approx / brute, label='MST Accuracy')
    plt.plot(x_axis, approx2 / brute, label='Random ST Accuracy')
    plt.xlabel('Number of Edges')
    plt.ylabel('Answer')
    plt.legend()
    plt.show()


if __name__ == '__main__':
    main()
