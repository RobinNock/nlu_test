/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Date           Author       Notes
 * 2019-07-15     Zero-Free    first implementation
 */

#ifndef _AUDIO_AUDIO_SERVICE_AUDIO_PLAYER_RAW_PLAYER_PLAYER_INSTANCE_WAV_PLAYER_H__
#define _AUDIO_AUDIO_SERVICE_AUDIO_PLAYER_RAW_PLAYER_PLAYER_INSTANCE_WAV_PLAYER_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "pcm_player.h"
#include "config.h"


/**
 * wav sWavPlayer status
 */
enum PLAYER_STATE {
    PLAYER_STATE_STARTED = 0,
    PLAYER_STATE_PAUSED = 1,
    PLAYER_STATE_STOPPED = 2,
    PLAYER_STATE_FINISHED = 3,
    PLAYER_STATE_FAILED = 4,
};

/**
 * @brief             Stop music
 *
 * @return
 *      - 0      Success
 *      - others Failed
 */
int wav_player_stop(void);

/**
 * @brief             Pause music
 *
 * @return
 *      - 0      Success
 *      - others Failed
 */
int wav_player_pause(void);

/**
 * @brief             Resume music
 *
 * @return
 *      - 0      Success
 *      - others Failed
 */
int wav_player_resume(void);

/**
 * @brief             Sev volume
 *
 * @param volume      volume value(0 ~ 99)
 *
 * @return
 *      - 0      Success
 *      - others Failed
 */
int wav_player_set_volume(int volume);

/**
 * @brief             Get volume
 *
 * @return            volume value(0~00)
 */
int wav_player_volume(void);

/**
 * @brief             Get wav sWavPlayer state
 *
 * @return
 *      - PLAYER_STATE_STOPPED   stoped status
 *      - PLAYER_STATE_STARTED  playing status
 *      - PLAYER_STATE_PAUSED   paused
 */
int wav_player_state(void);

/**
 * @brief             Get the url that is currently playing
 *
 * @return            url that is currently playing
 */
char *wav_player_url(void);

//todo 添加事件监听机制
void wav_player_set_event_callback(void (*callback)(enum PLAYER_STATE status));

//todo 播放链接 目前需本地文件
void wav_player_set_url(const char *url);

int wav_player_play();

int wav_player_init();


#endif
