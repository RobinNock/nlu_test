/**
 * @file poem.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 诗
 * @version 0.1
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_LIST_POEM_H__
#define __NLU_RESPONSE_LIST_POEM_H__

#include <cJSON.h>
#include <rtthread.h>

#include "library/nlu_handler/nlu_response/util.h"

typedef struct
{

    char *type;
    char *url;
    char *title;
    char *content;
    char *author;
    char *dynasty;
} Poem, *PoemPtr, *const PoemCPtr;

typedef struct
{
    Poem (*from_cjson)(const cJSON *cjson);
    PoemPtr (*from_cjson_to_ptr)(const cJSON *cjson);
    void (*drop_memory)(const PoemPtr *poem_ptr);
    void (*drop_memory_and_self)(const PoemPtr poem_ptr);
    void (*println)(const PoemPtr *poem_ptr);
} _PoemManager;

extern const _PoemManager PoemManager;

#endif