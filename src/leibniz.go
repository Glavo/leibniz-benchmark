package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

func calculate(rounds int) float64 {
	rounds += 2

	pi := 1.0
	x := 1.0

	for i := 2; i < rounds; i++ {
		x *= -1
		pi += x / float64(2*i-1)
	}

	return pi
}

func main() {
	file, err := os.ReadFile("rounds.txt")
	if err != nil {
		fmt.Print(err)
	}

	rounds, _ := strconv.Atoi(strings.TrimSpace(string(file)))

	for i := 0; i < 10; i++ {
		start := time.Now()
		pi := calculate(rounds)

		t := float64(time.Since(start).Nanoseconds()) / 1000_000

		fmt.Printf("Warmup Iteration %d (%.3f ms): %.16f\n", i, t, pi)
	}

	totalTime := 0.0

	for i := 0; i < 10; i++ {
		start := time.Now()
		pi := calculate(rounds)

		t := float64(time.Since(start).Nanoseconds()) / 1000_000

		fmt.Printf("Iteration %d (%.3f ms): %.16f\n", i, t, pi)

		totalTime += t
	}

	fmt.Printf("Average: %fms\n", totalTime/10)
}
