#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTY 10

void wypelnij(float *a, float *b, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = (float)(rand() % 1000) / 10.0f;
        b[i] = (float)(rand() % 1000 + 1) / 10.0f;
    }
}

float licz_czas(char op, float *a, float *b, float *wynik, int n) {
    clock_t start, end;
    float suma = 0;

    for (int t = 0; t < TESTY; t++) {
        start = clock();

        for (int i = 0; i < n; i++) {
            switch (op) {
                case '+':
                    __asm__ volatile(
                        "flds %1\n\t"
                        "fadds %2\n\t"
                        "fstps %0\n\t"
                        : "=m"(wynik[i])
                        : "m"(a[i]), "m"(b[i])
                    );
                    break;
                case '-':
                    __asm__ volatile(
                        "flds %1\n\t"
                        "fsubs %2\n\t"
                        "fstps %0\n\t"
                        : "=m"(wynik[i])
                        : "m"(a[i]), "m"(b[i])
                    );
                    break;
                case '*':
                    __asm__ volatile(
                        "flds %1\n\t"
                        "fmuls %2\n\t"
                        "fstps %0\n\t"
                        : "=m"(wynik[i])
                        : "m"(a[i]), "m"(b[i])
                    );
                    break;
                case '/':
                    __asm__ volatile(
                        "flds %1\n\t"
                        "fdivs %2\n\t"
                        "fstps %0\n\t"
                        : "=m"(wynik[i])
                        : "m"(a[i]), "m"(b[i])
                    );
                    break;
            }
        }

        end = clock();
        suma += (float)(end - start) / CLOCKS_PER_SEC;
    }

    return suma / TESTY;
}

void zapisz(FILE *plik, int n, float d, float o, float m, float dz) {
    fprintf(plik, "Typ obliczen: SISD \n");
    fprintf(plik, "Liczba liczb: %d\n", n);
    fprintf(plik, "Sredni czas [s]:\n");
    fprintf(plik, "+ %.6f\n", d);
    fprintf(plik, "- %.6f\n", o);
    fprintf(plik, "* %.6f\n", m);
    fprintf(plik, "/ %.6f\n\n", dz);
}

int main() {
    srand(time(NULL));
    FILE *plik = fopen("wynik_SISD.txt", "w");

    if (!plik) {
        printf("Nie można otworzyć pliku.\n");
        return 1;
    }

    int rozmiary[] = {2048, 4096, 8192};

    for (int r = 0; r < 3; r++) {
        int n = rozmiary[r];
        float *a = malloc(sizeof(float) * n);
        float *b = malloc(sizeof(float) * n);
        float *wynik = malloc(sizeof(float) * n);

        wypelnij(a, b, n);

        float t1 = licz_czas('+', a, b, wynik, n);
        float t2 = licz_czas('-', a, b, wynik, n);
        float t3 = licz_czas('*', a, b, wynik, n);
        float t4 = licz_czas('/', a, b, wynik, n);

        zapisz(plik, n, t1, t2, t3, t4);

        free(a);
        free(b);
        free(wynik);
    }

    fclose(plik);
    printf("Zapisano wyniki SISD do pliku wynik_SISD.txt.\n");
    return 0;
}

