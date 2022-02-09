//
// Created by 202106 on 2021/11/23.
//

#ifndef _AUDIO_AUDIO_SERVICE_RAW_PLAYER_PLAYER_INSTANCE_CONFIG_H__
#define _AUDIO_AUDIO_SERVICE_RAW_PLAYER_PLAYER_INSTANCE_CONFIG_H__

#define PCM_DEFAULT_SAMPLE_RATE 16000
#define PCM_DEFAULT_CHANNEL 2
#define PCM_DEFAULT_SAMPLE_BITS 2
#define PCM_DEFAULT_VOLUME 60
#define PCM_PLAYER_LOOP_THREAD_NAME "pcm_player_loop_thread\0"
#define PCM_AUDIO_MEM_POOL_NAME "pcm_audio_mem_pool\0"
#define PCM_AUDIO_MEM_POOL_BLOCK_SIZE 1024
#define PCM_AUDIO_MEM_POOL_BLOCK_COUNT 4
#define PCM_AUDIO_MQ_NAME "pcm_audio_mq\0"
#define PCM_AUDIO_MQ_MAX_COUNT 8
#define PCM_AUDIO_PLAY_EVENT_NAME "pcm_audio_play_event\0"
#define PCM_PLAYER_THREAD_STACK_SIZE (1024 * 4)
#define PCM_PLAYER_THREAD_PRIORITY 15
#define PCM_PLAYER_THREAD_TICK 50
#define PCM_PLAYER_DEVICE_HARDWARE_NAME "pcm1\0"
#define PCM_PLAYER_MAX_VOLUME 99
#define PCM_PLAYER_MIN_VOLUME 0
#define PCM_PLAYER_ON_PLAYE_EVENT_MUTEX_NAME "pcm_play_event_mutex\0"
#define VOLUME_MIN 0
#define VOLUME_MAX 99
#define WAV_PLAYER_BUFFER_SIZE 2048
#define WAV_PLAYER_VOLUME_DEFAULT 55
#define WAV_PLAYER_THREAD_STACK_SIZE (1024 * 6)
#define WAV_PLAYER_THREAD_PRIORITY 15


#endif
