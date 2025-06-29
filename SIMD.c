#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTY 10

void generate_random_vectors(float *a, float *b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = (float)(rand() % 1000) / 10.0f;
        b[i] = (float)(rand() % 1000 + 1) / 10.0f;
    }
}

int main() {
    srand(time(NULL));
    FILE *plik = fopen("wynik_SIMD.txt", "w");
    if (!plik) {
        perror("Błąd otwierania pliku");
        return 1;
    }

    int rozmiary[] = {2048, 4096, 8192};
    char operacje[] = {'+', '-', '*', '/'};

    for (int z = 0; z < 3; z++) {
        int n = rozmiary[z];
        float *a = malloc(sizeof(float) * n);
        float *b = malloc(sizeof(float) * n);
        float *wynik = malloc(sizeof(float) * n);
        generate_random_vectors(a, b, n);

        double suma[4] = {0};

        for (int o = 0; o < 4; o++) {
            char op = operacje[o];

            for (int t = 0; t < TESTY; t++) {
                clock_t start = clock();

                for (int i = 0; i < n; i += 4) {
                    switch (op) {
    			case '+':
        			__asm__ volatile (
            			"movups (%1), %%xmm0 \n\t"
            			"movups (%2), %%xmm1 \n\t"
            			"addps %%xmm1, %%xmm0 \n\t"
            			"movups %%xmm0, (%0) \n\t"
            			:
            			: "r"(&wynik[i]), "r"(&a[i]), "r"(&b[i])
            			: "xmm0", "xmm1", "memory"
        			);break;

    			case '-':
        			__asm__ volatile (
            				"movups (%1), %%xmm0 \n\t"
            				"movups (%2), %%xmm1 \n\t"
            				"subps %%xmm1, %%xmm0 \n\t"
            				"movups %%xmm0, (%0) \n\t"
            				:
            				: "r"(&wynik[i]), "r"(&a[i]), "r"(&b[i])
            				: "xmm0", "xmm1", "memory"
       					);break;

    			case '*':
        			__asm__ volatile (
            				"movups (%1), %%xmm0 \n\t"
            				"movups (%2), %%xmm1 \n\t"
            				"mulps %%xmm1, %%xmm0 \n\t"
            				"movups %%xmm0, (%0) \n\t"
            				:
            				: "r"(&wynik[i]), "r"(&a[i]), "r"(&b[i])
            				: "xmm0", "xmm1", "memory"
        				);break;

    			case '/':
        			__asm__ volatile (
            				"movups (%1), %%xmm0 \n\t"
            				"movups (%2), %%xmm1 \n\t"
            				"divps %%xmm1, %%xmm0 \n\t"
            				"movups %%xmm0, (%0) \n\t"
            				:
            				: "r"(&wynik[i]), "r"(&a[i]), "r"(&b[i])
            				: "xmm0", "xmm1", "memory"
        				);break;
					}
                	}

                clock_t end = clock();
                suma[o] += (double)(end - start) / CLOCKS_PER_SEC;
            }

            suma[o] /= TESTY;
        }

        fprintf(plik, "Typ obliczen: SIMD \n");
        fprintf(plik, "Liczba liczb: %d\n", n);
        fprintf(plik, "Sredni czas [s]:\n");
        fprintf(plik, "+ %.6f\n", suma[0]);
        fprintf(plik, "- %.6f\n", suma[1]);
        fprintf(plik, "* %.6f\n", suma[2]);
        fprintf(plik, "/ %.6f\n\n", suma[3]);

        free(a);
        free(b);
        free(wynik);
    }

    fclose(plik);
    printf("Zapisano wyniki do 'wynik_SIMD.txt'\n");
    return 0;
}

