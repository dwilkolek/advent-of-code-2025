#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
regex_t regex;

const int BTN_COUNT = 15;

void print_bits(unsigned int x) {
    int i;
    for (i = 8 * sizeof(x) - 1; i >= 0; i--) {
        (x & (1 << i)) ? putchar('1') : putchar('0');
    }
    printf("\n");
}

typedef struct Button {
    int lts[10];
    int pressed_count;
} Button;


typedef struct PC {
    int state;
    int end_setup;
    int buttons[BTN_COUNT];
    int total_buttons;
    int last_button_pressed;
    int total_button_pressed;
} PC;

PC *pcs[200];
int pc_count = 0;

typedef struct Node {
    PC *pc;
    struct Node *next;
} Node;

const int HASH_LIMIT = 100000;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    int hashes[HASH_LIMIT];
    int hash_count;
} LinkedList;

Node *pop(LinkedList *list) {
    if (list->head == NULL) {
        return NULL;
    }

    Node *temp = list->head;
    list->head = list->head->next;

    if (list->head == NULL) {
        list->tail = NULL;
    }

    return temp;
}

int state_to_hash(const int state[10]) {
    int hash = 0;
    for (int i = 0; i < 10; i++) {
        hash ^= state[i] << i;
    }
    return hash;
}

void append(LinkedList *list, PC *pc) {
    for (int i = 0; i < list->hash_count; i++) {
        if (list->hashes[i] == pc->state) {
            free(pc);
            return;
        }
    }
    if (list->hash_count < HASH_LIMIT) {
        list->hashes[list->hash_count++] = pc->state;
    }

    Node *node = malloc(sizeof(Node));
    node->pc = pc;
    node->next = NULL;


    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }

    list->tail->next = node;
    list->tail = node;
}

void btn_click(PC *pc, int btn_i) {
    pc->last_button_pressed = btn_i;
    pc->total_button_pressed++;
    pc->state ^= pc->buttons[btn_i];
    // for (int i = 0; i < 10; i++) {
    //     int light = btn->lts[i];
    //     if (light == -1) break;
    //     pc->state[light] = !pc->state[light];
    // }
}

void light(const Node *node, LinkedList *list) {
    const PC *pc = node->pc;
    for (int i = 0; i < pc->total_buttons; i++) {
        PC *new_pc = malloc(sizeof(PC));
        memcpy(new_pc, pc, sizeof(PC));
        btn_click(new_pc, i);
        append(list, new_pc);
    }
}

int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day10.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    while (getline(&line, &len, stream) != -1) {
        int light_start = 0;
        int light_end = 0;
        PC *pc = malloc(sizeof(PC));
        pc->state = 0;
        pc->end_setup = 0;
        pc->last_button_pressed = -1;
        pc->total_button_pressed = 0;

        for (int i = 0; i < strlen(line); i++) {
            if ('[' == line[i]) {
                light_start = i + 1;
            }
            if (']' == line[i]) {
                light_end = i - 1;
            }
        }

        int c[10];
        for (int i=0; i<10; i++) {
            c[i] = 0;
        }
        for (int i = light_end; i >= light_start; i--) {
            if (line[i] == '#') {
                c[i - light_start] = 1;
            }
        }

        pc->end_setup = state_to_hash(c);


        const char *tok = strtok(line, " (");
        bool f = true;
        int bti = 0;

        while (tok != NULL) {
            if (f) {
                f = false;
                tok = strtok(NULL, " (");
                continue;
            }
            if (strstr(tok, "{") != NULL) {
                tok = strtok(NULL, " (");
                continue;
            }

            regmatch_t pmatch[10];
            regcomp(&regex, "([0-9]+)", REG_ICASE | REG_EXTENDED);
            Button *btn = malloc(sizeof(Button));
            btn->pressed_count = 0;
            for (int i = 0; i < 10; i++) {
                btn->lts[i] = -1;
            }

            regcomp(&regex, "([0-9]+)", REG_EXTENDED);
            int offset = 0;
            int g = 0;
            int b = 0;
            while (regexec(&regex, tok + offset, 1, pmatch, 0) == 0) {
                char sourceCopy[strlen(tok) + 1];
                strcpy(sourceCopy, tok + offset);
                sourceCopy[pmatch[0].rm_eo] = 0;

                btn->lts[g++] = atoi(sourceCopy + pmatch[0].rm_so);

                offset += pmatch[0].rm_eo;
                b ^= 1 << atoi(sourceCopy + pmatch[0].rm_so);
                if (g >= BTN_COUNT) break;
            }
            regfree(&regex);
            tok = strtok(NULL, " (");
            pc->buttons[bti] = b;
            bti++;
        }

        pc->total_buttons = bti;

        pcs[pc_count++] = pc;
    }
    int part1 = 0;
    for (int i = 0; i < pc_count; i++) {
        LinkedList *list = malloc(sizeof(LinkedList));
        list->head = NULL;
        list->tail = NULL;
        list->hash_count = 0;
        append(list, pcs[i]);
        Node *node = pop(list);
        printf("%d of %d Expecting state:", i, pc_count);
        print_bits(pcs[i]->end_setup);
        while (node != NULL) {
            if (node->pc->state == node->pc->end_setup) {
                printf("Machine %d VALID with %d presses\n", i, node->pc->total_button_pressed);
                part1 += node->pc->total_button_pressed;
                break;
            }
            light(node, list);

            free(node->pc);
            free(node);
            node = pop(list);
        }
        free(list);
    }

    printf("Part 1: %d\n", part1);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
