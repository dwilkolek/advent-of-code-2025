#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day4.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    char lines[140][140];
    for (int i = 0; i < 140; i++) {
        for (int j = 0; j < 140; j++) {
            lines[i][j] = '.';
        }
    }
    int size = 1;
    while (getline(&line, &len, stream) != -1) {
        int len = strlen(line) - 1;
        for (int i = 0; i < len; i++) {
            lines[size][i + 1] = line[i];
        }
        size++;
    }
    for (int y = 1; y < size; y++) {
        for (int x = 1; x < size; x++) {
            printf("%c", lines[y][x]);
        }
        printf("\n");
    }

    printf("\n");

    printf("\n");

    printf("\n");
    int part1 = 0;
    for (int y = 1; y < size; y++) {
        for (int x = 1; x < size; x++) {
            int up_left = lines[y - 1][x - 1] == '@';
            int up_mid = lines[y - 1][x] == '@';
            int up_right = lines[y - 1][x + 1] == '@';

            int bottom_left = lines[y + 1][x - 1] == '@';
            int bottom_mid = lines[y + 1][x] == '@';
            int bottom_right = lines[y + 1][x + 1] == '@';

            int mid_right = lines[y][x + 1] == '@';
            int mid_left = lines[y][x - 1] == '@';
            int sum = up_left+ up_mid + up_right + bottom_left + bottom_mid + bottom_right + mid_right + mid_left;
            if (lines[y][x] == '@' && sum < 4) {
                part1++;
                // printf("x");
            } else {
                // printf("%c", lines[y][x]);
            }
        }
        // printf("\n");
    }

    // for (int y = 0; y <= size; y++) {
    //     for (int x = 0; x <= size; x++) {
    //         printf("%c", lines[y][x]);
    //     }
    //     printf("\n");
    // }

    printf("Part 1: %d\n", part1);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
