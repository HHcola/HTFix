package com.htfixlib;

import android.os.Build;
import android.util.Log;

import com.htfixlib.utils.ArtMethodUtils;
import com.htfixlib.utils.HTFixHookConfig;

import java.lang.reflect.Method;

public class HTFixNative {
    static {
        HTFixHookConfig.libLoader.loadLib();
        ArtMethodUtils.init();
        htfixInitNative(HTFixHookConfig.SDK_INT);
    }

    /**
     * initialize
     *
     * @return true if initialize success
     */
    public static boolean setup() {
        try {
            final String vmVersion = System.getProperty("java.vm.version");
            boolean isArt = vmVersion != null && vmVersion.startsWith("2");
            int apiLevel = Build.VERSION.SDK_INT;
            return setup(isArt, apiLevel);
        } catch (Exception e) {
            Log.e("HTFixNative", "setup", e);
            return false;
        }
    }
    private static native boolean setup(boolean isArt, int apilevel);
    public static native void htfixInitNative(int sdk);
    public static native int htfixHookMethod(Method targetMethod, Method hookMethod);
}
