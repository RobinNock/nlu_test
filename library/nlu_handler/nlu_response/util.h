/**
 * @file util.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_REQUEST_UTIL_H__
#define __NLU_REQUEST_UTIL_H__

#include <cJSON.h>
#include <rtthread.h>

inline static char *copy_valuestring_from_cjson(const cJSON *cjson)
{
    char *value = RT_NULL;
    if (cJSON_IsString(cjson))
    {
        value = rt_strdup(cjson->valuestring);
    }
    return value;
}

#define INVALID_INT -1

inline static int copy_valueint_from_cjson(const cJSON *cjson)
{
    int value = INVALID_INT;
    if (cJSON_IsNumber(cjson))
    {
        value = cjson->valueint;
    }
    return value;
}

inline static rt_bool_t copy_valuebool_from_cjson(const cJSON *cjson)
{
    rt_bool_t value = RT_FALSE;
    if (cJSON_IsBool(cjson))
    {
        value = cjson->valueint;
    }
    return value;
}

#endif /* __NLU_REQUEST_UTIL_H__ */