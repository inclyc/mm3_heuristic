#!/usr/bin/env python
import sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


# for each vertex, allocate a new vertex numer and assign to it
class Allocator:
    def __init__(self):
        self.cnt = 0
        self.mp = {}

    def get(self):
        def mapPoint(node: int):
            if node in self.mp:
                return self.mp[node]
            self.cnt += 1
            self.mp[node] = self.cnt
            return self.cnt
        return mapPoint


def main():
    points = set()

    edge = []
    for line in sys.stdin:
        if line.startswith("Graph"):
            # Ignore Graph: xxx xxx, we don't need this
            continue
        e = eval(line)
        points.add(e[0])
        points.add(e[1])
        edge.append(e)

    print("{} {}".format(len(points), len(edge)))
    mapPoint = Allocator().get()
    for u, v, w in edge:
        print(mapPoint(u), mapPoint(v), w)

    for node in points:
        eprint("{} {}".format(node, mapPoint(node)))


if __name__ == '__main__':
    main()
