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
    MemoryPatch gadgetUnlock, uWear, cWear2, cWear1, modKeys, maxLevel, unban, tgod, tgod1, tgod2, tgod3, rgod, rgod1,
  removedrone, godmode, godmode1, ammo, ammo1, removedrone1, collectibles, negCollectibles, ezsuper, ezsuper1, currencycheck, crithit,
  blackMarket, couponClicker, setsClicker;  // etc...
}gPatches;

static int selectedScene = 0;
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
bool maxLevel, levelApplied, cWear, cWearApplied, uWear, uWearApplied, gadgetUnlock,
gadgetUnlockApplied, isLoadScenePressed, modKeys, modKeysApplied, tgod, tgodapplied,
removedrone, removedroneapplied, god, godapplied, ammo, ammoapplied, collectibles, collectiblesApplied, ezsuper, ezsuperApplied,
crithit, crithitapplied, damage, charm, weakness,fte,enemymarker, enableEditor, killboost, electric, kspeedboost, daterweapon, grenade,
doublejump, catspam, coindrop, itemParams, blackMarket, blackMarketApplied, couponClicker, couponClickerApplied, setsClicker, setsClickerApplied,
negativeCollectibles;

// specify pointers to call here
void(*SetString)(monoString* key, monoString* value);
void(*LoadLevel)(monoString* key);
void Pointers() {
    SetString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + 0x4340BE0);
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + 0x46F498C );

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

    } else if (!uWear && uWearApplied) {
        gPatches.uWear.Restore();
        uWearApplied = false;
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

    //for godmode
    if (god && !godapplied) {
        gPatches.godmode.Modify(); gPatches.godmode1.Modify();
        godapplied = true;
    } else if (!god && godapplied)
    {
        gPatches.godmode.Restore(); gPatches.godmode1.Restore();
        godapplied = false;
    }

    //for collectibles
    if (collectibles && !collectiblesApplied) {
        gPatches.collectibles.Modify();
        collectiblesApplied = true;
    } else if (!collectibles && collectiblesApplied)
    {
        gPatches.collectibles.Restore();
        collectiblesApplied = false;
    }

    //for collectibles (-)
    if (negativeCollectibles && !collectiblesApplied) {
        gPatches.negCollectibles.Modify();
        collectiblesApplied = true;
    } else if (!negativeCollectibles && collectiblesApplied)
    {
        gPatches.negCollectibles.Restore();
        collectiblesApplied = false;
    }

    if (ezsuper && !ezsuperApplied) {
        gPatches.ezsuper.Modify();  gPatches.ezsuper1.Modify();
        ezsuperApplied = true;
    } else if (!ezsuper && ezsuperApplied)
    {
        gPatches.ezsuper.Restore();  gPatches.ezsuper1.Restore();
        ezsuperApplied = false;
    }

    if (crithit && !crithitapplied) {
        gPatches.crithit.Modify();
        crithitapplied = true;
    } else if (!crithit && crithitapplied)
    {
        gPatches.crithit.Restore();
        crithitapplied = false;
    }

    if (blackMarket && !blackMarketApplied) {
        gPatches.blackMarket.Modify();
        blackMarketApplied = true;
    } else if (!blackMarket && blackMarketApplied)
    {
        gPatches.blackMarket.Restore();
        blackMarketApplied = false;
    }

    if (couponClicker && !couponClickerApplied) {
        gPatches.couponClicker.Modify();
        couponClickerApplied = true;
    } else if (!couponClicker && couponClickerApplied)
    {
        gPatches.couponClicker.Restore();
        couponClickerApplied = false;
    }

    if (setsClicker && !setsClickerApplied) {
        gPatches.setsClicker.Modify();
        setsClickerApplied = true;
    } else if (!setsClicker && setsClickerApplied)
    {
        gPatches.setsClicker.Restore();
        setsClickerApplied = false;
    }
}

void(*oldWeaponSounds)(void* obj);
void WeaponSounds(void* obj){
    if(obj != nullptr){
        if(damage){
            *(float*)((uint64_t) obj + 0x200) = 200;//poisonDamageMultiplier
            *(float*)((uint64_t) obj + 0x21C) = 200;//curseDamageMultiplier
            *(float*)((uint64_t) obj + 0x168) = 200;//curseDamageMultiplier
            *(bool*)((uint64_t) obj + 0x200) = true;//isHeadshotDamageIncreased
            *(float*)((uint64_t) obj + 0x274) = 200;//increasedHeadshotDamageMultiplier
            *(bool*)((uint64_t) obj + 0x278) = false;//isReducedHeadshotDamage
            *(float*)((uint64_t) obj + 0x27C) = 0;//reducedHeadshotDamageMultiplier
            *(float*)((uint64_t) obj + 0x388) = 999;//shotgunMaxDamageDistance
            *(float*)((uint64_t) obj + 0x38C) = 999;//shotgunMinDamageCoef
            *(float*)((uint64_t) obj + 0x390) = 999;//shotgunOverDamageDistance
            *(float*)((uint64_t) obj + 0x394) = 999;//shotgunOverDamageCoef
        }

        if(killboost){
            *(bool*)((uint64_t) obj + 0x3F8) = true;//isIncreasedDamageFromKill
            *(bool*)((uint64_t) obj + 0x3FC) = true;//damageMultiplier
            *(int*)((uint64_t) obj + 0x400) = 999;//maxStackIncreasedDamage
        }

        if(charm){
            *(bool*)((uint64_t) obj + 0x260) = true;//isCharm
            *(float*)((uint64_t) obj + 0x264) = 9999;//charmTime
        }

        if(weakness){
            *(bool*)((uint64_t) obj + 0x26C) = true;//isWeaknessEffect
            *(float*)((uint64_t) obj + 0x26C) = 999;//weaknessEffectTime
            *(bool*)((uint64_t) obj + 0x254) = true;//isBlindEffect
            *(float*)((uint64_t) obj + 0x25C) = 999;//isBlindEffectTime
            *(int*)((uint64_t) obj + 0x258) = 999; //blindEffect
            *(bool*)((uint64_t) obj + 0x220) = true;//isSlowdown
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
            *(float*)((uint64_t) obj + 0x408) = 99999;//teammateDamageBoostBuffTime
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

        if(catspam){
            *(bool*)((uint64_t) obj + 0x640) = false;//is3CatSpam
            *(bool*)((uint64_t) obj + 0x641) = true;//for3CatSpam
        }

        if(damage){
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
    if (obj != nullptr && isLoadScenePressed){
        // this update is always active, use it to call LoadScene or whatever
        LoadLevel(CreateIl2cppString(sceneList[selectedScene]));
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
        ImGui::Begin("Pixel Gun 3D 23.0.1 Mod Menu (0.1) - chr1s#4191 && fedesito#0052 - https://discord.gg/dmaBN3MzNJ");
        if (ImGui::CollapsingHeader("Account Mods")) {
            ImGui::Checkbox("Max Level", &maxLevel);
            ImGui::Text("Gives the player Max Level after you complete a match. (Use this after you get Level 3)");
            ImGui::Checkbox("Collectibles (Test)", &collectibles);
            ImGui::Text("Sets the value of items to 2000");
            ImGui::Checkbox("Negative Collectibles (Test)", &negativeCollectibles);
            ImGui::Text("Sets the value of items to 2000");
            ImGui::Checkbox("Free Craftables", &cWear);
            ImGui::Text("Unlocks Craftables (Only works on Wear and Gadgets)");
            ImGui::Checkbox("Gadget Unlocker", &gadgetUnlock);
            ImGui::Text("Unlocks Clan Gadgets");
            ImGui::Checkbox("Free Lottery", &modKeys);
            ImGui::Text("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)");
            ImGui::Checkbox("Black Market Clicker", &blackMarket);
            ImGui::Text("Go to black market and enjoy");
            ImGui::Checkbox("Coupon Gems (Weapons)", &couponClicker);
            ImGui::Checkbox("Coupon Gems (Sets)", &setsClicker);
            ImGui::Text("Lets you click in gallery to get gems");
        }
        if (ImGui::CollapsingHeader("Player Mods")) {
            ImGui::Checkbox("Godmode", &god);
            ImGui::Text("Makes you invincible (others can kill you but you won't die and just become invisible)");
            ImGui::Checkbox("Force Double Jump", &doublejump);
        }
        if (ImGui::CollapsingHeader("Weapon Mods")) {
            ImGui::Checkbox("One Shot Kill", &damage);
            ImGui::Text("Kills any entity instantly.");
            ImGui::Checkbox("Force Critical Hits", &crithit);
            ImGui::Text("Forces Critical Shots each time you hit someone.");
            ImGui::Checkbox("No Grenade Cooldown", &grenade);
            ImGui::Checkbox("Unlimited Ammo", &ammo);
        }
        if (ImGui::CollapsingHeader("Effects Mods")) {
            ImGui::Checkbox("Force Weakness Effects", &weakness);
            ImGui::Text("Add effects which ruin your enemys stats.");
            ImGui::Checkbox("Force Charm", &charm);
            ImGui::Text("Adds the charm effect (Used to reduce half of the enemy's weapon efficiency)");
            ImGui::Checkbox("Force Kill Damage Boost", &killboost);
            ImGui::Text("Gives you damage boost after every kill.");
            ImGui::Checkbox("Force Kill Speed Boost", &killboost);
            ImGui::Text("Gives you speed boost after every kill.");
            ImGui::Checkbox("No Fire and Toxic Effects", &fte);
            ImGui::Text("Removes the burning and being intoxicated effect on you.");
            ImGui::Checkbox("Force Electric Shock", &electric);
            ImGui::Text("Adds the electric shock effect");
        }
        if (ImGui::CollapsingHeader("Visual Mods")) {
            ImGui::Checkbox("Show marker", &enemymarker);
            ImGui::Text("Shows the enemy after you aim at them.");
        }
        if (ImGui::CollapsingHeader("Game Mods")) {
            ImGui::Checkbox("Turret Godmode", &tgod);
            ImGui::Text("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does.");
            ImGui::Checkbox("Drone Godmode", &removedrone);
            ImGui::Text("The drone gadget will never despawn. (Don't get more than 1 drone or you'll be kicked)");
            ImGui::Checkbox("Force Weapons in Sandbox", &daterweapon);
            ImGui::Text("You can equip normal weapons in sandbox.");
            ImGui::Checkbox("Force Coin Drop", &coindrop);
            ImGui::Text("Always drops coins when someone dies.");
        }
        if (ImGui::CollapsingHeader("Misc Mods"))
        {
            ImGui::Checkbox("Spoof Editor", &enableEditor);
            ImGui::Text("Makes the game think its on the Unity Editor");
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
    ImGui::SetNextWindowSize(ImVec2((float) 500, (float) 0.0f));
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
    gPatches.tgod = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BC8EB8,"C0035FD6");//MinusLive
    gPatches.tgod1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BCE010,"C0035FD6");//MinusLive
    gPatches.tgod2 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1BCE2A8,"C0035FD6");//MinusLiveReal
    gPatches.removedrone = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x4755120,"C0035FD6");//dear future self, if this game ever updates kys (find gadgetinfo by using analyze on an older vers, and then analyze gadgetinfo and find it (hopefully) )
    gPatches.removedrone1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x47551D8,"C0035FD6");//dear future self, if this game ever updates kys (find gadgetinfo by using analyze on an older vers, and then analyze gadgetinfo and find it (hopefully) )
    gPatches.godmode = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x476323C,"1F2003D5C0035FD6");//dear future self, if this game ever updates kys (look for player_move_c and try to find the enum with himself, headshot etc and pray you find the right thing, has alot of stuff in the args )
    gPatches.godmode1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x3C958B0,"1F2003D5C0035FD6");//dear future self, if this game ever updates kys (get the saltedint chinese bullshit name, find it and try to find the class around those fields. )
    gPatches.collectibles = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x3BBD870,"00FA80D2C0035FD6"); // 2000 0x3BBD870
    gPatches.negCollectibles = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x3BBD870,"603E8012C0035FD6"); // -500 0x3BBD870
    gPatches.ezsuper = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x39CE814,"200080D2C0035FD6");//GameEventProgressBar ints
    gPatches.ezsuper1 = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x39CE860,"200080D2C0035FD6");//GameEventProgressBar ints
    gPatches.currencycheck = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x206D13C,"C0035FD6");//CurrencyUpdater the method with int, possible bypass?
    gPatches.crithit = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1714718,"200080D2C0035FD6");//NextHitCritical
    gPatches.blackMarket = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1595AE0,"200080D2C0035FD6");
    gPatches.couponClicker = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1DD567C,"200080D2C0035FD6");
    gPatches.setsClicker = MemoryPatch::createWithHex(g_il2cppBaseMap, 0x1DD609C,"200080D2C0035FD6");

    // hooks
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x4051E70), (void*)PixelTime, (void**)&old_PixelTime);
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x17139E8), (void*)WeaponSounds, (void**)&oldWeaponSounds);
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x438120C), (void*)isEditor, (void**)&old_isEditor);
    DobbyHook((void*)(g_il2cppBaseMap.startAddress + 0x2ADECFC), (void*)isDev, (void**)&old_isDev);
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
