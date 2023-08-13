#include <cstdio>
#include <cstdlib>
#include <chrono>

double calculate(unsigned rounds) {
    rounds += 2u; // do this outside the loop

    double res = 1.0;
    for (unsigned i = 2u ; i < rounds ; ++i)
    {
        double x = -1.0 + 2.0 * (i & 0x1);
        res += (x / (2u * i - 1u)); // double / unsigned = double
    }

    return res * 4;
}

int main()
{
    auto infile = std::fopen("rounds.txt", "r");    // open file
    if (infile == NULL) {
        perror("open file");
        return EXIT_FAILURE;
    }

    unsigned rounds;
    if (std::fscanf(infile, "%u", &rounds) != 1) {  // read from file
        perror("read file");
        return EXIT_FAILURE;
    }
    std::fclose(infile);                            // close file

    for (int i = 0; i < 10; i++) {
        auto start = std::chrono::steady_clock::now();
        double pi = calculate(rounds);
        auto end = std::chrono::steady_clock::now();

        double time = ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1000000;

        std::printf("Warmup Iteration %d (%.3lfms): %.16lf\n", i, time, pi);
    }

    double totalTime = 0;

    for (int i = 0; i < 10; i++) {
        auto start = std::chrono::steady_clock::now();
        double pi = calculate(rounds);
        auto end = std::chrono::steady_clock::now();

        double time = ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1000000;

        std::printf("Iteration %d (%.3lfms): %.16lf\n", i, time, pi);

        totalTime += time;
    }

    std::printf("Average: %lfms\n", totalTime / 10);
    return 0;
}
