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
    MemoryPatch vd2, tutorial, tutorial1, vd1, gadgetUnlock, uWear, cWear2, cWear1, modKeys, maxLevel, unban, tgod, tgod1, tgod2, tgod3, rgod, rgod1,
  removedrone, godmode, godmode1, ammo, ammo1, removedrone1;  // etc...
}gPatches;

static int selectedScene = 0;
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
bool maxLevel, levelApplied, cWear, cWearApplied, uWear, uWearApplied, gadgetUnlock,
gadgetUnlockApplied, isLoadScenePressed, modKeys, modKeysApplied, vd, vdApplied, afdist, tgod, tgodapplied, rocketgodapplied,
rocketgod,removedrone,removedroneapplied, god, godapplied, ammo, ammoapplied, collectibles;

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
        gPatches.maxLevel.Modify();
        levelApplied = true;
    } else if (!maxLevel && levelApplied) {
        gPatches.maxLevel.Restore();
        levelApplied = false;
    }

    //for uWear
    if (uWear && !uWearApplied) {
        gPatches.uWear.Modify();
        uWearApplied = true;
    } else if (!uWear && uWearApplied) {
        gPatches.uWear.Restore();
        uWearApplied = false;
    }


    //for vd
    if (vd && !vdApplied) {
        gPatches.vd1.Modify();  gPatches.vd2.Modify();
        vdApplied = true;
    } else if (!vd && vdApplied)
    {
        gPatches.vd1.Restore(); gPatches.vd2.Restore();
        vdApplied = false;
    }

    //for gadgetUnlock
    if (gadgetUnlock && !gadgetUnlockApplied) {
        gPatches.gadgetUnlock.Modify();
        gadgetUnlockApplied = true;
    } else if (!gadgetUnlock && gadgetUnlockApplied)
    {
        gPatches.gadgetUnlock.Restore();
        gadgetUnlockApplied = false;
    }

    //for cWear
    if (cWear && !cWearApplied) {
        gPatches.cWear1.Modify(); gPatches.cWear2.Modify();
        cWearApplied = true;
    } else if (!cWear && cWearApplied) {
        gPatches.cWear1.Restore(); gPatches.cWear2.Restore();
        cWearApplied = false;
    }

    //for modKeys
    if (modKeys && !modKeysApplied) {
        gPatches.modKeys.Modify();
        modKeysApplied = true;
    } else if (!modKeys && modKeysApplied)
    {
        gPatches.modKeys.Restore();
        modKeysApplied = false;
    }

    //for turret godmode
    if (tgod && !tgodapplied) {
        gPatches.tgod.Modify(); gPatches.tgod1.Modify(); gPatches.tgod2.Modify(); gPatches.tgod3.Modify();
        tgodapplied = true;
    } else if (!tgod && tgodapplied)
    {
        gPatches.tgod.Restore(); gPatches.tgod1.Restore(); gPatches.tgod2.Restore(); gPatches.tgod3.Restore();
        tgodapplied = false;
    }

    //for removedrone
    if (removedrone && !removedroneapplied) {
        gPatches.removedrone.Modify(); gPatches.removedrone1.Modify();
        removedroneapplied = true;
    } else if (!removedrone && removedroneapplied)
    {
        gPatches.removedrone.Restore(); gPatches.removedrone1.Restore();
        removedroneapplied = false;
    }

    //for removedrone
    if (god && !godapplied) {
        gPatches.godmode.Modify(); gPatches.godmode1.Modify();
        godapplied = true;
    } else if (!god && godapplied)
    {
        gPatches.godmode.Restore(); gPatches.godmode1.Restore();
        godapplied = false;
    }

    //for ammo
    if (ammo && !ammoapplied) {
        gPatches.ammo.Modify(); gPatches.ammo1.Modify();
        ammoapplied = true;
    } else if (!ammo && ammoapplied)
    {
        gPatches.ammo.Restore(); gPatches.ammo1.Restore();
        ammoapplied = false;
    }
}

void (*old_PixelTime)(void *obj);
void PixelTime(void *obj) {
    if (obj != nullptr){
        // load level instance, even though i should hook a different function
        if (isLoadScenePressed)
        {
            LOGI("trying to load scene");
            LoadLevel(CreateIl2cppString(sceneList[selectedScene]));
            isLoadScenePressed = false;
        }
    }
    old_PixelTime(obj);
}


int (*oldCollectibles)(void* obj, int* value);
int Collectibles(void* obj, int* value) {
    return 3000;
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
            ImGui::Text("Gives the player Max Level after you complete a match. (Use this after you get Level 3)");
            ImGui::Checkbox("Collectibles (Test)", &collectibles);
            ImGui::Text("Sets the value of items to a specific number");
            ImGui::Checkbox("Free Craftables", &cWear);
            ImGui::Text("Unlocks Craftables (Only works on Wear and Gadgets)");
            ImGui::Checkbox("Free Lottery", &modKeys);
            ImGui::Text("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)");
        }
        if (ImGui::CollapsingHeader("Player Mods")) {
            ImGui::Checkbox("Godmode", &god);
            ImGui::Text("Makes you invincible, others can kill you but you won't die and just become invisible");
        }
        if (ImGui::CollapsingHeader("Weapon Mods")) {
            ImGui::Checkbox("Infinite Ammo", &ammo);
        }
        if (ImGui::CollapsingHeader("Game Mods")) {
            ImGui::Checkbox("Turret Godmode", &tgod);
            ImGui::Text("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does.");
            ImGui::Checkbox("Drone Godmode", &removedrone);
            ImGui::Text("The drone gadget will never respawn. (Don't get more than 1 drone or you'll be kicked)");
        }
        if (ImGui::CollapsingHeader("Misc Mods"))
        {
            ImGui::ListBox("Select Scene", &selectedScene, sceneList, IM_ARRAYSIZE(sceneList), 4);
            if (ImGui::Button("Load Scene"))
            {
                LOGI("%s", sceneList[selectedScene]);
                isLoadScenePressed = true;
            }
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


void Modifications(){

    // hex patches
    gPatches.maxLevel = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1C26554,"A0F08FD2C0035FD6");
    gPatches.uWear = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x257B7B4,"802580D2C0035FD6");
    gPatches.cWear1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F87C14,"802580D2C0035FD6");
    gPatches.cWear2 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F87AFC,"802580D2C0035FD6");
    gPatches.gadgetUnlock = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2C54AFC,"00008052C0035FD6");
    gPatches.modKeys = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x48EF240,"603E8012C0035FD6");
    gPatches.vd1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F87D98,"00FA80D2C0035FD6");
    gPatches.vd2 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x2F95CF8,"00FA80D2C0035FD6");
    gPatches.tgod = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BC8EB8,"C0035FD6");//MinusLive
    gPatches.tgod1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BCE010,"C0035FD6");//MinusLive
    gPatches.tgod2 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BCE2A8,"C0035FD6");//MinusLiveReal
    gPatches.rgod = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x320A640,"200080D2C0035FD6");//search GameObject in Rocket
    gPatches.rgod1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x321A6BC,"200080D2C0035FD6");//search GameObject in Rocket
    gPatches.removedrone = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x4755120,"C0035FD6");//dear future self, if this game ever updates kys (find gadgetinfo by using analyze on an older vers, and then analyze gadgetinfo and find it (hopefully) )
    gPatches.removedrone1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x47551D8,"C0035FD6");//dear future self, if this game ever updates kys (find gadgetinfo by using analyze on an older vers, and then analyze gadgetinfo and find it (hopefully) )
    gPatches.godmode = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x476323C,"1F2003D5C0035FD6");//dear future self, if this game ever updates kys (look for player_move_c and try to find the enum with himself, headshot etc and pray you find the right thing, has alot of stuff in the args )
    gPatches.godmode1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x3C958B0,"1F2003D5C0035FD6");//dear future self, if this game ever updates kys (get the saltedint chinese bullshit name, find it and try to find the class around those fields. )
    gPatches.ammo = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x14193E4,"200180922C0035FD6");//dear future self, if this game ever updates kys (get the saltedint chinese bullshit name, find it and try to find the class around those fields. )
    gPatches.ammo1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x14193D8,"200180922C0035FD6");//dear future self, if this game ever updates kys ( find the Weapon class within player_move_c and some subclass which also has an ItemRecord field.)

    // hooks
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x3BBD870), (void*)Collectibles, (void**)&oldCollectibles);
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x4051E70), (void*)PixelTime, (void**)&old_PixelTime);

}

void *hack_thread(void *arg) {
    do {
        sleep(1);
        g_il2cppBaseMap = KittyMemory::getLibraryBaseMap("libil2cpp.so");
    } while (!g_il2cppBaseMap.isValid());
    KITTY_LOGI("il2cpp base: %p", (void*)(g_il2cppBaseMap.startAddress));

    Pointers();
    Modifications();

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
