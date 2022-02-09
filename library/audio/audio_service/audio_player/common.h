//
// Created by 202106 on 2021/11/18.
//

#ifndef _AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_H__
#define _AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_H__

#include "config.h"

typedef enum {
    PLAY_EVENT_READY = 1,
    PLAY_EVENT_START = 2,
    PLAY_EVENT_PAUSE = 3,
    PLAY_EVENT_STOP = 4,
    PLAY_EVENT_FINISH = 5,
    PLAY_EVENT_FAIL = 6,
    PLAY_EVENT_UNKNOWN = 7,
} PlayStateEvent;

typedef void (*PlayEventCallback)(const PlayStateEvent event);

typedef struct {
    bool (*init)();

    void (*register_on_play_event)(const PlayEventCallback callback);

    void (*set_volume)(const int volume);

    int (*volume)();

    void (*set_url)(const char *url);

    void (*play)();

    void (*pause)();

    void (*stop)();

    PlayStateEvent (*status)();
} AudioPlayer;


typedef enum {
    AUDIO_ENCODE_RAW,
    AUDIO_ENCODE_MEDIA,
} AudioEncodeType;

#endif //_AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_H__
