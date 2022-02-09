/**
 * @file content.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __DATA_CONTENT_CONTENT_H__
#define __DATA_CONTENT_CONTENT_H__

#include "library/nlu_handler/nlu_request/device/device.h"
#include "library/nlu_handler/nlu_request/user/user.h"
#include <rtthread.h>

typedef struct
{
    User user;
    Device device;
    char *query;
} Body, *BodyPtr, *const BodyCPtr;

typedef struct
{
    Body (*new)(const User, const Device, const char *);
    Base base;
} _BodyManager;

/**
 * @brief
 *
 */
extern const _BodyManager BodyManager;
#endif