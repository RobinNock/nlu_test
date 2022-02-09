/**
 * @file user.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "user.h"

const static char USER_MID[] = "mid\0";
const static char USER_UID[] = "uid\0";

static cJSON *user_to_cjson(void *user)
{
    const UserPtr user_ptr = (UserPtr)(user);
    const cJSON *user_json = cJSON_CreateObject();
    cJSON_AddStringToObject(user_json, USER_MID, user_ptr->mid);
    cJSON_AddStringToObject(user_json, USER_UID, user_ptr->uid);
    return user_json;
}

static User user_new(const char *mid, const char *uid)
{
    char *_mid = rt_strdup(mid);
    char *_uid = rt_strdup(uid);
    const User user = {
        .mid = _mid,
        .uid = _uid,
    };
    return user;
}

static void user_drop_memory(const UserPtr user_ptr)
{
    if (user_ptr != RT_NULL)
    {
        rt_free(user_ptr->mid);
        rt_free(user_ptr->uid);
    }
}

const _UserManager UserManager = {
    .new = user_new,
    .base = {
        .json = {
            .to_cjson = user_to_cjson,
        },
        .drop = {
            .drop_memery = user_drop_memory,
        }}};