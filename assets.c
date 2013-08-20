#include "assets.h"
#include "mem_wrap.h"
#include "int_list.h"
#include "string.h"

asset_node_t * asset_node_new(void *asset, const char *name) {
    asset_node_t *asset_node = mem_alloc(sizeof(asset_node_t));
    asset_node->name = name;
    asset_node->asset = asset;
    link_init(&(asset_node->nodes_link));
    return asset_node;
}

void asset_node_free(asset_node_t *asset_node) {
    link_remove_from_list(&(asset_node->nodes_link));
    mem_free(asset_node);
}

asset_manager_t * asset_manager_new(free_cb_t free_cb) {
    asset_manager_t *a_manager = mem_alloc(sizeof(asset_manager_t));
    a_manager->nodes = list_new(asset_node_t, nodes_link);
    a_manager->free_cb = free_cb;
    return a_manager;
}

void asset_manager_free(asset_manager_t *a_manager) {
    if (a_manager->free_cb != NULL) {
        list_for_each(a_manager->nodes, asset_node_t *, asset_node) {
            a_manager->free_cb(asset_node->asset);
        }
    }
    list_for_each(a_manager->nodes, asset_node_t *, asset_node) {
        asset_node_free(asset_node);
    }
    list_free(a_manager->nodes);
    mem_free(a_manager);
}



void asset_manager_add(asset_manager_t *a_manager, void *asset, const char *name) {
    asset_node_t *asset_node = asset_node_new(asset, name);
    list_insert_tail(a_manager->nodes, asset_node);
}


void * asset_manager_get(asset_manager_t *a_manager, const char *name) {
    list_for_each(a_manager->nodes, asset_node_t *, asset_node){
        if(strncmp(name, asset_node->name, MAX_STR_CMP) == 0){
            return asset_node->asset;
        }
    }
    return NULL;
}
