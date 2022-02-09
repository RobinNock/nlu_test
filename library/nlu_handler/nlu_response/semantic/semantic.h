/**
 * @file semantic.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE__SEMANTIC_H__
#define __NLU_RESPONSE__SEMANTIC_H__

#include "library/nlu_handler/nlu_response/util.h"
#include <cJSON.h>
#include <rtthread.h>
extern const char SEMANTIC[];

typedef struct
{
    char *domain;
    char *intent;
    rt_bool_t session_complete;
    char *skill;
} Semantic, *SemanticPtr, *const SemanticCPtr;

typedef struct
{
    Semantic (*from_cjson)(const cJSON *cjson);
    SemanticCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const SemanticPtr semantic_ptr);
    void (*drop_memory_and_self)(const SemanticPtr semantic_ptr);
    void (*println)(const SemanticPtr semantic_ptr);
} _SemanticManager;
extern const _SemanticManager SemanticManager;
#endif //
