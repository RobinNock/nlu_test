/**
 * @file params.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_PAYLOAD_PARAMS_H__
#define __NLU_RESPONSE_PAYLOAD_PARAMS_H__

#include "degree/degree.h"
#include "library/nlu_handler/nlu_response/util.h"

extern const char PARAMS[];

typedef struct
{
    DegreePtr degree;
} Params, *ParamsPtr, *const ParamsCPtr;

typedef struct
{
    Params (*from_cjson)(const cJSON *cjson);
    ParamsCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const ParamsPtr params_ptr);
    void (*drop_memory_and_self)(const ParamsPtr params_ptr);
    void (*println)(const ParamsPtr params_ptr);
} _ParamsManager;

extern const _ParamsManager ParamsManager;

#endif //