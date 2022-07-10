#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct CG_ListNode {
    void *data;
    struct CG_ListNode *next;
} CG_ListNode;

CG_ListNode *cg_create_list() {
    CG_ListNode *node = calloc(1, sizeof(CG_ListNode));
    node->data = NULL;
    node->next = NULL;
    return node;
}

/* Aloca memória dinamicamente e copia o conteudo de `data`.
 */
void *cg_copy(void *data, size_t size) {
    assert(data != NULL);

    void *copy = malloc(size);
    memcpy(copy, data, size);
    return copy;
}

/* Copia `size` bytes de `data` para um novo `CG_ListNode`
 * e adicionada esse novo node no final da lista `node`.
 */
void cg_list_add(CG_ListNode *list, void *data, size_t size) {
    assert(list != NULL);
    assert(data != NULL);

    // first node of newly created list
    if (list->data == NULL && list->next == NULL) {
        list->data = cg_copy(data, size);
        return;
    }

    CG_ListNode *new = calloc(1, sizeof(CG_ListNode));
    new->data = cg_copy(data, size);
    new->next = NULL;

    CG_ListNode *last = list;
    while (last->next != NULL)
        last = last->next;

    last->next = new;
}

size_t cg_list_count_nodes(CG_ListNode *list) {
    size_t count = 1;

    while (list->next != NULL) {
        count += 1;
        list = list->next;
    }

    return count;
}

void _cg_destroy_list_node(CG_ListNode *node) {
    assert(node != NULL);
    assert(node->data != NULL);

    free(node->data);
    node->data = NULL;
    node->next = NULL;

    free(node);
}

void cg_destroy_list(CG_ListNode *list) {
    assert(list != NULL);

    CG_ListNode *current = list;
    while (current->next != NULL) {
        CG_ListNode *prev = current;
        current = current->next;
        _cg_destroy_list_node(prev);
    }

    _cg_destroy_list_node(current);
}
