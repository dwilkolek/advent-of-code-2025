#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
const int TO_LIMIT = 25;
const int NODE_LIMIT = 650;
const int ROUTE_LIMIT = 3000000;

typedef struct Node {
    char *name;
    struct Node *tos[TO_LIMIT];
    struct Node *froms[TO_LIMIT];
    int to_count;
    int from_count;
} Node;




char *routes[ROUTE_LIMIT];
int route_count = 0;


typedef struct Visitor {
    Node *node;
    Node *visited_nodes[NODE_LIMIT];
    int step_count;
    bool seen_dac;
    bool seen_fft;
} Visitor;


bool seen(Visitor *visitor, Node *to) {
    char *route_hash = malloc(sizeof(char) * (visitor->step_count + 1) * 5);
    for (int i = 0; i < visitor->step_count; i++) {
        strcat(route_hash, "-");
        strcat(route_hash, visitor->visited_nodes[i]->name);
    }
    strcat(route_hash, to->name);
    for (int r = 0; r < route_count; r++) {
        if (strcmp(routes[r], route_hash) == 0) {
            return true;
        }
    }

    routes[route_count++] = strdup(route_hash);
    for (int i = 0; i < visitor->step_count; i++) {
        if (visitor->visited_nodes[i] == to) {
            return true;
        }
    }
    return false;
}

typedef struct LinkedListNode {
    struct LinkedListNode *next;
    Visitor *visitor;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head;
    LinkedListNode *tail;
    int size;
} LinkedList;


Visitor *pop(LinkedList *list) {
    if (list->head == NULL) {
        return NULL;
    }

    LinkedListNode *temp = list->head;
    list->head = list->head->next;

    if (list->head == NULL) {
        list->tail = NULL;
    }
    list->size--;
    return temp->visitor;
}

void prepend(LinkedList *list, Visitor *visitor) {
    LinkedListNode *node = malloc(sizeof(LinkedListNode));
    node->visitor = visitor;
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }
    node->next = list->head;
    list->head = node;
}

void append(LinkedList *list, Visitor *visitor) {
    LinkedListNode *node = malloc(sizeof(LinkedListNode));
    node->visitor = visitor;
    node->next = NULL;
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }

    list->tail->next = node;
    list->tail = node;
}


Node *nodes[NODE_LIMIT];
int node_count = 0;

Node *find_node(char *name) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(nodes[i]->name, name) == 0) {
            return nodes[i];
        }
    }
    return NULL;
}

Node *find_or_create_node(char *name) {
    Node *node = find_node(name);
    if (node == NULL) {
        node = malloc(sizeof(Node));
        node->name = strdup(name);
        node->to_count = 0;
        nodes[node_count++] = node;
    }
    return node;
}
const int CACHE_LIMIT = 1000000;
char *cache_keys[CACHE_LIMIT];
long long int cache_values[CACHE_LIMIT];
int cache_count = 0;

long long int visit(Node *node, bool dac, bool fft) {
    char *key = malloc(sizeof(char) * 100);
    sprintf(key, "%s-%d-%d", node->name, dac, fft);
    for (int r = 0; r < cache_count; r++) {
        if (strcmp(cache_keys[r], key) == 0) {
            printf("Cache hit %s: %lld\n", key, cache_values[r]);
            return cache_values[r];
        }
    }
    if (strcmp(node->name, "out") == 0) {
        if (dac && fft) {
            return 1;
        }
        return 0;
    }
    long long int out = 0;
    for (int i = 0; i < node->to_count; i++) {
        Node *to_node = node->tos[i];

        if (strcmp(to_node->name, "dac") == 0) {
            out += visit(to_node, true, fft);
        } else if (strcmp(to_node->name, "fft") == 0) {
            out += visit(to_node, dac, true);
        } else {
            out += visit(to_node, dac, fft);
        }
    }
    cache_keys[cache_count] = strdup(key);
    cache_values[cache_count] = out;

    cache_count++;


    return out;


}


int main(void) {
    char *line = NULL;
    size_t len = 0;

    for (int i = 0; i < CACHE_LIMIT; i++) {
        cache_keys[i] = NULL;
        cache_values[i] = 0;
    }

    FILE *stream = fopen("inputs/day11.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);


    while (getline(&line, &len, stream) != -1) {
        char *tok = strtok(line, " ");
        Node *node = NULL;
        bool first = true;
        while (tok != NULL) {
            char *name = strdup(tok);
            if (first) {
                name[strlen(name) - 1] = 0;
                first = false;
                node = find_or_create_node(name);
            } else {
                if (name[strlen(name) - 1] == '\n') {
                    name[strlen(name) - 1] = 0;
                }
                Node *new_to = find_or_create_node(name);
                node->tos[node->to_count++] = new_to;
            }
            tok = strtok(NULL, " ");
        }
    }

    for (int i = 0; i < node_count; i++) {
        Node *end_node = nodes[i];
        for (int j = 0; j < end_node->to_count; j++) {
            Node *to_node = end_node->tos[j];
            to_node->froms[to_node->from_count++] = end_node;
        }
    }

    Node *start = find_node("svr");
    Visitor *visitor = malloc(sizeof(Visitor));
    visitor->node = start;
    visitor->step_count = 1;
    visitor->seen_dac = false;
    visitor->seen_fft = false;
    visitor->visited_nodes[0] = start;
    const long long int part2 = visit(start, false, false);
    printf("Part 2: %lld\n", part2);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
