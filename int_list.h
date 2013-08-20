#ifndef __INT_LIST_H__
#define __INT_LIST_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stddef.h>
#include "macros.h"

typedef struct link_s link_t;
struct link_s {
    link_t *prev_link;
    void *next_node;
};
typedef struct list_s {
    link_t sentinel;
    unsigned int offset;
} list_t;






void link_init(link_t *link);

void link_remove_from_list(link_t *link);

void link_insert_before(link_t *link, void *node, link_t *next_link);
void link_insert_after(link_t *link, void *node, link_t *prev_link);

bool link_is_linked(link_t *link);


void link_unlink(link_t *link);
link_t * link_prev_link(link_t *link);
link_t * link_next_link(link_t *link);

void * link_prev(link_t *link);
void * link_next(link_t *link);



#define list_init(list, node_type, link_name) _list_init(list, offsetof(node_type, link_name));
void _list_init(list_t *list, unsigned int offset);

#define list_new(node_type, link_name) _list_new(offsetof(node_type, link_name));
list_t * _list_new(unsigned int offset);


#define list_for_each(list, type, var)\
  type var; \
  type next##__LINE__  = list_head(list);\
  while (((var = next##__LINE__) != NULL)\
    && ((next##__LINE__ = list_next(list, var)) || TRUE))

void list_free(list_t *list);

bool list_is_empty(list_t *list);
void list_unlink_all(list_t *list);

void  * list_head(list_t *list);
void  * list_tail(list_t *list);

link_t * list_get_link(list_t *list, void *node);
void * list_prev(list_t *list, void *node);
void * list_next(list_t *list, void *node);

void list_insert_head(list_t *list, void *node);
void list_insert_tail(list_t *list, void *node);

void list_insert_before(list_t *list, void *node, void *before);
void list_insert_after(list_t *list, void *node, void *after);

#ifdef __cplusplus
}
#endif

#endif
