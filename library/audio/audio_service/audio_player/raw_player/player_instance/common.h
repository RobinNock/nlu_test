//
// Created by 202106 on 2021/11/29.
//

#ifndef RTTHREAD_COMMON_H
#define RTTHREAD_COMMON_H

typedef enum {
    ALLOC_BYTES_FLAG_OK,
    ALLOC_BYTES_FLAG_ERROR,
    ALLOC_BYTES_LAST_LAST,
} AllocBytesFlag;

typedef struct {
    AllocBytesFlag flag;
    size_t size;
    char *bytes;
} AllocBytes;

typedef enum {
    PCM_PLAY_START = 1 << 2,
    PCM_PLAY_PAUSE = 1 << 4,
    PCM_PLAY_STOP = 1 << 6,
    PCM_PLAY_FINISH = 1 << 8,
    PCM_PLAY_TIMEOUT = 1 << 10,
    PCM_PLAY_ERROR = 1 << 12,
} PcmPlayEvent;

typedef void(*OnPcmPlayerEventCallback)(PcmPlayEvent e);

#endif //RTTHREAD_COMMON_H
