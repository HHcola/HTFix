package com.htfixlib;

import com.htfixlib.utils.ArtMethodUtils;
import com.htfixlib.utils.HTFixHookConfig;

import java.lang.reflect.Member;
import java.lang.reflect.Method;

public class HTFixNative {
    static {
        HTFixHookConfig.libLoader.loadLib();
        ArtMethodUtils.init();
        htfixInitNative(HTFixHookConfig.SDK_INT);
    }
    public static native void htfixInitNative(int sdk);
    public static native int htfixHookMethod(Method targetMethod, Method hookMethod);
}
