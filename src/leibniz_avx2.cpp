#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <immintrin.h>

double calculate(unsigned rounds) {
    rounds += 2u;

    __m256d x       = _mm256_set_pd(-1.0,1.0,-1.0,1.0);
    __m256d den     = _mm256_set_pd(0.0,0.0,0.0,0.0);
    __m256d inc     = _mm256_set_pd(4.0,4.0,4.0,4.0);
    __m256d two     = _mm256_set_pd(2.0,2.0,2.0,2.0);
    __m256d mone    = _mm256_set_pd(-1.0,-1.0,-1.0,-1.0);
    __m256d one     = _mm256_set_pd(1.0,1.0,1.0,1.0);
    __m256d ivec    = _mm256_set_pd(2.0,3.0,4.0,5.0);
    __m256d pivec   = _mm256_set_pd(0.0,0.0,0.0,0.0);

    unsigned int unroll = 4;
    unsigned int vec_end = rounds - rounds % unroll;

    double _x = 1.0;
    double pi = 1.0;
    for (unsigned i=2u; i < vec_end ; i+=unroll) 
    {
        //#x = -x; // some compilers optimize this better than x *= -1
        // compute den = (2 * i - 1)
        den     = _mm256_add_pd(_mm256_mul_pd(two,ivec),mone);

        // increment ivec, so ivec += inc
        ivec    = _mm256_add_pd(ivec,inc);

        // compute partial sums
        pivec   = _mm256_add_pd(pivec,_mm256_div_pd(x,den));
    }

    // gather the partial sums
    double* pi_v = (double*)&pivec;
    pi +=  pi_v[0] + pi_v[1] + pi_v[2] + pi_v[3];

    // now the wind-down loop
    for (unsigned i=vec_end ; i < rounds ; ++i) 
    {
        _x = -_x;
        pi += (_x / (2u * i - 1u));
    }

    return pi * 4;
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
