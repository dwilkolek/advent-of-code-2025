#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


int remove_rolls(int size, char lines[140][140]) {
    int removed = 0;
    char array_new[140][140];
    memcpy(array_new, lines, sizeof(char)*140*140);

    for (int y = 1; y < size; y++) {
        for (int x = 1; x < size; x++) {
            const int up_left = array_new[y - 1][x - 1] == '@';
            const int up_mid = array_new[y - 1][x] == '@';
            const int up_right = array_new[y - 1][x + 1] == '@';

            const int bottom_left = array_new[y + 1][x - 1] == '@';
            const int bottom_mid = array_new[y + 1][x] == '@';
            const int bottom_right = array_new[y + 1][x + 1] == '@';

            const int mid_right = array_new[y][x + 1] == '@';
            const int mid_left = array_new[y][x - 1] == '@';
            const int sum = up_left + up_mid + up_right + bottom_left + bottom_mid + bottom_right + mid_right + mid_left;
            if (array_new[y][x] == '@' && sum < 4) {
                removed++;
                lines[y][x] = 'x';
            }
        }
    }
    return removed;
}

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
    // for (int y = 1; y < size; y++) {
    //     for (int x = 1; x < size; x++) {
    //         printf("%c", lines[y][x]);
    //     }
    //     printf("\n");
    // }

    const int part1 = remove_rolls(size, lines);
    int part2 = 0;
    int removed = 0;
    do {
        removed = remove_rolls(size, lines);
        part2 += removed;
    } while (removed != 0);


    printf("Part 1: %d\n", part1);
    printf("Part 2: %d\n", part2 + part1);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
