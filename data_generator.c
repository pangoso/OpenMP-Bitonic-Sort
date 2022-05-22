#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *f;
    int n = 16384;

    if ((f = fopen("dane16384.txt", "w+")) == NULL) {
        printf("Nie mozna otworzyc pliku.\n");
        exit(1);
    }

    fprintf(f, "%d\n", n);

    srand(time(0));
    int max = 1000000000;
    int min = -1000000000;
    int liczba;

    for (int i=0; i<n; ++i) {
        liczba = (rand() % (max + 1 - min)) + min;
        fprintf(f, "%d ", liczba);
    }

    fclose(f);

    return 0;
}