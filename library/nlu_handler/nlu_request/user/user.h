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

#ifndef __DATA_USER_USER_H__
#define __DATA_USER_USER_H__

#include "library/nlu_handler/common/common.h"
#include <rtthread.h>

typedef struct
{
    char *mid;
    char *uid;
} User, *UserPtr, *const UserCPtr;

typedef struct
{
    User (*new)(const char *, const char *);
    Base base;
} _UserManager;

/**
 * @brief
 * Usage:
 *  create new user instance:
 *    const User user = UserManager.new(mid,uid);
 *  convert user to cJson:
 *    const cJson *const json = UserManager.json.to_json(&user);
 */
extern const _UserManager UserManager;
#endif