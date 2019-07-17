package com.htfixlib;

import com.htfixlib.utils.HTFixHookConfig;

import java.lang.reflect.Member;
import java.lang.reflect.Method;

public class HTFixNative {
    static {
        HTFixHookConfig.libLoader.loadLib();
        initNative(HTFixHookConfig.SDK_INT, HTFixHookConfig.DEBUG);
    }

    public static native boolean initNative(int sdk, boolean debug);
    public static native int hookMethod(Member targetMethod, Method hookMethod, Method backupMethod, int hookMode);
    public static native void ensureMethodCached(Method hook, Method backup);
    public static native void ensureDeclareClass(Member origin, Method backup);

    public static native boolean compileMethod(Member member);
    public static native boolean canGetObject();
    public static native Object getObjectNative(long thread, long address);

    public static native boolean disableVMInline();

}
