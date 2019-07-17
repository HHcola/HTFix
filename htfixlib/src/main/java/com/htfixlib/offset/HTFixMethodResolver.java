package com.htfixlib.offset;

import com.htfixlib.HTFixNative;
import com.htfixlib.utils.ArtMethodUtils;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class HTFixMethodResolver {
    public static Field resolvedMethodsField;
    public static Field dexCacheField;
    public static Field dexMethodIndexField;
    public static Field artMethodField;
    public static Field fieldEntryPointFromCompiledCode;
    public static Field fieldEntryPointFromInterpreter;

    public static boolean canResolvedInJava = false;
    public static boolean isArtMethod = false;

    public static long resolvedMethodsAddress = 0;
    public static int dexMethodIndex = 0;

    public static long entryPointFromCompiledCode = 0;
    public static long entryPointFromInterpreter = 0;

    public static Method testMethod;
    public static Object testArtMethod;

    public static void init() {
        testMethod = ArtMethodUtils.testOffsetMethod1;
        checkSupport();
    }

    private static void checkSupport() {
        try {
            artMethodField = ArtMethodUtils.getField(Method.class, "artMethod");

            testArtMethod = artMethodField.get(testMethod);

            if (ArtMethodUtils.hasJavaArtMethod() && testArtMethod.getClass() == ArtMethodUtils.artMethodClass) {
                checkSupportForArtMethod();
                isArtMethod = true;
            } else if (testArtMethod instanceof Long) {
                checkSupportForArtMethodId();
                isArtMethod = false;
            } else {
                canResolvedInJava = false;
            }

        } catch (Exception e) {
        }
    }

    // may 5.0
    private static void checkSupportForArtMethod() throws Exception {
        try {
            dexMethodIndexField = ArtMethodUtils.getField(ArtMethodUtils.artMethodClass, "dexMethodIndex");
        } catch (NoSuchFieldException e) {
            //may 4.4
            dexMethodIndexField = ArtMethodUtils.getField(ArtMethodUtils.artMethodClass, "methodDexIndex");
        }
        dexCacheField = ArtMethodUtils.getField(Class.class, "dexCache");
        Object dexCache = dexCacheField.get(testMethod.getDeclaringClass());
        resolvedMethodsField = ArtMethodUtils.getField(dexCache.getClass(), "resolvedMethods");
        if (resolvedMethodsField.get(dexCache) instanceof Object[]) {
            canResolvedInJava = true;
        }
        try {
            try {
                fieldEntryPointFromCompiledCode = ArtMethodUtils.getField(ArtMethodUtils.artMethodClass, "entryPointFromQuickCompiledCode");
            } catch (Exception e) {
                fieldEntryPointFromCompiledCode = ArtMethodUtils.getField(ArtMethodUtils.artMethodClass, "entryPointFromCompiledCode");
            }
            if (fieldEntryPointFromCompiledCode.getType() == int.class) {
                entryPointFromCompiledCode = fieldEntryPointFromCompiledCode.getInt(testArtMethod);
            } else if (fieldEntryPointFromCompiledCode.getType() == long.class) {
                entryPointFromCompiledCode = fieldEntryPointFromCompiledCode.getLong(testArtMethod);
            }
            fieldEntryPointFromInterpreter = ArtMethodUtils.getField(ArtMethodUtils.artMethodClass, "entryPointFromInterpreter");
            if (fieldEntryPointFromInterpreter.getType() == int.class) {
                entryPointFromInterpreter = fieldEntryPointFromInterpreter.getInt(testArtMethod);
            } else if (fieldEntryPointFromCompiledCode.getType() == long.class) {
                entryPointFromInterpreter = fieldEntryPointFromInterpreter.getLong(testArtMethod);
            }
        } catch (Throwable e) {
        }
    }

    // may 6.0
    private static void checkSupportForArtMethodId() throws Exception {
        dexMethodIndexField = ArtMethodUtils.getField(Method.class, "dexMethodIndex");
        dexMethodIndex = (int) dexMethodIndexField.get(testMethod);
        dexCacheField = ArtMethodUtils.getField(Class.class, "dexCache");
        Object dexCache = dexCacheField.get(testMethod.getDeclaringClass());
        resolvedMethodsField = ArtMethodUtils.getField(dexCache.getClass(), "resolvedMethods");
        Object resolvedMethods = resolvedMethodsField.get(dexCache);
        if (resolvedMethods instanceof Long) {
            canResolvedInJava = false;
            resolvedMethodsAddress = (long) resolvedMethods;
        } else if (resolvedMethods instanceof long[]) {
            //64bit
            canResolvedInJava = true;
        } else if (resolvedMethods instanceof int[]) {
            //32bit
            canResolvedInJava = true;
        }
    }

    public static void resolveMethod(Method hook, Method backup) {
        if (canResolvedInJava && artMethodField != null) {
            // in java
            try {
                resolveInJava(hook, backup);
            } catch (Exception e) {
                // in native
                resolveInNative(hook, backup);
            }
        } else {
            // in native
            resolveInNative(hook, backup);
        }
    }

    private static void resolveInJava(Method hook, Method backup) throws Exception {
        Object dexCache = dexCacheField.get(hook.getDeclaringClass());
        if (isArtMethod) {
            Object artMethod = artMethodField.get(backup);
            int dexMethodIndex = (int) dexMethodIndexField.get(artMethod);
            Object resolvedMethods = resolvedMethodsField.get(dexCache);
            ((Object[])resolvedMethods)[dexMethodIndex] = artMethod;
        } else {
            int dexMethodIndex = (int) dexMethodIndexField.get(backup);
            Object resolvedMethods = resolvedMethodsField.get(dexCache);
            if (resolvedMethods instanceof long[]) {
                long artMethod = (long) artMethodField.get(backup);
                ((long[])resolvedMethods)[dexMethodIndex] = artMethod;
            } else if (resolvedMethods instanceof int[]) {
                int artMethod = Long.valueOf((long)artMethodField.get(backup)).intValue();
                ((int[])resolvedMethods)[dexMethodIndex] = artMethod;
            } else {
                throw new UnsupportedOperationException("un support");
            }
        }
    }

    private static void resolveInNative(Method hook, Method backup) {
        HTFixNative.ensureMethodCached(hook, backup);
    }

}
