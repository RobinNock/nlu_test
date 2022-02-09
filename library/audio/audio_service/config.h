//
// Created by 202106 on 2021/11/12.
//

#ifndef _AUDIO_SERVICE_CONFIG_H__
#define _AUDIO_SERVICE_CONFIG_H__

#define AUDIO_SERVICE_LOOP_NAME  "audio_service_background_thread\0"
#define AUDIO_SERVICE_BACKGROUND_MEDIA_NAME "audio_service_background_thread_media\0"
#define AUDIO_SERVICE_BACKGROUND_TTS_NAME "audio_service_background_thread_TTS\0"
#define AUDIO_MEDIA_MUTEX_NAME "audio_media_mutex\0"
#define AUDIO_TTS_MUTEX_NAME "audio_TTS_mutex\0"
#define AUDIO_SERVICE_EVENT_NAME "audio_service_event\0"
#define AUDIO_SERVICE_LOOP_STACK_SIZE  (1024 * 4)
#define AUDIO_SERVICE_LOOP_PRIORITY  10
#define AUDIO_SERVICE_LOOP_TICK  50
#define AUDIO_SERVICE_EVENT_WAITED_TIMEOUT 10
#define AUDIO_SERVICE_MQ_NAME  "audio_service_mq\0"
#define AUDIO_SERVICE_MAILBOX_NAME "audio_service_mailbox\0"
#define AUDIO_SERVICE_MQ_POOL_SIZE 512

#endif //_AUDIO_SERVICE_CONFIG_H__
