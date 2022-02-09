//
// Created by 202106 on 2021/11/12.
//

#ifndef _AUDIO_MANAGER_CONFIG_H__
#define _AUDIO_MANAGER_CONFIG_H__

#include<stddef.h>
#include "library/audio/audio_service/config.h"
/**
 * 回调函数信号量
 */
#define PLAY_EVENT_CALLBACK_SEM_NAME "audio_manager_init_sem\0"
#define MAX_PLAY_EVENT_CALLBACK_SIZE  3

#endif
