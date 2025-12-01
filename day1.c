#include <stdio.h>
#include <stdlib.h>


int main(void) {
    char *line = NULL;
    size_t len = 0;
    int zeroCount = 0;
    int zero_cross_count = 0;

    int dial = 50;

    FILE *stream = fopen("inputs/day1.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    while (getline(&line, &len, stream) != -1) {
        char dir;
        int dist;
        if (sscanf(line, "%c%d", &dir, &dist) == 2) {
            const int total_dial_positions = 100;
            const int effective_dist = dist % total_dial_positions;
            const int overflow_count = (dist - effective_dist)/total_dial_positions;

            if ('L' == dir) {
                //crossing past 0
                if (dial - effective_dist < 0 && dial != 0) {
                    zero_cross_count += 1;
                }
                dial = dial - effective_dist;
            } else {
                dial = dial + effective_dist;
                //crossing past 0
                if (dial > total_dial_positions) {
                    zero_cross_count += 1;
                }
            }
            // count 360deg rotation
            zero_cross_count += overflow_count;

            //normalization
            if (dial < 0) {
                dial = total_dial_positions - abs(dial) % total_dial_positions;
            }
            dial = dial % total_dial_positions;

            // count stopping on 0
            if (dial == 0) {
                zeroCount++;
                zero_cross_count += 1;
            }
        }
    }

    free(line);
    fclose(stream);
    printf("Final dial position: %d\n", dial);
    printf("Part 1: %d\n", zeroCount);
    printf("Part 2: %d\n", zero_cross_count);
    exit(EXIT_SUCCESS);
}
