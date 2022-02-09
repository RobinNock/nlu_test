/**
 * @file radio.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-10-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "radio.h"

const static char RADIO_URL[] = "url\0";
const static char RADIO_TITLE[] = "title\0";
const static char RADIO_CONTENT[] = "content\0";
const static char RADIO_IMAGE[] = "image\0";

inline static radio_parse_url_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static radio_parse_title_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static radio_parse_content_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

inline static radio_parse_image_from_cjson(const cJSON *cjson)
{
    return copy_valuestring_from_cjson(cjson);
}

static Radio radio_from_cjson(const cJSON *cjson)
{
    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, RADIO_URL);
    const cJSON *title_cjson = cJSON_GetObjectItem(cjson, RADIO_TITLE);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, RADIO_CONTENT);
    const cJSON *image_cjson = cJSON_GetObjectItem(cjson, RADIO_IMAGE);

    Radio radio = {
        .url = radio_parse_url_from_cjson(url_cjson),
        .title = radio_parse_title_from_cjson(title_cjson),
        .content = radio_parse_content_from_cjson(content_cjson),
        .image = radio_parse_image_from_cjson(image_cjson),
    };

    return radio;
}

static RadioCPtr radio_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL)
    {
        return RT_NULL;
    }

    if (cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    const cJSON *url_cjson = cJSON_GetObjectItem(cjson, RADIO_URL);
    const cJSON *title_cjson = cJSON_GetObjectItem(cjson, RADIO_TITLE);
    const cJSON *content_cjson = cJSON_GetObjectItem(cjson, RADIO_CONTENT);
    const cJSON *image_cjson = cJSON_GetObjectItem(cjson, RADIO_IMAGE);

    RadioPtr radio_ptr = (RadioPtr)rt_malloc(sizeof(Radio));
    radio_ptr->url = radio_parse_url_from_cjson(url_cjson);
    radio_ptr->title = radio_parse_title_from_cjson(title_cjson);
    radio_ptr->content = radio_parse_content_from_cjson(content_cjson);
    radio_ptr->image = radio_parse_image_from_cjson(image_cjson);

    return radio_ptr;
}

static void radio_drop_memory(const RadioPtr radio_ptr)
{
    if (radio_ptr != RT_NULL)
    {
        rt_free(radio_ptr->url);
        rt_free(radio_ptr->title);
        rt_free(radio_ptr->content);
        rt_free(radio_ptr->image);
    }
}

static void radio_drop_memory_and_self(const RadioPtr radio_ptr)
{
    radio_drop_memory(radio_ptr);
    rt_free(radio_ptr);
}

static void radio_println(const RadioPtr radio_ptr)
{
    if (radio_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::radio\n");
    rt_kprintf("\turl:%s\n", radio_ptr->url);
    rt_kprintf("\ttitle:%s\n", radio_ptr->title);
    rt_kprintf("\tcontent:%s\n", radio_ptr->content);
    rt_kprintf("\timage:%s\n", radio_ptr->image);
    rt_kprintf("\n");
}

const _RadioManager RadioManager = {
    .from_cjson = radio_from_cjson,
    .from_cjson_to_cptr = radio_from_cjson_to_cptr,
    .drop_memory = radio_drop_memory,
    .drop_memory_and_self = radio_drop_memory_and_self,
    .println = radio_println,
};