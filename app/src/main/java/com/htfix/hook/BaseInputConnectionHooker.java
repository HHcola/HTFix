package com.htfix.hook;

import android.os.Bundle;
import android.text.Spannable;
import android.text.Spanned;
import android.util.Log;
import android.view.inputmethod.BaseInputConnection;

import com.htfix.annotation.HookClass;
import com.htfix.annotation.HookMethod;
import com.htfix.annotation.HookMethodBackup;
import com.htfix.annotation.MethodParams;

import java.lang.reflect.Method;

@HookClass(BaseInputConnection.class)
public class BaseInputConnectionHooker {
    private static final String TAG = "InputConnectionHooker";


    @HookMethodBackup("removeComposingSpans")
    @MethodParams(Spannable.class)
    static Method removeComposingSpansBackup;


    @HookMethod("removeComposingSpans")
    @MethodParams(Spannable.class)
    public static final void hookRemoveComposingSpans(Spannable text) {
//        text.removeSpan(COMPOSING);
        Log.d(TAG, "hookRemoveComposingSpans");
        Object[] sps = text.getSpans(0, text.length(), Object.class);
        if (sps != null) {
            for (int i=sps.length-1; i>=0; i--) {
                Object o = sps[i];
                if ((text.getSpanFlags(o)& Spanned.SPAN_COMPOSING) != 0) {
                    text.removeSpan(o);
                }
            }
        }

    }
}
