#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <_string.h>


const int BOX_LIMIT = 1200;
const int CONN_LIMIT = 1000;


typedef struct Box {
    int id;
    int x, y, z;
    int circut_id;
} Box;

typedef struct Connection {
    Box *from;
    Box *to;
    double distance;
} Connection;


int CIRCUT_ID = 0;
int TOTAL_CIRCUTS = 0;


bool connections[BOX_LIMIT][BOX_LIMIT];

Box *boxes[BOX_LIMIT];
int box_count = 0;

long double distances[BOX_LIMIT][BOX_LIMIT];

long double calc3dDist(Box *box1, Box *box2) {
    if (distances[box1->id][box2->id] != DBL_MAX) {
        return distances[box1->id][box2->id];
    }
    long double dist = pow(box1->x - box2->x, 2) + pow(box1->y - box2->y, 2) + pow(box1->z - box2->z, 2);
    if (dist < 0) {
        printf("Negative distance: %Lf\n", dist);
        exit(EXIT_FAILURE);
    }
    distances[box1->id][box2->id] = sqrt(dist);
    distances[box2->id][box1->id] = sqrt(dist);
    return distances[box1->id][box2->id];
}

Connection *find_closest() {
    Connection *closest = malloc(sizeof(Connection));
    closest->from = NULL;
    closest->to = NULL;
    closest->distance = DBL_MAX;

    for (int i = 0; i < BOX_LIMIT; i++) {
        Box *from = boxes[i];
        if (from == NULL) continue;


        // if (from->connected_box != NULL) continue;
        for (int j = i; j < BOX_LIMIT; j++) {
            Box *to = boxes[j];
            if (to == NULL) continue;
            if (from == to) continue;
            if (connections[from->id][to->id]) continue;
            // if (connections[to->id][from->id]) continue;
            // if (to->circut_id == from->circut_id) continue;

            long double calc_dist = calc3dDist(from, to);
            // printf("Dist: %f\n", calc_dist);
            if (calc_dist < closest->distance) {
                closest->distance = calc_dist;
                closest->from = from;
                closest->to = to;
            }
        }
    }
    if (closest->to == NULL) {
        return NULL;
    }
    return closest;
}

// 162,817,812 and 425,690,689
// 162,817,812 and 431,825,988


void connect_boxes(Box *from, Box *to) {
    //move all `from` to `to`
    const int from_circut_id = from->circut_id;
    const int to_circut_id = to->circut_id;
    if (from_circut_id != to_circut_id) {
        TOTAL_CIRCUTS--;
    }
    const int new_circut_id = from_circut_id;
    for (int i = 0; i < BOX_LIMIT; i++) {
        Box *box = boxes[i];
        if (box == NULL) continue;
        if (box->circut_id == from_circut_id || box->circut_id == to_circut_id) {
            box->circut_id = new_circut_id;
        }
    }
    from->circut_id = new_circut_id;
    to->circut_id = new_circut_id;

    connections[to->id][from->id] = true;
    connections[from->id][to->id] = true;
}

void reset() {
    for (int i = 0; i < BOX_LIMIT; i++) {
        boxes[i] = NULL;
    }
    for (int i = 0; i < BOX_LIMIT; i++) {
        for (int j = 0; j < BOX_LIMIT; j++) {
            connections[i][j] = false;
            distances[i][j] = DBL_MAX;
        }
    }
}

Connection *connect() {
    int conn_count = 0;
    Connection *conn = NULL;
    do {
        conn = find_closest();
        if (conn == NULL) {
            printf("No more connections found\n");
            return conn;
        }
        conn_count++;
        connect_boxes(conn->from, conn->to);

        if (TOTAL_CIRCUTS == 1) {
            return conn;
        }
    } while (conn_count < CONN_LIMIT);

    return conn;
}

int main(void) {
    reset();
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day8.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    while (getline(&line, &len, stream) != -1) {
        Box *box = malloc(sizeof(Box));
        sscanf(line, "%d,%d,%d", &box->x, &box->y, &box->z);
        box->id = box_count;
        box->circut_id = CIRCUT_ID++;

        boxes[box_count] = box;
        box_count++;
    }
    TOTAL_CIRCUTS = CIRCUT_ID;

    // PART 1
    Connection *last = connect();

    int circut_ids[BOX_LIMIT];
    for (int i = 0; i < BOX_LIMIT; i++) {
        circut_ids[i] = 0;
    }

    for (int i = 0; i < BOX_LIMIT; i++) {
        if (boxes[i] != NULL) {
            circut_ids[boxes[i]->circut_id]++;
        }
    }

    int largest_circuts[3] = {0, 0, 0};
    for (int i = 0; i < BOX_LIMIT; i++) {
        if (circut_ids[i] != 0) {
            if (circut_ids[i] > largest_circuts[0]) {
                largest_circuts[2] = largest_circuts[1];
                largest_circuts[1] = largest_circuts[0];
                largest_circuts[0] = circut_ids[i];
            } else if (circut_ids[i] > largest_circuts[1]) {
                largest_circuts[2] = largest_circuts[1];
                largest_circuts[1] = circut_ids[i];
            } else if (circut_ids[i] > largest_circuts[2]) {
                largest_circuts[2] = circut_ids[i];
            }
        }
    }

    int part1 = largest_circuts[0] * largest_circuts[1] * largest_circuts[2];

    printf("Part 1: %d %d %d = %d\n", largest_circuts[0], largest_circuts[1], largest_circuts[2], part1);


    while (TOTAL_CIRCUTS != 1) {
        last = connect();
    }

    printf("Part 2: %d * %d = %d\n", last->from->x, last->to->x, last->from->x * last->to->x);


    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
