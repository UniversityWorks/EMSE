#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 30                
#define SIZE      (N + 10)  
#define MIN_VAL   1
#define MAX_VAL   5            
#define K         (MAX_VAL - MIN_VAL + 1)   

void bubble_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {   
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%3d", a[i]);
        if ((i + 1) % 10 == 0 || i == n - 1)
            printf("\n");
    }
}

int main(int argc, char *argv[])
{

    int data[SIZE];      
    int series[SIZE];    
    int m[K] = {0};      
    int m_cum[K];        
    double f[K];         
    double F[K];         

    /* Ініціалізація генератора БВВ */
    unsigned int seed = (argc > 1) ? (unsigned int)atoi(argv[1])
                                   : (unsigned int)time(NULL);
    srand(seed);

    /* Генерація вибірки */
    for (int i = 0; i < SIZE; i++) {
        double z = rand() / (RAND_MAX + 1.0);
        data[i] = (int)(K * z) + MIN_VAL;
    }

    /*Варіаційний ряд*/
    for (int i = 0; i < SIZE; i++)
        series[i] = data[i];
    bubble_sort(series, SIZE);

    /*Частоти m_i для кожної варіанти */
    for (int i = 0; i < SIZE; i++)
        m[series[i] - MIN_VAL]++;

    /*Частості та накопичені частоти */
    for (int i = 0; i < K; i++) {
        f[i] = (double)m[i] / SIZE;
        m_cum[i] = m[i] + (i > 0 ? m_cum[i - 1] : 0);
        F[i]     = f[i]    + (i > 0 ? F[i - 1]     : 0.0);
    }

    printf("Обсяг вибірки n = %d (N = %d), початкове значення генератора: %u\n\n",
           SIZE, N, seed);

    printf("Вихідні дані:\n");
    print_array(data, SIZE);

    printf("\nВаріаційний ряд:\n");
    print_array(series, SIZE);

    printf("\nСтатистичний розподіл:\n");
    printf("+------------------------------------------------------+\n");
    printf("| x_i | m_i |   f_i    | m_i(накоп.) | F_i (накоп.)    |\n");
    printf("+------------------------------------------------------+\n");

    int sum_m = 0;
    double sum_f = 0.0;
    for (int i = 0; i < K; i++) {
        printf("| %3d | %3d | %8.4f | %11d | %15.4f |\n",
               i + MIN_VAL, m[i], f[i], m_cum[i], F[i]);
        sum_m += m[i];
        sum_f += f[i];
    }
    printf("+------------------------------------------------------+\n");
    printf("| Сума| %3d | %8.4f |             |                 |\n", sum_m, sum_f);
    printf("+------------------------------------------------------+\n");

    printf("\nПеревірка: сума m_i = %d (має дорівнювати n = %d), "
           "сума f_i = %.4f (має дорівнювати 1)\n", sum_m, SIZE, sum_f);

    return 0;
}
