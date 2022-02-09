/**
 * @file nlu_response.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_H__
#define __NLU_RESPONSE_H__

#include "common/domain_utils.h"
#include "list/list.h"
#include "payload/payload.h"
#include "response/response.h"
#include "semantic/semantic.h"
#include "status/status.h"

typedef struct
{
    StatusPtr status_ptr;
    SemanticPtr semantic_ptr;
    ResponsePtr response_ptr;
    ListPtr list_ptr;
} Result, *ResultPtr, *const ResultCPtr;

typedef struct
{
    Result (*from_cjson)(const cJSON *cjson);
    Result (*from_string)(const char *str);
    void (*drop_memory)(const ResultPtr result_ptr);
    void (*println)(const ResultPtr result_ptr);
} _NluResponse;

extern const _NluResponse NluResponse;

#endif //