/**
 * @file device.h
 * @author qr-kou (codinlog@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-22
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __DATA_DEVICE_DEVICE_H__
#define __DATA_DEVICE_DEVICE_H__

#include "library/nlu_handler/common/common.h"
#include <rtthread.h>

typedef struct
{
    char *mac_wifi;
    char *mac_voice;
    char *mid;
} Device, *DevicePtr, *const DeviceCPtr;

typedef struct
{
    Device (*new)(const char *mac_wifi, const char *mac_voice, const char *mid);
    Base base;
} _DeviceManager;

/**
 * @brief
 *    Usage:
 *      create device instance:
 *         const Device device  = DeviceManager.new(mac_wifi,mac_voice,mid);
 *      free device instance:
 *         DeviceManager.base.drop.drop_memery(&device);
 *      convert device to cjson:
 *          DeviceManager.base.json.to_json(&device);
 */
extern const _DeviceManager DeviceManager;
#endif