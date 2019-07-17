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

    public static synchronized void hook(HookWrapper.HookEntity entity) throws HookErrorException {

        if (entity == null)
            throw new HookErrorException("null hook entity");

        Member target = entity.target;
        Method hook = entity.hook;
        Method backup = entity.backup;

        if (target == null || hook == null)
            throw new HookErrorException("null input");

        if (globalHookEntityMap.containsKey(entity.target))
            throw new HookErrorException("method <" + entity.target.toString() + "> has been hooked!");

        if (HookBlackList.canNotHook(target))
            throw new HookErrorException("method <" + entity.target.toString() + "> can not hook, because of in blacklist!");

        resolveStaticMethod(target);
        resolveStaticMethod(backup);
        if (backup != null && entity.resolveDexCache) {
            HTFixMethodResolver.resolveMethod(hook, backup);
        }
        if (target instanceof Method) {
            ((Method)target).setAccessible(true);
        }

        int mode = HookMode.AUTO;
        if (hookModeCallBack != null) {
            mode = hookModeCallBack.hookMode(target);
        }

        globalHookEntityMap.put(entity.target, entity);

        int res;
        if (mode != HookMode.AUTO) {
            res = HTFixNative.hookMethod(target, hook, backup, mode);
        } else {
            HookMode hookMode = hook.getAnnotation(HookMode.class);
            res = HTFixNative.hookMethod(target, hook, backup, hookMode == null ? HookMode.AUTO : hookMode.value());
        }

        if (res > 0 && backup != null) {
            backup.setAccessible(true);
        }

        entity.hookMode = res;

        if (hookResultCallBack != null) {
            hookResultCallBack.hookResult(res > 0, entity);
        }

        if (res < 0) {
            globalHookEntityMap.remove(entity.target);
            throw new HookErrorException("hook method <" + entity.target.toString() + "> error in native!");
        }

        if (entity.backup != null) {
            globalBackupMap.put(entity.backup, entity);
        }

        HookLog.d("method <" + entity.target.toString() + "> hook <" + (res == HookMode.INLINE ? "inline" : "replacement") + "> success!");
    }


    public final static Object callOriginMethod(Member originMethod, Object thiz, Object... args) throws Throwable {
        HookWrapper.HookEntity hookEntity = globalHookEntityMap.get(originMethod);
        if (hookEntity == null || hookEntity.backup == null)
            return null;
        return callOriginMethod(hookEntity.backupIsStub, originMethod, hookEntity.backup, thiz, args);
    }

    public final static Object callOriginByBackup(Method backupMethod, Object thiz, Object... args) throws Throwable {
        HookWrapper.HookEntity hookEntity = globalBackupMap.get(backupMethod);
        if (hookEntity == null)
            return null;
        return callOriginMethod(hookEntity.backupIsStub, hookEntity.target, backupMethod, thiz, args);
    }

    public final static Object callOriginMethod(Member originMethod, Method backupMethod, Object thiz, Object[] args) throws Throwable {
        return callOriginMethod(true, originMethod, backupMethod, thiz, args);
    }

    public final static Object callOriginMethod(boolean backupIsStub, Member originMethod, Method backupMethod, Object thiz, Object[] args) throws Throwable {
        //reset declaring class
        if (!backupIsStub && HTFixHookConfig.SDK_INT >= Build.VERSION_CODES.N) {
            //holder in stack to avoid moving gc
            Class originClassHolder = originMethod.getDeclaringClass();
            HTFixNative.ensureDeclareClass(originMethod, backupMethod);
        }
        if (Modifier.isStatic(originMethod.getModifiers())) {
            try {
                return backupMethod.invoke(null, args);
            } catch (InvocationTargetException throwable) {
                if (throwable.getCause() != null) {
                    throw throwable.getCause();
                } else {
                    throw throwable;
                }
            }
        } else {
            try {
                return backupMethod.invoke(thiz, args);
            } catch (InvocationTargetException throwable) {
                if (throwable.getCause() != null) {
                    throw throwable.getCause();
                } else {
                    throw throwable;
                }
            }
        }
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
