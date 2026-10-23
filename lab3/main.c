
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define STUDENT_N 30             
#define SAMPLE_SIZE (STUDENT_N + 10)   
#define MAX_VALUE   (STUDENT_N + 1)    
#define MAX_MODES   64
#define EPS 1e-9

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Генерація випадкової послідовності*/
void generate_sample(int *a, int n, int max_value)
{
    for (int i = 0; i < n; i++)
        a[i] = rand() % max_value + 1;
}

/* Упорядкування за зростанням */
void sort_sample(const int *src, int *dst, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
    qsort(dst, (size_t)n, sizeof(int), cmp_int);
}

void print_array(const char *title, const int *a, int n)
{
    printf("%s\n", title);
    for (int i = 0; i < n; i++)
        printf("%d%s", a[i], i + 1 < n ? ", " : "\n");
}

/* Середнє арифметичне   */
double mean_value(const int *a, int n)
{
    if (n <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return sum / n;
}

/* Медіана         */
double median_value(const int *sorted, int n)
{
    if (n <= 0) return 0.0;
    if (n % 2 == 1)
        return sorted[n / 2];                                /* x_{(n+1)/2} */
    return (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;        /* (x_{n/2}+x_{n/2+1})/2 */
}
int mode_values(const int *sorted, int n, double *modes)
{
    int vals[MAX_MODES * 4], freq[MAX_MODES * 4];
    int k = 0;                             

    if (n <= 0) return 0;

    for (int i = 0; i < n; i++) {
        if (k > 0 && vals[k - 1] == sorted[i]) {
            freq[k - 1]++;
        } else {
            vals[k] = sorted[i];
            freq[k] = 1;
            k++;
        }
    }

    int fmax = 0, fmin = freq[0];
    for (int j = 0; j < k; j++) {
        if (freq[j] > fmax) fmax = freq[j];
        if (freq[j] < fmin) fmin = freq[j];
    }

    if (fmax == fmin)         
        return 0;

    int count = 0, j = 0;
    while (j < k) {
        if (freq[j] == fmax) {
            double sum = vals[j];
            int len = 1;
            while (j + len < k && freq[j + len] == fmax) {
                sum += vals[j + len];
                len++;
            }
            modes[count++] = sum / len;    
            j += len;
        } else {
            j++;
        }
    }
    return count;
}

void print_modes(const double *modes, int count)
{
    if (count == 0) {
        printf("Мода: відсутня\n");
        return;
    }
    printf("Мода (кількість мод: %d):", count);
    for (int i = 0; i < count; i++)
        printf(" Mo%d = %.2f%s", i + 1, modes[i], i + 1 < count ? ";" : "\n");
}

typedef struct {
    const char *description;
    int data[32];
    int n;
    int exp_count;          
    double exp_modes[4];    
} ModeTest;

int run_mode_tests(void)
{
    ModeTest tests[] = {
        {"Одна мода",
         {2,2,3,3,3,3,4,4,4,5,5}, 11, 1, {3}},
        {"Мода відсутня",
         {2,2,3,3,4,4,5,5}, 8, 0, {0}},
        {"Суміжні варіанти - середнє",
         {2,2,3,4,4,4,5,5,5}, 9, 1, {4.5}},
        {"Бімодальність",
         {2,2,3,3,3,4,5,5,5}, 9, 2, {3, 5}},
        {"Приклад із методички (n=20)",
         {6,7,7,7,8,8,8,8,8,9,9,9,10,10,11,11,11,11,12,13}, 20, 1, {8}},
        {"Усі значення однакові",
         {5,5,5,5}, 4, 0, {0}},
        {"Одне значення",
         {7}, 1, 0, {0}}
    };
    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    printf("\n===== Перевірка обчислення моди =====\n");
    for (int t = 0; t < total; t++) {
        int sorted[32];
        double modes[MAX_MODES];
        sort_sample(tests[t].data, sorted, tests[t].n);
        int cnt = mode_values(sorted, tests[t].n, modes);

        int ok = (cnt == tests[t].exp_count);
        for (int i = 0; ok && i < cnt; i++)
            if (fabs(modes[i] - tests[t].exp_modes[i]) > EPS) ok = 0;

        printf("Тест %d: %s | очікувано: ", t + 1, tests[t].description);
        if (tests[t].exp_count == 0) printf("відсутня");
        for (int i = 0; i < tests[t].exp_count; i++)
            printf("%.2f ", tests[t].exp_modes[i]);
        printf("| отримано: ");
        if (cnt == 0) printf("відсутня");
        for (int i = 0; i < cnt; i++)
            printf("%.2f ", modes[i]);
        printf("| %s\n", ok ? "OK" : "ПОМИЛКА");
        passed += ok;
    }
    printf("Пройдено тестів: %d з %d\n", passed, total);
    return passed == total;
}

int main(int argc, char *argv[])
{
    unsigned seed = (argc > 1) ? (unsigned)strtoul(argv[1], NULL, 10)
                               : (unsigned)time(NULL);
    srand(seed);

    int data[SAMPLE_SIZE], sorted[SAMPLE_SIZE];
    double modes[MAX_MODES];

    printf("N = %d, обсяг вибірки n = %d, значення з набору 1..%d, seed = %u\n\n",
           STUDENT_N, SAMPLE_SIZE, MAX_VALUE, seed);

    generate_sample(data, SAMPLE_SIZE, MAX_VALUE);
    sort_sample(data, sorted, SAMPLE_SIZE);

    print_array("Початкова послідовність:", data, SAMPLE_SIZE);
    print_array("Впорядкована послідовність:", sorted, SAMPLE_SIZE);
    printf("\n");

    print_modes(modes, mode_values(sorted, SAMPLE_SIZE, modes));
    printf("Медіана: Md = %.2f\n", median_value(sorted, SAMPLE_SIZE));
    printf("Середнє арифметичне: X = %.2f\n", mean_value(data, SAMPLE_SIZE));

    run_mode_tests();
    return 0;
}
