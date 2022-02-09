/**
 * @file data.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "data.h"

const static char DATA_DATA[] = "data\0";

/**
 * @brief Create a data object
 *
 * @param data
 * @return const Data
 */
static Data data_new(const char *data);

/**
 * @brief
 *
 * @param data
 * @return cJSON*
 */
static cJSON *data_to_cjson(void *data);

/**
 * @brief
 *
 * @param data_ptr
 */
static void data_drop_memory(const DataPtr data_ptr);

static Data
data_new(const char *chs)
{
    char *_data = rt_strdup(chs);
    Data data = {
        .data = _data,
    };
    return data;
}

static cJSON *data_to_cjson(void *data)
{
    const DataPtr data_ptr = (DataPtr)(data);
    cJSON *data_json = cJSON_CreateObject();
    cJSON_AddStringToObject(data_json, DATA_DATA, data_ptr->data);
    return data_json;
}

static void data_drop_memory(const DataPtr data_ptr)
{
    if (data_ptr != RT_NULL)
    {
        rt_free(data_ptr->data);
    }
}

const _DataManager DataManager = {
    .new = data_new,
    .base = {
        .json = {
            .to_cjson = data_to_cjson,
        },
        .drop = {
            .drop_memery = data_drop_memory,
        }}};
