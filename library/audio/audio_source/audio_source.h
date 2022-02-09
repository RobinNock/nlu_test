/**
 * @file audio_source.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-11-12
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__

#include <stdbool.h>
#include <stddef.h>
#include "common.h"
#include "config.h"

typedef int(*WriteBytesCallback)(const char *bytes, const size_t len);

typedef enum {
    AUDIO_SOURCE_FROM_ONLINE,
    AUDIO_SOURCE_FROM_LOCAL,
} AudioSourceFrom;

typedef struct AudioSource {
    AudioSourcePriority priority;
    AudioSourceFrom from;
    union {
        char *url;
        WriteBytesCallback write_bytes;
    } provider;

    void (*on_play_event)(const PlayStateEvent event);

    void (*close)(struct AudioSource *ptr, const size_t id);

    void *action;
} AudioSource, *AudioSourcePtr;

#endif