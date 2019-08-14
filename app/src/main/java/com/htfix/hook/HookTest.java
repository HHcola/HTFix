package com.htfix.hook;

import com.htfixlib.HTFixNative;

import java.lang.reflect.Method;

import lab.galaxy.yahfa.HookMain;

/**
 * Created by timothyhe on 2019/8/14
 */
public class HookTest {
    public static void hootTest() {
        try {
//            new BTest().getText();
            Method targetMethod = ATest.class.getMethod("getText");
            Method hookMethod = BTest.class.getMethod("getText");
            HTFixNative.nativeHookMethod(targetMethod, hookMethod);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static void hootCanyTest() {
        try {
            ClassLoader classLoader = HookTest.class.getClassLoader();
            HookMain.doHookDefault(HookMethodInfo.class.getName(), classLoader, classLoader);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
