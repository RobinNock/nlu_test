/**
 * @file content.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "body.h"

const static char CONTENT_USER[] = "user\0";
const static char CONTENT_DEVICE[] = "device\0";
const static char CONTENT_QUERY[] = "query\0";

/**
 * @brief Create a content object
 *
 * @param user
 * @param device
 * @param query
 * @return const Content
 */
static Body body_new(const User user, const Device device, const char *query);

/**
 * @brief convert content to cjson object
 * @param data
 * @return const cJSON* const
 */
static cJSON *body_to_cjson(void *data);

/**
 * @brief 释放申请的堆空间
 *
 * @param content_ptr
 */
static void body_drop_memory(const BodyPtr content_ptr);

static Body body_new(const User user, const Device device, const char *query)
{
    char *_query = rt_strdup(query);
    const Body content = {
        .user = user,
        .device = device,
        .query = _query,
    };
    return content;
}

static cJSON *body_to_cjson(void *data)
{
    const BodyPtr content_ptr = (BodyPtr)(data);
    const cJSON *content_json = cJSON_CreateObject();
    cJSON_AddItemToObject(content_json, CONTENT_USER, UserManager.base.json.to_cjson(&content_ptr->user));
    cJSON_AddItemToObject(content_json, CONTENT_DEVICE, DeviceManager.base.json.to_cjson(&content_ptr->device));
    cJSON_AddStringToObject(content_json, CONTENT_QUERY, content_ptr->query);
    return content_json;
}

static void body_drop_memory(const BodyPtr content_ptr)
{
    if (content_ptr != RT_NULL)
    {
        UserManager.base.drop.drop_memery((void *)(&content_ptr->user));
        DeviceManager.base.drop.drop_memery((void *)(&content_ptr->device));
        rt_free(content_ptr->query);
    }
}

const _BodyManager BodyManager = {
    .new = body_new,
    .base = {
        .json = {
            .to_cjson = body_to_cjson},
        .drop = {
            .drop_memery = body_drop_memory,
        }}};