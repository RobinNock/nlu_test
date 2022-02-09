/**
 * @file audio_manager.c
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-11-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "audio_manager.h"
#include <rtthread.h>


static rt_sem_t sPlayEventCallbacksSem = NULL;

static PlayEventCallback sPlayEventCallbacks[MAX_PLAY_EVENT_CALLBACK_SIZE] = {NULL, NULL, NULL};

static void init() {
    sPlayEventCallbacksSem = rt_sem_create(PLAY_EVENT_CALLBACK_SEM_NAME, 1, RT_IPC_FLAG_FIFO);
    AudioService.init();
}

const _AudioManager AudioManager = {
        .init = init,
};