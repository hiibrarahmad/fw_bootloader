/**
 * Des: 
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 */

#include "jl_bootloader.h"
#include "debug.h"
#include "jl_bootloader_jni.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <android/log.h>
#include <jni.h>
#include <unistd.h>

#define tag "jl_loader"

#define JL_SU_CMD_DEVICE_INIT 0xC0
#define JL_SU_CMD_DEVICE_CHECK 0xC1
#define JL_SU_CMD_ERASE 0xC2
#define JL_SU_CMD_WRITE 0xC3
#define JL_SU_CMD_FLASH_CRC 0xC4
#define JL_SU_CMD_EX_KEY 0xC5
#define JL_SU_CMD_REBOOT 0xCA

/* reply code */
#define JL_SU_RSP_SUCC 0x0
#define JL_SU_RSP_CRC_ERROR 0x1
#define JL_SU_RSP_SDK_ID_ERROR 0x2
#define JL_SU_RSP_OTHER_ERROR 0x3

#define JL_ERASE_TYPE_PAGE 1
#define JL_ERASE_TYPE_SECTOR 2
#define JL_ERASE_TYPE_BLOCK 3

#define JL_MAX_CRC_LIST_COUNT 16
#define JL_MAX_WRITE_SIZE 1024

#define JL_USB_VID 0X4c4a
#define JL_USB_PID 0X4155

#define SYNC_MARK0 0xAA
#define SYNC_MARK1 0x55

//10进制，注意要与上位机一致，不然无法升级
static uint32_t communication_key = 12345678;

uint16_t JL_Bootloader::crc16(const void *_ptr, uint32_t len) {
    const auto *ptr = (const uint8_t *) _ptr;
    uint16_t crc_ta[16] = {
            0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
            0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    };
    uint16_t crc;
    uint8_t da;

    crc = 0;
    while (len-- != 0) {
        da = crc >> 12;
        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr / 16)];

        da = crc >> 12;

        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr & 0x0f)];
        ptr++;
    }

    return (crc);
}

uint16_t *JL_Bootloader::getBufferCrcList(const char *fileBuf, uint32_t blockCount, uint32_t eraseUnitSize) {
    auto ubootFileCrcList = new uint16_t[blockCount];

    for (uint32_t offset = 0; offset < blockCount; offset++) {
        ubootFileCrcList[offset] = crc16(fileBuf + offset * eraseUnitSize, eraseUnitSize);
    }
    return ubootFileCrcList;
}

void JL_Bootloader::decryptCommand(uint8_t *buf, uint32_t len) {
    uint32_t keyIdx = 0;
    auto *encKey = (uint8_t * ) & communication_key;

    for (uint32_t i = 0; i < len; ++i, ++keyIdx) {
        buf[i] ^= encKey[keyIdx & 0x3];
    }
}

//static u8 hid_rx_buf[65];
int JL_Bootloader::readData(JL_SECTOR_COMMAND_DEV_ITEM *commandDevItem) {
    commandDevItem->rsp_status = 0xff;
    memset(commandDevItem, 0xff, sizeof(JL_SECTOR_COMMAND_DEV_ITEM));
    u32 timeout = 3000;
    while (timeout--)
    {
        bool isOK = context->readHid();
        int r = readSize;
//        logi(tag, "readSize =%d", r);
        if (r >= HID_RW_MAX && isOK) {
//            memcpy(hid_rx_buf, data, r);
            decryptCommand(hid_rx_buf + 4, 60);
            memcpy(commandDevItem, hid_rx_buf, sizeof(JL_SECTOR_COMMAND_DEV_ITEM));
            logw(tag, "receive cmd=0x%x, cmd len=%d", commandDevItem->cmd, commandDevItem->cmd_len);
            break;
        }
        usleep(1000);// sleep 1 milliseconds
    }
    return 0;
}

//static u8 hid_tx_buf[65];
void JL_Bootloader::makeCommand(uint8_t opcode, const std::vector <std::pair<const void *, uint32_t>> &bodys) {
    uint32_t bodyLen = 0;
    for (auto &v: bodys) {
        bodyLen += v.second;
    }

    auto totalLen = 2 + 2 + 2 + bodyLen + 2;
//    hid_tx_buf[0] = 0;
//    auto p = &hid_tx_buf[1];
    auto p = &hid_tx_buf[0];
    p[0] = 0xAA;
    p[1] = 0x55;
    p[2] = ((bodyLen + 2) >> 0) & 0xFF;
    p[3] = ((bodyLen + 2) >> 8) & 0xFF;
    p[4] = opcode;
    p[5] = JL_SU_RSP_SUCC;

    auto bodyPtr = &p[6];
    for (auto &v: bodys) {
        memcpy(bodyPtr, v.first, v.second);
        bodyPtr += v.second;
    }

    // we encrypt now
    auto *theKey = (uint8_t * ) & communication_key;
    uint32_t keyIdx = 0;
    for (unsigned i = 4; i < totalLen - 2; ++i, keyIdx++) {
        p[i] ^= theKey[keyIdx & 0x3];
    }

    auto crc = crc16(p, 2 + 2 + 2 + bodyLen);
    p[2 + 2 + 2 + bodyLen + 0] = (crc >> 0) & 0xFF;
    p[2 + 2 + 2 + bodyLen + 1] = (crc >> 8) & 0xFF;

    if (context != nullptr) {
        context->writeHid(reinterpret_cast<char *>(hid_tx_buf), HID_RW_MAX);
    }
}

void JL_Bootloader::makeDeviceInitCmd(const char *area, uint8_t mode) {
    char buf[16 + 1];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, area, 15);
    buf[16] = (char)mode;
    makeCommand(JL_SU_CMD_DEVICE_INIT, {{buf, sizeof(buf)}});
}

int JL_Bootloader::deviceUpdateZoneInit(const char *name, int mode, DeviceInitInfo &info) {
    logi(tag, "send device init cmd: %s %d", name, mode);

    makeDeviceInitCmd(name, mode);

    readData(&dev_cmd_ops);

    if (dev_cmd_ops.rsp_status == JL_SU_RSP_SUCC) {
        logi(tag, "rsp succ"); //
        logi(tag, "upgrade_addr:0x%x", dev_cmd_ops.init.upgrade_addr);
        logi(tag, "upgrade_len: %d", dev_cmd_ops.init.upgrade_len);
        logi(tag, "flash_alignsize: %d", dev_cmd_ops.init.flash_alignsize);
        logi(tag, "upgrade_eoffset: %d", dev_cmd_ops.init.upgrade_eoffset);
        info.eraseUnitSize = dev_cmd_ops.init.flash_alignsize;
        info.zoneAddr = dev_cmd_ops.init.upgrade_addr;
        info.flashEoffsetSize = dev_cmd_ops.init.upgrade_eoffset;
        info.upgradeLen = dev_cmd_ops.init.upgrade_len;
    } else {
        loge(tag, "rsp error:%d", dev_cmd_ops.rsp_status);
    }
    return 0;
}

bool JL_Bootloader::isReplyFlashCrcCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus, uint32_t blockCount,
                        uint16_t *crcList) {
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();
    if (uint8_t(bin[0]) != JL_SU_CMD_FLASH_CRC || uint8_t(bin[1]) != JL_SU_RSP_SUCC) {
        rspStatus = uint8_t(bin[1]);
        return false;
    }
    memcpy(crcList, bin.data() + 2, blockCount * 2);
    return true;
}

bool JL_Bootloader::fetchChipCrcList(uint32_t address, uint32_t blockCount, uint32_t blockSize, uint16_t *crcList) {

    while (blockCount > 0) {
        uint32_t readCount = blockCount > JL_MAX_CRC_LIST_COUNT ? JL_MAX_CRC_LIST_COUNT : blockCount;
        int len = (int)(readCount * blockSize);

        makeCommand(JL_SU_CMD_FLASH_CRC, {{&address,   4},
                                          {&len,       4},
                                          {&blockSize, 4}});

        readData(&dev_cmd_ops);
        std::vector<uint8_t> cmd;
        cmd.resize(2 + dev_cmd_ops.cmd_len);
        memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);
        uint8_t rspStatus;
        // parse result
        if (!isReplyFlashCrcCmd(cmd, rspStatus, readCount, crcList)) {
            loge(tag, "[Error, invalid reply, rspStatus %d]", uint32_t(rspStatus));
            return false;
        }

        address += readCount * blockSize;
        blockCount -= readCount;
        crcList += readCount;
    }

    return true;
}

bool JL_Bootloader::isReplyEraseCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus) {
    //qDebug() << __func__ << bin.toHex() << ", size " << bin.size();

    if (uint8_t(bin[0]) != JL_SU_CMD_ERASE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}

bool JL_Bootloader::doEraseFlash(uint32_t address, uint32_t len) {
    uint32_t type = 0;
    if (len == 64 * 1024) {
        type = JL_ERASE_TYPE_BLOCK;
    } else if (len == 4096) {
        type = JL_ERASE_TYPE_SECTOR;
    } else if (len == 256) {
        type = JL_ERASE_TYPE_PAGE;
    }
    makeCommand(JL_SU_CMD_ERASE, {{&address, 4}, {&type,    4}});
    readData(&dev_cmd_ops);
    std::vector<uint8_t> cmd;
    cmd.resize(2 + dev_cmd_ops.cmd_len);
    memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);

    uint8_t rspStatus;
    if (!isReplyEraseCmd(cmd, rspStatus)) {
        loge(tag, "invalid erase reply");
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        loge(tag, "erase reply %d", rspStatus);
        return false;
    }
    return true;
}

bool JL_Bootloader::eraseFlash(uint32_t address, uint32_t len) {
    if (len == 256) {
        return doEraseFlash(address, len);
    } else {
        while (len) {
            if (!doEraseFlash(address, 4096)) {
                return false;
            }
            address += 4096;
            len -= 4096;
        }
    }
    return true;
}

void JL_Bootloader::makeWriteCmd(uint32_t address, uint32_t len, const void *content) {
    return makeCommand(JL_SU_CMD_WRITE, {{&address, 4}, {&len,     4},
                                         {content,  len}});
}

bool JL_Bootloader::isReplyWriteCmd(const std::vector<uint8_t> &bin, uint8_t &rspStatus) {
    if (uint8_t(bin[0]) != JL_SU_CMD_WRITE) {
        return false;
    }
    rspStatus = uint8_t(bin[1]);
    return true;
}

bool JL_Bootloader::doWriteFlash(const void *buffer, uint32_t address, uint32_t len) {
    makeWriteCmd(address, len, buffer);

    readData(&dev_cmd_ops);

    std::vector<uint8_t> cmd;
    cmd.resize(2 + dev_cmd_ops.cmd_len);
    memcpy(cmd.data(), hid_rx_buf + 4, 2 + dev_cmd_ops.cmd_len);

    uint8_t rspStatus;
    if (!isReplyWriteCmd(cmd, rspStatus)) {
        loge(tag, "invalid write reply");
        return false;
    }
    if (rspStatus != JL_SU_RSP_SUCC) {
        loge(tag, "write reply %d", rspStatus);
        return false;
    }
    return true;
}

bool JL_Bootloader::writeFlash(const void *_buffer, uint32_t address, uint32_t len, uint32_t flash_erase_unit) {
    const uint32_t max_write_size = 64 - sizeof(JL_SECTOR_COMMAND_ITEM);
    const auto *buffer = (const uint8_t *) _buffer;
    while (len > 0) {
        uint32_t writeLen = len > max_write_size ? max_write_size : len;
        if (!doWriteFlash(buffer, address, writeLen)) {
            return false;
        }
        address += writeLen;
        buffer += writeLen;
        len -= writeLen;
//        logi(tag, "after write flash: len =%d, writeLen =%d", len, writeLen);
    }
    return true;
}

bool JL_Bootloader::ensureCrcMatch(const uint16_t *fileCrcList, uint32_t addr, uint32_t blockCount, uint32_t eraseUnitSize) {
    auto ubootChipCrcList = new uint16_t[blockCount];
    // check if we write to backup area successfully
    if (!fetchChipCrcList(addr, blockCount, eraseUnitSize, ubootChipCrcList)) {
        delete[] ubootChipCrcList;
        return false;
    }

    if (memcmp(ubootChipCrcList, fileCrcList, 2 * blockCount) != 0) {
        loge(tag, "after uboot write 64K, crc mismatch");
        delete[] ubootChipCrcList;
        return false;
    }
    delete[] ubootChipCrcList;
    return true;
}

void JL_Bootloader::reboot() {
    return makeCommand(JL_SU_CMD_REBOOT, {});
}

//int main(int argc, char *argv[]) {
int JL_Bootloader::startUpgrading(const std::vector<char> &fileData) {

    DeviceInitInfo app{};
    deviceUpdateZoneInit("app_dir_head", 0, app);

    uint32_t appZoneAddr = app.zoneAddr;
    uint32_t flashEoffsetSize = app.flashEoffsetSize;
    uint32_t eraseUnitSize = app.eraseUnitSize;

    std::vector<char> fileContent = fileData;

    uint32_t fileSize = fileContent.size() - appZoneAddr;
    uint32_t blockCount = (fileSize + eraseUnitSize - 1) / eraseUnitSize;
    uint32_t alignedFileSize = blockCount * eraseUnitSize;
    if (alignedFileSize != fileSize) {
        loge(tag, "aligned file size %d != file size %d", alignedFileSize, fileSize);
//        fileContent.append(alignedFileSize - fileSize, '\xFF');
        std::fill_n(fileContent.end(), alignedFileSize - fileSize, '\xFF');
    }
    const char *fileBuf = reinterpret_cast<const char *>(fileContent.data() + appZoneAddr);

    auto fileCrcList = getBufferCrcList(fileBuf, blockCount, eraseUnitSize);

    auto chipCrcList = new uint16_t[blockCount];

    // read crc in chip
    if (!fetchChipCrcList(0 + flashEoffsetSize, blockCount, eraseUnitSize, chipCrcList)) {
        loge(tag, "error: fetch Chip Crc List");
        goto __reboot;
    }

    if (memcmp(fileCrcList, chipCrcList, 2 * blockCount) == 0) {
        // same crc, no need to upgrade
        loge(tag, "same crc, no need to upgrade");
        goto __reboot;
    }

    {
        auto upgradeAddr = 0 + flashEoffsetSize;
        if (!eraseFlash(upgradeAddr, eraseUnitSize)) {
            loge(tag, "error: erase Flash");
            goto error_output;
        }

        for (uint32_t i = 1; i < blockCount; ++i) {
            uint32_t off = i * eraseUnitSize;
            if (fileCrcList[i] != chipCrcList[i]) {
                logw(tag, "should erase %d, off =%d, size =%d", i, off, eraseUnitSize);
                if (!eraseFlash(upgradeAddr + off, eraseUnitSize)) {
                    goto error_output;
                }
                if (!writeFlash(fileBuf + off, upgradeAddr + off, eraseUnitSize, eraseUnitSize)) {
                    goto error_output;
                }
            }
        }

        // and finally write first unit
        if (!writeFlash(fileBuf + 0 * eraseUnitSize, upgradeAddr + 0 * eraseUnitSize,
                        eraseUnitSize,eraseUnitSize)) {
            loge(tag, "error: write flash");
            goto error_output;
        }

        if (!ensureCrcMatch(fileCrcList, 0 + flashEoffsetSize, blockCount, eraseUnitSize)) {
            loge(tag, "update failed, crc mismatch");
            goto error_output;
        }
    }

__reboot:
    // ask for reboot
    logi(tag, "update ok");
    logi(tag, "send reboot");
    reboot();
    delete[]  chipCrcList;
    delete[]  fileCrcList;

    return 0;

error_output:
    logi(tag, "update failed");
    delete[]  chipCrcList;
    delete[]  fileCrcList;
    return -1;
}