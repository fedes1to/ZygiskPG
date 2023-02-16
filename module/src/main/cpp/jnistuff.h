//
// Created by lbert on 2/15/2023.
//

#ifndef ZYGISKPG_JNISTUFF_H
#define ZYGISKPG_JNISTUFF_H

#include <cstring>
#include <jni.h>
#include "hook.h"

JavaVM *jvm;
jclass UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;
JNIEnv* getEnv() {
    JNIEnv *env;
    int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if(status < 0) {
        status = jvm->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOGE("Error Getting JNI");
            return nullptr;
        }
    }
    return env;
}

void displayKeyboard(bool pShow) {
    JNIEnv *env = getEnv();
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

/*
std::string getPublicStaticString(JNIEnv *env, const char *className, const char *fieldName) {
    jclass clazz = env->FindClass(className);
    if (clazz != nullptr) {
        jfieldID fid = env->GetStaticFieldID(clazz, fieldName, "Ljava/lang/String;");
        if (fid != nullptr) {
            jstring GladioReceiver = (jstring) env->GetStaticObjectField(clazz, fid);
            jboolean blnIsCopy;
            std::string mystr = env->GetStringUTFChars(GladioReceiver, &blnIsCopy);
            return mystr;
        }
    }
    return "ERROR";
}

std::string CreateDeviceUniqueID() {
    JNIEnv *env = getEnv();
    std::string strReturn;
    std::string board = getPublicStaticString(env, "android/os/Build", "BOARD");
    std::string brand = getPublicStaticString(env, "android/os/Build", "BRAND");
    std::string display = getPublicStaticString(env, "android/os/Build", "DISPLAY");
    std::string device = getPublicStaticString(env, "android/os/Build", "DEVICE");
    std::string manufacturer = getPublicStaticString(env, "android/os/Build", "MANUFACTURER");
    std::string model = getPublicStaticString(env, "android/os/Build", "MODEL");
    std::string product = getPublicStaticString(env, "android/os/Build", "PRODUCT");

    int mod = 10;
    int a1 = ((int) board.size()) % mod;
    int a2 = ((int) brand.size()) % mod;
    int a3 = ((int) display.size()) % mod;
    int a4 = ((int) device.size()) % mod;
    int a5 = ((int) manufacturer.size()) % mod;
    int a6 = ((int) model.size()) % mod;
    int a7 = ((int) product.size()) % mod;

    strReturn = "35" +
                std::to_string(a1) +
                std::to_string(a2) +
                std::to_string(a3) +
                std::to_string(a4) +
                std::to_string(a5) +
                std::to_string(a6) +
                std::to_string(a7);

    return strReturn;
}
*/
#endif //ZYGISKPG_JNISTUFF_H
