package com.htfixlib.utils;

import android.os.Build;

import com.htfixlib.ArtMethodSize;
import com.htfixlib.offset.HTFixMethodResolver;

import java.io.File;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class ArtMethodUtils {
    public static Class artMethodClass;
    public static Field nativePeerField;
    public static Method testOffsetMethod1;
    public static Method testOffsetMethod2;
    public static Object testOffsetArtMethod1;
    public static Object testOffsetArtMethod2;

    public static int testAccessFlag;


    private static void init() {
        initTestOffset();
        initThreadPeer();
        HTFixMethodResolver.init();
    }

    private static void initThreadPeer() {
        try {
            nativePeerField = getField(Thread.class, "nativePeer");
        } catch (NoSuchFieldException e) {

        }
    }

    private static void initTestOffset() {
        // make test methods sure resolved!
        ArtMethodSize.method1();
        ArtMethodSize.method2();
        // get test methods
        try {
            testOffsetMethod1 = ArtMethodSize.class.getDeclaredMethod("method1");
            testOffsetMethod2 = ArtMethodSize.class.getDeclaredMethod("method2");
        } catch (NoSuchMethodException e) {
            throw new RuntimeException("SandHook init error", e);
        }
        initTestAccessFlag();
    }


    private static void initTestAccessFlag() {
        if (hasJavaArtMethod()) {
            try {
                loadArtMethod();
                Field fieldAccessFlags = getField(artMethodClass, "accessFlags");
                testAccessFlag = (int) fieldAccessFlags.get(testOffsetArtMethod1);
            } catch (Exception e) {
            }
        } else {
            try {
                Field fieldAccessFlags = getField(Method.class, "accessFlags");
                testAccessFlag = (int) fieldAccessFlags.get(testOffsetMethod1);
            } catch (Exception e) {
            }
        }
    }

    private static void loadArtMethod() {
        try {
            Field fieldArtMethod = getField(Method.class, "artMethod");
            testOffsetArtMethod1 = fieldArtMethod.get(testOffsetMethod1);
            testOffsetArtMethod2 = fieldArtMethod.get(testOffsetMethod2);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
    }

    public static long getThreadId() {
        if (nativePeerField == null)
            return 0;
        try {
            if (nativePeerField.getType() == int.class) {
                return nativePeerField.getInt(Thread.currentThread());
            } else {
                return nativePeerField.getLong(Thread.currentThread());
            }
        } catch (IllegalAccessException e) {
            return 0;
        }
    }

    public static boolean passApiCheck() {
        return ReflectionUtils.passApiCheck();
    }


    public static Field getField(Class topClass, String fieldName) throws NoSuchFieldException {
        while (topClass != null && topClass != Object.class) {
            try {
                Field field = topClass.getDeclaredField(fieldName);
                field.setAccessible(true);
                return field;
            } catch (Exception e) {
            }
            topClass = topClass.getSuperclass();
        }
        throw new NoSuchFieldException(fieldName);
    }


    public static boolean hasJavaArtMethod() {
        if (HTFixHookConfig.SDK_INT >= Build.VERSION_CODES.O)
            return false;
        if (artMethodClass != null)
            return true;
        try {
            if (HTFixHookConfig.initClassLoader == null) {
                artMethodClass = Class.forName("java.lang.reflect.ArtMethod");
            } else {
                artMethodClass = Class.forName("java.lang.reflect.ArtMethod", true, HTFixHookConfig.initClassLoader);
            }
            return true;
        } catch (ClassNotFoundException e) {
            return false;
        }
    }
}
