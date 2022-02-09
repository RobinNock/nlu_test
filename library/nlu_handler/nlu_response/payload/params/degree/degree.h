/**
 * @file degree.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_PAYLOAD_PARAMS_DEGREE_H__
#define __NLU_RESPONSE_PAYLOAD_PARAMS_DEGREE_H__

#include "library/nlu_handler/nlu_response/util.h"

extern const char DEGREE[];

typedef struct
{
    char *orgin;
    char *norm;
    int code;
} Degree, *DegreePtr, *const DegreeCPtr;

typedef struct
{
    Degree (*from_cjson)(const cJSON *cjson);
    DegreeCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const DegreePtr degree_ptr);
    void (*drop_memory_and_self)(const DegreePtr degree_ptr);
    void (*println)(const DegreePtr degree_ptr);
} _DegreeManager;

extern const _DegreeManager DegreeManager;

#endif //