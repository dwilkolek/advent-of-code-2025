#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day3.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);
    int part1 = 0;
    while (getline(&line, &len, stream) != -1) {
        int hi = -1;
        int lo = -1;
        const int pack_size = strlen(line) - 1;
        for (int i = 0; i < pack_size; i++) {
            const int is_last = pack_size - 1 == i;
            const int battery = line[i] - '0';
            if (battery > hi && !is_last) {
                hi = battery;
                lo = -1;
                continue;
            }
            if (battery > lo) {
                lo = battery;
            }

        }
        int joltage = hi *10 + lo;
        printf("hi: %d, lo: %d = %d\n", hi, lo, joltage);
        part1 += joltage;
    }

    printf("Part 1: %d\n", part1);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
