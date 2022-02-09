/**
 * @file degree.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "degree.h"

const char DEGREE[] = "degree";
static const char DEGREE_ORGIN[] = "orgin\0";
static const char DEGREE_NORM[] = "norm\0";
static const char DEGREE_CODE[] = "code\0";

inline static char *degree_parse_orgin_from_cjson(const cJSON *cjson)
{
    char *orgin = copy_valuestring_from_cjson(cjson);
    return orgin;
}

inline static char *degree_parse_norm_from_cjson(const cJSON *cjson)
{
    char *norm = copy_valuestring_from_cjson(cjson);
    return norm;
}

inline static int degree_parse_code_from_cjson(const cJSON *cjson)
{
    int code = copy_valueint_from_cjson(cjson);
    return code;
}

/**
 * @brief Construct a new degree from cjson object
 *
 * @param cjson
 * @return Degree
 */
static Degree degree_from_cjson(const cJSON *cjson)
{
    const cJSON *orgin_cjson = cJSON_GetObjectItem(cjson, DEGREE_ORGIN);
    const cJSON *norm_cjson = cJSON_GetObjectItem(cjson, DEGREE_NORM);
    const cJSON *code_cjson = cJSON_GetObjectItem(cjson, DEGREE_CODE);

    const Degree degree = {
        .orgin = degree_parse_orgin_from_cjson(orgin_cjson),
        .norm = degree_parse_norm_from_cjson(norm_cjson),
        .code = degree_parse_code_from_cjson(code_cjson),
    };
    return degree;
}

static DegreeCPtr degree_from_cjson_to_cptr(const cJSON *cjson)
{
    if (cjson == RT_NULL || cJSON_IsNull(cjson))
    {
        return RT_NULL;
    }
    const cJSON *orgin_cjson = cJSON_GetObjectItem(cjson, DEGREE_ORGIN);
    const cJSON *norm_cjson = cJSON_GetObjectItem(cjson, DEGREE_NORM);
    const cJSON *code_cjson = cJSON_GetObjectItem(cjson, DEGREE_CODE);

    DegreeCPtr degree_ptr = (DegreePtr)rt_malloc(sizeof(Degree));
    degree_ptr->orgin = degree_parse_orgin_from_cjson(orgin_cjson);
    degree_ptr->norm = degree_parse_norm_from_cjson(norm_cjson);
    degree_ptr->code = degree_parse_code_from_cjson(code_cjson);
    return degree_ptr;
}

/**
 * @brief 释放内部占用内存
 *
 * @param degree_ptr
 */
static void degree_drop_memory(const DegreePtr degree_ptr)
{
    if (degree_ptr != RT_NULL)
    {
        rt_free(degree_ptr->orgin);
        rt_free(degree_ptr->norm);
    }
}

static void degree_drop_memory_and_self(const DegreePtr degree_ptr)
{
    degree_drop_memory(degree_ptr);
    rt_free(degree_ptr);
}

static void degree_println(const DegreePtr degree_ptr)
{

    if (degree_ptr == RT_NULL)
    {
        return;
    }
    rt_kprintf("tag::degree\n");
    rt_kprintf("\torgin:%s", degree_ptr->orgin);
    rt_kprintf("\tnorm:%s", degree_ptr->norm);
    rt_kprintf("\tcode:%d", degree_ptr->code);
    rt_kprintf("\n");
}

const _DegreeManager DegreeManager = {
    .from_cjson = degree_from_cjson,
    .from_cjson_to_cptr = degree_from_cjson_to_cptr,
    .drop_memory = degree_drop_memory,
    .drop_memory_and_self = degree_drop_memory_and_self,
    .println = degree_println,
};