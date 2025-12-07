#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;
    int part1 = 0;

    FILE *stream = fopen("inputs/day7.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    const int LIMIT = 160;

    long long int plan[LIMIT][LIMIT];


    int line_no = 2;
    const int start_int = 1;
    const int dash_int = -5;
    const int empty_int = 0;

    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            plan[i][j] = empty_int;
        }
    }

    while (getline(&line, &len, stream) != -1) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\n') {
                continue;
            }
            int ch_int = empty_int;
            if (line[i] == 'S') {
                ch_int = start_int;
            } else if (line[i] == '^') {
                ch_int = dash_int;
            }
            plan[line_no][i + 2] = ch_int;
        }
        line_no++;
    }


    for (int row = 0; row < LIMIT; row++) {
        for (int i = 0; i < LIMIT - 1; i++) {
            long long int ch = plan[row][i];
            long long int below = plan[row + 1][i];
            if (below >= empty_int && ch >= empty_int) {
                plan[row + 1][i] = plan[row + 1][i] + ch;
            }
            if (ch == start_int || ch > empty_int) {
                if (below == dash_int) {
                    plan[row + 1][i - 1] = plan[row + 1][i - 1] + ch;
                    plan[row + 1][i + 1] = plan[row + 1][i + 1] + ch;
                    part1++;
                }
            }
        }
    }
    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            printf("\t%lli", plan[i][j]);
        }
        printf("\n");
    }
    long long int part2 = 0;
    for (int i = 0; i < LIMIT; i++) {
        if (plan[LIMIT - 1][i] > 0) {
            part2 += plan[LIMIT - 1][i];
        }
    }

    printf("Part 1: %d\n", part1);
    printf("Part 2: %lli\n", part2);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
