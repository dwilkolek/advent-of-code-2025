#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
regex_t regex;

const int BTN_COUNT = 15;
const int OVER = 100;
const int ALL_MATCH = 1000;
const int INVALID = 69;
const int DONE = 13112;

typedef struct Button {
    int lts[10];
    int lts_count;
} Button;

typedef struct PC {
    Button *buttons[BTN_COUNT];
    int total_buttons;
    int button_muls[BTN_COUNT];
    int target_counters[BTN_COUNT];
    int clicks;
} PC;

typedef struct State {
    int record;
} State;

PC *pcs[200];
int pc_count = 0;

int check_counter(PC *pc) {
    int counter[BTN_COUNT];
    for (int i = 0; i < BTN_COUNT; i++) {
        counter[i] = 0;
    }
    for (int i = 0; i < pc->total_buttons; i++) {
        Button *btn = pc->buttons[i];
        for (int j = 0; j < btn->lts_count; j++) {
            counter[btn->lts[j]] += pc->button_muls[i];
        }
    }
    bool all_match = true;
    for (int i = 0; i < BTN_COUNT; i++) {
        if (counter[i] != pc->target_counters[i]) all_match = false;
        if (counter[i] > pc->target_counters[i]) return OVER;
    }

    return all_match ? ALL_MATCH : INVALID;
}

bool inc_mul(PC *pc, int btn_i, int limit, bool over) {
    if (btn_i >= pc->total_buttons) return false;
    if (pc->button_muls[btn_i] >= limit) {
        pc->button_muls[btn_i] = 0;
        return inc_mul(pc, btn_i + 1, limit, over);
    } else {
        if (over) {
            pc->button_muls[btn_i] = limit;
            return inc_mul(pc, btn_i, limit, false);
        }
        pc->button_muls[btn_i]++;
    }
    return true;
}

int count_clicks(PC *pc) {
    int c = 0;
    for (int i = 0; i < pc->total_buttons; i++) {
        c += pc->button_muls[i];
    }
    return c;
}

int progress(PC *pc, int limit, int record) {
    while (true) {
        int result = check_counter(pc);
        if (result == ALL_MATCH) {
            int clicks = count_clicks(pc);
            printf("clicks: %d\n", clicks);
            if (clicks < record) record = clicks;
        }
        if (!inc_mul(pc, 0, limit, false)) {
            break;
        }
    }
    return record;
}


int solve(PC *pc) {
    int max = 0;
    for (int i = 0; i < BTN_COUNT; i++) {
        if (pc->target_counters[i] > max) max = pc->target_counters[i];
    }
    return progress(pc, max, INT32_MAX);
}


int main(void) {
    char *line = NULL;
    size_t len = 0;

    FILE *stream = fopen("inputs/day10.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    while (getline(&line, &len, stream) != -1) {
        PC *pc = malloc(sizeof(PC));
        pc->total_buttons = 0;
        pc->clicks = 0;
        for (int i = 0; i < BTN_COUNT; i++) {
            pc->buttons[i] = NULL;
            pc->target_counters[i] = 0;
            pc->button_muls[i] = 0;
        }

        const char *tok = strtok(line, " (");
        bool f = true;
        int bti = 0;

        while (tok != NULL) {
            if (f) {
                f = false;
                tok = strtok(NULL, " (");
                continue;
            }
            bool compute_target = strstr(tok, "{") != NULL;

            regmatch_t pmatch[10];
            regcomp(&regex, "([0-9]+)", REG_EXTENDED);
            int offset = 0;
            int g = 0;
            if (!compute_target) {
                Button *btn = malloc(sizeof(Button));
                for (int i = 0; i < 10; i++) {
                    btn->lts[i] = -1;
                }

                while (regexec(&regex, tok + offset, 1, pmatch, 0) == 0) {
                    char sourceCopy[strlen(tok) + 1];
                    strcpy(sourceCopy, tok + offset);
                    sourceCopy[pmatch[0].rm_eo] = 0;

                    btn->lts[g++] = atoi(sourceCopy + pmatch[0].rm_so);

                    offset += pmatch[0].rm_eo;
                    if (g >= BTN_COUNT) break;
                }
                regfree(&regex);
                tok = strtok(NULL, " (");
                btn->lts_count = g;
                pc->buttons[bti] = btn;

                bti++;
            } else {
                while (regexec(&regex, tok + offset, 1, pmatch, 0) == 0) {
                    char sourceCopy[strlen(tok) + 1];
                    strcpy(sourceCopy, tok + offset);
                    sourceCopy[pmatch[0].rm_eo] = 0;

                    pc->target_counters[g++] = atoi(sourceCopy + pmatch[0].rm_so);

                    offset += pmatch[0].rm_eo;
                    if (g >= BTN_COUNT) break;
                }
                regfree(&regex);
                tok = strtok(NULL, " (");
            }
        }

        pc->total_buttons = bti;

        pcs[pc_count++] = pc;
    }
    int part2 = 0;
    for (int i = 0; i < pc_count; i++) {
        int pcres = solve(pcs[i]);
        printf("pc %d: %d\n", i, pcres);
        part2 += pcres;
    }
    printf("Part 2: %d\n", part2);
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
