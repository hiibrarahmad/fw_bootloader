package com.jieli.bootloader;

import android.Manifest;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.snackbar.Snackbar;
import com.jieli.bootloader.cb.Callback;
import com.jieli.bootloader.databinding.ActivityMainBinding;
import com.jieli.bootloader.model.ErrorCode;
import com.jieli.bootloader.util.Const;
import com.jieli.bootloader.util.AppUtil;
import com.jieli.bootloader.util.UsbHelper;


public class MainActivity extends AppCompatActivity {
    private final String TAG = getClass().getSimpleName();
    private ActivityMainBinding binding;
    private UsbHelper usbHelper;
    private byte[] buffer = null;
    private ProgressDialog progressDialog;
    private final static int MAX_FILE_DATA = 20 * 1024 * 1024;// max file data size
    private static final String ACTION_USB_PERMISSION = "com.jieli.bootloader.action.USB_PERMISSION";

    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                context.unregisterReceiver(this);
                synchronized (this) {
                    UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false) && device != null) {
                        for (int i = 0; i < device.getInterfaceCount(); i++) {
                            UsbInterface usbInterface = device.getInterface(i);
                            if (usbInterface.getInterfaceClass() == Const.INTERFACE_CLASS_HID) {
                                openDevice(device);
                            }
                        }
                    } else {
                        showTips(getString(R.string.no_permission));
                    }
                }
            }
        }
    };

    private final ActivityResultLauncher<String[]> launcher = registerForActivityResult(
            new ActivityResultContracts.RequestMultiplePermissions(), result -> {
                boolean isGrantAll = true;
                for (Boolean value : result.values()) {
                    isGrantAll = isGrantAll && value;
                    Log.i(TAG, "isGrantAll=" + isGrantAll + ", " + value);
                }

                if (isGrantAll) {
                    tryToSelectFile();
                } else {
                    Log.e(TAG, "Permission Not OK");
                }
            });

    private final ActivityResultLauncher<String> filePickerLauncher =
            registerForActivityResult(new ActivityResultContracts.GetContent(), result -> {
                if (result != null) {
                    Log.i(TAG, "result=" + result.getPath());
                    String str = getString(R.string.filename);
                    String filename = AppUtil.getFilename(getApplicationContext(), result);
                    str +=  ": " + filename;
                    binding.tvFilename.setText(str);

                    buffer = AppUtil.readToBuffer(getApplicationContext(), result, MAX_FILE_DATA);
                }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        usbHelper = UsbHelper.getInstance();

        binding.btnSelectFile.setOnClickListener(v -> {
            String readPermission = Manifest.permission.READ_EXTERNAL_STORAGE;
            if (AppUtil.checkIfGranted(this, readPermission)) {
                tryToSelectFile();
            } else {
                String[] permissions = {readPermission};
                launcher.launch(permissions);
            }
        });

        findDevice();

        binding.btnUpgrade.setOnClickListener(v -> {
            if (buffer != null) {
                new Thread(() -> {
                    runOnUiThread(this::showWaiting);

                    if (usbHelper.tryToUpgrade(buffer)) {
                        Log.i(TAG, "Upgrade success");
                        runOnUiThread(() -> updateUpgradingState(true) );
                    } else {
                        Log.e(TAG, "Upgrade failed");
                        runOnUiThread(() -> updateUpgradingState(false));
                    }
                    buffer = null;// reset data buffer
                }).start();
            } else {
                Log.e(TAG, "Data is null");
                showTips(getString(R.string.no_file_data));
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (item.getItemId() == R.id.menu_check_device) {
            findDevice();
        } else if (item.getItemId() == R.id.menu_about) {
            showVersion();
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        usbHelper.release();
        dismissWaiting();
//        if (mUsbReceiver != null) {
//            MainApplication.getContext().unregisterReceiver(mUsbReceiver);
//        }
    }

    private void findDevice() {
        UsbDevice usbDevice = usbHelper.findDevice(0x4c4a, 0x4155);
        if (usbDevice != null) {
            tryToOpen(usbDevice);
        } else {
            Log.e(TAG, "No devices");
            updateDeviceState(false);
        }
    }

    private void tryToOpen(UsbDevice usbDevice) {
        UsbManager usbManager = (UsbManager) MainApplication.getContext().getSystemService(Context.USB_SERVICE);
        if (!usbManager.hasPermission(usbDevice)) {
            Log.w(TAG, "Request permission now");
            PendingIntent permissionIntent = PendingIntent.getBroadcast(MainApplication.getContext(),
                    0, new Intent(ACTION_USB_PERMISSION), 0);
            usbManager.requestPermission(usbDevice, permissionIntent);

            IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
            MainApplication.getContext().registerReceiver(mUsbReceiver, filter);
        } else {
            Log.i(TAG, "Have permission now");
            openDevice(usbDevice);
        }
    }

    private void openDevice(UsbDevice usbDevice) {
        usbHelper.open(usbDevice, new Callback<ErrorCode>() {

            @Override
            public void onSuccess() {
                updateDeviceState(true);
            }

            @Override
            public void onFailure(ErrorCode error) {
                Log.e(TAG, "error:" + error);
                updateDeviceState(false);
            }
        });
    }

    private void updateDeviceState(boolean state) {
        String str = getString(R.string.device);
        if (state) {
            binding.btnUpgrade.setEnabled(true);
            str += ": " + getString(R.string.online);
        } else {
            showTips(str + " " + getString(R.string.offline));
            binding.btnUpgrade.setEnabled(false);
            str += ": " + getString(R.string.offline);
        }
        binding.tvDeviceStatus.setText(str);
    }

    private void updateUpgradingState(boolean state) {
        String str = getString(R.string.fail);
        if (state) {
            str = getString(R.string.success);
        }
        progressDialog.setMessage(str);
        progressDialog.setIndeterminateDrawable(null);
        progressDialog.setCancelable(true);
    }

    private void showTips(String str) {
        Snackbar.make(binding.getRoot(), str, Snackbar.LENGTH_LONG).show();
    }

    private void tryToSelectFile() {
        filePickerLauncher.launch("application/octet-stream");
    }

    private void showVersion() {
        String versionName = BuildConfig.VERSION_NAME;
        AlertDialog.Builder builder = new AlertDialog.Builder(this)
                .setIcon(R.mipmap.ic_launcher)
                .setTitle(R.string.version)
                .setMessage(versionName)
                .setPositiveButton(getString(android.R.string.ok), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        dialogInterface.dismiss();
                    }
                });
        builder.create().show();
    }

    private void showWaiting() {
        if (progressDialog != null && progressDialog.isShowing()) {
            return;
        }
        progressDialog = new ProgressDialog(this);
        progressDialog.setIcon(R.mipmap.ic_launcher);
        progressDialog.setTitle(R.string.upgrade);
        progressDialog.setMessage(getString(R.string.upgrade) + "...");
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.setOnDismissListener(dialog -> findDevice());
        progressDialog.show();
    }

    private void dismissWaiting() {
        if (progressDialog != null && !progressDialog.isShowing()) {
           progressDialog.dismiss();
        }
        progressDialog = null;
    }
}