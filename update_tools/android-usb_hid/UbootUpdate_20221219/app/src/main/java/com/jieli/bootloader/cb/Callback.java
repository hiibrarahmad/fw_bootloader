package com.jieli.bootloader.cb;

/**
 * Des:
 * author: Bob
 * date: 2022/12/13
 * Copyright: Jieli Technology
 * Modify date:
 * Modified by:
 */
public interface Callback<T> {
    void onSuccess();
    void onFailure(T error);
}
