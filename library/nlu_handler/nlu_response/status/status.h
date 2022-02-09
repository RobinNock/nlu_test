/**
 * @file status.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-20
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __NLU_RESPONSE_STATUS_H__
#define __NLU_RESPONSE_STATUS_H__
#include "library/nlu_handler/nlu_response/util.h"
#include <CJSON.h>
#include <rtthread.h>
#include <stdint.h>

extern const char STATUS[];

typedef struct
{
    int code;
    char *error_type;
} Status, *StatusPtr, *const StatusCPtr;

typedef struct
{
    Status (*from_cjson)(const cJSON *cjosn);
    StatusCPtr (*from_cjson_to_cptr)(const cJSON *cjosn);
    void (*drop_memory)(const StatusPtr status_ptr);
    void (*drop_memory_and_self)(const StatusPtr status_ptr);
    void (*println)(const StatusPtr status_ptr);
} _StatusManager;

extern const _StatusManager StatusManager;
#endif //