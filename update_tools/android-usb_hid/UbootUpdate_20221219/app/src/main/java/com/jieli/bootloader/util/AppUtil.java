package com.jieli.bootloader.util;

import android.content.Context;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.OpenableColumns;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public final class AppUtil {
    private final static String TAG = "AppUtil";
    public static final int GRANTED = PackageManager.PERMISSION_GRANTED;

    public static boolean checkIfGranted(@NonNull Context context, String permission) {
        // 适配android M，检查权限
        if (Build.VERSION.SDK_INT >= 23) {
            return ContextCompat.checkSelfPermission(context, permission) == PackageManager.PERMISSION_GRANTED;
        }
        return true;
    }

    public static String getFilename(Context context, Uri uri) {
        if (context == null || context.getContentResolver() == null) {
            return null;
        }
        Cursor  cursor = context.getContentResolver().query(uri, null, null, null, null);
        int nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
        cursor.moveToFirst();
        String filename =cursor.getString(nameIndex);
        cursor.close();
        return filename;
    }
    public static byte[] readToBuffer(Context context, Uri uri, int maxSize) {
        InputStream inputStream = null;
        try {
            inputStream = context.getContentResolver().openInputStream(uri);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        if (inputStream != null) {
            int size = -1;
            try {
                size = inputStream.available();
            } catch (IOException e) {
                e.printStackTrace();
            }
            byte[] buffer = null;
            if (size > maxSize) {
                Log.e(TAG, "File data too large:" + size);
            } else if (size > 0) {
                buffer = new byte[size];
                try {
                    if (inputStream.read(buffer) < 0) {
                        Log.e(TAG, "No data");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                inputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }

            return buffer;
        }
        return null;
    }
}
