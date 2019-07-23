package com.htfixlib;

import android.os.Build;

import com.htfix.annotation.HookMode;
import com.htfixlib.blacklist.HookBlackList;
import com.htfixlib.offset.HTFixMethodResolver;
import com.htfixlib.utils.FileUtils;
import com.htfixlib.utils.HTFixHookConfig;
import com.htfixlib.utils.HookLog;
import com.htfixlib.wrapper.HookErrorException;
import com.htfixlib.wrapper.HookWrapper;

import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;


public class HTFixHook {
    static Map<Member,HookWrapper.HookEntity> globalHookEntityMap = new ConcurrentHashMap<>();
    static Map<Method,HookWrapper.HookEntity> globalBackupMap = new ConcurrentHashMap<>();

    private static HookModeCallBack hookModeCallBack;
    public static void setHookModeCallBack(HookModeCallBack hookModeCallBack) {
        HTFixHook.hookModeCallBack = hookModeCallBack;
    }

    private static HookResultCallBack hookResultCallBack;
    public static void setHookResultCallBack(HookResultCallBack hookResultCallBack) {
        HTFixHook.hookResultCallBack = hookResultCallBack;
    }

    public static void addHookClass(Class... hookWrapperClass) throws HookErrorException {
        HookWrapper.addHookClass(hookWrapperClass);
    }

    public static void addHookClass(ClassLoader classLoader, Class... hookWrapperClass) throws HookErrorException {
        HookWrapper.addHookClass(classLoader, hookWrapperClass);
    }

    public static synchronized void hookHTFix(Method targetMethod, Method hookMethod) {
        resolveStaticMethod(targetMethod);
        HTFixNative.htfixHookMethod(targetMethod, hookMethod);
    }

    //disable JIT/AOT Profile
    public static boolean tryDisableProfile(String selfPackageName) {
        if (HTFixHookConfig.SDK_INT < Build.VERSION_CODES.N)
            return false;
        try {
            File profile = new File("/data/misc/profiles/cur/" + HTFixHookConfig.curUse + "/" + selfPackageName + "/primary.prof");
            if (!profile.getParentFile().exists()) return false;
            try {
                profile.delete();
                profile.createNewFile();
            } catch (Throwable throwable) {}
            FileUtils.chmod(profile.getAbsolutePath(), FileUtils.FileMode.MODE_IRUSR);
            return true;
        } catch (Throwable throwable) {
            return false;
        }
    }

    public static void resolveStaticMethod(Member method) {
        //ignore result, just call to trigger resolve
        if (method == null)
            return;
        try {
            if (method instanceof Method && Modifier.isStatic(method.getModifiers())) {
                ((Method) method).setAccessible(true);
                ((Method) method).invoke(new Object(), getFakeArgs((Method) method));
            }
        } catch (Throwable throwable) {
        }
    }


    private static Object[] getFakeArgs(Method method) {
        Class[] pars = method.getParameterTypes();
        if (pars == null || pars.length == 0) {
            return new Object[]{new Object()};
        } else {
            return null;
        }
    }

    @FunctionalInterface
    public interface HookModeCallBack {
        int hookMode(Member originMethod);
    }

    @FunctionalInterface
    public interface HookResultCallBack {
        void hookResult(boolean success, HookWrapper.HookEntity hookEntity);
    }

}
