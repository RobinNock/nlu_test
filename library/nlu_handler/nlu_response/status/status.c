/**
 * @file status.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "status.h"

const char STATUS[] = "status\0";
const static int ERR_STATUS_FROM_CJSON = -100;
const static char STATUS_CODE[] = "code\0";
const static char STATUS_ERRORTYPE[] = "errorType\0";

inline static int status_parse_code_from_cjson(const cJSON *cjson)
{
    int code = copy_valueint_from_cjson(cjson);
    return code;
}

inline static char *static_parse_error_type_from_cjson(const cJSON *cjson)
{
    char *error_type = copy_valuestring_from_cjson(cjson);
    return error_type;
}

static Status status_from_cjson(const cJSON *cjson)
{
    const cJSON *code_cjson = cJSON_GetObjectItem(cjson, STATUS_CODE);
    const cJSON *error_type_cjson = cJSON_GetObjectItem(cjson, STATUS_ERRORTYPE);

    const int code = status_parse_code_from_cjson(code_cjson);
    const char *error_type = static_parse_error_type_from_cjson(error_type_cjson);

    Status status = {
        .code = code,
        .error_type = error_type,
    };
    return status;
}

static StatusCPtr status_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }

    const cJSON *code_cjson = cJSON_GetObjectItem(cjson, STATUS_CODE);
    const cJSON *error_type_cjson = cJSON_GetObjectItem(cjson, STATUS_ERRORTYPE);

    const int code = status_parse_code_from_cjson(code_cjson);
    const char *error_type = static_parse_error_type_from_cjson(error_type_cjson);

    StatusCPtr status_ptr = (StatusPtr)rt_malloc(sizeof(Status));
    status_ptr->code = code;
    status_ptr->error_type = error_type;
    return status_ptr;
}

static void status_drop_memory(const StatusPtr status_ptr)
{
    if (status_ptr != RT_NULL)
    {
        rt_free(status_ptr->error_type);
    }
}

static void status_drop_memory_and_self(const StatusPtr status_ptr)
{
    status_drop_memory(status_ptr);
    rt_free(status_ptr);
}

static void status_println(const StatusPtr status_ptr)
{
    if (status_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::status\n");
    rt_kprintf("\tcode:%d\n", status_ptr->code);
    rt_kprintf("\terror_type:%s\n", status_ptr->error_type);
    rt_kprintf("\n");
}

const _StatusManager StatusManager = {
    .from_cjson = status_from_cjson,
    .from_cjson_to_cptr = status_from_cjson_to_cptr,
    .drop_memory = status_drop_memory,
    .drop_memory_and_self = status_drop_memory_and_self,
    .println = status_println,
};