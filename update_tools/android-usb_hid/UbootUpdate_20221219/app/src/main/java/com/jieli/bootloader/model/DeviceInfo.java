package com.jieli.bootloader.model;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;

import com.jieli.bootloader.util.Const;

/**
 * Des:用户授权打开的USB设备信息
 * Author: Bob
 * Date:22-12-13
 * UpdateRemark:
 */
public class DeviceInfo {

    private UsbDevice usbDevice;
    private UsbInterface usbInterface;
    private UsbDeviceConnection usbDeviceConnection;
    private UsbEndpoint inUsbEndpoint;
    private UsbEndpoint outUsbEndpoint;
    private int timeout = Const.DEFAULT_TIMEOUT;   //设置超时

    public UsbDevice getUsbDevice() {
        return usbDevice;
    }

    public void setUsbDevice(UsbDevice usbDevice) {
        this.usbDevice = usbDevice;
    }

    public UsbInterface getUsbInterface() {
        return usbInterface;
    }

    public void setUsbInterface(UsbInterface usbInterface) {
        this.usbInterface = usbInterface;
    }

    public UsbDeviceConnection getUsbDeviceConnection() {
        return usbDeviceConnection;
    }

    public void setUsbDeviceConnection(UsbDeviceConnection usbDeviceConnection) {
        this.usbDeviceConnection = usbDeviceConnection;
    }

    public UsbEndpoint getInUsbEndpoint() {
        return inUsbEndpoint;
    }

    public void setInUsbEndpoint(UsbEndpoint inUsbEndpoint) {
        this.inUsbEndpoint = inUsbEndpoint;
    }

    public UsbEndpoint getOutUsbEndpoint() {
        return outUsbEndpoint;
    }

    public void setOutUsbEndpoint(UsbEndpoint outUsbEndpoint) {
        this.outUsbEndpoint = outUsbEndpoint;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setTimeout(int timeout) {
        if (timeout < 0) timeout = Const.DEFAULT_TIMEOUT;
        this.timeout = timeout;
    }
}
