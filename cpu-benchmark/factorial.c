#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <stdint.h>
#include <time.h>

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

#define ITERATIONS 500000000ULL

typedef struct
{
    uint64_t iterations;
    double result;
} worker_args;

static int cpu_count(void)
{
#ifdef __APPLE__
    int count;
    size_t size = sizeof(count);

    sysctlbyname("hw.logicalcpu", &count, &size, NULL, 0);

    return count;
#else
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

static double now_seconds(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (double)ts.tv_sec +
           (double)ts.tv_nsec / 1e9;
}

static void *worker(void *arg)
{
    worker_args *a = arg;

    double x = 1.0;

    for (uint64_t i = 0; i < a->iterations; i++)
        x = x * 1.0000001 + 0.0000001;

    a->result = x;

    return NULL;
}

double multicore_benchmark(void)
{
    int cores = cpu_count();
    printf("Cores: %d\n", cores);

    pthread_t *threads =
        malloc(sizeof(*threads) * cores);

    worker_args *args =
        malloc(sizeof(*args) * cores);

    double start = now_seconds();

    for (int i = 0; i < cores; i++)
    {
        args[i].iterations = ITERATIONS;
        args[i].result = 0.0;

        pthread_create(
            &threads[i],
            NULL,
            worker,
            &args[i]);
    }

    double result = 0.0;

    for (int i = 0; i < cores; i++)
    {
        pthread_join(threads[i], NULL);
        result += args[i].result;
    }

    free(threads);
    free(args);

    /* Prevent compiler from eliminating the calculation. */
    volatile double sink = result;
    (void)sink;

    double time = now_seconds() - start;
    double score = 100.0 / time;
    printf("Multi score Score:   %.2f\n", score);
    return score;
}

unsigned long long factorial(int n)
{
    unsigned long long fact = 1;

    for (int i = 1; i <= n; i++)
    {
        fact *= i;
    }

    return fact;
}

void factorials(int a, int reps)
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
    const size_t size = 256ULL * 1024 * 1024; // 1 GiB
    const int reps = 10;

    char *src = malloc(size);
    char *dst = malloc(size);

    if (!src || !dst)
    {
        free(src);
        free(dst);
        printf("Memory test failed\n");
        return 0.0;
    }

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

    double gbps = ((double)size * reps * 2.0) / elapsed / 1e9;
    printf("%.2f GB/s\n", gbps);
    return gbps;
}

double singlecore_benchmark(void (*f)(int, int), int n, int reps)
{
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

    printf("Single core score:   %.2f\n", score);
    return score;
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

        memory_bandwidth_gbps();
        singlecore_benchmark(factorials, n, reps);
        multicore_benchmark();
    }

    return 0;
}