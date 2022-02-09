/**
 * @file domain_utils.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 领域判断
 * @version 0.1
 * @date 2021-11-02
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_COMMON_H__
#define __NLU_RESPONSE_COMMON_H__

#include "domain_consts.h"

typedef enum
{
    MUSIC,
    RADIO,
    JOKE,
    POEM,
    CONTROL,
    HOLIDAY,
    AUDIO_SOURCE_TYPE_UNKNOWN,
} DomainTypeEnum;

inline static DomainTypeEnum semantic_domain_type(const char *domain)
{
    if (rt_strcmp(domain, DOMAIN_MUSTC) == 0)
    {
        return MUSIC;
    }

    if (rt_strcmp(domain, DOMAIN_JOKE) == 0)
    {
        return JOKE;
    }

    if (rt_strcmp(domain, DOMAIN_RADIO) == 0)
    {
        return RADIO;
    }

    if (rt_strcmp(domain, DOMAIN_POEM) == 0)
    {
        return POEM;
    }

    if (rt_strcmp(domain, DOMAIN_HOLIDAY) == 0)
    {
        return HOLIDAY;
    }

    if (rt_strcmp(domain, DOMAIN_CONTROL) == 0)
    {
        return CONTROL;
    }

    return AUDIO_SOURCE_TYPE_UNKNOWN;
}

#endif