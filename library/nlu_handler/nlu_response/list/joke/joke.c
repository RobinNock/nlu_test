/**
 * @file joke.c
 * @author joke (codinlog@foxmail.com)
 * @brief 笑话
 * @version 0.1
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "joke.h"

const static char JOKE_URL[] = "url\0";
const static char JOKE_CONTENT[] = "content\0";

inline static joke_parse_url_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static joke_parse_content_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

static Joke joke_from_cjson(const cJSON *cjson)
{
    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, JOKE_URL);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, JOKE_CONTENT);

    Joke joke = {
        .url = joke_parse_url_from_cjson(url_cjson),
        .content = joke_parse_content_from_cjson(content_cjson),
    };

    return joke;
}

static JokeCPtr joke_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL)
    {
        return RT_NULL;
    }

    if (cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, JOKE_URL);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, JOKE_CONTENT);

    JokePtr joke_ptr = (JokePtr)rt_malloc(sizeof(Joke));
    joke_ptr->url = joke_parse_url_from_cjson(url_cjson);
    joke_ptr->content = joke_parse_content_from_cjson(content_cjson);

    return joke_ptr;
}

static void joke_drop_memory(const JokePtr joke_ptr)
{
    if (joke_ptr != RT_NULL)
    {
        rt_free(joke_ptr->url);
        rt_free(joke_ptr->content);
    }
}

static void joke_drop_memory_and_self(const JokePtr joke_ptr)
{
    joke_drop_memory(joke_ptr);
    rt_free(joke_ptr);
}

static void joke_println(const JokePtr joke_ptr)
{
    if (joke_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::joke\n");
    rt_kprintf("\turl:%s\n", joke_ptr->url);
    rt_kprintf("\tcontent:%s\n", joke_ptr->content);
    rt_kprintf("\n");
}

const _JokeManager JokeManager = {
    .from_cjson = joke_from_cjson,
    .from_cjson_to_cptr = joke_from_cjson_to_cptr,
    .drop_memory = joke_drop_memory,
    .drop_memory_and_self = joke_drop_memory_and_self,
    .println = joke_println,
};