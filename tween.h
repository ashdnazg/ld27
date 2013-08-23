#ifndef __TWEEN_H__
#define __TWEEN_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "int_list.h"
#include <stdint.h>


typedef enum tween_type_e {
    TWEEN_TYPE_UBYTE,
    TWEEN_TYPE_INT,
    TWEEN_TYPE_UINT,
    TWEEN_TYPE_FLOAT,
    TWEEN_TYPE_DOUBLE,
} tween_type_t;

typedef enum tween_dir_e {
    TWEEN_IN,
    TWEEN_OUT,
} tween_dir_t;

typedef union tween_ptr_u {
    uint8_t *ubptr;
    int     *iptr;
    unsigned int *uiptr;
    float   *fptr;
    double  *dptr;
    void *ptr;
} tween_ptr_t;

typedef union tween_value_u {
    uint8_t ubval;
    int     ival;
    unsigned int uival;
    float   fval;
    double  dval;
} tween_value_t;

typedef struct tween_s tween_t;

typedef void (*tween_func_t)(tween_t *tween);

struct tween_s {
    link_t all_tweens_link;
    link_t tweens_link;
    tween_ptr_t ptr;
    tween_type_t type;
    tween_dir_t dir;
    tween_value_t current;
    tween_value_t end;
    tween_func_t tween_cb;
    unsigned int steps;
    unsigned int total_steps;
};

#define PTR(tween) ((tween)->type == TWEEN_TYPE_UBYTE ? (tween)->ptr.ubptr :\
                    (tween)->type == TWEEN_TYPE_INT ? (tween)->ptr.iptr :\
                    (tween)->type == TWEEN_TYPE_UINT ? (tween)->ptr.uiptr :\
                    (tween)->type == TWEEN_TYPE_FLOAT ? (tween)->ptr.fptr : \
                    (tween)->type == TWEEN_TYPE_DOUBLE ? (tween)->ptr.dptr : (tween)->ptr.iptr)

#define SET_PTR(tween,value) \
    switch ((tween)->type) {\
        case TWEEN_TYPE_UBYTE:   *((tween)->ptr.ubptr)  = (value);  break;\
        case TWEEN_TYPE_INT:     *((tween)->ptr.iptr)   = (value);  break;\
        case TWEEN_TYPE_UINT:    *((tween)->ptr.iptr)   = (value);  break;\
        case TWEEN_TYPE_FLOAT:   *((tween)->ptr.fptr)   = (value);  break;\
        case TWEEN_TYPE_DOUBLE:  *((tween)->ptr.dptr)   = (value);  break;\
    }


#define INC_PTR(tween,value) \
    switch ((tween)->type) {\
        case TWEEN_TYPE_UBYTE:   *((tween)->ptr.ubptr)  += (value);  break;\
        case TWEEN_TYPE_INT:     *((tween)->ptr.iptr)   += (value);  break;\
        case TWEEN_TYPE_UINT:    *((tween)->ptr.iptr)   += (value);  break;\
        case TWEEN_TYPE_FLOAT:   *((tween)->ptr.fptr)   += (value);  break;\
        case TWEEN_TYPE_DOUBLE:  *((tween)->ptr.dptr)   += (value);  break;\
    }
#define SET_VAL(tween, val, new_value) \
    switch ((tween)->type) {\
        case TWEEN_TYPE_UBYTE:   val.ubval  = (new_value);  break;\
        case TWEEN_TYPE_INT:     val.ival   = (new_value);  break;\
        case TWEEN_TYPE_UINT:    val.ival   = (new_value);  break;\
        case TWEEN_TYPE_FLOAT:   val.fval   = (new_value);  break;\
        case TWEEN_TYPE_DOUBLE:  val.dval   = (new_value);  break;\
    }
    
#define GET_VAL(tween, val) ((tween)->type == TWEEN_TYPE_UBYTE ? (val).ubval :\
                    (tween)->type == TWEEN_TYPE_INT ? (val).ival :\
                    (tween)->type == TWEEN_TYPE_UINT ? (val).uival :\
                    (tween)->type == TWEEN_TYPE_FLOAT ? (val).fval : \
                    (tween)->type == TWEEN_TYPE_DOUBLE ? (val).dval : (val).ival)

#define START(tween) GET_VAL((tween),(tween)->start)
#define CURRENT(tween) GET_VAL((tween),(tween)->current)                    
#define END(tween) GET_VAL((tween),(tween)->end)
                    
typedef struct tween_manager_s {
    list_t *all_tweens;
} tween_manager_t;


void tween_free(tween_t *tween);

tween_manager_t * tween_manager_new(void);
void tween_manager_free(tween_manager_t *t_manager);

void tween_manager_tween(tween_manager_t *t_manager);

tween_t * tween_manager_add_tween(tween_manager_t *t_manager, list_t *parent_tweens_list, void *ptr, tween_type_t type, unsigned int total_steps,
                                                        tween_value_t start, tween_value_t end, tween_dir_t dir, tween_func_t tween_cb);
                                                        
void linear_tween(tween_t *tween);
void quad_tween(tween_t *tween);
void smooth_tween(tween_t *tween);
#ifdef __cplusplus
}
#endif

#endif
