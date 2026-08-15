#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <n> <reps>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int reps = atoi(argv[2]);

    for (int test = 1; test <= 3; test++)
    {
        printf("TEST - %d\n", test);

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

        printf("Total:   %.6f seconds\n", total_time);
        printf("Average: %.9f seconds\n", average);
        printf("Score:   %.2f\n", score);
    }

    return 0;
}