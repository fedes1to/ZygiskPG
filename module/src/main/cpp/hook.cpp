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
#include <iostream>
#include <chrono>

#define GamePackageName "com.pixel.gun3d"

//dear future self, if this game ever updat

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
const char* wepList[] = { "", "FirstPistol", "FirstShotgun", "UziWeapon", "Revolver", "Machingun", "MinersWeapon", "AK47", "Knife", "m16", "Eagle 1", "SPAS", "FAMAS", "Glock", "Shovel", "Hammer", "Beretta", "IronSword", "SteelAxe", "WoodenBow", "Chainsaw 2", "SteelCrossbow", "Hammer 2", "Mace 2", "Staff 2", "DoubleShotgun", "AlienGun", "m16_2", "Tree", "Fire_Axe", "SVD", "Barrett_2", "Uzi2", "Hunter_Rifle", "Bazooka", "railgun", "tesla", "grenade_launcher", "grenade_launcher2", "Bazooka_3", "GravityGun", "AUG", "katana_3", "bigbuddy", "Mauser", "Shmaiser", "Tompson", "Tompson_2", "basscannon", "SparklyBlaster", "CherryGun", "AK74", "FreezeGun", "3pl_Shotgun_3", "flowerpower_3", "Revolver4", "Scythe_3", "plazma_3", "plazma_pistol_2", "plazma_pistol_3", "Razer_3", "Flamethrower_3", "FreezeGun_0", "Minigun_3", "Barret_3", "LightSword_3", "Sword_2_3", "Staff 3", "DragonGun", "Bow_3", "Bazooka_2_3", "m79_3", "Red_Stone_3", "XM8_1", "PumpkinGun_1", "TwoBolters", "RayMinigun", "SignalPistol", "AutoShotgun", "TwoRevolvers", "SnowballGun", "SnowballMachingun", "HeavyShotgun", "Solar_Ray", "Water_Pistol", "Solar_Power_Cannon", "Water_Rifle", "Needle_Throw", "Valentine_Shotgun", "Carrot_Sword", "Easter_Bazooka", "RailRevolverBuy", "Assault_Machine_GunBuy", "Impulse_Sniper_RifleBuy", "Autoaim_RocketlauncherBuy", "PX-3000", "Sunrise", "Bastion", "DualHawks", "StormHammer", "Badcode_gun", "ElectroBlastRifle", "PlasmaShotgun", "Devostator", "Dark_Matter_Generator", "Hydra", "TacticalBow", "FutureRifle", "RapidFireRifle", "DualUzi", "LaserDiscThower", "Tesla_Cannon", "Photon_Pistol", "Fire_orb", "Hand_dragon", "Alligator", "Hippo", "Alien_Cannon", "Alien_Laser_Pistol", "Dater_Flowers", "Dater_DJ", "Dater_Arms", "Dater_Bow", "FriendsUzi", "Alien_rifle", "VACUUMIZER", "Fireworks_Launcher", "Shotgun_Pistol", "Range_Rifle", "Pit_Bull", "Tiger_gun", "Balloon_Cannon", "Mech_heavy_rifle", "Shuriken_Thrower", "BASIC_FLAMETHROWER", "snowball", "MysticOreEmitter", "Laser_Crossbow", "Nutcracker", "SPACE_RIFLE", "Icicle_Generator", "PORTABLE_DEATH_MOON", "Candy_Baton", "Hockey_stick", "Space_blaster", "mp5_gold_gift", "Dynamite_Gun_1", "Dual_shotguns_1", "Antihero_Rifle_1", "HarpoonGun_1", "Red_twins_pistols_1", "Toxic_sniper_rifle_1", "NuclearRevolver_1", "NAIL_MINIGUN_1", "DUAL_MACHETE_1", "Fighter_1", "Gas_spreader", "LaserBouncer_1", "magicbook_fireball", "magicbook_frostbeam", "magicbook_thunder", "TurboPistols_1", "Laser_Bow_1", "loud_piggy", "Trapper_1", "chainsaw_sword_1", "dark_star", "toy_bomber", "zombie_head", "mr_squido", "RocketCrossbow", "spark_shark", "power_claw", "zombie_slayer", "AcidCannon", "frank_sheepone", "Ghost_Lantern", "Semiauto_sniper", "Chain_electro_cannon", "Barier_Generator", "Demoman", "charge_rifle", "minigun_pistol", "bad_doctor_1", "dual_laser_blasters", "toxic_bane", "Charge_Cannon", "Heavy_Shocker", "ruler_sword_1", "pencil_thrower_1", "napalm_cannon", "sword_of_shadows", "dracula", "xmas_destroyer", "santa_sword", "snow_storm", "heavy_gifter", "bell_revolver", "elfs_revenge", "photon_sniper_rifle", "subzero", "mercenary", "laser_spear", "nunchuks", "double_dragon", "magicbook_love", "casanova", "romeo_and_juliette", "Q_grenade_launcher", "Q_machine_gun", "Q_electrogun", "Q_railgun", "Q_rocket_launcher", "Q_shotgun", "Q_plasma_gun", "Q_chopper", "Q_BFG", "Blade_of_Justice", "lab_eight_barrel_shotgun", "Tactical_tomahawk", "Masterpiece_musket", "Executioner", "Serious_argument", "lab_piranha", "lab_oneshot", "lab_machinegun", "lab_combatyoyo", "lab_orbital_pistol", "Defender_of_the_law", "fidget_thrower", "festive_bazooka", "thunderer", "bomber_slingshot", "future_sniper_rifle", "beast", "invader", "core_sword", "hedgehog", "third_eye", "eraser", "excalibur", "poseidon_trident", "reaper", "void_ray_rifle", "black_mamba", "chip_sword", "vampire_hunter", "mountain_wolf", "hurricane", "dino_slayer", "soulstone", "electrosphere", "prototype_s", "frozen_dragon", "special_agent", "Power_Fists", "VerticalShotgun", "astral_bow", "zeus_chain_ sword", "music_lover", "veteran", "mini_alien_spaceship", "flag_keeper", "leaders_sword", "last_christmas", "laser_assistant", "dual_cryo_pistols", "automatic_decorator", "elder_force_saber", "Champion_Mercenary", "champion_peacemaker", "adamant_laser_cannon", "Adamant_Revolver", "adamant_spear", "champion_electric_arc", "adamant_sniper_rifle", "Anti-Champion_Rifle", "Champion_Solar_Cannon", "Adamant_Bomber", "sharp_snowflake", "little_cthulhu", "magicbook_curse_spell", "airblade_spell", "magicbook_poison_spell", "magicbook_earth_spikes_spell", "magicbook_shadow_spell", "Dragon_King", "angry_pumpkin", "Steam_Knuckes", "electro_blast_pistol", "wyvern_sniper", "hero", "sly_avenger", "bright_reflector", "sakura", "deadly_beat", "killer_whale", "renegade", "demon_sword", "bolters2", "Stormtrooper_Exoskeleton", "Sniper_Exoskeleton", "demolition_exosceleton", "hitman_exoskeleton", "berserk_exoskeleton", "engineer_exoskeleton", "death_claws", "Eva", "space_desinsector", "mega_gun", "laser_revolver", "Sword_and_Shield", "last_samurai", "manga_gun", "anime_schtye", "nitrogen_spreader", "future_police_rifle", "stakegun", "alive_blaster", "plasma_cutter", "good_doctor", "devastator", "supercharged_rifle", "archangel", "manticore", "soul_thief", "multitaskers", "yin_yang", "last_hope", "Jet_hammer", "energy_drill", "Spender", "graviton", "acid_pistol", "Rhino", "antique_pistol", "lava_thrower", "Inventors_Rifle", "vandals", "revolver_sniper_rifle", "captain", "void_energy_pistol", "destruction_system", "circus_cannon", "military_grenade_launcher", "cyborg_hands", "ritual_blade", "tactical_rifle", "sparkling_eel", "alien_sniper_rifle", "neutralizer", "bone_sniper_rifle", "prehistoric_shotgun", "shamans_bow", "poison_darts", "primal_beast", "eye_of_ra", "gym_rifle", "bone_crusher", "soda_launcher", "rocket_launcher", "royale_fighter", "royale_revolver", "royale_sniper_rifle", "royale_ashbringer", "", "", "", "", "", "shover_laser", "", "penilizer", "fan_revolver", "football_cannon", "", "cursed_pirate", "Viking", "gladiator", "heart_of_volcano", "", "", "", "", "", "toy_hammer", "igniter", "infiltrator", "warmaster", "combat_slinger", "paintball_rifle", "energy_rings_exoskeleton", "particle_accelerator", "RitualRevolvers", "magicbook_Spell_of_Bats", "deadmans_rifle", "Witchunter", "industrial_nailer", "trick_and_treat", "ghost_launcher", "poisonous_vine", "johnny_p", "witch_broom", "cyber_slicer", "ricochet_rifle", "cyber_revolver", "neon_lighting", "quadruple_death", "", "", "rocket_hand", "cyber_laser", "scull_crossbow", "", "sniper_cyber_module", "cops_club", "ultimatum", "lamp_revolver", "drum_rifle", "bp_flamethrower", "hand_gatling", "steamer", "gold_digger", "bow_of_spirit", "", "", "", "corn_launcher", "pie_thrower", "triple_musket", "demolition_gun", "ecco_hunter", "like_throver", "photo_gun", "liquid_fire", "electric_vortex", "cursed_staff", "blinder", "peppermint_guardians", "freon_gun", "", "frostomat", "frosty_railgun", "winter_staff", "ice_spiker", "", "total_annihilator", "torpedo_launcher", "sawed_off_shotguns", "gauss_machine_gun", "ultra_beam", "sock_bomber", "energy_assault_rifle", "", "", "futuristic_minigun", "sniper_ice", "gloves_of_power", "space_redeemer", "snow_blaster", "alien_bounser", "alien_shuriken", "spruce_pistols", "christmas_spirit", "ice_lightning", "ice_generator", "icecream_pistol", "whip_with_cake", "festive_guitar", "evil_snowman", "gangerbread_homezooka", "portable_gift_factory", "saturn", "venus_n_mercury", "Asteroid", "comet_sniper_rifle", "meteor_shower", "neclaces_ice_king", "Ice_Paws", "frozen_lich_bow", "gift_stitcher", "ice_king_anger", "gifted_revolvers", "yetti_spirit", "bambo", "new_year_defender", "candy_axe", "cold_silence", "proton_freezer", "coldstarter", "", "", "", "", "", "", "spec_ops_backup", "pixel_cola_refresher", "warden", "instigator", "storm", "semi_auto_shotgun", "stinger_3000", "airstrike_laptop", "deadly_digital_system", "night_hunter", "spec_ops_pistol", "tactical_knife", "hand_minigun", "heavy_sniper_rifle", "armadillo", "police_zapper", "temple_guardian", "healing_carp", "moon_halberd", "", "", "", "", "", "", "agent_valentine", "lovebirds", "Serenade", "heartbreakers", "Succubus", "postman", "harpy", "runic_hammer", "blade_rifle", "arahna_bite", "medusa", "bigb_oven", "mr_mixer", "cookie_minigun", "magic_blades", "alchemic_staff", "", "", "system_admin", "flash_drive", "disconnector", "network_streamer", "аntivirus", "big_data", "cyclops_siling", "rainbow_dispenser", "rainbow_destroyer", "mister_l", "adamant_claws", "hypersound_uzi", "double_gum", "black_hole", "", "", "protector_of_peace", "portalius_wrath", "chipping_whip", "portalium_rifle", "nuclear_rifle", "deadly_umbrella", "big_bang_revolver", "acid_shotgun", "batohawk", "combat_gautlent", "spider_sense", "werewolf", "sharp_flame", "charged_injector", "garbage_rifle", "firefighter", "huge_boy", "", "fatal_melter", "dual_anger", "brutal_thinner", "liquidator_z1", "eliminator_z2", "pursuer_z3", "", "", "", "block_rifle", "block_shotgun", "block_flamethrower", "plastic_rifle", "plastic_bazooka", "plastic_carabin", "cloud_launcher", "sucker_gun", "witch_doctor", "portal_cannon", "healing_staff", "laser_ray_rifle", "clockwork_cannon", "transformed_blaster", "transformed_machinegun", "witch_cauldron", "combat_darts", "throwing_spears", "alloha_boom", "professional_bow", "artifact", "ouroboros", "keen_courier", "uncertainty_emitter", "camp_leader_rifle", "knife_and_compass", "super_loudspeaker", "raccoon_with_trumpet", "", "", "werewolf_paws", "ice_cream_thrower", "water_juice_cannon", "pop_soda", "pyro_1116", "sparkling_hydra", "tiny_resizer", "pawn_cannon", "horse_axe", "battle_rook", "inevitability", "", "", "checkmate", "proud_eagle", "sly_wolf", "brave_lion", "offensive_catapult", "battle_horn", "sharp_ring", "ornate_bagpipes", "pew_pew_rifle", "pixel_stream_sword", "tube_backpack", "supershot_rifle", "hyper_mega_cannon", "digital_sunrise", "lasercycler", "wave_pulser", "arcade_rifle", "arcade_flamethrower", "fatality_hook", "fireball_gauntlets", "anubis", "duck_hunter", "disc_battle_station", "", "holy_shotgun", "aspen_spear", "dual_hunter_crossbows", "plague_purifier", "lives_oppressor", "sword_of_silence", "", "hot_greetings", "cooking_totem", "spiritual_rifle", "storm_all_seas", "", "hard_corn", "", "lancelot", "galahad", "percival_n_lamorak", "mordred", "morgana", "bediver", "neutron_pulsator", "clapper_gun", "royal_marryer", "snowqueen_wand", "heroic_epee", "ice_club", "last_squeak", "fierce_pack", "mouse_scepter", "ice_chaser", "sneaky_revolver", "coal_frightener", "", "golden_bros", "gangsta_shield", "double_cashback", "luxury_beats", "sniper_dude", "gems_power", "spirit_staff", "offensive_fireworks", "sharp_fans", "river_dragon", "explosive_lemonade", "", "fire_spirit", "thunder_storm", "water_blades", "deadly_stings", "dragon_bite", "ancient_scroll", "potato_implant", "ai_pistol", "super_drill", "meteorite_fighter", "gravitator", "", "zap_blaster", "pew_blaster", "boom_blaster", "psionic_spores", "poisonous_grasshopper", "shrimposaur", "", "egg_colorizer", "hocus_pocus", "bunny_smasher", "a_killer", "carrot_crossbow", "", "", "watchy_snipy", "spiny_shooty", "pully_blasty", "chickoboom", "battle_grainthrowers", "last_cry", "", "dragon_emitter", "fate_fist", "demon_catcher", "aboard_puncher", "deadly_tale", "", "", "sword_soul_card", "lance_soul_card", "bow_soul_card", "vertical_grip_device", "siege_mode_glove", "one_punch_glove", "treasurer", "", "shell_bubbler", "tide_bracers", "call_of_sirens", "deep_ambusher", "cthulhu_legacy", "", "", "mooring_anchor", "heavy_drill_rifle", "rivet_gun", "submarine_drone_station", "echo_scanner", "underwater_carbine", "project_dolphin", "project_rapid_flock", "project_marlin", "advanced_scout_rifle", "harsh_punisher", "manual_inferno", "offensive_rocket_system", "headhunter_pistols", "thunderbolt", "spear_of_war", "aphrodite_crossbow", "winged_revolver", "ferocious_poleaxe", "", "", "cerberus", "souls_conductor", "triple_bite", "wraith_of_fire", "earth_trembler", "primal_cold", "boar_roar", "gorgon_head", "invincible_lion", "advanced_scout_rifle_l", "harsh_punisher_l", "manual_inferno_l", "offensive_rocket_system_l", "headhunter_pistols_l", "debugger", "likes_booster", "ban_hammer", "vr_blasters", "killer_chord", "invigorating_awaker", "total_planner", "money_rain", "render_station", "gun_force", "digital_pad", "", "cheater_hunter", "super_fixer", "bugs_cleaner", "", "good_code", "ambush_crocorifle", "bubble_splasher", "disguised_rifle", "", "", "enchanting_altar", "spirit_mask", "magical_boomerang", "foam_sword", "sand_tower_pistol", "watermans_rifle", "high_pressure_rifle", "sharkado", "shower_sprinkler", "star_shard", "solar_flare", "magnetic_storm", "MonsterHunterMechBody1", "ectoplasmic_grenade_launcher", "reversed_pistol", "activity_neutralizer", "", "", "goo_substance_rifle", "goo_substance_pistol", "goo_substance_scythe", "pga_tv", "pga_doll", "pga_gates", "pga_living_rifle", "pga_slender_touch", "pga_screaming_saw", "atomic_splitter", "detimerian_divider", "restructurer", "anomaly_concentrator", "MonsterHunterMechBody2", "MonsterHunterMechBody3", "bloq_plasma_module", "plasma_beast", "drone_station", "", "", "max_cruelty", "represser", "fear_and_espair", "rude_bully", "laser_ninja", "rebel", "mobile_outpost", "outlaw_catcher", "fair_sheriff", "minigun_cyber_module", "med_surgery_module", "supply_airdrop", "battle_falcon", "super_mega_tool", "pneumatic_needle_rifle", "summon_dagger", "", "", "frozen_heart", "eye_of_the_storm", "frosty_fangs", "combat_candy", "ho_ho_rocket", "expllosive_caramel", "candy_revolver", "elven_killer_rifle", "assassin_daggers", "farewell_volley", "mystic_potion", "siege_crossbow", "storm_poleaxes", "prophet_staff", "hammer_of_thor", "", "", "geri_n_freki", "sleipnir", "huginn", "deceitful_cheater", "cunning_flame", "harbinger_of_ragnarok", "sun_eater", "call_of_valhalla", "winged_bow", "war_horse", "glitch_machine_gun", "ideal_rocket_launcher", "noob_rifle", "evil_rubiks_cube", "super_pixel_gun", "", "final_sentence", "piercer", "border_eraser", "deminic_crusher", "dual_beam", "ronin", "brutal_rainbow", "tomianom", "time_scythe", "cyber_cat_lantern", "laser_cat_rifle", "bomber_cat", "", "evidence_hunter", "patrol_officer_747", "redneck_bazooka", "mayor_trophy_revolver", "pixel_cola_cans", "", "", "large_machinegun", "tactical_pistol", "mammoth", "debts_collector", "godfathers_assistant", "black_label", "plan_b", "traitor", "incinerator", "saboteur", "rock_smelter", "tunnel_riveter", "complete_grinder", "drilling_railgun", "plasmatic_digger", "", "", "", "portal_rifle", "extracting_ray_pistol", "terramorphing_stone", "aerosaur_rifle", "bone_saur_sword", "heavy_spik_saur", "ritual_rifle", "tooth_of_fate", "staff_crystals_will", "spark_horn", "adventure_swords", "flat_rifle", "secret_burger", "charles", "ink_spreader", "eraser_crossbow", "pencil_spears", "bro_steamer", "sad_magnet", "anvil_pistol", "masterly_trumpet", "blot_axe", "blot_shotgun", "blot_cannon", "", "", "", "veteran_rifle", "veteran_sniper_rifle", "veteran_rocket_system", "lives_harvester", "jet_dragon", "thermal_scythe", "simple_dimple", "fx_camera", "dance_rifle", "rifle_me_online", "virtual_orchestra", "sharp_chaser", "zap_katana", "cyber_lantern", "bro_rifle", "ninja_swordpad", "hype_yo_yo", "streaming_beast", "dislike", "likes_absorber", "bad_comment", "", "", "mysterious_obelisk", "ritual_embalmer", "sacred_cat_will", "sentinel", "howl_of_the_mummy", "punishing_light", "piercing_gaze_of_ra", "eternal_suns_wrath", "fair_judge", "black_obelisk", "plaguebringer", "scarab_shrine", "disturbed_sarcophagus", "royal_cobra_spirit", "pharaon_3_breath", "", "", "", "", "ultrasonic_rifle", "electronics_burner", "hounds_rx03", "system_scorpion", "rumble", "", "", "experimental_rifle", "jet_spear", "tactical_scorcher", "siege_minigun", "thermal_mecha_knife", "disruptive_howitzer", "anti_aircraft_module", "engineering_saw_thrower", "corpuscular_revolver", "uranium_grenade_launcher", "golden_eagles", "wayfarer", "fate", "holographic_cards", "", "mutation_cannon", "", "", "c4_basket", "head_of_statue", "tank_turret", "space_parasite_dreadnought", "space_parasite_fighter", "alien_parasite_mothership", "bad_clown", "mutant_weapon_one", "mutant_weapon_two", "splutter_weapon_one", "splutter_weapon_two", "jumper_weapon_one", "jumper_weapon_two", "hound_weapon_one", "thorn_weapon_one", "thorn_weapon_two", "supermutant_weapon_one", "disturbing_balloons", "clown_chainsaw", "irradiated_smg", "mutant_fish", "spiky_limbs", "predatory_rifle", "automatic_injector", "antidote_missile_launcher", "purifying_shotgun", "mutant_slayer", "mutagen_neutralizer", "antidote_dropper", "power_gauntlet", "class_bullet_rifle", "class_knight_hammer", "class_knight_shield", "class_valk_rocket", "class_lifeline_gun", "class_sara_sniper", "class_sara_shotgun", "class_saber_disk", "class_saber_katana", "class_mex_flame", "class_viktor_grenade", "full_moon_rifle", "fireflies_blessing", "branch_of_wisdom", "eternal_light", "mirrorer", "otherworld_wanderer", "dark_deer_bow", "reflected_santa_sword", "combat_fireplace", "combat_candy_machine", "gift_stealer", "tricky_glance", "end_of_fairytale", "fairy_axe", "butterfly_eater", "", "", "class_lifeline_healgun", "handy_darts", "spirits_whisper", "makeshift_piercer", "battle_kusarigama", "temple_claws", "", "", "vortex_core", "storm_bringer", "clouds_disperser", "field_plasma_station", "still_working_windmill", "thunder_formula", "bubblegum_01", "tnt_thrower", "ak5_56", "disguised_weapon", "intelligence_swords", "controlled_drone_system", "deadly_beamer", "", "", "rune_buster", "ice_wyvern_horns", "runic_sentinel", "crystal_touch", "crystal_bow", "enchanted_crystal_of_ancients", "elves_energy_crystal", "tricky_electrizer", "plague_spreader", "ancestors_curse", "moon_hunter_bow", "elf_heavy_crossbow", "lightbringer", "", "", "eco_rifle", "ice_giant_rifle", "moonblades", "solar_lens", "supernova_generator", "event_horizon", "", "blacklight_pistol", "", "unfriendly_bat", "tamagochi_revolver", "mad_colorer", "nachos_shotgun", "", "", "crescent_power", "shockorizer", "sharingan_vengeance", "odm_special_gear", "grenade_quirk", "", "", "doubled_quirk", "guardian_spirit", "ghost_epee", "the_sorting_pistol", "raven_staff", "additional_biting_limb", "corrupted_speelbook", "", "", "transforming_rifle", "navigation_shotgun", "holographic_hook_saber", "monkey_pirate", "", "", "ships_drowner", "three_barrels_cannon", "wardens_trident", "deadly_shark", "nets_thrower", "tick_tock_pistol", "heavy_cone_minigun", "starfall", "dreams_warden", "snowstorm_fury", "spirits_overseer", "s_q_dominion", "", "", "x_mas_vibes_setter", "fatal_decorator", "x_mas_spirits_defender", "armor_piercing_sniper_rifle", "harsh_dragon", "leaders_butterfly", "leaders_bestfriend", "reckoning_day_sniper_rifle", "reckoning_day_pistols", "flaming_reckoning_day", "spirit_exorcist", "festive_eliminator", "wealth_spreader"};
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
const char* curList[] = { OBFUSCATE("ClanSilver"), OBFUSCATE("ClanLootBoxPoints"),  OBFUSCATE("Coupons"), OBFUSCATE("PixelPassCurrency"), OBFUSCATE("RouletteAdsCurrency"), OBFUSCATE("PixelBucks"), OBFUSCATE("GemsCurrency"), OBFUSCATE("Coins") };
bool maxLevel, cWear, uWear, gadgetUnlock, isLoadScenePressed, modKeys, tgod,
removedrone, god, ammo, collectibles, ezsuper, changeID, isOpenKeyboard,
crithit, charm, fte,enemymarker, enableEditor, killboost, electric, kspeedboost, daterweapon, grenade,
doublejump, coindrop, itemParams, blackMarket, couponClicker, setsClicker,
negativeCollectibles, nullcollectibles, isDiscordPressed, webLevel, blindness, wepSkins, kniferange, expbull,
spleef, shotbull, railbull, poison, jumpdisable, slowdown, headmagnify, destroy, recoilandspread, quickscope, speedup, speed,
isAddCurPressed, coins, gems, clsilver, coupons, clanlootboox, pixelpass, pixelbucks, craftcurrency, roullette,
isAddWeapons, isAddWeapons2, isAddWeapons3, isAddWeapons4, isAddWeapons5, shotBull, ninjaJump,spamchat,pgod, prespawn,gadgetdisabler, xray, scopef,
bypassName, isBuyEasterSticker, gadgetsEnabled, xrayApplied, kniferangesex, playstantiate, portalBull, snowstormbull, polymorph, harpoonBull, dash, spoofMe;

float damage, rimpulseme, rimpulse, pspeed, snowstormbullval;
int reflection, amountws;

void (*SetString) (monoString* key, monoString* value);
void (*LoadLevel) (monoString* key);
void (*OpenURL) (monoString* url);
void (*OpenKeyboard) (monoString* TextUnformatted, int* keyboardType, bool* autoCorrection);
void (*setSomething) (void* instance, monoString* currencyType, int* value, int* num);
void (*addWeapon) (void* instance, monoString* weaponName, int* num);
bool (*SetMasterClient) (void* masterClientPlayer);
void* (*get_LocalPlayer) ();
void (*DestroyPlayerObjects)(void *player);
monoArray<void**> *(*PhotonNetwork_playerListothers)();
void (*DestroyAll) ();
// public static GameObject Instantiate(string prefabName, Vector3 position, Quaternion rotation, byte group = 0, object[] data = null)
void* (*Instantiate) (monoString* prefabName, void* position, void* rotation, unsigned char group);
void (*BuyStickerPack) (int* type);

enum RPCList
{
    ActivateMechRPC,
    AddBonusAfterKillPlayerRPC,
    AddForceRPC,
    AddFreezerRayWithLength,
    AddPaticleBazeRPC,
    AddPlayerInCapturePoint,
    AddScoreDuckHuntRPC,
    AddWeaponAfterKillPlayerRPC,
    AdvancedEffectRPC,
    AdvancedEffectWithSenderRPC,
    ApplyDamageRPC,
    ApplyDebuffRPC,
    ChargeGunAnimation,
    ClearScoreCommandInFlagGameRPC,
    Collide,
    CountKillsCommandSynch,
    CreateChestRPC,
    DeactivateMechRPC,
    DestroyRpc,
    GetInPlayerRPC,
    ShowSelectedFortNotification,
    fireFlash,
    GetBonusRewardRPC,
    ActivateGadgetRPC,
    GetDamageRPC,
    RespawnRPC,
    Go,
    GoBazaRPC,
    GoMatchRPC,
    HoleRPC,
    ImDeadInHungerGamesRPC,
    imDeath,
    ImKilledRPC,
    IsVisible_RPC,
    KilledByRPC,
    LikeRPC,
    MakeBonusRPC,
    MeKillRPC,
    OpponentLeftGame,
    PlayDestroyEffectRpc,
    PlayerEffectRPC,
    PlayMusic,
    PlayPortalSoundRPC,
    PlayZombieAttackRPC,
    PlayZombieRunRPC,
    plusCountKillsCommand,
    CheckActivityRPC,
    HitByVehicleRPC,
    ReloadGun,
    RemoveBonusRPC,
    RemoveBonusWithRewardRPC,
    RemovePlayerInCapturePoint,
    ResumeMatchRPC,
    RevengeRequestRPC,
    SendBuffParameters,
    SendChatMessageWithIcon,
    SendSystemMessegeFromFlagAddScoreRPC,
    SendSystemMessegeFromFlagDroppedRPC,
    SendSystemMessegeFromFlagReturnedRPC,
    SetArmorVisInvisibleRPC,
    setBootsRPC,
    SetBotHealthRPC,
    setCapeRPC,
    SetCaptureRPC,
    SetEnemyArmor,
    SetEnemyBoots,
    SetEnemyCape,
    SetEnemyHat,
    SetEnemyMask,
    SetEnemyPet,
    SetEnemySkin,
    SetEnemyWeapon,
    SetGadgetEffectActiveRPC,
    SetGadgetesRPC,
    SetHatWithInvisebleRPC,
    SetBigHeadRPC,
    SetJetpackEnabledRPC,
    SetJetpackParticleEnabledRPC,
    SetMaskRPC,
    SetMyClanTexture,
    SetMySkin,
    SetNickName,
    SetNOCaptureRPC,
    SetPixelBookID,
    SetRocketActive,
    SetRocketActiveWithCharge,
    SetRocketStickedRPC,
    SetTargetRPC,
    SetVisibleFireEffectRpc,
    SetWeaponRPC,
    SetWeaponSkinRPC,
    SetWearIsInvisibleRPC,
    ShotRPC,
    ShowBonuseParticleRPC,
    ShowExplosion,
    ShowMultyKillRPC,
    SinchCapture,
    SlowdownRPC,
    StartFlashRPC,
    StartGame,
    StartMatchRPC,
    StartNewRespanObjectRpc,
    StartRocketRPC,
    StartShootLoopRPC,
    StartTurretRPC,
    SynchCaptureCounter,
    SynchCaptureCounterNewPlayer,
    SynchGameRating,
    SynchGameTimer,
    SynchNamberSpawnZoneRPC,
    SynchNumUpdateRPC,
    SynchronizeTimeRPC,
    SynchScoreCommandRPC,
    SynchScoresCommandsNewPlayerRPC,
    SynchScoresCommandsRPC,
    SynchStartTimer,
    SynhCommandRPC,
    SynhCountKillsRPC,
    SynhDeltaHealthRPC,
    SynhHealthRPC,
    SynhIsZoming,
    SynhNameRPC,
    SynhRanksRPC,
    SynhScoreRPC,
    SynhDeltaArmorRPC,
    PropertyRPC,
    winInHungerRPC,
    SetPlayerUniqID,
    SynchLivesItems,
    RegisterPlayerRPC,
    SetGamemodeRPC,
    SetMapRPC,
    StartGameRPC,
    SetReadyRPC,
    QuitFromSquadRPC,
    ShowStartGameRPC,
    StartTimerRPC,
    KickPlayerRPC,
    SynchCurrentCategory,
    ResetVersusTimerRPC,
    StartAirDropItemRPC,
    MakeBetRPC,
    GoTeamMatchRPC,
    StartTeamMatchRPC,
    SynchronizeMatchTimeRPC,
    SetRocketActiveWithNumSmoke,
    SynhDeathCountRPC,
    GoRevengeMatchRPC,
    EndRoundRPC,
    SynchronizeGoTimeRPC,
    SyncronizeTeamWins,
    FireFlashDamagebleRPC,
    SetMaxArmorAndHealthRPC,
    SendEmojiRPC,
    SynchGetGadgets,
    SyncTramPosition,
    UnregisterPlayerRPC,
    SyncLastMoveTime,
    RemoveTargetRPC,
    AttackGateRPC,
    ShotArtilleryRPC,
    SetDestinationRPC,
    SetEquipRPC,
    ShowEndEventRPC,
    ShowEndEventNotificationRPC,
    PlayingInSquadRPC,
    SynchCombatLevelRPC,
    SendPetIdRPC,
    SynchGameLeague,
    ActiveChunkChangedRPC,
    GetBonusRPC,
    SetGliderRPC,
    AddWeaponAfterBonusPreviewRPC,
    RemoveWeaponPreviewRPC,
    SetRoyaleAvatarRPC,
    SetMapPositionSquadRPC,
    PlayerInjuredRPC,
    SynhHealthInjuredRPC,
    PlayerInjuryHealedRPC,
    SetLandingTrailRPC,
    SetUserMapMarkerRPC,
    RemoveUserMapMarkerRPC,
    FlyOnGliderSynchRPC,
    BattleRoyaleSquadMemberKilledRPC,
    HealingByPlayerRPC,
    PlayerInjuryHealingProgressRPC,
    WinRequestRPC,
    SetMyAvatar,
    SetEnemyAvatar,
    SetEquipTypeRPC,
    SynchMinigunRotationRPC,
    PlayZombieBeforeAttackRPC,
    SendNewEmojiRPC,
    SynchVehicleParamsRPC,
    SendBattleEmojiRPC,
    SynchVehicleColorRPC,
    SendChangeMobRPC,
    EnterInTrigger,
    SynchWeaponModulesRPC,
    SynchArmorModulesRPC,
    SetModuleGadgetEffectActiveRPC,
    CreateRocketRPC,
    ShowEffectOnOtherPlayersRPC,
    SetParticleToWeaponRPC,
    RegenerateHealthMob,
    SendKillMob,
    SynhDeltaHealthFromWeaponRPC,
    JumpDisableRPC,
    SetPortalRPC,
    ActivatePolymorphRPC,
    DeactivatePolymorphRPC,
    AlternativeMobAttack,
    AlternativeMobShot,
    DetonateKamikadze,
    SetWeaponLevel,
    MarkEnemy,
    GetDamageToShieldRPC,
    SendOnGroundEffect,
    PluginRPC,
    EnableTeleportEffectsRPC,
    AddAmmoFromWeaponRPC,
    KillerInvisiblityRPC,
    SyncGadgetReflectorCoeffRPC,
    SynchVipStatusRPC,
    InviteToSquadRPC,
    JoinSquadRPC,
    InviteToSquadFailRPC,
    SyncBreakableObjects,
    FreeFallSynchRPC,
    SyncCableCarRPC,
    StartPlantingBomb,
    ResetPlantingBomb,
    StartDefusingBomb,
    ResetDefusingBomb,
    SetIsPaidTransport,
    SyncOwnerTransport,
    SyncCharIdRPC,
    SyncTeammateRegenByMe
};

enum PhotonTargets
{
    All,
    Others,
    MasterClient,
    AllBuffered,
    OthersBuffered,
    AllViaServer,
    AllBufferedViaServer,
    SeflViaServer
};

void* (*GetComponent) (void* gameObject, void* type);
void (*SendChat) (void* obj, monoString* text, bool isClan, monoString* logoid);
void (*EnableXray) (void* obj, bool enable);

void (*JoinToRoomPhotonAfterCheck) (void* obj);

// Type
void* (*Type$GetType)(void* type);

// Vector3
float (Vector3$get_x)(void* ref) {
    if (ref != nullptr) {
        return *(float *) ((uint64_t) ref + 0x0);
    }
}
float (Vector3$get_y)(void* ref) {
    if (ref != nullptr) {
        return *(float *) ((uint64_t) ref + 0x4);
    }
}
float (Vector3$get_z)(void* ref) {
    if (ref != nullptr) {
        return *(float *) ((uint64_t) ref + 0x8);
    }
}

// Component
void* (*Component$get_gameObject)(void* component);
void* (*Component$get_transform)(void* component);
monoString* (*Component$get_tag)(void* component);

// Transform
void* (*Transform$get_position)(void* object);

// Quaternion
void* (*Quaternion$get_identity)();

// GameObject
bool (*GameObject$get_active)(void* gameObject);
void (*GameObject$set_active)(void* gameObject, bool active);
void* (*GameObject$get_transform)(void* gameObject);

// Camera
void* (*Camera$get_Main)();

// PhotonView
void (*PhotonView$RPC)(void* ref, int rpc, int targets, void* args[]);

// FirstPersonControlSharp
void (FirstPersonControlSharp$set_ninjaJumpUsed)(void* ref, bool used) {
    if (ref != nullptr) {
        *(bool *) ((uint64_t) ref + 0x101) = used;
    }
}

// SkinName

void* (SkinName$firstPersonControl)(void* ref) {
    if (ref != nullptr) {
        return *(void **) ((uint64_t) ref + 0x1C8);
    }
}

// Player_move_c
void* (Player_move_c$photonView)(void* ref) {
    if (ref != nullptr) {
        return *(void **) ((uint64_t) ref + 0x3C8);
    }
}

void* (Player_move_c$skinName)(void* ref) {
    if (ref != nullptr) {
        return *(void **) ((uint64_t) ref + 0x648);
    }
}

void Pointers() {
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x46F498C")));
    OpenURL = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43807DC")));
    OpenKeyboard = (void(*)(monoString*, int*, bool*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435E2D4")));
    SetString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2ECD530")));
    setSomething = (void(*) (void*, monoString*, int*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16EB844")));
    SendChat = (void(*) (void*, monoString*, bool, monoString *)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x471AB70")));
    addWeapon = (void(*) (void*, monoString*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4BCE4C0")));
    SetMasterClient = (bool(*)(void*)) (bool*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ADAE0"));
    get_LocalPlayer = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43A6504"));
    DestroyPlayerObjects = (void (*)(void *)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ADF9C"));
    PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) (monoArray<void**>*) (g_il2cppBaseMap.startAddress + string2Offset("0x43A6814"));
    // DestroyAll = (void(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43AE1C0"));
    Instantiate = (void*(*)(monoString*, void*, void*, unsigned char)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x43ACB28"));
    EnableXray = (void(*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x470CF0C"));
    BuyStickerPack = (void(*)(int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x40A8384")));
    JoinToRoomPhotonAfterCheck = (void(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x38DA1B4")));
    // UNITY FUNC
    Camera$get_Main = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435A454")));
    Component$get_gameObject = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4375C90")));
    Component$get_tag = (monoString*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4375F80")));
    Component$get_transform = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4375C54")));
    Type$GetType = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4D288E0")));
    GameObject$get_active = (bool(*)(void*)) (bool) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435F850")));
    GameObject$set_active = (void(*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x435F88C")));
    GameObject$get_transform = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4346414")));
    Quaternion$get_identity = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x437D668")));
    Transform$get_position = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43813FC")));
    PhotonView$RPC = (void(*)(void*, int, int, void*[])) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43B3B60")));
}

// 0x435FA0C <- offset for gameobject.tag
// 0x434733C <- offset for object.name

void (*old_WeaponManager)(void *obj);
void WeaponManager(void *obj) {
    if (obj != nullptr) {
        auto start = std::chrono::steady_clock::now();
        auto delay = std::chrono::seconds(15);
        if (isAddWeapons) {
            for (int i = 0; i < 300; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (12));
            }
            isAddWeapons = false;
            LoadLevel(CreateIl2cppString("AppCenter"));
            while (std::chrono::steady_clock::now() - start < delay) {}
            isAddWeapons2 = true;
        }
        if (isAddWeapons2) {
            for (int i = 300; i < 500; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (12));
            }
            isAddWeapons2 = false;
            LoadLevel(CreateIl2cppString("AppCenter"));
            while (std::chrono::steady_clock::now() - start < delay) {}
            isAddWeapons3 = true;
        }
        if (isAddWeapons3) {
            for (int i = 500; i < 700; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (12));
            }
            isAddWeapons3 = false;
            LoadLevel(CreateIl2cppString("AppCenter"));
            while (std::chrono::steady_clock::now() - start < delay) {}
            isAddWeapons4 = true;
        }
        if (isAddWeapons4) {
            for (int i = 700; i < 900; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (12));
            }
            isAddWeapons4 = false;
            LoadLevel(CreateIl2cppString("AppCenter"));
            while (std::chrono::steady_clock::now() - start < delay) {}
            isAddWeapons5 = true;
        }
        if (isAddWeapons5) {
            for (int i = 900; i < 1186; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (12));
            }
            isAddWeapons5 = false;
            LoadLevel(CreateIl2cppString("ClosingScene"));
        }
    }
    old_WeaponManager(obj);
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

        if(gadgetdisabler){
            *(bool*)((uint64_t) obj + 0x1F0) = true;//isGadgetDisabler
            *(float*)((uint64_t) obj + 0x1F4) = 99999;//gadgetDisableTime
        }
        if(killboost){
            *(bool*)((uint64_t) obj + 0x3F8) = true;//isIncreasedDamageFromKill
            *(int*)((uint64_t) obj + 0x400) = 3;//maxStackIncreasedDamage
        }

        if(portalBull){
            *(bool*)((uint64_t) obj + 0x3B4) = true;//portalGun
            *(bool*)((uint64_t) obj + 0x3B5) = true;//isPortalExplosion
            *(int*)((uint64_t) obj + 0x3C0) = 1;//isPortalExplosion
        }

        if(charm){
            *(bool*)((uint64_t) obj + 0x260) = true;//isCharm
            *(float*)((uint64_t) obj + 0x264) = 99999;//charmTime
        }

        if(scopef){
            *(bool*)((uint64_t) obj + 0xC5) = true;//isCharm
        }

        if(fte){
            *(bool*)((uint64_t) obj + 0x234) = true;//fireImmunity
            *(bool*)((uint64_t) obj + 0x235) = true;//toxicImmunity
        }

        if(fte){
            *(bool*)((uint64_t) obj + 0x234) = true;//fireImmunity
            *(bool*)((uint64_t) obj + 0x235) = true;//toxicImmunity
        }

        if(dash){
            *(bool*)((uint64_t) obj + 0x294) = true;//isDash
            *(float*)((uint64_t) obj + 0x298) = 100;//dashMaxImpulse
            *(float*)((uint64_t) obj + 0x29C) = 100;//dashDecaySpeed
        }

        if(snowstormbull){
            *(bool*)((uint64_t) obj + 0x2E4) = true;//snowStorm
            *(float*)((uint64_t) obj + 0x2E8) = 6;//snowStormBonusMultiplier
            if(snowstormbullval != NULL){
                *(float*)((uint64_t) obj + 0xDC) = snowstormbullval; // shootDistance
                *(float*)((uint64_t) obj + 0x5F8) = snowstormbullval; // range
            }
            else{
                kniferangesex = true;
            }
        }

        if(polymorph){
            *(bool*)((uint64_t) obj + 0x2D4) = true;//polymorpher
            *(float*)((uint64_t) obj + 0x2D8) = 999999999;//polymorphDuarationTime
            *(int*)((uint64_t) obj + 0x3B4) = 0;//polymorphType
            *(int*)((uint64_t) obj + 0x3B4) = 99999999;//polymorphMaxHealth
            harpoonBull = true;
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
            *(bool*)((uint64_t) obj + 0x346) = true;//isFrostSword
            *(float*)((uint64_t) obj + 0x348) = 5;//frostDamageMultiplier
            *(float*)((uint64_t) obj + 0x34C) = 999999;//frostRadius
        }

        if(ammo){
            *(int*)((uint64_t) obj + 0x19C) = 99;//countShots
        }

        if(expbull){
            *(bool*)((uint64_t) obj + 0x1A5) = true;//bulletExplode
        }

        if(shotbull){
            *(bool*)((uint64_t) obj + 0x1A6) = true; //isShotBull
        }

        if(kniferangesex && !kniferange){
            *(float*)((uint64_t) obj + 0xDC) = MAXFLOAT; // shootDistance
            *(float*)((uint64_t) obj + 0x5F8) = MAXFLOAT; // range
        }

        if(railbull){
            *(bool*)((uint64_t) obj + 0x1BC) = true;//railgun
            *(bool*)((uint64_t) obj + 0x1C8) = false;//railgunStopAtWall
        }

        if(harpoonBull){
            *(bool*)((uint64_t) obj + 0x2A4) = true;//harpoon
            *(float*)((uint64_t) obj + 0x2C8) = 99999;//harpoonMaxDistance
            *(float*)((uint64_t) obj + 0x2C0) = 99999;//harpoonMaxDistance
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
            *(float*)((uint64_t) obj + 0x124) = 0;//recoilCoeffZoom
        }

        if(quickscope){
            *(float*)((uint64_t) obj + 0xF8) = 9999;//scopeSpeed
        }

        if(xray){ *(bool*)((uint64_t) obj + 0xC6) = true;}
        /*void* ItemRecord = *(void**)((uint64_t) obj + 0x648);
        if(ItemRecord != nullptr){
            if(spleef){
                *(bool*)((uint64_t) obj + 0x6D) = true;//isSpleef
            }
        }*/
    }
    oldWeaponSounds(obj);
}

void(*oldPlayerMoveC)(void* obj);
void(PlayerMoveC)(void* obj){
    if(obj != nullptr){
        if(spamchat){
            SendChat(obj, CreateIl2cppString("buy zygiskpg - https://discord.gg/ZVP2kuJXww"), false, CreateIl2cppString("0"));
        }

        if(xrayApplied){
            EnableXray(obj, true);
        }

        if(ninjaJump){
            FirstPersonControlSharp$set_ninjaJumpUsed(SkinName$firstPersonControl(Player_move_c$skinName(obj)), false);
        }

        if (spoofMe) {
            void* argsForSetPixelBookID[] = {CreateIl2cppString(OBFUSCATE("ZYGISKPG ON TOP"))};
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetPixelBookID, PhotonTargets::All, argsForSetPixelBookID);
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetPlayerUniqID, PhotonTargets::All, argsForSetPixelBookID);
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetNickName, PhotonTargets::All, argsForSetPixelBookID);
            spoofMe = false;
        }

        if (playstantiate) {
            SetMasterClient(get_LocalPlayer());
            LOGE("instantiating");
            void* skinName = Player_move_c$skinName(obj);
            LOGE("skinname is null: %s", std::to_string(skinName == nullptr).c_str());
            void* pos = Transform$get_position(Component$get_transform(skinName));
            LOGE("pos is null: %s", std::to_string(pos == nullptr).c_str());
            LOGE("getidentity is null: %s", std::to_string(Quaternion$get_identity() == nullptr).c_str());
            Instantiate(CreateIl2cppString("RacingCar"), pos, Quaternion$get_identity(), 0);
            LOGE("instantiated");
            float x = Vector3$get_x(pos);
            float y = Vector3$get_y(pos);
            float z = Vector3$get_z(pos);
            LOGE("pos: %f, %f, %f", x, y, z);
            playstantiate = false;
        }
    }
    oldPlayerMoveC(obj);
}

void(*old_UIInput)(void* obj);
void(UIInput)(void* obj){
    if(obj != nullptr){
        *(float*)((uint64_t) obj + 0x44) = 0;
    }
    old_UIInput(obj);
}

void(*old_HandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args);
void(HandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args){
    if(obj != nullptr){
        JoinToRoomPhotonAfterCheck(obj);
        return;
    }
    old_HandleJoinRoomFromEnterPasswordBtnClicked(obj, sender, args);
}

enum StickerType {
    none = 0,
    classic = 1,
    winter = 2,
    easter = 3
};

void (*old_PixelTime)(void *obj);
void PixelTime(void *obj) {
    if (obj != nullptr) {
        if (isBuyEasterSticker) {
            isBuyEasterSticker = false;
            BuyStickerPack((int*)StickerType::easter);
        }
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
            setSomething(webInstance(), CreateIl2cppString(curList[selectedCur]), (int *)(amountws), (int *)(12));
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

float (*oldSpeed)();
float Speed(){
    if(speed){
        return 100;
    }
    else{
        return 4;
    }
}

float (*oldPetGod)();
float PetGod(){//丏丏世三下万丞上万
    if(pgod){
        return 999999;
    }
    else{
        return 100;
    }
}

float (*oldPetRespawnTime)();
float PetRespawnTime(){
    if(prespawn){
        return 0;
    }
}

void(*oldPetEngine)(void* obj);
void PetEngine(void* obj){
    if(obj != nullptr){
        if(pspeed != NULL){
            *(float*)((uint64_t) obj + 0x1B8) = pspeed;
            *(float*)((uint64_t) obj + 0x1B4) = pspeed;
        }
    }
    oldPetEngine(obj);
}

bool(*oldIsGadgetEnabled)(void* obj);
bool IsGadgetEnabled(void* obj){
    if(obj != nullptr && gadgetsEnabled){
        return true;
    }
    oldIsGadgetEnabled(obj);
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
   HOOK("0x4BCA3D4", WeaponManager, old_WeaponManager);
   // HOOK("0x41FD8D4", Speed, oldSpeed);CRASH
    HOOK("0x473F064", PlayerMoveC, oldPlayerMoveC);
    HOOK("0x38DB748", HandleJoinRoomFromEnterPasswordBtnClicked, old_HandleJoinRoomFromEnterPasswordBtnClicked);
    HOOK("0x15ECC04", UIInput, old_UIInput);
    HOOK("0x474FEFC", IsGadgetEnabled, oldIsGadgetEnabled);
    //HOOK("0x3D37C34", PetGod, oldPetGod);c
    //HOOK("0x3D3A0A8", PetEngine, oldPetEngine);c
    //HOOK("0x3F7C62C", PetRespawnTime, oldPetRespawnTime);c
}

void Patches() {
    PATCH_SWITCH("0x476323C", "1F2003D5C0035FD6", god);//dear future self, if this game ever updates kys (look for player_move_c and try to find the enum with himself, headshot etc and pray you find the right thing, has alot of stuff in the args )
    PATCH_SWITCH("0x3C958B0", "1F2003D5C0035FD6", god);
    PATCH_SWITCH("0x4FBDCF0", "1F2003D5C0035FD6", god);
    PATCH_SWITCH("0x4FBD460", "1F2003D5C0035FD6", god);
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
    PATCH_SWITCH("0x41FA918", "200180922C0035FD6", ninjaJump);
    PATCH_SWITCH("0x2862258", "1F2003D5C0035FD6", xray);//attempt
    PATCH("0x206D13C", "C0035FD6");
    PATCH("0x3C962E4", "C0035FD6");
    PATCH("0x4504710", "000080D2C0035FD6");
    PATCH("0x3B4BA00", "200080D2C0035FD6");
    PATCH("0x3B4BC40", "200080D2C0035FD6");
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
           if (ImGui::Button(OBFUSCATE("Add All Weapons"))) {
                isAddWeapons = true;
            }
            ImGui::TextUnformatted((OBFUSCATE("Gives the player all the weapons (It will take a while, Freezing is normal)")));
            ImGui::Checkbox(OBFUSCATE("All Weapon Skins"), &wepSkins);
            ImGui::TextUnformatted(OBFUSCATE("Makes all weapon skins purchasable"));
            ImGui::Checkbox(OBFUSCATE("Free Lottery"), &modKeys);
            ImGui::TextUnformatted(OBFUSCATE("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)"));
            if (ImGui::Button(OBFUSCATE("Buy Easter Pack"))) {
                isBuyEasterSticker = true;
            }
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Currency Mods"))) {
            ImGui::SliderInt(OBFUSCATE("Amount"), &amountws, 0, 1000000);
            ImGui::TextUnformatted(OBFUSCATE("Will be counted as the value that the game will use."));
            ImGui::ListBox(OBFUSCATE("Currency"), &selectedCur, curList, IM_ARRAYSIZE(curList), 4);
            if (ImGui::Button(OBFUSCATE("Add Currency"))) {
                isAddCurPressed = true;
            }
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Player Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Godmode"), &god);
            ImGui::TextUnformatted(OBFUSCATE("Makes you invincible (others can kill you but you won't die and just become invisible)"));
            ImGui::Checkbox(OBFUSCATE("Force Double Jump"), &doublejump);
            ImGui::Checkbox(OBFUSCATE("Infinite Jump"), &ninjaJump);
            ImGui::Checkbox(OBFUSCATE("Player Speed"), &speed);
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Weapon Mods"))) {
            ImGui::SliderFloat(OBFUSCATE("Shotgun Damage Buff"),&damage, 0.0f, 15.0f);
            ImGui::TextUnformatted(OBFUSCATE("Amplifys the shotgun  damage. (Anything above 8 might kick after a few kills)"));
            ImGui::Checkbox(OBFUSCATE("Force Critical Hits"), &crithit);
            ImGui::TextUnformatted(OBFUSCATE("Forces Critical Shots each time you hit someone."));
            ImGui::Checkbox(OBFUSCATE("Unlimited Ammo"), &ammo);
            ImGui::Checkbox(OBFUSCATE("Silent Aim"),&snowstormbull);
            ImGui::TextUnformatted(OBFUSCATE("Shooting anywhere will hit others."));
            ImGui::Checkbox(OBFUSCATE("Force Explosive Bullets"), &expbull);
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot explosive bullets."));
            ImGui::Checkbox(OBFUSCATE("Force Railgun Bullets"), &railbull);
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot railgun bullets."));
            ImGui::Checkbox(OBFUSCATE("Force Shotgun Bullets"), &shotBull);//add it chris dont forget
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot shotgun bullets."));
            ImGui::SliderInt(OBFUSCATE("Reflection Rays"),&reflection, 0, 1000);
            ImGui::TextUnformatted(OBFUSCATE("Amplifys the reflection ray ricochet."));
            ImGui::Checkbox(OBFUSCATE("Infinite Knife/Flamethrower Range"),&kniferangesex);
            ImGui::TextUnformatted(OBFUSCATE("Allows you to aim and hit people with a knifer or a framethrower at any distance."));
            ImGui::Checkbox(OBFUSCATE("No Recoil and Spread"),&recoilandspread);
            ImGui::Checkbox(OBFUSCATE("Force Scope"),&scopef);
            ImGui::TextUnformatted(OBFUSCATE("Every weapon will have a scope."));
            ImGui::Checkbox(OBFUSCATE("Force Portal Bullets"),&portalBull);
            ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot portal bullets."));
            ImGui::Checkbox(OBFUSCATE("Force Polymorph Bullets"),&polymorph);
            ImGui::TextUnformatted(OBFUSCATE("Forces bullets to make players turn into sheep."));
            ImGui::Checkbox(OBFUSCATE("Force Harpoon Bullets"),&harpoonBull);
            ImGui::TextUnformatted(OBFUSCATE("Explosive Bullets v2"));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Effect Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Force Charm"), &charm);
            ImGui::TextUnformatted(OBFUSCATE("Adds the charm effect (Used to reduce half of the enemy's weapon efficiency)"));
            ImGui::Checkbox(OBFUSCATE("No Fire and Toxic Effects"), &fte);
            ImGui::TextUnformatted(OBFUSCATE("Removes the burning and being intoxicated effect on you."));
            ImGui::Checkbox(OBFUSCATE("Force Electric Shock"), &electric);
            ImGui::TextUnformatted(OBFUSCATE("Adds the electric shock effect"));
            ImGui::Checkbox(OBFUSCATE("Force Blindness Effect"), &blindness);
            ImGui::TextUnformatted(OBFUSCATE("Adds the electric shock effect"));
            ImGui::Checkbox(OBFUSCATE("Force Speed-Up Effect"), &speedup);
            ImGui::TextUnformatted(OBFUSCATE("Adds a speed-up effect (Will speed up any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Slow-down Effect"), &slowdown);
            ImGui::TextUnformatted(OBFUSCATE("Adds a slow-down effect (Will freeze any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Jump Disabler Effect"), &jumpdisable);
            ImGui::TextUnformatted(OBFUSCATE("Adds the jump disabler effect (Will disable jump for any player you shoot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Head Magnifier Effect"), &headmagnify);
            ImGui::TextUnformatted(OBFUSCATE("Adds the head magnifier effect (Will magnify the player's head once shot until they die)"));
            ImGui::Checkbox(OBFUSCATE("Force Gadget Disabler Effect"), &gadgetdisabler);
            ImGui::TextUnformatted(OBFUSCATE("Adds the head gadget disabler effect (Will disable player's gadget once shot until they die)"));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Visual Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Chams"), &xray);
            ImGui::TextUnformatted(OBFUSCATE("Shows the enemy body through walls."));
            ImGui::Checkbox(OBFUSCATE("Show marker"), &enemymarker);
            ImGui::TextUnformatted(OBFUSCATE("Shows the enemy after you shoot them once."));
            ImGui::Checkbox(OBFUSCATE("Quick-Scope"), &quickscope);
            ImGui::TextUnformatted(OBFUSCATE("Opens the scope instantly."));
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Pet Mods"))) {
            //ImGui::Checkbox(OBFUSCATE("Godmode"), &pgod);
           // ImGui::TextUnformatted(OBFUSCATE("Pets never die"));
           // ImGui::SliderFloat(OBFUSCATE("Pet Speed"), &pspeed, 0.0f, 500.0f);

        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Game Mods"))) {
            ImGui::Checkbox(OBFUSCATE("Kill All"),&kniferange);
            ImGui::TextUnformatted(OBFUSCATE("Kill everyone"));
            ImGui::Checkbox(OBFUSCATE("Spam Chat"), &spamchat);
            ImGui::Checkbox(OBFUSCATE("Turret Godmode"), &tgod);
            ImGui::TextUnformatted(OBFUSCATE("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does."));
            ImGui::Checkbox(OBFUSCATE("Drone Godmode"), &removedrone);
            ImGui::TextUnformatted(OBFUSCATE("The drone gadget will never despawn. (Don't get more than 1 drone or you'll be kicked)"));
            ImGui::Checkbox(OBFUSCATE("Force Coin Drop"), &coindrop);
            ImGui::TextUnformatted(OBFUSCATE("Always drops coins when someone dies."));
            ImGui::Checkbox(OBFUSCATE("Glitch Everyone"), &xrayApplied);
            ImGui::TextUnformatted(OBFUSCATE("Every weapon will have a scope."));
            if (ImGui::Button(OBFUSCATE("Crash Everyone"))) {
                destroy = true;
            }
        }
        if (ImGui::CollapsingHeader(OBFUSCATE("Experimental Mods")))
        {
            ImGui::Checkbox(OBFUSCATE("Spoof Editor"), &enableEditor);
            ImGui::TextUnformatted(OBFUSCATE("Makes the game think its on the Unity Editor"));
            ImGui::ListBox(OBFUSCATE("Select Scene"), &selectedScene, sceneList, IM_ARRAYSIZE(sceneList), 4);
            if (ImGui::Button(OBFUSCATE("Load Scene"))) {
                isLoadScenePressed = true;
            }
            /*if (ImGui::Button(OBFUSCATE("Become god"))) {
                SetMasterClient(get_LocalPlayer());
            }*/
            if (ImGui::Button(OBFUSCATE("Playstantiate the playfab"))) {
                playstantiate = true;
            }
            if (ImGui::Button(OBFUSCATE("Mask me"))) {
                spoofMe = true;
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
