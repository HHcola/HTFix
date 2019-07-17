package com.htfixlib.utils;

import android.annotation.SuppressLint;
import android.os.Build;

import com.htfixlib.BuildConfig;

public class HTFixHookConfig {

    public volatile static int SDK_INT = Build.VERSION.SDK_INT;
    public volatile static boolean DEBUG = BuildConfig.DEBUG;
    public volatile static boolean compiler = true;
    public volatile static ClassLoader initClassLoader;
    public volatile static int curUse = 0;

    public volatile static String libSandHookPath;
    public volatile static LibLoader libLoader = new LibLoader() {
        @SuppressLint("UnsafeDynamicallyLoadedCode")
        @Override
        public void loadLib() {
            if (HTFixHookConfig.libSandHookPath == null) {
                System.loadLibrary("htfix");
            } else {
                System.load(HTFixHookConfig.libSandHookPath);
            }
        }
    };


    public interface LibLoader {
        void loadLib();
    }
}
