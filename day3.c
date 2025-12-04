#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int probe(int battery, int position, int *pack, int pack_size) {
    if (pack[position] < battery) {
        pack[position] = battery;
        for (int i = position + 1; i < pack_size; i++) {
            pack[i] = -1;
        }
        return 1;
    }

    return 0;
}

unsigned long long int find_joultage(int powered_size, char* line) {
    int powered[powered_size];
    for (int i = 0; i < powered_size; i++) {
        powered[i] = -1;
    }

    const int bank_size = strlen(line) - 1;
    for (int i = 0; i < bank_size; i++) {
        const int battery = line[i] - '0';
        for (int j = 0; j < powered_size; j++) {
            int worth_continue = bank_size - i >= powered_size - j;
            if (worth_continue == 1 && probe(battery, j, powered, powered_size) == 1) {
                break;
            }
        }
    }
    unsigned long long int joltage = 0;
    for (int i = 0; i < powered_size; i++) {
        const long long int base = pow(10.0, powered_size - i - 1);
        joltage += powered[i] * base;
    }
    return joltage;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day3.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);
    unsigned long long int part1 = 0;
    unsigned long long int part2 = 0;
    while (getline(&line, &len, stream) != -1) {
        part1 += find_joultage(2, line);
        part2 += find_joultage(12, line);
    }

    printf("Part 1: %llu\n", part1);
    printf("Part 2: %llu\n", part2);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
