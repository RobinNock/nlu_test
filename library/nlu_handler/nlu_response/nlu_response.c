/**
 * @file nlu_response.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "nlu_response.h"

inline static StatusCPtr result_status_cptr_form_cjson(const cJSON *cjson)
{
    return StatusManager.from_cjson_to_cptr(cjson);
}

inline static SemanticCPtr result_semantic_cptr_from_cjson(const cJSON *cjson)
{
    return SemanticManager.from_cjson_to_cptr(cjson);
}

inline static ResponseCPtr result_response_cptr_from_cjson(const cJSON *cjson)
{
    return ResponseManager.from_cjson_to_cptr(cjson);
}

inline static ListCPtr result_list_cptr_from_cjson(const cJSON *cjson, DomainTypeEnum type)
{
    return ListManager.from_cjson_to_cptr(cjson, type);
}

static Result result_from_cjson(const cJSON *cjson)
{
    const cJSON *status_cjson = cJSON_GetObjectItem(cjson, STATUS);
    const cJSON *semantic_cjson = cJSON_GetObjectItem(cjson, SEMANTIC);
    const cJSON *response_cjson = cJSON_GetObjectItem(cjson, RESPONSE);
    const cJSON *list_cjson = cJSON_GetObjectItem(cjson, LIST);

    Result result = {
        .status_ptr = result_status_cptr_form_cjson(status_cjson),
        .semantic_ptr = result_semantic_cptr_from_cjson(semantic_cjson),
        .response_ptr = result_response_cptr_from_cjson(response_cjson),
    };
    if (result.semantic_ptr != RT_NULL)
    {
        DomainTypeEnum type = semantic_domain_type(result.semantic_ptr->domain);
        result.list_ptr = result_list_cptr_from_cjson(list_cjson, type);
    }
    else
    {
        result.list_ptr = RT_NULL;
    }
    return result;
}

static Result result_from_string(char *str)
{
    const cJSON *cjson = cJSON_Parse(str);
    const Result result = result_from_cjson(cjson);
    cJSON_Delete(cjson);
    return result;
}

static void result_drop_memory(const ResultPtr result_ptr)
{
    if (result_ptr != RT_NULL)
    {
        StatusManager.drop_memory_and_self(result_ptr->status_ptr);
        SemanticManager.drop_memory_and_self(result_ptr->semantic_ptr);
        ResponseManager.drop_memory_and_self(result_ptr->response_ptr);
        ListManager.drop_memory_and_self(result_ptr->list_ptr);
    }
}

static void result_println(const ResultPtr result_ptr)
{
    if (result_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::result\n");
    StatusManager.println(result_ptr->status_ptr);
    SemanticManager.println(result_ptr->semantic_ptr);
    ResponseManager.println(result_ptr->response_ptr);
    ListManager.println(result_ptr->list_ptr);
    rt_kprintf("\n");
}

const _NluResponse NluResponse = {
    .from_cjson = result_from_cjson,
    .from_string = result_from_string,
    .drop_memory = result_drop_memory,
    .println = result_println,
};
