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
#include"Includes/Dobby/dobbyForHooks.h"
#include "Include/Unity.h"
#include "Misc.h"
#include "hook.h"
#include "Include/Roboto-Regular.h"

#define GamePackageName "com.pixel.gun3d"

/*
 *
 * LIST OF NOTES:
 * GAMEMODES LIST: 丂丛万世丝七丒与丈, GameMode.cs << DONT TRUST THIS, ITS 丈下东与三丆下丛丄
 * 丒丘丁丄丂丑丛丄东, ItemReward.cs (Scripts\\Store\\MainStore\\ItemReward.cs)
 * 丒下丗丈丕三丌丏不, ProgressUpdater.Currency.cs (Scripts\\ServerProgressSync\\ProgressUpdater.Currency.cs)
 * 丐三丐丄丝丅丈丄丞, OfferData.cs (Scripts/OfferPacks/OfferData.cs)
 * SCENEINFO IS SceneInfo, FULLY OBFUSCATED
 * SCENEINFO 且与丒与丈下丈丁一 = AddMode(GameMode)
 *
 */

monoString* CreateIl2cppString(const char* str)
{
    static monoString* (*CreateIl2cppString)(const char* str, int *startIndex, int *length) =
    (monoString* (*)(const char* str, int *startIndex, int *length))(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43F5F10")));
    int* startIndex = 0;
    int* length = (int *)strlen(str);
    return CreateIl2cppString(str, startIndex, length);
}

static int selectedScene = 0;
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
bool maxLevel, cWear, uWear, gadgetUnlock, isLoadScenePressed, modKeys, tgod,
removedrone, god, ammo, collectibles, ezsuper, changeID, isOpenKeyboard,
crithit, charm, fte,enemymarker, enableEditor, killboost, electric, kspeedboost, daterweapon, grenade,
doublejump, coindrop, itemParams, blackMarket, couponClicker, setsClicker,
negativeCollectibles, nullcollectibles, isDiscordPressed, isKaxzWeps;

float damage;

void (*SetString) (monoString* key, monoString* value);
void (*LoadLevel) (monoString* key);
void (*OpenURL) (monoString* url);
void (*OpenKeyboard) (monoString* text, int* keyboardType, bool* autoCorrection);
void Pointers() {
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x46F498C")));
    OpenURL = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43807DC")));
    OpenKeyboard = (void(*)(monoString*, int*, bool*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435E2D4")));
    SetString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2ECD530")));
}

void(*oldWeaponSounds)(void* obj);
void WeaponSounds(void* obj){
    if(obj != nullptr){
        if(damage != NULL){
            *(float*)((uint64_t) obj + 0x388) = damage;//shotgunMaxDamageDistance
            *(float*)((uint64_t) obj + 0x38C) = damage;//shotgunMinDamageCoef
            *(float*)((uint64_t) obj + 0x390) = damage;//shotgunOverDamageDistance
            *(float*)((uint64_t) obj + 0x394) = damage;//shotgunOverDamageCoef
            *(float*)((uint64_t) obj + 0x3FC) = damage;//damageMultiplier
        }

        if(killboost){
            *(bool*)((uint64_t) obj + 0x3F8) = true;//isIncreasedDamageFromKill
            *(int*)((uint64_t) obj + 0x400) = 3;//maxStackIncreasedDamage
        }

        if(charm){
            *(bool*)((uint64_t) obj + 0x260) = true;//isCharm
            *(float*)((uint64_t) obj + 0x264) = 99999;//charmTime
        }

        if(fte){
            *(bool*)((uint64_t) obj + 0x234) = true;//fireImmunity
            *(bool*)((uint64_t) obj + 0x235) = true;//toxicImmunity
        }

        if(enemymarker){
            *(bool*)((uint64_t) obj + 0x236) = true;//enemyMarker
            *(bool*)((uint64_t) obj + 0x238) = true;//enemyMarkerWhenShot
            *(bool*)((uint64_t) obj + 0x237) = true;//enemyMarkerWhenAiming
          //  *(float*)((uint64_t) obj + 0x240) = 999;//enemyMarkerAngle
           // *(float*)((uint64_t) obj + 0x240) = 0;//enemyMarketChargeTime
          //  *(float*)((uint64_t) obj + 0x240) = 999999;//enemyMarkerTriangleCathetLength
        }

        if(electric){
            *(bool*)((uint64_t) obj + 0x404) = true;//isElectricShock
            *(float*)((uint64_t) obj + 0x408) = 99999;//electricShockCoeff
            *(float*)((uint64_t) obj + 0x40C) = 99999;//electricShockTime
        }

        if(kspeedboost){
            *(bool*)((uint64_t) obj + 0x410) = true;//isFastAfterKill
            *(float*)((uint64_t) obj + 0x414) = 99999;//fastMultiplier
           *(int*)((uint64_t) obj + 0x418) = 99999;//maxStackAfterKill
            *(int*)((uint64_t) obj + 0x41C) = 99999;//timeFastAfterKill
        }

        if(daterweapon){
            *(bool*)((uint64_t) obj + 0x428) = true;//isDaterWeapon
        }

        if(crithit){
            *(float*)((uint64_t) obj + 0x438) = 9999;//criticalHitCoef
            *(float*)((uint64_t) obj + 0x434) = 100;//criticalHitChance
        }

        if(grenade){
            *(float*)((uint64_t) obj + 0x5B8) = 9999;//grenadeUseTime
        }

        if(doublejump){
            *(bool*)((uint64_t) obj + 0x42F) = true;//isDoubleJump
        }

        if(ammo){
            *(bool*)((uint64_t) obj + 0x421) = true;//isUnlimitedAmmo
        }

        if(coindrop){
            *(bool*)((uint64_t) obj + 0x318) = true;//isCoinDrop
            *(float*)((uint64_t) obj + 0x31C) = 9999;//coinDropChance
        }
    }
    oldWeaponSounds(obj);
}

void (*old_PixelTime)(void *obj);
void PixelTime(void *obj) {
    if (obj != nullptr && isLoadScenePressed) {
        // this update is always active, use it to call LoadScene or whatever
        LoadLevel(CreateIl2cppString(sceneList[selectedScene]));
        isLoadScenePressed = false;
    } else if (obj != nullptr && isDiscordPressed)
    {
        OpenURL(CreateIl2cppString(OBFUSCATE("https://discord.gg/g3pjD5M3BZ")));
        isDiscordPressed = false;
    } else if (obj != nullptr && changeID) {
        SetString(CreateIl2cppString(OBFUSCATE("AccountCreated")), CreateIl2cppString(OBFUSCATE("Solotov#2160")));
    } else if (obj != nullptr && isOpenKeyboard) {
        OpenKeyboard(CreateIl2cppString(""), (int *)(0), (bool *)(false));
    }
    old_PixelTime(obj);
}

int (*old_itemParamsInt)(void *obj);
int itemParamsInt(void *obj) {
    if (obj != nullptr && itemParams){
        return 2000;
    }
    old_itemParamsInt(obj);
}

bool (*old_isEditor)(void *obj);
bool isEditor(void *obj) {
    if (enableEditor)
    {
        return true;
    } else {
        return false;
    }
}

bool (*old_isDev)(void *obj);
bool isDev(void *obj) {
    if (enableEditor)
    {
        return true;
    } else {
        return false;
    }
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
            LOGW(OBFUSCATE("can't parse %s"), app_data_dir);
            return 0;
        }
    }
    if (strcmp(package_name, GamePackageName) == 0) {
        LOGI(OBFUSCATE("detect game: %s"), package_name);
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

void Hooks() {
    // hooks
    HOOK("0x4051E70", PixelTime, old_PixelTime);
    HOOK("0x17139E8", WeaponSounds, oldWeaponSounds);
    HOOK("0x438120C", isEditor, old_isEditor);
    HOOK("0x2ADECFC", isDev, old_isDev);
}

void Patches() {
    PATCH_SWITCH("0x476323C", "1F2003D5C0035FD6", god);
    PATCH_SWITCH("0x3C958B0", "1F2003D5C0035FD6", god);
    PATCH_SWITCH("0x1C26554", "A0F08FD2C0035FD6", maxLevel);
    PATCH_SWITCH("0x257B7B4", "802580D2C0035FD6", uWear);
    PATCH_SWITCH("0x2F87C14", "802580D2C0035FD6", cWear);
    PATCH_SWITCH("0x2F87AFC", "802580D2C0035FD6", cWear);
    PATCH_SWITCH("0x2C54AFC", "00008052C0035FD6", gadgetUnlock);
    PATCH_SWITCH("0x48EF240", "603E8012C0035FD6", modKeys);
    PATCH_SWITCH("0x1BC8EB8", "C0035FD6", tgod);
    PATCH_SWITCH("0x1BCE010", "C0035FD6", tgod);
    PATCH_SWITCH("0x1BCE2A8", "C0035FD6", tgod);
    PATCH_SWITCH("0x4755120", "C0035FD6", removedrone);
    PATCH_SWITCH("0x47551D8", "C0035FD6", removedrone);
    PATCH_SWITCH("0x3ED22F4", "00FA80D2C0035FD6", collectibles);
    PATCH_SWITCH("0x3ED22F4", "603E8012C0035FD6", negativeCollectibles);
    PATCH_SWITCH("0x3ED22F4", "000080D2C0035FD6", nullcollectibles);
    PATCH_SWITCH("0x1714718", "200080D2C0035FD6", crithit);
    PATCH_SWITCH("0x1595AE0", "200080D2C0035FD6", blackMarket);
    PATCH_SWITCH("0x1DD567C", "200080D2C0035FD6", couponClicker);
    PATCH_SWITCH("0x1DD609C", "200080D2C0035FD6", setsClicker);
    PATCH("0x206D13C", "C0035FD6");
}

void DrawMenu(){
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGui::Begin(OBFUSCATE("ZygiskPG 0.1a (23.0.1) - chr1s#4191 && fedesito#0052 && ohmyfajett#3500"));
        if (ImGui::Button(OBFUSCATE("Join Discord")))
        {
            isDiscordPressed = true;
        }
        ImGui::Text("Its Recommended to join the discord server for mod updates etc.");
        if (ImGui::CollapsingHeader(OBFUSCATE("Account Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Max Level"), &maxLevel);
            ImGui::Text("Gives the player Max Level after you complete a match. (Use this after you get Level 3)");
            ImGui::Checkbox("Free Craftables", &cWear);
            ImGui::Text("Unlocks Craftables (Only works on Wear and Gadgets)");
            ImGui::Checkbox(OBFUSCATE("Free Lottery"), &modKeys);
            ImGui::Text("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)");
            ImGui::Checkbox(OBFUSCATE("Infinite Gems"), &couponClicker);
            ImGui::Text("Go into gallery and spam click on the weapons to get gems.");
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Player Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Godmode"), &god);
            ImGui::Text("Makes you invincible (others can kill you but you won't die and just become invisible)");
            ImGui::Checkbox(OBFUSCATE("Force Double Jump"), &doublejump);
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Weapon Mods"))) {
            ImGui::SliderFloat(OBFUSCATE("Damage Buff"),&damage, 0.0f, 15.0f);
            ImGui::Text("Amplifys the damage. (Anything above 8 might kick after a few kills)");
            ImGui::Checkbox(OBFUSCATE("Force Critical Hits"), &crithit);
            ImGui::Text("Forces Critical Shots each time you hit someone.");
            ImGui::Checkbox(OBFUSCATE("Unlimited Ammo"), &ammo);
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Effect Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Force Charm"), &charm);
            ImGui::Text("Adds the charm effect (Used to reduce half of the enemy's weapon efficiency)");
            ImGui::Checkbox(OBFUSCATE("Force Kill Damage Boost"), &killboost);
            ImGui::Text("Gives you damage boost after every kill.");
            ImGui::Checkbox(OBFUSCATE("No Fire and Toxic Effects"), &fte);
            ImGui::Text("Removes the burning and being intoxicated effect on you.");
            ImGui::Checkbox(OBFUSCATE("Force Electric Shock"), &electric);
            ImGui::Text("Adds the electric shock effect");
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Visual Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Show marker"), &enemymarker);
            ImGui::Text("Shows the enemy after you shoot them once.");
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Game Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Turret Godmode"), &tgod);
            ImGui::Text("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does.");
            ImGui::Checkbox(OBFUSCATE("Drone Godmode"), &removedrone);
            ImGui::Text("The drone gadget will never despawn. (Don't get more than 1 drone or you'll be kicked)");
            ImGui::Checkbox(OBFUSCATE("Force Coin Drop"), &coindrop);
            ImGui::Text("Always drops coins when someone dies.");
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Misc Mods")))
        {
            ImGui::Checkbox(OBFUSCATE("Spoof Editor"), &enableEditor);
            ImGui::Text("Makes the game think its on the Unity Editor");
            ImGui::ListBox(OBFUSCATE("Select Scene"), &selectedScene, sceneList, IM_ARRAYSIZE(sceneList), 4);
            if (ImGui::Button(OBFUSCATE("Load Scene"))) {
                isLoadScenePressed = true;
            }
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Bannable Mods")))
        {
            ImGui::Checkbox(OBFUSCATE("Collectibles"), &collectibles);
            ImGui::Text("Sets the value of items to 2000");
            ImGui::Checkbox(OBFUSCATE("Null Collectibles"), &nullcollectibles);
            ImGui::Text("Sets the value of items to 0");
            ImGui::Checkbox(OBFUSCATE("Negative Collectibles"), &negativeCollectibles);
            ImGui::Text("Sets the value of items to -500");
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
    ImGui::GetStyle().ScaleAllSizes(6.0f);
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, 30, 30.0f);
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
    Hooks();

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
