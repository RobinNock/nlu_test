/**
 * @file response.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "response.h"

const char RESPONSE[] = "response\0";
const static char RESPONSE_TEXT[] = "text\0";
const static char RESPONSE_HINT[] = "hint\0";

/**
 * @brief 从cjson中获取text，若cjson为null，则返回null
 *
 * @param cjson
 * @return char*
 */
inline static char *response_parse_text_from_cjson(const cJSON *cjson)
{
    char *text = copy_valuestring_from_cjson(cjson);
    return text;
}

/**
 * @brief 从cjson中获取hint，若cjson为null，则返回null
 *
 * @param cjson
 * @return char*
 */
inline static char *response_parse_hint_from_cjson(const cJSON *cjson)
{
    char *hint = copy_valuestring_from_cjson(cjson);
    return hint;
}

/**
 * @brief 从cjson中获取response,如果为null,则其余text,hint为null
 *
 * @param cjson
 * @return Response
 */
static Response response_from_cjson(const cJSON *cjson)
{
    const cJSON *text_cjson = cJSON_GetObjectItem(cjson, RESPONSE_TEXT);
    const cJSON *hint_cjson = cJSON_GetObjectItem(cjson, RESPONSE_HINT);

    const Response response = {
        .text = response_parse_text_from_cjson(text_cjson),
        .hint = response_parse_hint_from_cjson(hint_cjson),
    };
    return response;
}

/**
 * @brief 从cjson中获取response,如果为null,则返回null
 *
 * @param cjson
 * @return ResponseCPtr
 */
static ResponseCPtr response_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }
    const cJSON *text_cjson = cJSON_GetObjectItem(cjson, RESPONSE_TEXT);
    const cJSON *hint_cjson = cJSON_GetObjectItem(cjson, RESPONSE_HINT);

    ResponseCPtr response_ptr = (ResponsePtr)rt_malloc(sizeof(Response));
    response_ptr->text = response_parse_text_from_cjson(text_cjson);
    response_ptr->hint = response_parse_hint_from_cjson(hint_cjson);

    return response_ptr;
}

static void response_drop_memory(const ResponsePtr response_ptr)
{
    if (response_ptr != RT_NULL)
    {
        rt_free(response_ptr->text);
        rt_free(response_ptr->hint);
    }
}

static void response_drop_memory_and_self(const ResponsePtr response_ptr)
{
    response_drop_memory(response_ptr);
    rt_free(response_ptr);
}

static void response_println(const ResponsePtr response_ptr)
{
    if (response_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::response\n");
    rt_kprintf("\ttext:%s\n", response_ptr->text);
    rt_kprintf("\thint:%s\n", response_ptr->hint);
    rt_kprintf("\n");
}

const _ResponseManager ResponseManager = {
    .from_cjson = response_from_cjson,
    .from_cjson_to_cptr = response_from_cjson_to_cptr,
    .drop_memory = response_drop_memory,
    .drop_memory_and_self = response_drop_memory_and_self,
    .println = response_println,
};