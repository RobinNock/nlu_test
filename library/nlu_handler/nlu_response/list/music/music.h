/**
 * @file music.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief 音乐
 * @version 0.1
 * @date 2021-10-26
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __NLU_RESPONSE_LIST_MUSIC_H__
#define __NLU_RESPONSE_LIST_MUSIC_H__

#include <cJSON.h>
#include <rtthread.h>

#include "library/nlu_handler/nlu_response/util.h"

typedef struct
{
    char *url;
    char *singer;
    char *song;
    char *image;
    char *album;
    char *song_id;
} Music, *MusicPtr, *const MusicCPtr;

typedef struct
{
    Music (*from_cjson)(const cJSON *cjson);
    MusicCPtr (*from_cjson_to_cptr)(const cJSON *cjson);
    void (*drop_memory)(const MusicPtr music_ptr);
    void (*drop_memory_and_self)(const MusicPtr music_ptr);
    void (*println)(const MusicPtr music_ptr);
} _MusicManager;

extern const _MusicManager MusicManager;

#endif //