/**
 * @file payload.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-21
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __NLU_REQUEST_PAYLOAD_H__
#define __NLU_REQUEST_PAYLOAD_H__

#include "library/nlu_handler/nlu_response/util.h"
#include "params/params.h"
#include <cJSON.h>
#include <rtthread.h>

extern const char PAYLOAD[];

typedef struct
{
    char *service;
    char *action;
    ParamsPtr params;
} Payload, *PayloadPtr, *const PayloadCPtr;

typedef struct
{
    Payload (*from_cjson)(const cJSON *cjson);
    PayloadCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const PayloadPtr payload_ptr);
    void (*drop_memory_and_self)(const PayloadPtr payload_ptr);
    void (*println)(const PayloadPtr payload_ptr);
} _PayloadManager;

extern const _PayloadManager PayloadManager;

#endif //