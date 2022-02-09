/**
 * @file poem.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 古诗处理
 * @version 0.1
 * @date 2021-10-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "poem.h"

const char POEM_TYPE[] = "type\0";
const char POEM_URL[] = "url\0";
const char POEM_TITLE[] = "title\0";
const char POEM_CONTENT[] = "content\0";
const char POEM_AUTHOR[] = "author\0";
const char POEM_DYNASTY[] = "dynasty\0";

inline static poem_parse_type_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static poem_parse_url_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static poem_parse_title_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static poem_parse_content_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static poem_parse_author_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static poem_parse_dynasty_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

static Poem poem_from_cjson(const cJSON *cjson)
{
    const cJSON *type_cjson = cJSON_GetObjectItem(cjson, POEM_TYPE);
    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, POEM_URL);
    const cJSON *title_cjson = cJSON_GetObjectItem(cjson, POEM_TITLE);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, POEM_CONTENT);
    const cJSON *author_cjson = cJSON_GetObjectItem(cjson, POEM_AUTHOR);
    const cJSON *dynasty_cjson = cJSON_GetObjectItem(cjson, POEM_DYNASTY);

    Poem poem = {
        .type = poem_parse_type_from_cjson(type_cjson),
        .url = poem_parse_url_from_cjson(url_cjson),
        .title = poem_parse_title_from_cjson(title_cjson),
        .content = poem_parse_content_from_cjson(content_cjson),
        .author = poem_parse_author_from_cjson(author_cjson),
        .dynasty = poem_parse_dynasty_from_cjson(dynasty_cjson),
    };

    return poem;
}

static PoemCPtr poem_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL)
    {
        return RT_NULL;
    }

    if (cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    const cJSON *type_cjson = cJSON_GetObjectItem(cjson, POEM_TYPE);
    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, POEM_URL);
    const cJSON *title_cjson = cJSON_GetObjectItem(cjson, POEM_TITLE);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, POEM_CONTENT);
    const cJSON *author_cjson = cJSON_GetObjectItem(cjson, POEM_AUTHOR);
    const cJSON *dynasty_cjson = cJSON_GetObjectItem(cjson, POEM_DYNASTY);

    PoemPtr poem_ptr = (PoemPtr)rt_malloc(sizeof(Poem));
    poem_ptr->type = poem_parse_type_from_cjson(type_cjson);
    poem_ptr->url = poem_parse_url_from_cjson(url_cjson);
    poem_ptr->title = poem_parse_title_from_cjson(title_cjson);
    poem_ptr->content = poem_parse_content_from_cjson(content_cjson);
    poem_ptr->author = poem_parse_author_from_cjson(author_cjson);
    poem_ptr->dynasty = poem_parse_dynasty_from_cjson(dynasty_cjson);
    return poem_ptr;
}

void poem_drop_memory(const PoemPtr poem_ptr)
{
    if (poem_ptr != RT_NULL)
    {
        rt_free(poem_ptr->type);
        rt_free(poem_ptr->url);
        rt_free(poem_ptr->title);
        rt_free(poem_ptr->content);
        rt_free(poem_ptr->author);
        rt_free(poem_ptr->dynasty);
    }
}

void poem_drop_memory_and_self(const PoemPtr poem_ptr)
{
    poem_drop_memory(poem_ptr);
    rt_free(poem_ptr);
}

static void poem_println(const PoemPtr poem_ptr)
{
    if (poem_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::peom\n");
    rt_kprintf("\ttype:%s\n", poem_ptr->type);
    rt_kprintf("\turl:%s\n", poem_ptr->url);
    rt_kprintf("\ttitle:%s\n", poem_ptr->title);
    rt_kprintf("\tcontent:%s\n", poem_ptr->content);
    rt_kprintf("\tauthor:%s\n", poem_ptr->author);
    rt_kprintf("\tdynasty:%s\n", poem_ptr->dynasty);
    rt_kprintf("\n");
}

const _PoemManager PoemManager = {
    .from_cjson = poem_from_cjson,
    .from_cjson_to_ptr = poem_from_cjson_to_cptr,
    .drop_memory = poem_drop_memory,
    .drop_memory_and_self = poem_drop_memory_and_self,
    .println = poem_println,
};
