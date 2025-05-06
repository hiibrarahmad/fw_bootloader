/**
 * Des: 
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 */
#include "jl_bootloader_jni.h"
#include "jl_bootloader.h"
#include "debug.h"
#include <jni.h>
#include <vector>

#define tag "loader_jni"

static JavaVM *gJVM = nullptr;

extern "C"
JNIEXPORT jlong JNICALL
Java_com_jieli_bootloader_hid_HidClient_nativeInit(JNIEnv *env, jobject thiz) {
    logi(tag, "nativeInit");
    auto *context = new BootloaderContext();
    jobject obj = env->NewGlobalRef(thiz);
    context->hidClientObj = obj;
    env->GetJavaVM( &gJVM);
    context->jlBootloader = new JL_Bootloader();
    context->jlBootloader->context = context;
    return reinterpret_cast<jlong>(context);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jieli_bootloader_hid_HidClient_nativeStartUpgrade(JNIEnv *env, jobject thiz, jlong jaddress,
                                                           jbyteArray jfile_data) {
    logi(tag, "nativeStartUpgrade");
    auto *context = reinterpret_cast<BootloaderContext *>(jaddress);
    if (nullptr == context) return JNI_FALSE;
    if (jfile_data == nullptr) return JNI_FALSE;
    jsize data_length = env->GetArrayLength(jfile_data);

    auto *fileData = env->GetByteArrayElements(jfile_data, nullptr);
    std::vector<char> v_data(fileData, fileData + data_length);

    int ret = context->jlBootloader->startUpgrading(v_data);
    env->ReleaseByteArrayElements(jfile_data, fileData, 0);
    return ((ret == 0) ? JNI_TRUE : JNI_FALSE);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jieli_bootloader_hid_HidClient_nativeDestroy(JNIEnv *env, jobject thiz, jlong jaddress) {
    logi(tag, "nativeDestroy");
    auto *context = reinterpret_cast<BootloaderContext *>(jaddress);
    if (nullptr == context) return ;
    if (context->hidClientObj) env->DeleteGlobalRef(context->hidClientObj);
    delete context->jlBootloader;
    delete context;
}

bool BootloaderContext::readHid() const {
//    loge(tag, "readHid........1");
    JNIEnv *env = nullptr;
    jboolean isAttached = JNI_FALSE;
    if ((gJVM)->GetEnv((void **) &env, JNI_VERSION_1_6) < 0)
    {
        if ((gJVM)->AttachCurrentThread(&env, nullptr) < 0)
            return false;
        isAttached = JNI_TRUE;
    }

    if (env == nullptr) return false;
    if (hidClientObj == nullptr) return false;
    jclass clazz = env->GetObjectClass(hidClientObj);

    jmethodID readId = env->GetMethodID(clazz, "readFromHid", "()[B");
    auto jdata = (jbyteArray)(env->CallObjectMethod(hidClientObj, readId));

    if (jdata == nullptr) {
        loge(tag, "jdata is null");
        if (isAttached) (gJVM)->DetachCurrentThread();
        return false;
    }

    int length = env->GetArrayLength(jdata);
    auto *data = reinterpret_cast<char *>(env->GetByteArrayElements(jdata, nullptr));
    if (data == nullptr) return false;

    bool result = true;
    if (length <= HID_RW_MAX) {
        jlBootloader->readSize = length;
        memcpy(jlBootloader->hid_rx_buf, data, length);
    } else{
        loge(tag, "Too large to copy!");
        result = false;
    }

    env->ReleaseByteArrayElements(jdata, reinterpret_cast<jbyte *>(data), 0);
    if (isAttached)
        (gJVM)->DetachCurrentThread();
    return result;
}

void BootloaderContext::writeHid(char *buf, int size) const {
    JNIEnv *env = nullptr;
    jboolean isAttached = JNI_FALSE;
    if ((gJVM)->GetEnv((void **) &env, JNI_VERSION_1_6) < 0)
    {
        if ((gJVM)->AttachCurrentThread(&env, nullptr) < 0)
            return ;
        isAttached = JNI_TRUE;
    }

    jclass clazz = env->GetObjectClass(hidClientObj);
    jmethodID writeId = env->GetMethodID(clazz, "writeToHid", "([B)V");
    jbyteArray jData = env->NewByteArray(size);
    env->SetByteArrayRegion(jData, 0, size, reinterpret_cast<const jbyte *>(buf));
    env->CallVoidMethod(hidClientObj, writeId, jData);
    env->DeleteLocalRef(jData);
    if (isAttached)
        (gJVM)->DetachCurrentThread();
}
