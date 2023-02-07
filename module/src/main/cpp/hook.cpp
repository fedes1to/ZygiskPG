#include "hook.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
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
#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyScanner.h"
#include "KittyMemory/KittyUtils.h"
#include "Include/obfuscate.h"
#include "Includes/Dobby/dobbyForHooks.h"
#include "Include/Unity.h"

using KittyMemory::ProcMap;
using KittyScanner::RegisterNativeFn;

ProcMap g_il2cppBaseMap;

#define GamePackageName "com.pixel.gun3d"

struct GlobalPatches {
    // let's assume we have patches for these functions for whatever game
    // boolean function
    MemoryPatch maxLevel;
    MemoryPatch unban;
    MemoryPatch vd;
    MemoryPatch cWear1;
    MemoryPatch cWear2;
    MemoryPatch uWear;
    MemoryPatch gadgetUnlock;
    // etc...
}gPatches;

static char loadLevel[] = "";
bool maxLevel, levelApplied, vd, vdApplied, cWear, cWearApplied, uWear, uWearApplied, gadgetUnlock, gadgetUnlockApplied;

// specify pointers to call here
void(*SetString)(monoString* key, monoString* value);
void(*LoadLevel)(monoString* key);
void Pointers() {
    SetString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + 0x4340BE0);
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + 0x46F498C);
}

void Patches() {

    // for maxLevel
    if (maxLevel && !levelApplied) {
        if (gPatches.maxLevel.Modify()) {
            KITTY_LOGI("maxLevel has been modified successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.maxLevel.get_CurrBytes().c_str());
        }
        levelApplied = true;
    } else if (!maxLevel && levelApplied)
    {
        if (gPatches.maxLevel.Restore()) {
            KITTY_LOGI("maxLevel has been restored successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.maxLevel.get_CurrBytes().c_str());
        }
        levelApplied = false;
    }

    //for vd
    if (vd && !vdApplied) {
        if (gPatches.vd.Modify()) {
            KITTY_LOGI("vd has been modified successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.vd.get_CurrBytes().c_str());
        }
        vdApplied = true;
    } else if (!vd && vdApplied)
    {
        if (gPatches.vd.Restore()) {
            KITTY_LOGI("vd has been restored successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.vd.get_CurrBytes().c_str());
        }
        vdApplied = false;
    }

    //for uWear
    if (uWear && !uWearApplied) {
        if (gPatches.uWear.Modify()) {
            KITTY_LOGI("uWear has been modified successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.uWear.get_CurrBytes().c_str());
        }
        uWearApplied = true;
    } else if (!uWear && uWearApplied)
    {
        if (gPatches.uWear.Restore()) {
            KITTY_LOGI("uWear has been restored successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.uWear.get_CurrBytes().c_str());
        }
        uWearApplied = false;
    }

    //for gadgetUnlock
    if (gadgetUnlock && !gadgetUnlockApplied) {
        if (gPatches.gadgetUnlock.Modify()) {
            KITTY_LOGI("uWear has been modified successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.gadgetUnlock.get_CurrBytes().c_str());
        }
        gadgetUnlockApplied = true;
    } else if (!gadgetUnlock && gadgetUnlockApplied)
    {
        if (gPatches.gadgetUnlock.Restore()) {
            KITTY_LOGI("uWear has been restored successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.gadgetUnlock.get_CurrBytes().c_str());
        }
        gadgetUnlockApplied = false;
    }

    //for cWear
    if (cWear && !cWearApplied) {
        if (gPatches.cWear1.Modify() && gPatches.cWear2.Modify()) {
            KITTY_LOGI("cWear has been modified successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.cWear1.get_CurrBytes().c_str());
            KITTY_LOGI("Current Bytes: %s", gPatches.cWear2.get_CurrBytes().c_str());
        }
        cWearApplied = true;
    } else if (!cWear && cWearApplied)
    {
        if (gPatches.cWear1.Restore() && gPatches.cWear2.Restore()) {
            KITTY_LOGI("cWear has been restored successfully");
            KITTY_LOGI("Current Bytes: %s", gPatches.cWear1.get_CurrBytes().c_str());
            KITTY_LOGI("Current Bytes: %s", gPatches.cWear2.get_CurrBytes().c_str());
        }
        cWearApplied = false;
    }
}

// here we start with the hooking methods
void (*old_WeaponSounds)(void *obj);
void WeaponSounds(void *obj) {
    if (obj != nullptr){
        // load level instance, even though i should hook a different function
        if (ImGui::IsItemClicked() && loadLevel != NULL)
        {
            LoadLevel(CreateIl2cppString(loadLevel));
        }
    }
    old_WeaponSounds(obj);
}

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

void DrawMenu(){
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGui::Begin("Pixel Gun 3D - chr1s#4191 && fedesito#0052 - https://discord.gg/dmaBN3MzNJ");
        if (ImGui::CollapsingHeader("Account Mods")) {
            ImGui::Checkbox("Max Level", &maxLevel);
            ImGui::Checkbox("Value Decrypt", &vd);
            ImGui::Checkbox("Unlock Wear", &uWear);
            ImGui::Checkbox("Craftable Wear", &cWear);
            ImGui::Checkbox("Gadget Unlocker", &gadgetUnlock);
            ImGui::InputText("", loadLevel, IM_ARRAYSIZE(loadLevel));
            ImGui::Button("Load Scene");
        }
        Patches();
        ImGui::End();
    }
}



void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(5.0f);
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
    do {
        sleep(1);
        g_il2cppBaseMap = KittyMemory::getLibraryBaseMap("libil2cpp.so");
    } while (!g_il2cppBaseMap.isValid());
    KITTY_LOGI("il2cpp base: %p", (void*)(g_il2cppBaseMap.startAddress));
    Pointers();

    // example of a hex patch
    gPatches.maxLevel = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1C26554,"A0F08FD2C0035FD6");
    gPatches.vd = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x3ED22F4,"00FA80D2C0035FD6");
    gPatches.uWear = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x257B7B4,"802580D2C0035FD6");
    gPatches.cWear1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F87C14,"802580D2C0035FD6");
    gPatches.cWear2 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F87AFC,"802580D2C0035FD6");
    gPatches.gadgetUnlock = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2C54AFC,"00008052C0035FD6");

    // example of a hook for arm64
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x17139E8), (void*)WeaponSounds, (void**)&old_WeaponSounds);

    auto eglhandle = dlopen("libunity.so", RTLD_LAZY);
    auto eglSwapBuffers = dlsym(eglhandle, "eglSwapBuffers");
    DobbyHook((void*)eglSwapBuffers,(void*)hook_eglSwapBuffers,
              (void**)&old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        DobbyHook(sym_input,(void*)myInput,(void**)&origInput);
    }
    LOGI("Draw Done!");
    return nullptr;
}