#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>
const int TO_LIMIT = 25;
const int NODE_LIMIT = 650;
typedef struct Node {
    char* name;
    struct Node* tos[TO_LIMIT];
    int to_count;
} Node;

typedef struct Visitor {
    Node *node;
    // char *steps[NODE_LIMIT];
    int step_count;
} Visitor;


typedef struct LinkedListNode {
    struct LinkedListNode *next;
    Visitor *visitor;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head;
    LinkedListNode *tail;
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

    return temp->visitor;
}

void append(LinkedList *list, Visitor *visitor) {
    LinkedListNode *node = malloc(sizeof(LinkedListNode));
    node->visitor = visitor;
    node->next = NULL;

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



int main(void) {
    char *line = NULL;
    size_t len = 0;

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
                name[strlen(name)-1] = 0;
                first = false;
                node = find_or_create_node(name);
            } else {
                if (name[strlen(name)-1] == '\n') {
                    name[strlen(name)-1] = 0;
                }
                Node *new_to = find_or_create_node(name);
                node->tos[node->to_count++] = new_to;
            }
            tok = strtok(NULL, " ");
        }
    }

    Node *start = find_node("you");
    Visitor *visitor = malloc(sizeof(Visitor));
    visitor->node = start;
    visitor->step_count = 1;
    // visitor->steps[0] = strdup("you");
    LinkedList *queue = malloc(sizeof(LinkedList));


    long long part1 = 0;
    int c = 0;
    while (visitor != NULL) {
        printf("%d\n", c);
        c++;
        for (int i = 0; i < visitor->node->to_count; i++) {
            Node *to_node = visitor->node->tos[i];
            // bool skip = false;
            // for (int j = 0; j < visitor->step_count; j++) {
            //     if (strcmp(visitor->steps[j], to_node->name) == 0 ) {
            //         skip = true;
            //         break;
            //     }
            // }

            // if (skip) {
            //     continue;
            // }
            if (to_node == NULL) {
                continue;
            }
            if (strcmp("out", to_node->name) == 0) {
                part1++;
                continue;
            }


            Visitor *new_visitor = malloc(sizeof(Visitor));
            memcpy(new_visitor, visitor, sizeof(Visitor));
            new_visitor->node = to_node;
            // new_visitor->steps[new_visitor->step_count++] = strdup(to_node->name);
            append(queue, new_visitor);
        }

        visitor = pop(queue);
    }

    printf("Part 1: %lld\n", part1);

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}
