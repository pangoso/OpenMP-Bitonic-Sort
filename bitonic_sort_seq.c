#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

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

        for (int i = start; i<(start+half); ++i) {
            compare(tab, i, i + half, direction);
        }
        bitonic_merge(half, tab, start, direction);
        bitonic_merge(half, tab, start + half, direction);
    }
}

void bitonic_sort_seq(int n, int tab[], int start, int direction) {
    if (n > 1) {
        int half = n / 2;

        //Sort ascending.
        bitonic_sort_seq(half, tab, start, 1);

        //Sort descending.
        bitonic_sort_seq(half, tab, start + half, 0);

        //Merge.
        bitonic_merge(n, tab, start, direction);
    }
}

int main() {
    int n;
    int checkIfPowerof2;

    FILE *f = fopen("dane16384.txt", "r");
    fscanf(f, "%i", &n);

    // Check if the lenth of sequence is appropriate.
    checkIfPowerof2 = isPowerOfTwo(n);

    if (checkIfPowerof2 == 0) {
        printf("The length of a sequence must be a power of 2.\n");
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
    int up = 1;
    bitonic_sort_seq(n, tab, 0, up);

    // Stop the time;
   time_t end = time(NULL);

    // Print result.
     for (int i=0; i<n; ++i) {
        printf("%d ", tab[i]);   
    }
    printf("\n");

    printf("Sequential bitonic took: %.2f sec.\n", difftime(end, start));
    
    return 0;
}