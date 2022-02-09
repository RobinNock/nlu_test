/**
 * @file params.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "params.h"

const char PARAMS[] = "params";
const static char PARAMS_DEGREE[] = "degree";

inline static DegreeCPtr params_parse_degree_from_cjson(const cJSON *cjson)
{
    DegreeCPtr degree_ptr = DegreeManager.from_cjson_to_cptr(cjson);
    return degree_ptr;
}

static Params params_from_cjson(const cJSON *cjson)
{
    const cJSON *degree_cjson = cJSON_GetObjectItem(cjson, PARAMS_DEGREE);

    Params params = {
        .degree = params_parse_degree_from_cjson(degree_cjson),
    };
    return params;
}

static ParamsCPtr params_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }
    const cJSON *degree_cjson = cJSON_GetObjectItem(cjson, PARAMS_DEGREE);

    ParamsCPtr params_ptr = (ParamsPtr)rt_malloc(sizeof(Params));
    params_ptr->degree = params_parse_degree_from_cjson(degree_cjson);

    return params_ptr;
}

static void params_drop_memory(const ParamsPtr params_ptr)
{
    if (params_ptr != RT_NULL)
    {
        DegreeManager.drop_memory_and_self(params_ptr->degree);
    }
}

static void params_drop_memory_and_self(const ParamsPtr params_ptr)
{
    params_drop_memory(params_ptr);
    rt_free(params_ptr);
}

static void params_println(const ParamsPtr params_ptr)
{
    if (params_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::params\n");
    DegreeManager.println(params_ptr->degree);
    rt_kprintf("\n");
}

const _ParamsManager ParamsManager = {
    .from_cjson = params_from_cjson,
    .from_cjson_to_cptr = params_from_cjson_to_cptr,
    .drop_memory = params_drop_memory,
    .drop_memory_and_self = params_drop_memory_and_self,
    .println = params_println};
