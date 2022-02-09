/**
 * @file payload.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "payload.h"

const char PAYLOAD[] = "payload\0";
const static char PAYLOAD_SERVICE[] = "service\0";
const static char PAYLOAD_ACTION[] = "action\0";
const static char PAYLOAD_PARAMS[] = "params\0";

inline static char *payload_parse_service_from_cjson(const cJSON *cjson)
{
    char *service = copy_valuestring_from_cjson(cjson);
    return service;
}

inline static char *payload_parse_action_from_cjson(const cJSON *cjson)
{
    char *service = copy_valuestring_from_cjson(cjson);
    return service;
}

inline static ParamsCPtr payload_parse_params_from_cjson(const cJSON *cjson)
{
    ParamsCPtr params_ptr = ParamsManager.from_cjson_to_cptr(cjson);
    return params_ptr;
}

static Payload payload_from_cjson(const cJSON *cjson)
{
    const cJSON *service_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_SERVICE);
    const cJSON *action_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_ACTION);
    const cJSON *params_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_PARAMS);

    const Payload payload = {
        .service = payload_parse_service_from_cjson(service_cjson),
        .action = payload_parse_action_from_cjson(action_cjson),
        .params = payload_parse_params_from_cjson(params_cjson),
    };
    return payload;
}

static PayloadCPtr payload_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    const cJSON *service_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_SERVICE);
    const cJSON *action_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_ACTION);
    const cJSON *params_cjson = cJSON_GetObjectItem(cjson, PAYLOAD_PARAMS);

    PayloadCPtr payload_ptr = (PayloadPtr)rt_malloc(sizeof(Payload));
    payload_ptr->service = payload_parse_service_from_cjson(service_cjson);
    payload_ptr->action = payload_parse_action_from_cjson(action_cjson);
    payload_ptr->params = payload_parse_params_from_cjson(params_cjson);

    return payload_ptr;
}

static void payload_drop_memery(const PayloadPtr payload_ptr)
{
    if (payload_ptr != RT_NULL)
    {
        rt_free(payload_ptr->service);
        rt_free(payload_ptr->action);
        ParamsManager.drop_memory_and_self(payload_ptr->params);
    }
}

static void payload_drop_memery_and_self(const PayloadPtr payload_ptr)
{
    payload_drop_memery(payload_ptr);
    rt_free(payload_ptr);
}

static void payload_println(const PayloadPtr payload_ptr)
{
    if (payload_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::payload\n");
    rt_kprintf("\tservice:%s\n", payload_ptr->service);
    rt_kprintf("\taction:%s\n", payload_ptr->action);
    ParamsManager.println(payload_ptr->params);
    rt_kprintf("\n");
}

const _PayloadManager PayloadManager = {
    .from_cjson = payload_from_cjson,
    .from_cjson_to_cptr = payload_from_cjson_to_cptr,
    .drop_memory = payload_drop_memery,
    .drop_memory_and_self = payload_drop_memery_and_self,
    .println = payload_println,
};