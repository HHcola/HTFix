package com.htfixlib;

import android.os.Build;
import android.util.Log;

import com.htfixlib.utils.ArtMethodUtils;
import com.htfixlib.utils.HTFixHookConfig;

import java.lang.reflect.Method;

public class HTFixNative {
    private static final String TAG = "HTFixNative";
    public static boolean canHookMethod = false;
    static {
        HTFixHookConfig.libLoader.loadLib();
        ArtMethodUtils.init();
        setup();
        canHookMethod = checkHookMethod();
    }

    public static int nativeHookMethod(Method targetMethod, Method hookMethod) {
        if (canHookMethod) {
            return htfixHookMethod(targetMethod, hookMethod);
        }
        Log.e("nativeHookMethod", "canHookMethod = false");
        return -1;
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
            Log.d(TAG, "setup isArt = " + isArt + " apiLevel = " + apiLevel);
            return setup(isArt, apiLevel);
        } catch (Exception e) {
            Log.e(TAG, "setup", e);
            return false;
        }
    }
    private static native boolean setup(boolean isArt, int apilevel);
    private static native boolean checkHookMethod();
//    private static native void htfixInitNative(int sdk);
    private static native int htfixHookMethod(Method targetMethod, Method hookMethod);


    private static native void htfixNativeOne();
    private static native void htfixNativeTwo();
}
