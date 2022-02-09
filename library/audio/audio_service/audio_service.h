/**
 * @file audio_service.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __AUDIO_SERVICE_H__
#define __AUDIO_SERVICE_H__

#include <stddef.h>
#include <rtthread.h>
#include "config.h"
#include "audio_player/media_player/media_player.h"
#include "audio_player/raw_player/raw_player.h"
#include "library/audio/audio_source/audio_source.h"
#include "ipc/completion.h"

typedef struct {
    size_t id;
    struct rt_mutex lock;
    struct rt_completion ack;
    AudioSourcePtr source_ptr;
} Channel, *ChannelPtr;


typedef struct {
    bool (*init)();

    size_t (*post)(AudioSourcePtr audio_source_ptr);

    void (*play_media)();

    void (*play_raw)();

    void (*pause_media)();

    void (*pause_raw)();

    void (*stop_media)();

    void (*stop_raw)();

    void (*set_media_volume)(const int volume);

    void (*set_raw_volume)(const int volume);

    int (*media_volume)();

    int (*raw_volume)();

} _AudioService;

extern const _AudioService AudioService;

#endif