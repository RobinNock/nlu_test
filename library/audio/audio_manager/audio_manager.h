/**
 * @file audio_manager.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-11-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <stddef.h>
#include <stdint.h>
#include "config.h"
#include "library/audio/audio_service/audio_service.h"

typedef struct {
    void (*init)();

    void (*play)();

    void (*stop)();
} _AudioManager;

extern const _AudioManager AudioManager;

#endif