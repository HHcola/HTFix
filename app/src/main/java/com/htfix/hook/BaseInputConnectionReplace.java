package com.htfix.hook;

import android.text.Spannable;
import android.text.Spanned;
import android.util.Log;
import android.view.inputmethod.BaseInputConnection;

import com.htfixlib.HTFixHook;
import com.htfixlib.HTFixNative;

import java.lang.reflect.Method;

public class BaseInputConnectionReplace {

    private static final String TAG = "InputConnectionReplace";

    public static void hookBaseInputConnectionReplace() {
        try {
            Method targetMethod = BaseInputConnection.class.getMethod("removeComposingSpans", Spannable.class);
            Method hookMethod = BaseInputConnectionReplace.class.getMethod("hookRemoveComposingSpans", Spannable.class);
            HTFixHook.hookHTFix(targetMethod, hookMethod);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }



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
