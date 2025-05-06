/**
 * Des: 
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 */

#ifndef JLBOOTLOADER_JL_BOOTLOADER_JNI_H
#define JLBOOTLOADER_JL_BOOTLOADER_JNI_H

#include <cstdint>
#include <jni.h>
class JL_Bootloader;
class BootloaderContext {


public:
    jobject hidClientObj;
    JL_Bootloader *jlBootloader;
    bool readHid() const;
    void writeHid(char *buf, int size) const;
};



#endif //JLBOOTLOADER_JL_BOOTLOADER_JNI_H
