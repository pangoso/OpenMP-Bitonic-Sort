#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int isPowerOfTwo(int n) {
    if(n==0) return 0;

    if (ceil(log2(n)) == floor(log2(n)))
        return 1;
    else
        return 0;
}

int max(int a, int b) {
    return (a>b ? 1 : 0);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void compare(int tab[], int a, int b, int direction) {
    if (direction == 0 && max(tab[a], tab[b]) == 0) {
        swap(&tab[a], &tab[b]);
    } else if (direction == 1 && max(tab[a], tab[b]) == 1) {
        swap(&tab[a], &tab[b]);
    }
}

void bitonic_merge(int n, int tab[], int start, int direction) {
    if (n > 1) {
        int half = n / 2;

        #pragma omp parallel for shared(half, tab, start, direction)
            for (int i = start; i<(start+half); ++i) {
                compare(tab, i, i + half, direction);
            }
        
        bitonic_merge(half, tab, start, direction);
        bitonic_merge(half, tab, start + half, direction);
    }
}

void bitonic_sort_par(int n, int tab[], int start, int direction)
{
    if (n > 1) {
        int half = n / 2;

        // #pragma omp parallel shared(half, tab, start)
        {
            // printf("Running thread no. %d\n", omp_get_thread_num());
            // #pragma omp task
            {
                bitonic_sort_par(half, tab, start, 1);
            }
            // #pragma omp task
            {
                bitonic_sort_par(half, tab, start + half, 0);
            }
        }
        
        bitonic_merge(n, tab, start, direction);
    }
}

void bitonic_sort(int n, int tab[], int t) {
    int div = n / t;
    for (int i=2; i<=t; i=2*i) {
        for (int j=0; j<t; j+=i) {
            // #pragma omp parallel shared(div, tab)
            {
                if ((j/i) % 2 == 0)
                    bitonic_sort_par(i * div, tab, j * div, 1);
                else
                    bitonic_sort_par(i * div, tab, j * div, 0);
            }
        }
    }
}

int main() {
    int n;
    int checkIfPowerof2;

    FILE *f = fopen("dane.txt", "r");
    fscanf(f, "%i", &n);

    // Check if the lenth of sequence is appropriate.
    checkIfPowerof2 = isPowerOfTwo(n);

    if (checkIfPowerof2 == 0) {
        printf("The length of a sequence must be a power of 2.\n");
        fclose(f);
        return 0;
    }

    // Check if input is enough.
    int numOfThreads, threadId;
    numOfThreads =  omp_get_max_threads();
    if (n < numOfThreads * 2) {
        printf("The length of sequence is not enough.\n");
        printf("Number of threads: %i\n", numOfThreads);
        fclose(f);
        return 0;
    }

    // Input data.
    int tab[n];
    for (int i=0; i<n; ++i) {
        fscanf(f, "%i", &tab[i]);
    }
    fclose(f);

    for (int i=0; i<n; ++i) {
        printf("%d ", tab[i]);   
    }
    printf("\n");

    // Start time.
    time_t start = time(NULL);

    // Sort data.
    bitonic_sort(n, tab, numOfThreads);

    // Stop the time;
    time_t end = time(NULL);

    // Print result.
     for (int i=0; i<n; ++i) {
        printf("%d ", tab[i]);   
    }
    printf("\n");
    
    printf("Parrarel bitonic took: %.2f sec.\n", difftime(end, start));
    
    return 0;
}