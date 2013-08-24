#ifndef __MACROS_H__
#define __MACROS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int bool;
#define FALSE 0
#define TRUE 1

#define ABS(x) ((x) > 0 ? (x) : (-(x)))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define CONCAT(a,b) a ## b

#ifdef __cplusplus
}
#endif

#endif
