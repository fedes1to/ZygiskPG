//
// Created by lbert on 2/15/2023.
//

#ifndef ZYGISKPG_JNISTUFF_H
#define ZYGISKPG_JNISTUFF_H

#include <cstring>
#include <jni.h>

JavaVM *jvm;
jclass UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;

void displayKeyboard(JNIEnv *env, bool pShow) {
    jclass ctx = env->FindClass(OBFUSCATE("android/content/Context"));
    jfieldID fid = env->GetStaticFieldID(ctx, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
    jmethodID mid = env->GetMethodID(ctx, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject context = env->GetStaticObjectField(UnityPlayer_cls, UnityPlayer_CurrentActivity_fid);
    jobject InputManObj = env->CallObjectMethod(context, mid, (jstring) env->GetStaticObjectField(ctx, fid));
    jclass ClassInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
    jmethodID toggleSoftInputId = env->GetMethodID(ClassInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
    if (pShow) {
        env->CallVoidMethod(InputManObj, toggleSoftInputId, 2, 0);
    } else {
        env->CallVoidMethod(InputManObj, toggleSoftInputId, 0, 0);
    }
}

#endif //ZYGISKPG_JNISTUFF_H
