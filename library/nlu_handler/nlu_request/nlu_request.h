/**
 * @file nlu_request.h
 * @author qr-kou(codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __NLU_REQUEST_H__
#define __NLU_REQUEST_H__

#include "consts.h"
#include "library/byte_buffer/byte_buffer.h"
#include "library/nlu_handler/nlu_request/body/body.h"
#include "library/nlu_handler/nlu_request/data/data.h"
#include "library/nlu_handler/nlu_request/device/device.h"
#include "library/nlu_handler/nlu_request/user/user.h"

#include <webclient.h>

typedef struct
{
    struct webclient_session *client;
    char *body;
} Session, *SessionPtr, *const SessionCPtr;

typedef struct
{
    Session (*new)(const BodyPtr content_ptr);
    int (*post)(const SessionPtr session_ptr, const BufferPtr buffer_ptr);
    void (*close)(const SessionPtr session_ptr);
} _NluRequest;

extern const _NluRequest NluRequest;
#endif //