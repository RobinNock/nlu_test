#include <cJSON.h>
#ifndef __DATA_COMMON_H__
#define __DATA_COMMON_H__
typedef struct
{
    const cJSON *(*to_cjson)(void *);
} Json, *JsonPtr;
typedef struct
{
    const void (*drop_memery)(void *);
} Drop, DropPtr;

typedef struct
{
    Json json;
    Drop drop;
} Base;

#endif