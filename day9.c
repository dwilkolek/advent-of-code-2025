#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>

int LIMIT = 500;

typedef struct Tile {
    long x, y;
} Tile;

typedef struct P2Area {
    struct P2Area *next;
    long long area;
    Tile *a;
    Tile *b;
} P2Area;

long long int max(const long long int a, const long long int b) {
    return a > b ? a : b;
}

long long int min(const long long int a, const long long int b) {
    return a < b ? a : b;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day9.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    Tile *tiles[LIMIT];
    for (int i = 0; i < LIMIT; i++) {
        tiles[i] = NULL;
    }
    int tileCount = 0;

    while (getline(&line, &len, stream) != -1) {
        long x, y = 0;
        sscanf(line, "%ld,%ld", &x, &y);
        tiles[tileCount] = malloc(sizeof(Tile));
        tiles[tileCount]->x = x;
        tiles[tileCount]->y = y;
        tileCount++;
    }

    long long part1 = 0;
    for (int i = 0; i < tileCount; i++) {
        for (int j = 0; j < tileCount; j++) {
            long long area = ((long) abs(tiles[i]->x - tiles[j]->x) + 1) * ((long) abs(tiles[i]->y - tiles[j]->y) + 1);
            if (area > part1) {
                part1 = area;
            }
        }
    }

    printf("Part 1: %lld\n", part1);


    P2Area *head = NULL;
    for (int i = 0; i < tileCount; i++) {
        for (int j = i + 1; j < tileCount; j++) {
            long long area = ((long) abs(tiles[i]->x - tiles[j]->x) + 1) * ((long) abs(tiles[i]->y - tiles[j]->y) + 1);
            P2Area *new_area = malloc(sizeof(P2Area));
            new_area->area = area;
            new_area->a = tiles[i];
            new_area->b = tiles[j];

            if (head == NULL) {
                head = new_area;
            } else {
                P2Area *curr = head;
                P2Area *prev = NULL;
                bool found = false;
                while (curr != NULL) {
                    if (area > curr->area) {
                        found = true;
                        if (prev != NULL) {
                            prev->next = new_area;
                        } else {
                            head = new_area;
                        }
                        new_area->next = curr;
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
                }
                if (!found) {
                    prev->next = new_area;
                }
            }
        }
    }

    P2Area *curr = head;
    bool found = false;
    while (curr != NULL && !found) {
        found = true;
        // printf("TEST: %d,%d %d,%d =%lld\n", curr->a->x, curr->a->y, curr->b->x, curr->b->y, curr->area);
        for (int t = 0; t < tileCount; t++) {
            Tile *ca = tiles[t];
            Tile *cb = tiles[(t + 1) % tileCount];

            double midx = (ca->x + cb->x) / 2.0;
            double midy = (ca->y + cb->y) / 2.0;

            if (midx > min(curr->a->x, curr->b->x) && midx < max(curr->a->x, curr->b->x) && midy >
                min(curr->a->y, curr->b->y) && midy < max(curr->a->y, curr->b->y)) {
                curr = curr->next;
                found = false;
                break;
            }
        }
    }

    printf("Part 2: %lld\n", curr->area);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
