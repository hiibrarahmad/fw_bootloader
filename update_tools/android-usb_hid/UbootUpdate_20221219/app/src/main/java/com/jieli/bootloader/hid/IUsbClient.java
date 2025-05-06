package com.jieli.bootloader.hid;

import android.hardware.usb.UsbDevice;

import com.jieli.bootloader.cb.Callback;
import com.jieli.bootloader.model.ErrorCode;

/**
 * Des:定义USB设备常用接口
 * Author: Bob
 * Date:22-12-13
 * UpdateRemark:
 */
public interface IUsbClient {
    void open(UsbDevice device, Callback<ErrorCode> callback);
    void close();
    boolean tryToUpgrade(byte[] firmware);
//    void setEventListener();
}
