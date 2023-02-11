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
#include "Include/Vector3.h"
#include "Include/Quaternion.h"

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
 * 0x43ADAE0 PhotonNetwork.SetMasterClient(Player masterClientPlayer)
 * 世丏丞丁丌丙丝丏丝 Player/PhotonPlayer
 * 世丏丞丁丌丙丝丏丝.三万万一丐与丐一下 LocalPlayer (0x43A6504 get)
 * 0x43ACB28 Instantiate()
 *
 */

/*
 *
 * COMPILE SETTINGS:
 * - BIGSEX: EXPERIMENTAL FEATURES, NOT FINAL AND SHOULDN'T BE BUILT UNTIL WORKING FINE
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

void* webInstance()
{
    static void*(*webInstance)() = (void* (*)())(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16E25C4")));
    return webInstance();
}

static int selectedScene = 0;
static int selectedCur = 0;
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
const char* curList[] = { "ClanSilver", "ClanLootBoxPoints", "Real", "RealCoins", "RealGems", "RealSubscriptions", "Free", "Advert", "KeySmallChest", "KeyBigChest", "KeyEventChest", "EventRouletteSuperSpin", "EventChestsSuperSpin", "PixelPassCurrency", "RouletteAdsCurrency", "RouletteAdsSpin", "CurrencyCompetitionTier1", "CurrencyCompetitionTier2", "PixelBucks", "TopUpCurrency", "AdventNyCurrency", "GemsCurrency", "Coins" };
bool maxLevel, cWear, uWear, gadgetUnlock, isLoadScenePressed, modKeys, tgod,
removedrone, god, ammo, collectibles, ezsuper, changeID, isOpenKeyboard,
crithit, charm, fte,enemymarker, enableEditor, killboost, electric, kspeedboost, daterweapon, grenade,
doublejump, coindrop, itemParams, blackMarket, couponClicker, setsClicker,
negativeCollectibles, nullcollectibles, isDiscordPressed, webLevel, blindness, wepSkins, kniferange, expbull,
spleef, shotbull, railbull, poison, jumpdisable, slowdown, headmagnify, destroy, recoilandspread, quickscope, speedup, speed,
isAddCurPressed;

float damage, rimpulseme, rimpulse;
int reflection;

void (*SetString) (monoString* key, monoString* value);
void (*LoadLevel) (monoString* key);
void (*OpenURL) (monoString* url);
void (*OpenKeyboard) (monoString* TextUnformatted, int* keyboardType, bool* autoCorrection);
void (*setSomething) (void* instance, monoString* currencyType, int* value, int* num);

bool (*SetMasterClient) (void* masterClientPlayer);
void* (*get_LocalPlayer) ();
void (*DestroyPlayerObjects)(void *player);
monoArray<void**> *(*PhotonNetwork_playerListothers)();

#ifdef BIGSEX
void (*DestroyAll) ();
// public static GameObject Instantiate(string prefabName, Vector3 position, Quaternion rotation, byte group = 0, object[] data = null)
void* (*Instantiate) (monoString* prefabName, Vector3 position, Quaternion rotation);
#endif

void Pointers() {
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x46F498C")));
    OpenURL = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43807DC")));
    OpenKeyboard = (void(*)(monoString*, int*, bool*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435E2D4")));
    SetString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2ECD530")));
    setSomething = (void(*) (void*, monoString*, int*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16EB844")));
    SetMasterClient = (bool(*)(void*)) (bool*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ADAE0"));
    get_LocalPlayer = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43A6504"));
    DestroyPlayerObjects = (void (*)(void *)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ADF9C"));
    PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) (monoArray<void**>*) (g_il2cppBaseMap.startAddress + string2Offset("0x43A6814"));

#ifdef BIGSEX
    DestroyAll = (void(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43AE1C0"));
    Instantiate = (void*(*)(monoString*, Vector3, Quaternion)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ACB28"));
#endif
}

// 0x435FA0C <- offset for gameobject.tag
// 0x434733C <- offset for object.name

#ifdef BIGSEX
monoString* (*old_getTag)(void *obj);
monoString* getTag(void *obj) {
    if (obj != nullptr && god) {
        if (old_getTag(obj) == CreateIl2cppString("DeadCollider")) {
            return CreateIl2cppString("Default");
        }
    }
    return old_getTag(obj);
}

monoString* (*old_getName)(void *obj);
monoString* getName(void *obj) {
    if (obj != nullptr && god) {
        if (old_getName(obj) == CreateIl2cppString("DeadCollider")) {
            return CreateIl2cppString("Default");
        }
    }
    return old_getName(obj);
}
#endif

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

        if(blindness){
            *(bool*)((uint64_t) obj + 0x254) = true;//blindness
            *(float*)((uint64_t) obj + 0x25C) = 99999;//blindness
        }

        if(crithit){
            *(float*)((uint64_t) obj + 0x438) = 9999;//criticalHitCoef
            *(float*)((uint64_t) obj + 0x434) = 9999;//criticalHitChance
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

        if(kniferange){
            *(bool*)((uint64_t) obj +0x346) = true;//isFrostSword
            *(float*)((uint64_t) obj + 0x348) = 5;//frostDamageMultiplier
            *(float*)((uint64_t) obj + 0x34C) = 999999;//frostRadius
        }

        if(ammo){
            *(int*)((uint64_t) obj + 0x19C) = 99;//countShots
        }

        if(expbull){
            *(bool*)((uint64_t) obj + 0x1A5) = true;//bulletExplode
        }

        if(railbull){
            *(bool*)((uint64_t) obj + 0x1BC) = true;//railgun
            *(bool*)((uint64_t) obj + 0x1C8) = false;//railgunStopAtWall
        }

        if(jumpdisable){
            *(bool*)((uint64_t) obj + 0x20E) = true;//jumpDisabler
            *(float*)((uint64_t) obj + 0x210) = 999999;//jumpDisableTime
        }

        if(speedup){
            *(bool*)((uint64_t) obj + 0x220) = true;//isSlowdown
            *(float*)((uint64_t) obj + 0x224) = MAXFLOAT;//slowdownCoeff
            *(float*)((uint64_t) obj + 0x228) = 999999;//slowdownTime
        }

        if(slowdown){
            *(bool*)((uint64_t) obj + 0x220) = true;//isSlowdown
            *(float*)((uint64_t) obj + 0x224) = -999999999999999;//slowdownCoeff
            *(float*)((uint64_t) obj + 0x228) = 999999;//slowdownTime
        }

        if(headmagnify){
            *(bool*)((uint64_t) obj + 0x24C) = true;//isHeadMagnifierisHeadMagnifier
            *(float*)((uint64_t) obj + 0x250) = 999999;//headMagnifierTime
        }

        if(reflection != NULL){
            *(int*)((uint64_t) obj + 0x1B8) = reflection;//countReflectionRay
        }

        if(speed){
            *(float*)((uint64_t) obj + 0x60C) = 999999;//speedModifier
        }

      /*  if(spleef){
            *(bool*)((uint64_t) obj + 0x424) = true;//isSpleef
        }*/

        if(recoilandspread){
            *(float*)((uint64_t) obj + 0x104) = 0;//maxKoofZoom
            *(float*)((uint64_t) obj + 0x108) = 0;//upKoofFireZoom
            *(float*)((uint64_t) obj + 0x10C) = 0;//downKoofFirstZoom
            *(float*)((uint64_t) obj + 0x10C) = 0;//downKoofZoom
            *(float*)((uint64_t) obj + 0x8C) = 0;//maxKoof
            *(float*)((uint64_t) obj + 0x90) = 0;//tekKoof
            *(float*)((uint64_t) obj + 0x94) = 0;//upKoofFire
            *(float*)((uint64_t) obj + 0x98) = 0;//downKoofFirst
            *(float*)((uint64_t) obj + 0x9C) = 0;//downKoof
            *(float*)((uint64_t) obj + 0xA4) = 0;//timerForTekKoofVisual
            *(float*)((uint64_t) obj + 0xA8) = 0;//timerForTekKoofVisualByFireRate
            *(float*)((uint64_t) obj + 0xA8) = 0;//timerForTekKoofVisualByFireRate
            *(float*)((uint64_t) obj + 0xAC) = 0;//timeForTekKoofVisual
            *(Vector2*)((uint64_t) obj + 0x84) = Vector2(0, 0);//startZone
        }

        if(quickscope){
            *(float*)((uint64_t) obj + 0xF8) = 9999;//scopeSpeed
        }

        if(rimpulse != NULL){
            *(float*)((uint64_t) obj + 0x150) = rimpulse;//bazookaImpulseRadius
            *(float*)((uint64_t) obj + 0x148) = rimpulse;//bazookaExplosionRadius
        }

        if(rimpulseme != NULL){
            *(float*)((uint64_t) obj + 0x14C) = rimpulse;//bazookaExplosionRadiusSelf
        }
        /*void* ItemRecord = *(void**)((uint64_t) obj + 0x648);
        if(ItemRecord != nullptr){
            if(spleef){
                *(bool*)((uint64_t) obj + 0x6D) = true;//isSpleef
            }
        }*/
    }
    oldWeaponSounds(obj);
}

void (*old_PixelTime)(void *obj);
void PixelTime(void *obj) {
    if (obj != nullptr) {
        if (isLoadScenePressed) {
            LoadLevel(CreateIl2cppString(sceneList[selectedScene]));
            isLoadScenePressed = false;
        }
        if (isDiscordPressed) {
            OpenURL(CreateIl2cppString(OBFUSCATE("https://discord.gg/g3pjD5M3BZ")));
            isDiscordPressed = false;
        }
        if (changeID) {
            SetString(CreateIl2cppString(OBFUSCATE("AccountCreated")), CreateIl2cppString(OBFUSCATE("")));
        }
        if (isOpenKeyboard) {
            OpenKeyboard(CreateIl2cppString(""), (int *) (0), (bool *) (false));
        }
        if (isAddCurPressed) {
            setSomething(webInstance(), CreateIl2cppString(curList[selectedCur]), (int *)(9999), (int *)(12));
            isAddCurPressed = false;
        }
        if (webLevel) {
            //  setLevel(webInstance(), (int*)(65));
            webLevel = false;
        }

        if(destroy){
            auto photonplayers = PhotonNetwork_playerListothers();
            SetMasterClient(get_LocalPlayer());
            for (int i = 0; i < photonplayers->getLength(); ++i) {
                auto photonplayer = photonplayers->getPointer()[i];
                DestroyPlayerObjects(photonplayer);
            }
            destroy = false;
        }
    }
    old_PixelTime(obj);
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
#ifdef BIGSEX
    HOOK("0x435FA0C", getTag, old_getTag);
    HOOK("0x434733C", getName, old_getName);
#endif
}

void Patches() {
    PATCH_SWITCH("0x476323C", "1F2003D5C0035FD6", god);//dear future self, if this game ever updates kys (look for player_move_c and try to find the enum with himself, headshot etc and pray you find the right thing, has alot of stuff in the args )
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
    PATCH_SWITCH("0x4755120", "C0035FD6", removedrone);//dear future self, if this game ever updates kys (look for player_move_c and try to find the enum with himself, headshot etc and pray you find the right thing, has alot of stuff in the args )
    PATCH_SWITCH("0x47551D8", "C0035FD6", removedrone);
    PATCH_SWITCH("0x3ED22F4", "00FA80D2C0035FD6", collectibles);
    PATCH_SWITCH("0x3ED22F4", "603E8012C0035FD6", negativeCollectibles);
    PATCH_SWITCH("0x3ED22F4", "000080D2C0035FD6", nullcollectibles);
    PATCH_SWITCH("0x1714718", "200080D2C0035FD6", crithit);
    PATCH_SWITCH("0x1595AE0", "200080D2C0035FD6", blackMarket);
    PATCH_SWITCH("0x1DD567C", "200080D2C0035FD6", couponClicker);
    PATCH_SWITCH("0x1DD609C", "200080D2C0035FD6", setsClicker);
    PATCH_SWITCH("0x438120C", "200080D2C0035FD6", enableEditor);
    PATCH_SWITCH("0x2ADECFC", "200080D2C0035FD6", enableEditor);
    PATCH_SWITCH("0x42679A0", "20080D02C0035FD6", wepSkins);
    PATCH_SWITCH("0x14193E4", "200180922C0035FD6", ammo);
    PATCH_SWITCH("0x14193D8", "200180922C0035FD6", ammo);
    PATCH_SWITCH("0x48F1E00", "200180922C0035FD6", speed);//ItemRecord.SpeedModifier
    PATCH("0x206D13C", "C0035FD6");
    PATCH("0x3C962E4", "C0035FD6");
}

void DrawMenu(){
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGui::Begin(OBFUSCATE("ZygiskPG Premium 1.0a (23.0.1) - chr1s#4191 && networkCommand()#7611 && ohmyfajett#3500"));
        if (ImGui::Button(OBFUSCATE("Join Discord"))) {
            isDiscordPressed = true;
        }
        ImGui::TextUnformatted("Its Recommended to join the discord server for mod updates etc.");
        if (ImGui::CollapsingHeader(OBFUSCATE("Account Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Max Level"), &maxLevel);
            ImGui::TextUnformatted((OBFUSCATE("Gives the player Max Level after you complete a match. (Use this after you get Level 3)")));
            ImGui::Checkbox(OBFUSCATE("Free Craftables"), &cWear);
            ImGui::TextUnformatted(OBFUSCATE("Unlocks Craftables (Only works on Wear and Gadgets)"));
            ImGui::ListBox(OBFUSCATE("Currency Type"), &selectedCur, curList, IM_ARRAYSIZE(curList), 4);
            if (ImGui::Button(OBFUSCATE("Add Currency"))) {
                isAddCurPressed = true;
            }
            ImGui::Checkbox(OBFUSCATE("All Weapon Skins"), &wepSkins);
            ImGui::TextUnformatted(OBFUSCATE("Makes all weapon skins purchasable"));
            ImGui::Checkbox(OBFUSCATE("Free Lottery"), &modKeys);
            ImGui::TextUnformatted(OBFUSCATE("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)"));
            ImGui::Checkbox(OBFUSCATE("Infinite Gems"), &couponClicker);
            ImGui::TextUnformatted(OBFUSCATE("Go into gallery and spam click on the weapons to get gems."));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Player Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Godmode"), &god);
            ImGui::TextUnformatted(OBFUSCATE("Makes you invincible (others can kill you but you won't die and just become invisible)"));
            ImGui::Checkbox(OBFUSCATE("Force Double Jump"), &doublejump);
            //ImGui::Checkbox(OBFUSCATE("Player Speed"), &speed);
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Weapon Mods"))) {
            ImGui::SliderFloat(OBFUSCATE("Shotgun Damage Buff"),&damage, 0.0f, 15.0f);
            ImGui::TextUnformatted(OBFUSCATE("Amplifys the shotgun  damage. (Anything above 8 might kick after a few kills)"));
            ImGui::Checkbox(OBFUSCATE("Force Critical Hits"), &crithit);
            ImGui::TextUnformatted(OBFUSCATE("Forces Critical Shots each time you hit someone."));
            ImGui::Checkbox(OBFUSCATE("Unlimited Ammo"), &ammo);
            ImGui::Checkbox(OBFUSCATE("Force Explosive Bullets"), &expbull);
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot explosive bullets."));
            ImGui::Checkbox(OBFUSCATE("Force Railgun Bullets"), &railbull);
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot railgun bullets."));
            ImGui::SliderInt(OBFUSCATE("Reflection Rays"),&reflection, 0, 200);
            ImGui::TextUnformatted(OBFUSCATE("Amplifys the reflection ray ricochet."));
            ImGui::Checkbox(OBFUSCATE("Infinite Knife Range"),&kniferange);
            ImGui::TextUnformatted(OBFUSCATE("Allows you to kill all with a knife."));
            ImGui::Checkbox(OBFUSCATE("No Recoil and Spread"),&recoilandspread);
            ImGui::SliderFloat(OBFUSCATE("Rocket Impulse for yourself"),&rimpulseme, 0.0f, 5000.0f);
            ImGui::TextUnformatted(OBFUSCATE("Modifys the rocket jump power."));
            ImGui::SliderFloat(OBFUSCATE("Rocket Impulse for others"),&rimpulse, 0.0f, 5000.0f);
            ImGui::TextUnformatted(OBFUSCATE("Modifys the rocket impulse power. (It will throw players around)"));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Effect Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Force Charm"), &charm);
            ImGui::TextUnformatted(OBFUSCATE("Adds the charm effect (Used to reduce half of the enemy's weapon efficiency)"));
            ImGui::Checkbox(OBFUSCATE("Force Kill Damage Boost"), &killboost);
            ImGui::TextUnformatted(OBFUSCATE("Gives you damage boost after every kill."));
            ImGui::Checkbox(OBFUSCATE("No Fire and Toxic Effects"), &fte);
            ImGui::TextUnformatted(OBFUSCATE("Removes the burning and being intoxicated effect on you."));
            ImGui::Checkbox(OBFUSCATE("Force Electric Shock"), &electric);
            ImGui::TextUnformatted(OBFUSCATE("Adds the electric shock effect"));
            ImGui::Checkbox(OBFUSCATE("Force Blindness Effect"), &blindness);
            ImGui::TextUnformatted(OBFUSCATE("Adds the electric shock effect"));
            ImGui::Checkbox(OBFUSCATE("Force Poisoning Effect"), &poison);
            ImGui::TextUnformatted(OBFUSCATE("Adds the poisoning effect (Will poison any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Speed-Up Effect"), &speedup);
            ImGui::TextUnformatted(OBFUSCATE("Adds a speed-up effect (Will speed up any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Slow-down Effect"), &slowdown);
            ImGui::TextUnformatted(OBFUSCATE("Adds a slow-down effect (Will freeze any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Jump Disabler Effect"), &jumpdisable);
            ImGui::TextUnformatted(OBFUSCATE("Adds the jump disabler effect (Will disable jump for any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Head Magnifier Effect"), &headmagnify);
            ImGui::TextUnformatted(OBFUSCATE("Adds the head magnifier effect (Will magnify the player's head for any player you shoot until they die)"));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Visual Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Show marker"), &enemymarker);
            ImGui::TextUnformatted(OBFUSCATE("Shows the enemy after you shoot them once."));
            ImGui::Checkbox(OBFUSCATE("Quick-Scope"), &quickscope);
            ImGui::TextUnformatted(OBFUSCATE("Opens the scope instantly."));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Game Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Kill All"),&kniferange);
            ImGui::TextUnformatted(OBFUSCATE("Just pull out your knife."));
            ImGui::Checkbox(OBFUSCATE("Turret Godmode"), &tgod);
            ImGui::TextUnformatted(OBFUSCATE("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does."));
            ImGui::Checkbox(OBFUSCATE("Drone Godmode"), &removedrone);
            ImGui::TextUnformatted(OBFUSCATE("The drone gadget will never despawn. (Don't get more than 1 drone or you'll be kicked)"));
            ImGui::Checkbox(OBFUSCATE("Force Coin Drop"), &coindrop);
            ImGui::TextUnformatted(OBFUSCATE("Always drops coins when someone dies."));
            if (ImGui::Button(OBFUSCATE("Crash Others"))) {
                destroy = true;
            }
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Misc Mods")))
        {
            ImGui::Checkbox(OBFUSCATE("Spoof Editor"), &enableEditor);
            ImGui::TextUnformatted(OBFUSCATE("Makes the game think its on the Unity Editor"));
            ImGui::ListBox(OBFUSCATE("Select Scene"), &selectedScene, sceneList, IM_ARRAYSIZE(sceneList), 4);
            if (ImGui::Button(OBFUSCATE("Load Scene"))) {
                isLoadScenePressed = true;
            }
            /*if (ImGui::Button(OBFUSCATE("Become god"))) {
                SetMasterClient(get_LocalPlayer());
            }
            if (ImGui::Button(OBFUSCATE("Playstantiate the playfab"))) {
                Instantiate(CreateIl2cppString(OBFUSCATE("ConnectScene/SelectMap")), Vector3::Zero(), Quaternion::Identity());
            }*/
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Bannable Mods")))
        {
            ImGui::Checkbox(OBFUSCATE("Collectibles"), &collectibles);
            ImGui::TextUnformatted(OBFUSCATE("Sets the value of items to 2000"));
            ImGui::Checkbox(OBFUSCATE("Null Collectibles"), &nullcollectibles);
            ImGui::TextUnformatted(OBFUSCATE("Sets the value of items to 0"));
            ImGui::Checkbox(OBFUSCATE("Negative Collectibles"), &negativeCollectibles);
            ImGui::TextUnformatted(OBFUSCATE("Sets the value of items to -500"));
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
