package com.htfix.hook;

import android.text.Spannable;
import android.text.Spanned;
import android.util.Log;
import android.view.inputmethod.BaseInputConnection;

public class BaseInputConnectionReplaceYH {
    public static final String TAG = "InputConnectionReplace";

    public static String className = BaseInputConnection.class.getName();
    public static String methodName = "removeComposingSpans";
    public static String methodSig =
            "(Landroid/text/Spannable;)V";

    public static final void hook(Spannable text) {
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
