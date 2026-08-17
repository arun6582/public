#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

unsigned long long factorial(int n)
{
    unsigned long long fact = 1;

    for (int i = 1; i <= n; i++)
    {
        fact *= i;
    }

    return fact;
}

void f(int a, int reps)
{
    for (int r = 0; r < reps; r++)
    {
        for (int i = 1; i <= a; i++)
        {
            factorial(i);
        }
    }
}

double memory_bandwidth_gbps(void)
{
    const size_t size = 1024ULL * 1024 * 1024; // 1 GiB
    const int reps = 10;

    char *src = aligned_alloc(64, size);
    char *dst = aligned_alloc(64, size);

    if (!src || !dst)
        return 0.0;

    memset(src, 1, size);
    memset(dst, 0, size);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < reps; i++)
        memcpy(dst, src, size);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    free(src);
    free(dst);

    return ((double)size * reps * 2.0) / elapsed / 1e9;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <n> <reps>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int reps = atoi(argv[2]);

    for (int test = 1; test <= 10; test++)
    {
        printf("TEST - #%d\n", test);
        printf("%.2f GB/s\n", memory_bandwidth_gbps());

        double total_time = 0.0;
        int total_reps = reps;

        do
        {
            clock_t start = clock();

            f(n, reps);

            clock_t end = clock();

            double run_time =
                (double)(end - start) / CLOCKS_PER_SEC;

            total_time += run_time;
            total_reps += reps;

        } while (total_time < 20.0);

        double average = total_time / total_reps;
        double score = 100.0 / average;
        printf("Score:   %.2f\n", score);
    }

    return 0;
}