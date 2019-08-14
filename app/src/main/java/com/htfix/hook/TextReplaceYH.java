package com.htfix.hook;


/**
 * Created by timothyhe on 2019/8/14
 */
public class TextReplaceYH {
    public static final String TAG = "InputConnectionReplace";

    public static String className = ATest.class.getName();
    public static String methodName = "getText";
    public static String methodSig =
            "()Ljava/lang/String;";

    public static String hook() {
        return "BTest";
    }
}
