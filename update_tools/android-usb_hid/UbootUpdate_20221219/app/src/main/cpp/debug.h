//
// Created by bob on 17-4-24.
//

#ifndef DVFLYING2_DEBUG_H
#define DVFLYING2_DEBUG_H

#ifdef __ANDROID__

#include <android/log.h>
#include <jni.h>

#define logd(TAG, ...)  //((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define logi(TAG, ...)  ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define logw(TAG, ...)  ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define loge(TAG, ...)  ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

#else

#define logi(...)
#define logw(...)
#define loge(...)

#endif

#endif //DVFLYING2_DEBUG_H
