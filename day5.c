#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>

long long int max(const long long int a, const long long int b) {
    return a > b ? a : b;
}

long long int min(const long long int a, const long long int b) {
    return a < b ? a : b;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day5.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);
    int read_mode = 0;
    long long int ranges[1000][3];
    int ranges_i = 0;
    long long int ingredients[2000];
    int ingredients_i = 0;
    while (getline(&line, &len, stream) != -1) {
        if (strcmp(line, "\n") == 0) {
            read_mode = 1;
            continue;
        }
        if (read_mode == 0) {
            //3-5
            long long int from, to;
            sscanf(line, "%lli-%lli", &from, &to);
            ranges[ranges_i][0] = from;
            ranges[ranges_i][1] = to;
            ranges[ranges_i][2] = 1;
            ranges_i++;
        } else {
            long long int ingredient;
            sscanf(line, "%lli", &ingredient);
            ingredients[ingredients_i++] = ingredient;
        }
    }

    int part1 = 0;
    for (int i = 0; i < ingredients_i; i++) {
        for (int j = 0; j < ranges_i; j++) {
            if (ingredients[i] >= ranges[j][0] && ingredients[i] <= ranges[j][1]) {
                part1++;
                break;
            }
        }
    }

    bool overlapped = true;
    while (overlapped) {
        overlapped = false;
        for (int i = 0; i < ranges_i; i++) {
            if (ranges[i][2] == 0) continue;
            for (int j = 0; j < ranges_i; j++) {
                if (ranges[j][2] == 0 || i == j) continue;

                const long long int maximus = max(ranges[i][1], ranges[j][1]);
                const long long int minimum = min(ranges[i][0], ranges[j][0]);
                const long long int size = maximus - minimum;
                if (size <= ranges[i][1] - ranges[i][0] + (ranges[j][1] - ranges[j][0])) {
                    ranges[i][0] = minimum;
                    ranges[i][1] = maximus;

                    ranges[j][2] = 0;
                    overlapped = true;
                }
            }
        }
    }
    long long int part2 = 0;
    for (int i = 0; i < ranges_i; i++) {
        if (ranges[i][2] == 1) {
            part2 += ranges[i][1] - ranges[i][0] + 1;
        }
    }

    printf("Part 1: %d\n", part1);
    printf("Part 2: %lli\n", part2);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
