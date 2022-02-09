/**
 * @file radio.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 电台
 * @version 0.1
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_LIST_RADIO_H__
#define __NLU_RESPONSE_LIST_RADIO_H__

#include <cJSON.h>
#include <rtthread.h>

#include "library/nlu_handler/nlu_response/util.h"

typedef struct
{
    char *url;
    char *title;
    char *content;
    char *image;
} Radio, *RadioPtr, *const RadioCPtr;

typedef struct
{
    Radio (*from_cjson)(const cJSON *cjosn);
    RadioCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const RadioPtr radio_ptr);
    void (*drop_memory_and_self)(const RadioPtr radio_ptr);
    void (*println)(const RadioPtr radio_ptr);
} _RadioManager;

extern const _RadioManager RadioManager;

#endif