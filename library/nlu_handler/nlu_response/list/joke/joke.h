/**
 * @file joke.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 笑话
 * @version 0.1
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_LIST_JOKE_H__
#define __NLU_RESPONSE_LIST_JOKE_H__

#include <cJSON.h>
#include <rtthread.h>

#include "library/nlu_handler/nlu_response/util.h"

typedef struct
{
    char *url;
    char *content;
} Joke, *JokePtr, *const JokeCPtr;

typedef struct
{
    Joke (*from_cjson)(const cJSON *cjosn);
    JokePtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const JokePtr joke_ptr);
    void (*drop_memory_and_self)(const JokePtr joke_ptr);
    void (*println)(const JokePtr joke_ptr);
} _JokeManager;

extern const _JokeManager JokeManager;

#endif