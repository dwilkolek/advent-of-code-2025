#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>

short int is_valid_pattern_of_n(char *text_number, int pattern_l) {
    int len = strlen(text_number);
    if (len % pattern_l != 0) {
        return 0;
    }
    char pattern[pattern_l];
    strncpy(pattern, text_number, pattern_l);
    const int chunks = len / pattern_l;
    // printf("pattern: %s\n", pattern);
    int is_valid = 1;
    for (int pattern_i = 0; pattern_i < chunks; pattern_i++) {
        char compare[pattern_l];
        strncpy(compare, text_number + pattern_i * pattern_l, pattern_l);
        // printf("compare: %s\n", compare);
        if (strcmp(pattern, compare) != 0) {
            is_valid = 0;
            break;
        }
    }
    return is_valid;
}

short int is_valid_pattern(char *text_number) {
    // printf("text_number: %s\n", text_number);
    int len = strlen(text_number);

    for (int pattern_l = 1; pattern_l <= len / 2; pattern_l++) {
        if (is_valid_pattern_of_n(text_number, pattern_l) == 1) return 1;
    }
    return 0;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day2.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    getline(&line, &len, stream);
    char *token;
    long long part1 = 0;
    long long part2 = 0;
    while ((token = strsep(&line, ","))) {
        long long int start;
        long long int end;
        if (sscanf(token, "%lli-%lli", &start, &end) == 2) {
            for (long long int start_p = start; start_p <= end; start_p++) {
                char pattern_as_s[1000];
                sprintf(pattern_as_s, "%lli", start_p);
                if (strlen(pattern_as_s) % 2 == 0 &&
                    is_valid_pattern_of_n(pattern_as_s, strlen(pattern_as_s) / 2) == 1) {
                    part1 = part1 + start_p;
                }
                if (is_valid_pattern(pattern_as_s) == 1) {
                    part2 = part2 + start_p;
                }
            }
        }
    }

    printf("Part 1: %lli\n", part1);
    printf("Part 2: %lli\n", part2);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
