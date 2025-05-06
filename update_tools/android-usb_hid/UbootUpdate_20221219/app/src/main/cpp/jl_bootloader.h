/**
 * Des: 
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 */

#ifndef JLBOOTLOADER_JL_BOOTLOADER_H
#define JLBOOTLOADER_JL_BOOTLOADER_H

#include <cstdint>
#include <vector>
#include "jl_bootloader_jni.h"


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define HID_RW_MAX 64 // HID read/write max length
//上位机结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u32 sdk_id;
        } __attribute__((packed)) device_check;

        struct {
            u32 address;        // 烧写FLASH地址
            u32 data_length;    //烧写长度
            u8 data[0];        // 烧写数据
        } __attribute__((packed)) write;

        struct {
            u32 address;    //需要对齐
            u32 type;
        } __attribute__((packed)) erase;

        struct {
            u32 address;
            u32 len;
            u32 block_size;
        } __attribute__((packed)) crc_list;

        struct {
            char file_name[16];
            u8 mode;
        } __attribute__((packed)) init;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_ITEM;

//设备结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u16 crc[0];
        } __attribute__((packed)) crc_list;

        struct {
            u32 upgrade_addr;
            u32 upgrade_len;
            u32 upgrade_eoffset;
            u32 flash_alignsize;
        } __attribute__((packed)) init;

        struct {
            u8 vid[4];
            u8 pid[16];
            u32 sdk_id;
        } __attribute__((packed)) device_check;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_DEV_ITEM;

struct DeviceInitInfo {
    uint32_t zoneAddr;
    uint32_t upgradeLen;
    uint32_t flashEoffsetSize;
    uint32_t eraseUnitSize;
};

class JL_Bootloader {
    u8 hid_tx_buf[HID_RW_MAX];
//    u8 hid_rx_buf[64];
//    int readSize;
    JL_SECTOR_COMMAND_DEV_ITEM dev_cmd_ops;

    uint16_t crc16(const void *_ptr, uint32_t len);
    uint16_t *getBufferCrcList(const char *fileBuf, uint32_t blockCount, uint32_t eraseUnitSize);
    void decryptCommand(uint8_t *buf, uint32_t len);
    int readData(JL_SECTOR_COMMAND_DEV_ITEM *commandDevItem);
    int deviceUpdateZoneInit(const char *name, int mode, DeviceInitInfo &info);

    bool isReplyFlashCrcCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus, uint32_t blockCount, uint16_t *crcList);
    bool fetchChipCrcList(uint32_t address, uint32_t blockCount, uint32_t blockSize, uint16_t *crcList);
    bool eraseFlash(uint32_t address, uint32_t len);
    bool doEraseFlash(uint32_t address, uint32_t len);
    bool doWriteFlash(const void *buffer, uint32_t address, uint32_t len);
    bool writeFlash(const void *_buffer, uint32_t address, uint32_t len, uint32_t flash_erase_unit);
    void reboot();
    bool ensureCrcMatch(const uint16_t *fileCrcList, uint32_t addr, uint32_t blockCount, uint32_t eraseUnitSize);

    // command api
    void makeCommand(uint8_t opcode, const std::vector <std::pair<const void *, uint32_t>> &bodys);
    void makeDeviceInitCmd(const char *area, uint8_t mode);
    bool isReplyEraseCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus);
    bool isReplyWriteCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus);
    void makeWriteCmd(uint32_t address, uint32_t len, const void *content);

protected:

public:
    u8 hid_rx_buf[HID_RW_MAX];
    int readSize;
    BootloaderContext *context;
    int startUpgrading(const std::vector<char> &fileData);
};


#endif //JLBOOTLOADER_JL_BOOTLOADER_H
