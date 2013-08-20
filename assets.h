#ifndef __ASSETS_H__
#define __ASSETS_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "int_list.h"

#define MAX_STR_CMP 255

typedef void (*free_cb_t)(void *);


typedef struct asset_node_s {
    link_t nodes_link;
    const char *name;
    void *asset;
} asset_node_t;

typedef struct asset_manager_s {
    list_t *nodes;
    free_cb_t free_cb;
} asset_manager_t;
    
asset_manager_t * asset_manager_new(free_cb_t free_cb);
void asset_manager_free(asset_manager_t *a_manager);

void asset_manager_add(asset_manager_t *a_manager, void *asset, const char *name);
void * asset_manager_get(asset_manager_t *a_manager, const char *name);

#ifdef __cplusplus
}
#endif

#endif 
