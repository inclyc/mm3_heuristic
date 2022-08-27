#!/usr/bin/env python
# 输入若干空格分割的数值，取每行第三个元素，求和
from sys import stdin


def main():
    sum = 0.
    for line in stdin:
        sum += list(map(float, line.split(' ')))[2]
    print(sum)


if __name__ == '__main__':
    main()
