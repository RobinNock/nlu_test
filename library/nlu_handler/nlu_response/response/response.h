/**
 * @file response.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_HANDLER_RESPONSE_H__
#define __NLU_HANDLER_RESPONSE_H__

#include "library/nlu_handler/nlu_response/util.h"
#include <cJSON.h>
#include <rtthread.h>

extern const char RESPONSE[];

typedef struct
{
    char *text;
    char *hint;
} Response, *ResponsePtr, *const ResponseCPtr;

typedef struct
{
    Response (*from_cjson)(const cJSON *cjson);
    ResponseCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const ResponsePtr response_ptr);
    void (*drop_memory_and_self)(const ResponsePtr response_ptr);
    void (*println)(const ResponsePtr response_ptr);
} _ResponseManager;

extern const _ResponseManager ResponseManager;
#endif //