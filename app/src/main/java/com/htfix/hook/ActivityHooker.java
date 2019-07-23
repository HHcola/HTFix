package com.htfix.hook;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.htfix.annotation.HookClass;
import com.htfix.annotation.HookMethod;
import com.htfix.annotation.HookMethodBackup;
import com.htfix.annotation.MethodParams;
import com.htfix.annotation.ThisObject;
import com.htfixlib.HTFixHook;
import com.htfixlib.wrapper.HookWrapper;

import java.lang.reflect.Method;

@HookClass(Activity.class)
public class ActivityHooker {

    @HookMethodBackup("onCreate")
    @MethodParams(Bundle.class)
    static Method onCreateBackup;

    @HookMethod("onCreate")
    @MethodParams(Bundle.class)
    public static void onCreate(Activity thiz, Bundle bundle) throws Throwable {
        Log.e("ActivityHooker", "hooked onCreate success " + thiz);
    }

    @HookMethod("onPause")
    public static void onPause(@ThisObject Activity thiz) throws Throwable {
        Log.e("ActivityHooker", "hooked onPause success " + thiz);
    }

}