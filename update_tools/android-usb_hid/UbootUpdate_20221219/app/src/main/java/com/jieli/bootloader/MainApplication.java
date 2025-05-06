package com.jieli.bootloader;

import android.app.Application;
import android.content.Context;

public class MainApplication extends Application {
    private static MainApplication application;
    @Override
    public void onCreate() {
        super.onCreate();
        application = this;
    }

    public static Context getContext() { return application; }
}
