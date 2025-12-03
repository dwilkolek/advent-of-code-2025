#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day2.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    getline(&line, &len, stream);
    char *token;
    long long sum = 0;
    while ((token = strsep(&line, ","))) {
        long long int start;
        long long int end;
        // 26255179562
        if (sscanf(token, "%lli-%lli", &start, &end) == 2) {
            // 3395595155
            printf("%s\n", token);
            char start_as_s[1000];
            char end_as_s[1000];
            sprintf(start_as_s, "%li", start);
            sprintf(end_as_s, "%li", end);
            int power_start = strlen(start_as_s) / 2;
            int power_end = strlen(end_as_s) / 2;

            for (int power_base_i = power_start; power_base_i <= power_end; power_base_i++) {
                if (power_base_i == 0) continue;
                const int power_base = pow(10, power_base_i);
                // printf("power base: %d\n", power_base);

                for (long long int start_p = start / power_base; start_p <= end / power_base; start_p++) {
                    const long long pattern_number = start_p * power_base + start_p;
                    char pattern_as_s[1000];
                    sprintf(pattern_as_s, "%lli", pattern_number);
                    if (pattern_number >= start && pattern_number <= end && strlen(pattern_as_s) % 2 == 0) {
                        sum = sum + pattern_number;
                        printf("\t pattern: %lli\n", pattern_number);
                    }
                }
            }

            //     const int str_len = strlen(str);
            //     for (int i = start; i <= end; i++) {
            //         sprintf(str, "%d", i);
            //         const int str_len = strlen(str);
            //         if (str_len % 2 == 0) {
            //             const int half_size = str_len/2;
            //             int p = 0;
            //             for ( p=0; p < half_size; p++) {
            //                 if (str[p] != str[p + half_size]) {
            //                     break;
            //                 }
            //             }
            //
            //             if (p == half_size) {
            //                 sum += i;
            //             }
            //         }
            //
            //     }
        }
    }

    printf("Part 1: %lli\n", sum);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}

int countDigits(int n) {
    int count = 0;
    while (n != 0) {
        n /= 10; // Reduce the number by a factor of ten
        count++; // Increment count for each reduction
    }
    return count;
}
