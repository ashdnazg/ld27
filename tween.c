#include "tween.h"
#include "int_list.h"
#include "mem_wrap.h"


tween_t * tween_new(void *ptr, tween_type_t type, unsigned int total_steps, tween_value_t start, tween_value_t end, 
                                tween_dir_t dir, tween_func_t tween_cb) {
    tween_t *tween = mem_alloc(sizeof(tween_t));
    tween->ptr.ptr = ptr;
    tween->type = type;
    tween->steps = (dir == TWEEN_IN ? 0 : total_steps);
    tween->total_steps = total_steps;
    SET_VAL(tween, tween->current, (dir == TWEEN_IN ? 0 : GET_VAL(tween, start) - GET_VAL(tween, end)));
    SET_VAL(tween, tween->end, (dir == TWEEN_IN ? GET_VAL(tween, end) - GET_VAL(tween, start) : GET_VAL(tween, start) - GET_VAL(tween, end)));
    tween->dir = dir;
    tween->tween_cb = tween_cb;
    link_init(&(tween->all_tweens_link));
    link_init(&(tween->tweens_link));
    return tween;
}

void tween_free(tween_t *tween){
    link_remove_from_list(&(tween->all_tweens_link));
    link_remove_from_list(&(tween->tweens_link));
    mem_free(tween);
}

tween_manager_t * tween_manager_new(void) {
    tween_manager_t * t_manager = mem_alloc(sizeof(tween_manager_t));
    t_manager->all_tweens = list_new(tween_t, all_tweens_link);
    return t_manager;
}

void tween_manager_free(tween_manager_t *t_manager) {
    list_for_each(t_manager->all_tweens, tween_t *, tween) {
        tween_free(tween);
    }
    list_free(t_manager->all_tweens);
    mem_free(t_manager);
}

tween_t * tween_manager_add_tween(tween_manager_t *t_manager, list_t *parent_tweens_list, void *ptr, tween_type_t type, unsigned int total_steps,
                                                        tween_value_t start, tween_value_t end, tween_dir_t dir, tween_func_t tween_cb) {
    tween_t *tween = tween_new(ptr, type, total_steps, start, end, dir, tween_cb);
    list_insert_tail(t_manager->all_tweens, tween);
    list_insert_tail(parent_tweens_list, tween);
    return tween;
}

void tween_manager_tween(tween_manager_t *t_manager) {
    list_for_each(t_manager->all_tweens, tween_t *, tween) {
        tween->steps += (tween->dir == TWEEN_IN ? 1 : -1);
        tween->tween_cb(tween);
        if (tween->dir == TWEEN_IN) {
            if (tween->steps == tween->total_steps) {
                tween_free(tween);
            }
        } else {
            if (tween->steps == 0) {
                tween_free(tween);
            }
        }
    }
}

void linear_tween(tween_t *tween) {
    tween_value_t temp;
    SET_VAL(tween, temp, END(tween) * tween->steps / tween->total_steps);
    INC_PTR(tween, GET_VAL(tween, temp) - CURRENT(tween));
    tween->current = temp;
}

void quad_tween(tween_t *tween) {
    tween_value_t temp;
    SET_VAL(tween, temp, END(tween) * tween->steps * tween->steps / tween->total_steps / tween->total_steps);
    INC_PTR(tween, GET_VAL(tween, temp) - CURRENT(tween));
    tween->current = temp;
}

void smooth_tween(tween_t *tween) {
    tween_value_t temp;
    SET_VAL(tween, temp, 3 * END(tween) * tween->steps * tween->steps / tween->total_steps / tween->total_steps - 2 * END(tween) * tween->steps * tween->steps * tween->steps / tween->total_steps / tween->total_steps / tween->total_steps);
    INC_PTR(tween, GET_VAL(tween, temp) - CURRENT(tween));
    tween->current = temp;
}