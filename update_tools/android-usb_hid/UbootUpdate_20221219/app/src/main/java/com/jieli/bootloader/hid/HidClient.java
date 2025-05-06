package com.jieli.bootloader.hid;

import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.util.Log;

import com.jieli.bootloader.cb.Callback;
import com.jieli.bootloader.model.DeviceInfo;
import com.jieli.bootloader.model.ErrorCode;
import com.jieli.bootloader.util.Const;

import java.util.Arrays;

/**
 * Des:
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 * Modify date:
 * Modified by:
 */
public abstract class HidClient implements IUsbClient {
    private final String tag = getClass().getSimpleName();

    static {
        System.loadLibrary("native-lib");
    }

    private long nativeAddress;
    private DeviceInfo hidDevice;

    private native long nativeInit();
    private native boolean nativeStartUpgrade(long address, byte[] fileData);
    private native void nativeDestroy(long address);
    private final byte[] buffer = new byte[Const.MAX_SIZE_TRANSFER];

    private synchronized byte[] readFromHid() {
        if (hidDevice == null || hidDevice.getInUsbEndpoint() == null) {
            Log.e(tag, "read hid object is null");
            return null;
        }
        int bytesRead = hidDevice.getUsbDeviceConnection().bulkTransfer(hidDevice.getInUsbEndpoint(),
                buffer, buffer.length, hidDevice.getTimeout());
        if (bytesRead > 0) {
            return Arrays.copyOf(buffer, bytesRead);
        } else {
            Log.e(tag, "No read data:" + bytesRead);
            return null;
        }
    }

    private synchronized void writeToHid(byte[] data) {
        int sendSize = hidDevice.getUsbDeviceConnection().bulkTransfer(hidDevice.getOutUsbEndpoint(),
                data, data.length, hidDevice.getTimeout());
//        Log.i(tag, "java writeHid="  + sendSize);
    }

    protected abstract UsbManager getUsbManager();

    public HidClient() {
        nativeAddress = nativeInit();
        if (nativeAddress == 0) {
            Log.e(tag, "Native error");
        }
    }

    @Override
    protected void finalize() {
        if (nativeAddress != 0) {
            close();
        }
    }

    @Override
    public void open(UsbDevice device, Callback<ErrorCode> callback) {
        if (null != device) {
            openDevice(device, callback);
        } else {
            onUsbFailureCallback(callback, ErrorCode.builder().setCode(ErrorCode.USB).setMessage("UsbDevice cannot be null"));
        }
    }

    @Override
    public void close() {
        if (hidDevice != null && hidDevice.getUsbDeviceConnection() != null) {
            hidDevice.getUsbDeviceConnection().close();
            hidDevice = null;
        }
        if (nativeAddress != 0) {
            nativeDestroy(nativeAddress);
            nativeAddress = 0;
        }
    }

    @Override
    public boolean tryToUpgrade(byte[] firmware) {
        if (hidDevice == null || hidDevice.getOutUsbEndpoint() == null) {
            //TODO use the control endpoint
            Log.e(tag, "hid is null");
        } else {
            return nativeStartUpgrade(nativeAddress, firmware);
        }
        return false;
    }

    private void openDevice(UsbDevice usbDevice, Callback<ErrorCode> callback) {
        UsbDeviceConnection connection = getUsbManager().openDevice(usbDevice);
        if (connection == null) {
            onUsbFailureCallback(callback, new ErrorCode(ErrorCode.USB, "Open failed"));
            return ;
        }

        UsbInterface usbInterface = getUsbInterface(usbDevice);
        if (usbInterface == null) {
            onUsbFailureCallback(callback, new ErrorCode(ErrorCode.USB, "No USB interface"));
            return ;
        }

        if (!connection.claimInterface(usbInterface, true)) {
            onUsbFailureCallback(callback, new ErrorCode(ErrorCode.USB, "Claim Interface failed"));
            return ;
        }
        connection.setInterface(usbInterface);


        DeviceInfo hidDevice = new DeviceInfo();
        hidDevice.setUsbDevice(usbDevice);
        hidDevice.setUsbDeviceConnection(connection);
        for (int i = 0; i < usbInterface.getEndpointCount(); i++) {
            UsbEndpoint endpoint = usbInterface.getEndpoint(i);
            int dir = endpoint.getDirection();
            int type = endpoint.getType();
            if (dir == UsbConstants.USB_DIR_IN && type == UsbConstants.USB_ENDPOINT_XFER_INT) {
                hidDevice.setInUsbEndpoint(endpoint);
            }
            if (dir == UsbConstants.USB_DIR_OUT && type == UsbConstants.USB_ENDPOINT_XFER_INT) {
                hidDevice.setOutUsbEndpoint(endpoint);
            }
        }
        this.hidDevice = hidDevice;
        if (callback != null) callback.onSuccess();
    }

    private void onUsbFailureCallback(Callback<ErrorCode> callback, ErrorCode errorCode) {
        if (callback != null) {
            callback.onFailure(errorCode);
        }
    }

    private UsbInterface getUsbInterface(UsbDevice device) {
        for (int i = 0; i < device.getInterfaceCount(); i++) {
            UsbInterface usbInterface = device.getInterface(i);
            if (usbInterface.getInterfaceClass() == Const.INTERFACE_CLASS_HID) {
                return usbInterface;
            }
        }
        return null;
    }
}
