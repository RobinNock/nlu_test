//
// Created by 202106 on 2021/11/18.
//

#ifndef _AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_RAW_PLAYER_CONFIG_H__
#define _AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_RAW_PLAYER_CONFIG_H__

#define  AUDIO_WAV_PLAYER_ON_PLAY_EVENT_MAILBOX_SIZE 8
#define  AUDIO_WAV_PLAYER_ON_PLAY_EVENT_CALLBACK_MUTEX_NAME "wav_mutex\0"
#define  AUDIO_WAV_PLAYER_ON_PLAY_EVENT_MQ "wav_mq\0"
#define AUDIO_RAW_PLAYER_ON_PLAY_LOOP_NAME "audio_raw_player_event_loop\0"
#define AUDIO_RAW_PLAYER_ON_PLAY_LOOP_STACK_SIZE 512
#define AUDIO_RAW_PLAYER_ON_PLAY_LOOP_PRIORITY 10
#define AUDIO_RAW_PLAYER_ON_PLAY_LOOP_TICK 20
#endif //_AUDIO_AUDIO_SERVICE_PLAYER_WRAPPER_RAW_PLAYER_CONFIG_H__
