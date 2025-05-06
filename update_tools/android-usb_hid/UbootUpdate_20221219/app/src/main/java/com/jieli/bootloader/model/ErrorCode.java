package com.jieli.bootloader.model;

/**
 * Des:用于返回/回调错误信息的类
 * Author: Bob
 * Date:22-12-13
 * UpdateRemark:
 */
public class ErrorCode {
    public static final int UNKNOWN = 1;
    public static final int EXCEPTION = 2;
    public static final int COMMAND = 3;
    public static final int USB = 4;
    public int code;
    public String message;

    public ErrorCode(int code, String message) {
        this.code = code;
        this.message = message;
    }

    private ErrorCode(){}

    public static ErrorCode builder() {
        return new ErrorCode();
    }

    public int getCode() {
        return code;
    }

    public ErrorCode setCode(int code) {
        this.code = code;
        return this;
    }

    public String getMessage() {
        return message;
    }

    public ErrorCode setMessage(String message) {
        this.message = message;
        return this;
    }

    @Override
    public String toString() {
        return "ErrorCode{" +
                "code=" + code +
                ", message='" + message + '\'' +
                '}';
    }
}
