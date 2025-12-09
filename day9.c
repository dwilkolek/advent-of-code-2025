#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>

int LIMIT = 500;

typedef struct Tile {
    long x, y;
} Tile;

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day9.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    Tile tiles[LIMIT];
    int tileCount = 0;

    while (getline(&line, &len, stream) != -1) {
        long x, y = 0;
        sscanf(line, "%ld,%ld", &x, &y);
        tiles[tileCount++] = (Tile){x, y};
    }

    long long max = 0;
    for (int i = 0; i < tileCount; i++) {
        for (int j = 0; j < tileCount; j++) {
            long long area = ((long)abs(tiles[i].x - tiles[j].x) + 1) * ((long)abs(tiles[i].y - tiles[j].y) + 1);
            if (area > max) {
                max = area;
            }
        }
    }

    for (int i = 0; i < tileCount; i++) {
        for (int j = 0; j < tileCount; j++) {
            printf("")   ;
        }
    }
    // 18377,86172  97008,58862
    printf("Part 1: %lld\n", max);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
