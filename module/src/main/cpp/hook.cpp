
#include "hook.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <dobby.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "Memory/MemoryPatch.h"





#define GamePackageName "com.pixel.gun3d"

struct GlobalPatches {
    // let's assume we have patches for these functions for whatever game
    // boolean function
    MemoryPatch maxlvl, maxlvl1;
    // etc...
}gPatches;

int isGame(JNIEnv *env, jstring appDataDir) {
    if (!appDataDir)
        return 0;
    const char *app_data_dir = env->GetStringUTFChars(appDataDir, nullptr);
    int user = 0;
    static char package_name[256];
    if (sscanf(app_data_dir, "/data/%*[^/]/%d/%s", &user, package_name) != 2) {
        if (sscanf(app_data_dir, "/data/%*[^/]/%s", package_name) != 1) {
            package_name[0] = '\0';
            LOGW("can't parse %s", app_data_dir);
            return 0;
        }
    }
    if (strcmp(package_name, GamePackageName) == 0) {
        LOGI("detect game: %s", package_name);
        game_data_dir = new char[strlen(app_data_dir) + 1];
        strcpy(game_data_dir, app_data_dir);
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 1;
    } else {
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 0;
    }
}






int glHeight, glWidth;
bool setupimg;





HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}


bool maxlvl, maxlvl1, maxlvl2, maxlvl3, maxlvl4, maxlvl5, maxlvl6, maxlvl7, maxlvl8;

void DrawMenu(){
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGui::Begin("Pixel Gun 3D - chr1s#4191 && fedesito#0052 - https://discord.gg/dmaBN3MzNJ");
        if (ImGui::CollapsingHeader("AccountMods Mods")) {
            ImGui::Checkbox("Max Level ( DISABLE 1 AND ENABLE ANOTHER ONE AFTER, check if it actually patched in logcat)", &maxlvl);
            ImGui::Checkbox("Max Level 1", &maxlvl1);
            ImGui::Checkbox("Max Level 2", &maxlvl2);
            ImGui::Checkbox("Max Level 3", &maxlvl3);
            ImGui::Checkbox("Max Level 4", &maxlvl4);
            ImGui::Checkbox("Max Level 5", &maxlvl5);
            ImGui::Checkbox("Max Level 6", &maxlvl6);
            ImGui::Checkbox("Max Level 7", &maxlvl7);
            ImGui::Checkbox("Max Level 8", &maxlvl8);
        }
        gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C26554, "A03A8FD2C0035FD6");
        if (maxlvl) {
           if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL1");
            }
        }

        if(maxlvl1){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C2655C, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL1");
            }
        }

        if(maxlvl2){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C26564, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL2");
            }
        }

        if(maxlvl3){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C2656C, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL3");
            }
        }

        if(maxlvl4){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C26574, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL4");
            }
        }

        if(maxlvl5){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C2657C, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL5");
            }
        }

        if(maxlvl6){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C2657D, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL6");
            }
        }

        if(maxlvl7){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C26584, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL7");
            }
        }

        if(maxlvl8){
            gPatches.maxlvl = MemoryPatch::createWithHex("libil2cpp.so", 0x1C2658C, "A03A8FD2C0035FD6");
            if (gPatches.maxlvl.Modify()) {
                LOGE("PATCHED MAXLVL8");
            }
        }
        ImGui::End();
    }
}

void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui::StyleColorsLight();

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!setupimg) {
        SetupImgui();
        setupimg = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    DrawMenu();

    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}






void *hack_thread(void *arg) {
    sleep(5);
    auto eglhandle = dlopen("libunity.so", RTLD_LAZY);
    auto eglSwapBuffers = dlsym(eglhandle, "eglSwapBuffers");
    DobbyHook((void*)eglSwapBuffers,(dobby_dummy_func_t)hook_eglSwapBuffers,
         (dobby_dummy_func_t*)&old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        DobbyHook(sym_input,(dobby_dummy_func_t)myInput,(dobby_dummy_func_t*)&origInput);
    }
    LOGI("Draw Done!");
    return nullptr;
}