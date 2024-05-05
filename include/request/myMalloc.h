#ifndef CDEMOS_MYMALLOC_H
#define CDEMOS_MYMALLOC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "malloc.h"
#include "stdlib.h"

#define MY_FREE(memory)  free(memory); \
                         memory = NULL

#define Malloc(type,size)           (type *)malloc(sizeof(type)*size)


#define     My_Error(_error)       printf(_error); exit(1)

#ifdef __cplusplus
}
#endif
#endif //CDEMOS_MYMALLOC_H
