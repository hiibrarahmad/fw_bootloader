package com.jieli.bootloader.util;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;

import com.jieli.bootloader.MainApplication;
import com.jieli.bootloader.hid.HidClient;

import java.util.Map;


public class UsbHelper extends HidClient {
    private final String tag = getClass().getSimpleName();
    private final UsbManager usbManager;
    @SuppressLint("StaticFieldLeak")
    private static volatile UsbHelper instance = null;

    public static UsbHelper getInstance() {
        if(null == instance) {
            synchronized(UsbHelper.class) {
                if(null == instance) {
                    instance = new UsbHelper();
                }
            }
        }
        return instance;
    }

    private UsbHelper() {
        usbManager = (UsbManager) MainApplication.getContext().getApplicationContext().getSystemService(Context.USB_SERVICE);
        if (usbManager == null) {
            throw new NullPointerException("No Usb service");
        }
    }

    @Override
    protected UsbManager getUsbManager() {
        return usbManager;
    }

    public UsbDevice findDevice(int vid, int pid) {
        Map<String, UsbDevice> devices = usbManager.getDeviceList();
        for (UsbDevice device : devices.values()) {
            if ((vid == 0 || device.getVendorId() == vid) && (pid == 0 || device.getProductId() == pid)) {
                for (int i = 0; i < device.getInterfaceCount(); i++) {
                    UsbInterface usbInterface = device.getInterface(i);
                    if (usbInterface.getInterfaceClass() == Const.INTERFACE_CLASS_HID) {
                        return device;
                    }
                }
            }
        }
        return null;
    }

    public void release() {
        instance = null;
        close();
    }
}
