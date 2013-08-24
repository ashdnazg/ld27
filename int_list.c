#include "int_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "macros.h"
#include "mem_wrap.h"


void _link_init(link_t *link, unsigned int offset) {
    // The 1 marks this node as the end of the list.
    link->next_node = (void *) ((size_t) link + 1 - offset);
    link->prev_link = link;
}

void link_init(link_t *link) {
    _link_init(link, 0);
}


link_t * link_next_link(link_t *link) {
    // Calculate the offset from a node pointer to a link structure
    size_t offset = (size_t) link - ((size_t) link->prev_link->next_node & ~1);

    // Get the link field for the next node
    return (link_t *) (((size_t) link->next_node & ~1) + offset);
}


void link_remove_from_list(link_t *link) {
    link_next_link(link)->prev_link = link->prev_link;
    link->prev_link->next_node = link->next_node;
}

void link_insert_before(link_t *link, void *node, link_t *next_link) {
    assert(node != NULL);
    link_remove_from_list(link);
    
    link->prev_link = next_link->prev_link;
    link->next_node = link->prev_link->next_node;

    next_link->prev_link->next_node = node;
    next_link->prev_link = link;
}

void link_insert_after(link_t *link, void *node, link_t *prev_link) {
    assert(node != NULL);
    link_remove_from_list(link);
    link->prev_link = prev_link;
    link->next_node = prev_link->next_node;

    link_next_link(prev_link)->prev_link = link;
    prev_link->next_node = node;
}

bool link_is_linked(link_t *link) {
    return link->prev_link != link;
}

void link_unlink(link_t *link) {
    link_remove_from_list(link);
    link->next_node = (void *) ((size_t) link + 1);
    link->prev_link = link;
}

link_t * link_prev_link(link_t *link) {
    return link->prev_link;
}

void * link_prev(link_t *link) {
    void * prev_node = link->prev_link->prev_link->next_node;
    if ((size_t) prev_node & 1) {
        return NULL;
    }
    return prev_node;
}

void * link_next(link_t *link) {
    if ((size_t) link->next_node & 1) {
        return NULL;
    }
    return link->next_node;
}

void _list_init(list_t *list, unsigned int offset) {
    _link_init(&(list->sentinel), offset);
    list->offset = offset;
}

list_t * _list_new(unsigned int offset) {
    list_t *list = mem_alloc(sizeof(list_t));
    _list_init(list, offset);
    return list;
}

bool list_is_empty(list_t *list) {
    return link_next(&(list->sentinel)) == NULL;
}

void list_unlink_all(list_t *list) {
    link_t *link = NULL;
    for (;;) {
        link = link_prev_link(&(list->sentinel));
        if (link == &(list->sentinel)) {
            break;
        }
        link_unlink(link);
    }
}

void list_free(list_t *list) {
    list_unlink_all(list);
    mem_free(list);
}

void  * list_head(list_t *list) {
    return link_next(&(list->sentinel));
}

void  * list_tail(list_t *list) {
    return link_prev(&(list->sentinel));
}
link_t * list_get_link(list_t *list, void *node) {
    assert(list->offset != (unsigned int) -1);
    return (link_t *) ((size_t) node + list->offset);
}
void * list_prev(list_t *list, void *node) {
    return link_prev(list_get_link(list, node));
}
void * list_next(list_t *list, void *node) {
    return link_next(list_get_link(list, node));
}

void list_insert_head(list_t *list, void *node) {
    list_insert_after(list, node, NULL);
}

void list_insert_tail(list_t *list, void *node) {
    list_insert_before(list, node, NULL);
}

void list_insert_before(list_t *list, void *node, void *before) {
    assert(!((size_t) node & 1));
    link_insert_before(list_get_link(list, node),
        node,
        before ? list_get_link(list, before) : &(list->sentinel)
    );
}

void list_insert_after(list_t *list, void *node, void *after) {
    assert(!((size_t) node & 1));
    link_insert_after(list_get_link(list, node),
        node,
        after ? list_get_link(list, after) : &(list->sentinel)
    );
}

void list_sort(list_t *list, key_cb_t key_cb) {
    void *p;
    void *q;
    void *next;
    int i, psize, qsize;
    int merges = 0;
    int sublist_size = 1;
    list_t *list_a;
    list_t *list_b;
    if (list_head(list) == NULL) {
        return;
    }
    list_a = _list_new(list->offset);
    list_b = _list_new(list->offset);
    list_t *list_from = list;
    list_t *list_to = list_a;
    while (merges != 1) {
        merges = 0;
        p = list_head(list_from);
        q = p;
        for (;;) {
            // printf("\nmerges: %d, p: %d, q: %d",merges, p , q);
            // printf("\nfrom_head: %d, to_head: %d, to_tail: %d",list_head(list_from), list_head(list_to), list_tail(list_to));
            // scanf("%d", &i);
            if (p == NULL) {
                break;
            }
            psize = 0;
            for (i = 0;i < sublist_size;++i) {
                q = list_next(list_from, q);
                // printf("\nadvanced q: %d", q);
                ++psize;
                if (q == NULL) {
                    break;
                }
            }
            qsize = sublist_size;
            while (psize > 0 || (qsize > 0 && q != NULL)){
                //printf("\npsize: %d, qsize: %d, p: %d, q: %d", psize, qsize, p, q);
                if (qsize == 0 || q == NULL || (psize > 0 && (key_cb(p) >= key_cb(q)))) {
                    // printf("\npicked p: %d", key_cb(p));
                    next = list_next(list_from, p);
                    list_insert_tail(list_to, p);
                    p = next;
                    --psize;
                } else {
                    // printf("\npicked q: %d", key_cb(q));
                    next = list_next(list_from, q);
                    list_insert_tail(list_to, q);
                    q = next;
                    --qsize;
                }
            }
            p = q;
            ++merges;
        }
        // printf("\nmerges: %d", merges);
        sublist_size *= 2;
        if (list_to == list_a) {
            list_from = list_a;
            list_to = list_b;
        } else {
            list_from = list_b;
            list_to = list_a;
        }
    }
    list_for_each(list_from, void *, node) {
        list_insert_tail(list, node);
    }
    list_free(list_a);
    list_free(list_b);
}
