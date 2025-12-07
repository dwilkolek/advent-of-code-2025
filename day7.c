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

    const int LIMIT = 150;

    char plan[LIMIT][LIMIT];
    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            plan[i][j] = '.';
        }
    }

    int line_no = 2;
    int start_x = 0;
    int start_y = 0;

    while (getline(&line, &len, stream) != -1) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\n') {
                continue;
            }
            if (line[i] == 'S') {
                start_x = i;
                start_y = line_no;
            }
            plan[line_no][i + 2] = line[i];
        }
        line_no++;
    }

    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            printf("%c", plan[i][j]);
        }
        printf("\n");
    }


    for (int row = 0; row < LIMIT; row++) {
        for (int i = 0; i < LIMIT - 1; i++) {
            char ch = plan[row][i];
            char below = plan[row + 1][i];
            if (ch == 'S' || ch == '|') {
                if (below == '^') {
                    plan[row + 1][i - 1] = '|';
                    plan[row + 1][i + 1] = '|';
                    part1++;
                } else if (below == '.') {
                    plan[row + 1][i] = '|';
                }
            }
        }
    }
    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            printf("%c", plan[i][j]);
        }
        printf("\n");
    }

    printf("Part 1: %d\n", part1);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
