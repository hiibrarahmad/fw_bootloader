package com.jieli.bootloader.util;

/**
 * Des:用户授权打开的USB设备信息
 * Author: Bob
 * Date:22-12-13
 * UpdateRemark:
 */
public class Const {
    /**
     * 接口描述符的InterfaceClass域为0x03
     */
    public static final int INTERFACE_CLASS_HID = 3;

    /**
     * Usb dongle每次交互数据最大是64字节
     */
    public static final int MAX_SIZE_TRANSFER = 64;

    /**
     * USB发送超时时间
     */
    public static final int DEFAULT_TIMEOUT = 1000;
}
