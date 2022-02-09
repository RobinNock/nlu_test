//
// Created by 202106 on 2021/11/23.
//

#ifndef _AUDIO_AUDIO_SERVICE_RAW_PLAYER_PLAYER_INSTANCE_PCM_H__
#define _AUDIO_AUDIO_SERVICE_RAW_PLAYER_PLAYER_INSTANCE_PCM_H__

#define DBG_TAG "PCM_PLAYER_TAG"
#define DBG_LVL DBG_LOG

#include <rtthread.h>
#include <rtdevice.h>
#include <stdbool.h>
#include <rtdbg.h>

#include "config.h"
#include "common.h"

void pcm_player_play();

void pcm_player_pause();

void pcm_player_stop();

void pcm_player_set_volume(int volume);

int pcm_player_volume();

void pcm_player_config(int sample_rate, int channel, int sample_bits);

void pcm_player_register_on_play_event(OnPcmPlayerEventCallback callback);

bool pcm_player_init();

void pcm_player_clear();

AllocBytes pcm_player_alloc_bytes();

rt_err_t pcm_player_write_bytes(AllocBytes allocated);

#endif //_AUDIO_AUDIO_SERVICE_RAW_PLAYER_PLAYER_INSTANCE_PCM_H__
