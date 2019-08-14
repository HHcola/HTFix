package com.htfix.hook;

import com.htfixlib.HTFixNative;

import java.lang.reflect.Method;

public class BTest {
    public String getText() {
        try {
            Method targetMethod = ATest.class.getMethod("getText");
            HTFixNative.clearHotnessCount(targetMethod);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return "BTest";
    }
}
