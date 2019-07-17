package com.htfix.hook;

import com.htfix.BuildConfig;
import com.htfixlib.HTFixHook;
import com.htfixlib.HTFixNative;
import com.htfixlib.utils.HTFixHookConfig;
import com.htfixlib.wrapper.HookErrorException;

public class Hook {
    public final static boolean testAndroidQ = false;
    public static void hook(String packageName) {
        HTFixHookConfig.DEBUG = BuildConfig.DEBUG;

        if (testAndroidQ) {
            HTFixHookConfig.SDK_INT = 29;
        }

        HTFixNative.disableVMInline();
        HTFixHook.tryDisableProfile(packageName);

        try {
            HTFixHook.addHookClass(
                    ActivityHooker.class,
                    BaseInputConnectionHooker.class);
        } catch (HookErrorException e) {
            e.printStackTrace();
        }
    }
}
