#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day6.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    char worksheet[6][1005][1000];
    char ops[1005];
    int row = 0;
    int token_count = 0;
    int offsets[1005];
    for (int i = 0; i < 1005; i++) {
        offsets[i] = -1;
    }

    while (getline(&line, &len, stream) != -1) {
        if ('*' == line[0] || '+' == line[0]) {
            int word_i = 0;
            for (int i = 0; i < strlen(line); i++) {
                printf("char[%d]: %c = %d\n", i, line[i], (int)line[i]);

                if ('*' == line[i] || '+' == line[i]) {
                    if (word_i == 0) {
                        offsets[word_i++] = i;
                    } else {
                        offsets[word_i] = i;
                        word_i++;
                    }
                    token_count++;
                }
            }

        }
    }

    rewind(stream);
    while (getline(&line, &len, stream) != -1) {
        // char *copy = strdup(line);
        bool ops_line = false;
        for (int word_i = 0; word_i < token_count; word_i++) {
            if (line[offsets[word_i]] == '*' || line[offsets[word_i]] == '+') {
                ops_line = true;
                ops[word_i] = line[offsets[word_i]];
            } else {
                int buf_size = offsets[word_i + 1];
                if (offsets[word_i + 1] == -1) {
                    buf_size = strlen(line) - offsets[word_i] - 1;
                } else {
                    buf_size = offsets[word_i + 1] - offsets[word_i] - 1;
                }
                strncpy(worksheet[row][word_i], line + offsets[word_i], buf_size);
            }
            printf("%s\n", worksheet[row][word_i]);
        }
        row++;
        if (ops_line) {
            break;
        }
    }
    long long int part1 = 0;
    for (int i = 0; i < token_count; i++) {
        char operator = ops[i];
        long long int value = atoi(worksheet[0][i]);

        printf("%lli", value);
        for (int j = 1; j < row - 1; j++) {
            int n_v = atoi(worksheet[j][i]);
            if (operator == '*') {
                value *= n_v;
                printf(" * %d", n_v);
            } else {
                value += n_v;
                printf(" + %d", n_v);
            }
        }
        part1 += value;
        printf("= %lli => %lli\n", value, part1);
    }

    // long long int part2 = 0;
    // for (int i = 0; i < token_count; i++) {
    //     char operator = ops[i];
    //
    //     int max_len = 0;
    //     for (int j = 0; j < row - 1; j++) {
    //         if (max_len < strlen(worksheet[j][i])) {
    //             max_len = strlen(worksheet[j][i]);
    //         }
    //     }
    //     long long int value = -1;
    //     for (int k = 0; k < max_len; k++) {
    //         char new_value[1000] = "";
    //         for (int j = 0; j < row - 1; j++) {
    //             const int old_wrks_value_len = strlen(worksheet[j][i]);
    //             if (k < strlen(worksheet[j][i])) {
    //                 const int new_value_len = strlen(new_value);
    //                 new_value[new_value_len] = worksheet[j][i][old_wrks_value_len - k - 1];
    //                 new_value[new_value_len + 1] = '\0';
    //             }
    //         }
    //
    //
    //         if (value == -1) {
    //             value = atoi(new_value);
    //             printf("%s", new_value);
    //         } else {
    //             if (operator == '*') {
    //                 value *= atoi(new_value);
    //                 printf(" * %d", atoi(new_value));
    //             } else {
    //                 value += atoi(new_value);
    //                 printf(" + %d", atoi(new_value));
    //             }
    //         }
    //     }
    //
    //
    //     printf(" = %lli\n", value);
    //     part2 += value;
    // }

    printf("Part 1: %lli\n", part1);
    // printf("Part 2: %lli\n", part2);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
