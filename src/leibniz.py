#!/usr/bin/env python
# coding=utf-8

import time

def calculate(rounds):
    pi = 1.0
    x = 1.0

    for i in range(2, rounds + 2):
        x *= -1
        pi += (x / (2 * i - 1))

    return 4 * pi

with open("rounds.txt") as file:
    rounds = int(file.read())

for i in range(10):
    start = time.time_ns()
    pi = calculate(rounds)
    end = time.time_ns()

    t = (end - start) / 1000_000
    print("Warmup Iteration {} ({:.3f}ms): {:.16f}".format(i, t, pi))

totalTime = 0

for i in range(10):
    start = time.time_ns()
    pi = calculate(rounds)
    end = time.time_ns()

    t = (end - start) / 1000_000
    print("Iteration {} ({:.3f}ms): {:.16f}".format(i, t, pi))
    totalTime += t

print("Average: {}ms".format(totalTime / 10))
