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
#include "Includes/Dobby/dobbyForHooks.h"
#include "Include/Unity.h"
#include "Misc.h"
#include "hook.h"
#include "Include/Roboto-Regular.h"
#include <iostream>
#include <chrono>
#include "Include/Quaternion.h"
#include "Rect.h"
#include <fstream>
#include <limits>
#define GamePackageName "com.pixel.gun3d"

int glHeight, glWidth;
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
static int selectedScene = 0;
static int selectedCur = 0;
static int selectedWeapon = 0;

std::string IDacc;

const char* loobyBundles[] = { "background_1_castle_1", "background_1_chess_1", "background_1_christmas_1", "background_1_future_1", "background_1_halloween_1", "background_1_heaven_3", "background_1_japan_1", "background_1_military_1", "background_1_presidential_1", "background_1_space_1", "background_1_western_1", "background_2_castle_1", "background_2_castle_2", "background_2_castle_3", "background_2_chess_1", "background_2_christmas_1", "background_2_custom_1", "background_2_future_1", "background_2_halloween_1", "background_2_heaven_1", "background_2_heaven_2", "background_2_heaven_3", "background_2_japan_1", "background_2_military_1", "background_2_military_2", "background_2_military_3", "background_2_presidential_1", "background_2_space_1", "background_2_space_2", "background_2_space_3", "background_2_western_1", "base_castle_1", "base_castle_2", "base_castle_3", "base_chess_1", "base_christmas_1", "base_future_1", "base_halloween_1", "base_heaven_1", "base_heaven_2", "base_heaven_3", "base_japan_1", "base_military_1", "base_military_2", "base_military_3", "base_presidential_1", "base_space_1", "base_space_2", "base_space_3", "base_western_1", "decor_big_arcade_1", "decor_big_castle_1", "decor_big_castle_2", "decor_big_castle_3", "decor_big_chess_1", "decor_big_christmas_1", "decor_big_custom_1", "decor_big_custom_2", "decor_big_custom_3", "decor_big_custom_4", "decor_big_future_1", "decor_big_halloween_1", "decor_big_heaven_1", "decor_big_heaven_2", "decor_big_heaven_3", "decor_big_military_1", "decor_big_military_2", "decor_big_military_3", "decor_big_phoenix_1", "decor_big_presidential_1", "decor_big_space_2", "decor_big_space_3", "decor_big_western_1", "decor_small_castle_1", "decor_small_castle_2", "decor_small_castle_3", "decor_small_chess_1", "decor_small_christmas_1", "decor_small_custom_1", "decor_small_custom_2", "decor_small_future_1", "decor_small_halloween_1", "decor_small_heaven_1", "decor_small_heaven_2", "decor_small_heaven_3", "decor_small_japan_1", "decor_small_military_1", "decor_small_military_2", "decor_small_military_3", "decor_small_phoenix_1", "decor_small_presidential_1", "decor_small_space_1", "decor_small_space_2", "decor_small_space_3", "decor_small_space_4", "decor_small_space_5", "decor_small_space_6", "decor_small_western_1", "device_1_1", "device_1_2", "device_1_4", "device_1_5", "device_1_6", "gate_castle_1", "gate_castle_2", "gate_castle_3", "gate_chess_1", "gate_christmas_1", "gate_future_1", "gate_halloween_1", "gate_heaven_1", "gate_heaven_2", "gate_heaven_3", "gate_japan_1", "gate_military_1", "gate_military_2", "gate_military_3", "gate_presidential_1", "gate_space_1", "gate_space_2", "gate_space_3", "gate_western_1", "kennel_base_1", "kennel_castle_1", "kennel_chess_1", "kennel_christmas_1", "kennel_fly_base_1", "kennel_fly_castle_1", "kennel_fly_heaven_1", "kennel_fly_military_1", "kennel_fly_premium_1", "kennel_fly_space_1", "kennel_future_1", "kennel_halloween_1", "kennel_heaven_1", "kennel_japan_1", "kennel_military_1", "kennel_premium_1", "kennel_presidential_1", "kennel_space_1", "kennel_western_1", "road_arcade_1", "road_castle_1", "road_castle_2", "road_castle_3", "road_chess_1", "road_christmas_1", "road_future_1", "road_halloween_1", "road_heaven_1", "road_heaven_2", "road_heaven_3", "road_japan_1", "road_military_1", "road_military_2", "road_military_3", "road_phoenix_1", "road_presidential_1", "road_space_1", "road_space_2", "road_space_3", "road_western_1", "skybox_1", "skybox_10", "skybox_11", "skybox_12", "skybox_14", "skybox_15", "skybox_16", "skybox_17", "skybox_18", "skybox_19", "skybox_2", "skybox_20", "skybox_21", "skybox_22", "skybox_23", "skybox_24", "skybox_25", "skybox_3", "skybox_4", "skybox_5", "skybox_6", "skybox_7", "skybox_8", "skybox_9", "terrain_arcade_1", "terrain_castle_1", "terrain_castle_2", "terrain_castle_3", "terrain_chess_1", "terrain_christmas_1", "terrain_future_1", "terrain_halloween_1", "terrain_heaven_1", "terrain_heaven_2", "terrain_heaven_3", "terrain_japan_1", "terrain_military_1", "terrain_military_2", "terrain_military_3", "terrain_phoenix_1", "terrain_presidential_1", "terrain_space_1", "terrain_space_2", "terrain_space_3", "terrain_western_1", "wall_castle_1", "wall_castle_2", "wall_castle_3", "wall_chess_1", "wall_christmas_1", "wall_future_1", "wall_halloween_1", "wall_heaven_1", "wall_heaven_2", "wall_heaven_3", "wall_japan_1", "wall_military_1", "wall_military_2", "wall_military_3", "wall_presidential_1", "wall_space_1", "wall_space_2", "wall_space_3", "wall_western_1", "bundle_my", "bundle_winter", "bundle_army", "bundle_heaven", "bundle_medieval", "bundle_space", "bundle_government", "bundle_christmas", "bundle_halloween", "bundle_oriental", "bundle_western", "bundle_future" };
const char* graffitiList[] = { "graffiti_1028", "graffiti_2028", "graffiti_3028", "graffiti_4028", "graffiti_5028", "graffiti_6028", "graffiti_7028", "graffiti_8028", "graffiti_9028", "graffiti_10028", "graffiti_11028", "graffiti_13028", "graffiti_12028", "graffiti_14028", "graffiti_15028" };
const char* goofyWeps[] = { "iron_sword", "hound_weapon_one", "desert_fighter", "locator", "advanced_scout_rifle_l", "harsh_punisher_l", "manual_inferno_l", "offensive_rocket_system_l", "headhunter_pistols_l" };
const char* petsList[] = { "pet_alien_cat", "pet_armadillo", "pet_arnold_3000", "pet_bat", "pet_bear", "pet_blocston_dogmatics", "pet_boar", "pet_cat", "pet_cat_2", "pet_cat_3", "pet_chick01", "pet_chicken", "pet_combat_robb_e", "pet_crab", "pet_cyberphoenix", "pet_dachshund", "pet_deer", "pet_dinosaur", "pet_dog", "pet_eagle", "pet_eaglespirit", "pet_fighting_turtle", "pet_flappy_fish", "pet_fox", "pet_goat", "pet_green_novice", "pet_griffin", "pet_griffin_owl", "pet_headbug", "pet_horse", "pet_horse_figure", "pet_husky", "pet_hyena", "pet_ice_dragon_egg", "pet_kangaroo", "pet_knitted_pony", "pet_lion", "pet_lucky_duck", "pet_magical_dragon", "pet_mammoth", "pet_mecha_cat", "pet_meerkat", "pet_monkey", "pet_ostrich", "pet_owl", "pet_panda", "pet_panther", "pet_parrot", "pet_penguin", "pet_pga_statue", "pet_phoenix", "pet_pocket_demon", "pet_porcupine", "pet_pterodactyl", "pet_rabbit", "pet_raccoon", "pet_ram", "pet_raven", "pet_red_panda", "pet_rhino", "pet_robo_dog", "pet_sabertooth_tiger", "pet_sacred_scarab", "pet_sheep", "pet_skunk", "pet_small_floppa", "pet_snake", "pet_squirrel", "pet_stone", "pet_stone_elemental", "pet_tactical_ferret", "pet_tiger", "pet_trex", "pet_tukan", "pet_turkey", "pet_unicorn", "pet_vacuum_helpbro_2000", "pet_wolf", "pet_zazou", "pet_zombie_head", "pet_zombiecat", "pet_support_piglet", "pet_firefly_cat", "pet_elementals_helper", "pet_tankmouse", "pet_backwater_dweller", "pet_shiny_companion", "pet_8_bit_boi", "pet_blue_buddy", "pet_wise_owl", "pet_stingray_3000", "pet_bullfinch_the_giver", "pet_festive_black_rabbit", "pet_servants_right_hand" };
const char* gadgetList[] { "gadget_armor_regeneration", "gadget_astral", "gadget_bionic_shield", "gadget_black_label", "gadget_Blizzard_generator", "gadget_christmastreeturret", "gadget_combat_spinner", "gadget_cyber_bees", "gadget_cybersamurai", "gadget_demon_stone", "gadget_denied", "gadget_disabler", "gadget_dragonwhistle", "gadget_fakebonus", "gadget_firecrackers", "gadget_firemushroom", "gadget_firework", "gadget_fraggrenade", "gadget_guardian", "gadget_hologram_projector", "gadget_jetpack", "gadget_leaderdrum", "gadget_mech", "gadget_medicalstation", "gadget_medkit", "gadget_mh_mech1", "gadget_mh_mech2", "gadget_mh_mech3", "gadget_mine", "gadget_molotov", "gadget_ninjashurickens", "gadget_nucleargrenade", "gadget_nutcracker", "gadget_pandorabox", "gadget_petbooster", "gadget_phoenix_blood", "gadget_pocket_harpoon", "gadget_reflector", "gadget_rocket_mech", "gadget_shield", "gadget_singularity", "gadget_snowman", "gadget_stealth", "gadget_sticky_turret", "gadget_stickycandy", "gadget_super_magnet", "gadget_timewatch", "gadget_turret", "gadget_x-ray_glasses", "gadget_class_bullet_medkit", "gadget_class_bullet_phonixblood", "gadget_class_knight_magnet", "gadget_class_valk_jetpack", "gadget_class_valk_cyberbees", "gadget_class_lifeline_medstation", "gadget_class_lifeline_biotiksheald", "gadget_class_sara_hook", "gadget_class_sara_xray", "gadget_class_saber_shurikens", "gadget_class_saber_reflector", "gadget_class_mex_turret", "gadget_class_mex_nucleargrenade", "gadget_class_viktor_smilemine", "gadget_class_viktor_nutcracker", "gadget_br_fraggrenade", "gadget_br_molotov", "gadget_br_mine", "gadget_hook_monster" };
const char* avatarList[] { "carpet_plane", "festive_glider", "glider_additional_deck", "glider_aero_suit", "glider_air_raider", "glider_air_surfboard", "glider_airborne_basket", "glider_basic", "glider_beerocopter", "glider_big_bomb", "glider_block", "glider_blogger", "glider_bloq_jetpack", "glider_boat_of_ra", "glider_bubble", "glider_cartoon_plane", "glider_copter", "glider_cybercop", "glider_d-day_pod", "glider_dragon", "glider_dragon_spirit", "glider_drill", "glider_drone", "glider_elven_cloak", "glider_flutter_pony", "glider_flying_manta", "glider_futuristic", "glider_garbage", "glider_gas_o_copter", "glider_giant_bat", "glider_giant_bubble", "glider_giant_scarab", "glider_hawk", "glider_ice_wings", "glider_icewing_dragon", "glider_inflatable_banana", "glider_judge_rocket", "glider_kite", "glider_leader", "glider_magic_broom", "glider_magic_horse", "glider_magic_wings", "glider_magical_hat", "glider_mattress", "glider_mechanical_wings", "glider_membrawinger", "glider_merchant", "glider_military", "glider_money_case", "glider_mutated_wings", "glider_neon", "glider_netrunner", "glider_pegasus", "glider_pew_pew_spaceship", "glider_phoenix", "Glider_phoenix_wings", "glider_police_cage", "glider_poncho", "glider_popit", "glider_portalius", "glider_pro_hoverboard", "glider_reponosaurus", "glider_satellite", "glider_soaring_sandals", "glider_soundman", "glider_starlet", "glider_steam", "glider_super_power", "glider_tank_for_mutant", "glider_toy_horse", "glider_transport_helicopter", "glider_ufo", "glider_vacation_baggage", "glider_valkyrie_wings", "glider_vecha_glider", "glider_vulture", "Hang_glider", "roast_turkey", "royale_glider_deer", "royale_glider_santa_chair", "royale_jet_scooter", "royale_leaf_glider", "superhero_cape", "tactical_glider", "wooden_glider", "Hat_easter", "merchant_hood", "royale_cubic_hat", "royale_cybercop_helmet", "royale_hat_alien_parasite", "royale_hat_alien_santa", "royale_hat_alpha_squad", "royale_hat_apple_with_arrow", "royale_hat_aquarium", "royale_hat_bat_santa", "royale_hat_beast", "royale_hat_bomb", "royale_hat_bow_ears", "royale_hat_boyscout", "royale_hat_boyscout_tail", "royale_hat_bravo_squad", "royale_hat_cap", "royale_hat_cap_glider", "royale_hat_chief", "royale_hat_combat_ushanka", "royale_hat_cook", "royale_hat_doctor_plague", "royale_hat_donkey", "royale_hat_donnyhelmet", "royale_hat_eared_beret", "royale_hat_ears", "royale_hat_elf", "royale_hat_feather", "royale_hat_fireman", "royale_hat_fish", "royale_hat_football", "royale_hat_gas_mask", "royale_hat_gnome_helmet", "royale_hat_guard", "royale_hat_headband", "royale_hat_hen_hat", "royale_hat_holographic_mohawk", "royale_hat_horned_helmet", "royale_hat_horse", "royale_hat_king", "royale_hat_laser_ears", "royale_hat_laurel_wreath", "royale_hat_leaders", "royale_hat_lich_crown", "royale_hat_mini_crown", "royale_hat_oakbreaker_helmet", "royale_hat_odin_helmet", "royale_hat_of_liberty", "royale_hat_of_phoenixwarrior", "royale_hat_oriental_hairstyle", "royale_hat_pan", "royale_hat_pawn_cap", "royale_hat_pigtail_helmet", "royale_hat_pilgrim", "royale_hat_pirate", "royale_hat_pixel_hero_headband", "royale_hat_psionicradar", "royale_hat_queen", "royale_hat_rangar_helmet", "royale_hat_sheriff", "royale_hat_snakes", "royale_hat_Snowdrift", "royale_hat_snowman", "royale_hat_spec_headset", "royale_hat_spec_ops_mask", "royale_hat_sword_in_the_head", "royale_hat_tours", "royale_hat_tribe_leader", "royale_hat_unicorn", "royale_hat_ushanka", "royale_hat_welding_mask", "royale_hat_zombie_cap", "avatar_ programmer", "avatar_3catspammer", "avatar_aborigine", "avatar_absolute_champion", "avatar_advanced_soldier", "avatar_adventure_dog", "avatar_agent_p", "avatar_agent_r", "avatar_agent_s", "avatar_agent_u", "avatar_agent_x", "avatar_agressivebot", "jubilee_trail", "avatar_alchemist", "avatar_alien_santa", "avatar_alpha_squad", "avatar_amazing_robert_cat", "avatar_amazon", "avatar_angler", "avatar_animator", "avatar_anubis", "avatar_aphrodite", "avatar_ares", "avatar_arma_colonel", "avatar_author", "avatar_awakened_mummy", "avatar_bad_guy", "avatar_bad_santa", "avatar_balloonman", "avatar_bandits_leader", "avatar_barbarian", "avatar_benny", "avatar_BigB", "avatar_bizarre_macho", "avatar_blockcity_detective", "avatar_blockman", "avatar_blogger", "avatar_blogger_girl", "avatar_bloq_directors", "avatar_blot", "avatar_blue_soldier", "avatar_boy_with_duck", "avatar_brave_fireman", "avatar_brave_knight", "avatar_brave_slayer", "avatar_bravo_squad", "avatar_breadboy", "avatar_bug_catcher", "avatar_camp_counselor", "avatar_campaign_girl", "avatar_captain_pixelgun", "avatar_caramel", "avatar_carrot", "avatar_cat_admirer", "avatar_cat_in_boots", "avatar_chicken_667", "avatar_chosen_crystal", "avatar_chronos", "avatar_chubby_viking", "avatar_city_fighter", "avatar_clan_legend", "avatar_colonist", "avatar_colonist_2", "avatar_community_manager", "avatar_confident_researcher", "avatar_cosmus", "avatar_crab", "avatar_cursed_pharaoh", "avatar_cyber_citizen", "avatar_cyber_girl", "avatar_cyber_hare", "avatar_cybercop", "avatar_cyborg_boar", "avatar_cyborg_rhino", "avatar_cyclop", "avatar_dance_queen", "avatar_daugter", "avatar_deep_monster", "avatar_digital_gambler", "avatar_digital_hero", "avatar_diligent_novice", "avatar_dolphin_willy", "avatar_don", "avatar_donny_dark", "avatar_dons_hand", "avatar_dr_plague", "avatar_dragon_child", "avatar_dragoon", "avatar_drilling_drone", "avatar_druid", "avatar_dwarves_king", "avatar_eared_spec_forces", "avatar_eirik_oakbreaker", "avatar_elite_marine_ops", "avatar_elite_police_drone", "avatar_elven_prince", "avatar_enchanted_nutcracker", "avatar_expert_scout", "avatar_far_future_loner", "avatar_fearless_nun", "avatar_fenrir", "avatar_final_boss", "avatar_firefly", "avatar_flat_boy", "avatar_flory", "avatar_fluff", "avatar_forgotten_astronaut", "avatar_friendlybot", "avatar_froggy", "avatar_frozen", "avatar_future_ninja", "avatar_fx_master", "avatar_game_designer", "avatar_gamer_samurai", "avatar_general_bullet", "avatar_general_green", "avatar_ghost_designer", "avatar_ghost_in_shell", "avatar_gingerbread", "avatar_glitch_player", "avatar_golden_ra", "avatar_goliath", "avatar_good_yeti", "avatar_graph_blockula", "avatar_greensoldier", "avatar_grumbler", "avatar_guardian", "avatar_gum_beast", "avatar_guns_bunny", "avatar_gunslinger", "avatar_halfmetal_mechanist", "avatar_hater", "avatar_head_of_academy", "avatar_heavy_dive_suit", "avatar_heavy_siege_robot", "avatar_helga_the_breaker", "avatar_helicopter_pilot", "avatar_hercules", "avatar_hermes", "avatar_hooligan_girl", "avatar_humpty_dumpty", "avatar_ice_dragon_adept", "avatar_ice_dragon_shaman", "avatar_ice_titan", "avatar_igor", "avatar_impositor_black", "avatar_impositor_blue", "avatar_impositor_cyan", "avatar_impositor_green", "avatar_impositor_orange", "avatar_impositor_pink", "avatar_impositor_red", "avatar_impositor_violet", "avatar_impositor_white", "avatar_impositor_yellow", "avatar_indian_boy", "avatar_indian_girl", "avatar_infected_scientist", "avatar_infection_fighter", "avatar_insidious_hades", "avatar_iron_warrior", "avatar_jason_drill", "avatar_joystick", "avatar_kartoszewski", "avatar_katty_pop", "avatar_khepri", "avatar_king", "avatar_king_of_bros", "avatar_knight", "avatar_knitteddoll", "avatar_kraken_cult_knight", "avatar_krampus", "avatar_lab_prototype", "avatar_lava_titan", "avatar_lee_gun", "avatar_legendary_captain", "avatar_legionary", "avatar_log", "avatar_loki", "avatar_mad", "avatar_mafia_head", "avatar_magical_rabbit", "avatar_mayor", "avatar_mech_pilot_exo1", "avatar_mech_pilot_exo2", "avatar_med_paramedic", "avatar_mermaid_princess", "avatar_mikhail_steel", "avatar_minotaurus", "avatar_modern_ninja", "avatar_mouse_king", "avatar_mr_bite", "avatar_mr_bucket", "avatar_mr_streamer", "avatar_mr_ultimatum", "avatar_ms_doc", "avatar_mysterious_warden", "avatar_mystic_merchant", "avatar_navy_tactical_suit", "avatar_neon", "avatar_netrunner", "avatar_nimble_viking", "avatar_ninja_fox", "avatar_ninja_robot", "avatar_noob", "avatar_novice", "avatar_oak_bot", "avatar_obscurant", "avatar_odin", "avatar_oldrobot", "avatar_orange", "avatar_orc", "avatar_pawn", "avatar_pga_goo", "avatar_phoenix_warrior", "avatar_pi_xel", "avatar_pilgrim_boy", "avatar_pilgrim_girl", "avatar_pirqates_queen", "avatar_pixel_cola_mascot", "avatar_player_1", "avatar_player_2", "avatar_polar_archaeologist", "avatar_policeman", "avatar_portalius", "avatar_professor_stone", "avatar_project_manager", "avatar_prospector", "avatar_pyramid_builder", "avatar_queen", "avatar_radioactive_girl", "avatar_raptor", "avatar_red_skull", "avatar_red_soldier", "avatar_redneck", "avatar_redskin", "avatar_rescuer", "avatar_robo_elf", "avatar_robotic", "avatar_robotron", "avatar_rock_titan", "avatar_rocket_jumper", "avatar_rocksy", "avatar_rook", "avatar_rubus", "avatar_ruin_demon", "avatar_runaway_statue", "avatar_same_prince", "avatar_santa_helper", "avatar_scarab_priest", "avatar_scout", "avatar_scuba_diving_scout", "avatar_secret_character", "avatar_flying_dutchman", "avatar_secret_mech_prototype", "avatar_sharko", "avatar_sheriff", "avatar_shining_moon", "avatar_shiny", "avatar_silent_survivor", "avatar_skeleton_pirate", "avatar_skeptic_boy", "avatar_ski_girl", "avatar_soundman", "avatar_spark", "avatar_spec_ops", "avatar_spec_ops_boss", "avatar_spec_ops_boy", "avatar_spec_ops_girl", "avatar_spec_ops_tactical_mech", "avatar_spiky", "avatar_steel_man", "avatar_strange_creature", "avatar_street_king", "avatar_strongman", "avatar_summer_spirit", "avatar_sun", "avatar_super_mutant", "avatar_super_portalius", "avatar_super_pro", "avatar_super_star_girl", "avatar_supreme_lich", "avatar_supreme_prophet", "avatar_supreme_space_parasite", "avatar_thor", "avatar_treasure_raider", "avatar_triceratops", "avatar_turkey_guy", "avatar_twinkle", "avatar_undercover_agent", "avatar_vacation_girl", "avatar_valkyrie", "avatar_vampire_hunter", "avatar_snow_queen", "avatar_virtual_blogger", "avatar_water_warrior", "avatar_werewolf", "avatar_winter_boy", "avatar_winter_girl", "avatar_wooden", "avatar_yakuza", "avatar_yellowsoldier", "avatar_zeus", "avatar_zombie_clown", "avatar_zombie_dude", "avatar_zombified_alien", "avatar_еmperor_son", "aaaaa_trail", "air_trail", "ancient_ice_trail", "balloons_trail", "bats_trail", "beautiful_shells_trail", "berserker_trail", "block_trail", "bloq_commercial_trail", "bombs_trail", "bugs_trail", "bullet_trail", "buttons_trail", "candies_trail", "carrots_trail", "cartoon_clouds_trail", "cartoon_start_trail", "checkerboard_trail", "chess_trail", "clan_trail", "colorful_smoke_trail", "comet_trail", "confetti_trail", "decorated_feathers_trail", "digits_trail", "dislikes_trail", "disputes_trail", "drill_trail", "dust_trail", "energetic_trail", "extinguiser_trail", "fast_furious_trail", "fiery_trail", "firecrackers_trail", "firework_trail", "flashlights_trail", "glitch_trail_trail", "goo_trail", "graphic_trail", "gravilith_trail", "gum_trail", "holy_light_trail", "ice_trail", "lava_lamp_trail", "leaf_trail", "lemonade_bubbles_trail", "likes_trail", "love_trail", "mafia_trail", "magic_trail", "marshmallow_trail", "menacing_trail", "merchant_trail", "mini_helicopters_trail", "money_trail", "moonlight_trail", "mutated_dna_trail", "mystery_notes_trail", "neon_trail", "new_ear_trail", "old_scrolls_trail", "ominous_trail", "ovum_trail", "pixel_cola_trail", "plague_trail", "poisonous_trail", "poisonous_vines_trail", "police_alert_hologram_trail", "power_up_trail", "quick_feathers_trail", "rainbow_trail", "rocket_pursuit_trail", "rocks_trail", "rune_trail_trail", "scout_trail", "secret_documents_trail", "shiny_mutagen_trail", "signal_trail", "sinister_tentacles_trail", "snow_trail", "snowballs_trail", "snowflakes_trail", "sos_trail", "sparkling_stardust_trail", "speed_up_trail", "star_trail", "stardust_trail", "storm_trail", "storm_vortex_trail", "strange_potion_trail", "summer_flowers_trail", "sun_rays_trail", "sunny_trail", "ultimatum_trail_trail", "vampire_trail", "virtual_trail", "virus_trail", "volcano_trail", "water_trail", "wedding_rings_trail", "zeus_wrath_trail", "сurvature_of_space_trail", "black_bullet_car", "royale_car_all_terrain_vehicle", "royale_car_armored_stryker", "royale_car_basic", "royale_car_battle_transport", "royale_car_block", "royale_car_blocks_transport", "royale_car_blogger", "royale_car_bugs_harvester", "royale_car_cyber_cop_car", "royale_car_duck", "royale_car_heavy_rover", "royale_car_irriadiated_taxi", "royale_car_lands_walker", "royale_car_laser_bunny", "royale_car_mining_transport", "royale_car_motorcycle_phoenix", "royale_car_mutated_limps", "royale_car_passenger_beetle", "royale_car_patrol", "royale_car_quicksilver", "royale_car_robot_carrier", "royale_car_secret_explorer", "royale_car_siege_ram", "royale_car_space_traveller", "royale_car_steam", "royale_car_stonecutter", "royale_car_summer_van", "royale_car_survivors", "royale_car_sweeper_z", "royale_car_train", "royale_car_trojan_horse", "royale_car_winter_carriage", "royale_car_yorkshire_buggy", "royale_road_warrior", "royale_snowmobile", "glider_poison_bubble", "avatar_irradiated_pixelman", "avatar_squid_001", "avatar_squid_002", "avatar_squid_004", "avatar_squid_005", "avatar_squid_003", "avatar_front_man", "avatar_security_circle", "avatar_security_square", "avatar_security_triangle", "brave_priest", "glider_chainsaw", "avatar_brave_priest", "royale_car_antimutant_bus", "avatar_sara_zoom", "avatar_viktor_grom", "avatar_madcap_girlie", "avatar_desperate_lab_assistant", "glider_antidote_spray", "barbed_wire_trail", "super_antidote_trail", "glider_tamed_zombolisk", "avatar_ice_lich_king", "helicopter_military", "royale_atv", "avatar_divided_santa", "avatar_treasure_warden", "avatar_the_ancient_god", "avatar_quirky_boy", "avatar_wildwood_creature", "avatar_mystical_fairy", "hare_firefly_trail", "mirrored_night_trail", "avatar_the_mirror", "avatar_forest_guardian", "avatar_summoned_snowman", "avarar_krampus_elf", "avatar_dazzling_moth", "avatar_dazzling_sage", "avatar_krampus_elf", "royale_car_santas_autonomic_sleigh", "glider_cristmas_keeper", "glider_butterfly_of_light", "royale_car_mysterious_carriage", "glider_reality_border", "royale_snowmobile_freeplay", "fps_all_terrain_vehicle", "glider_handmade", "glider_tranquility_stone", "avatar_armed_assistant", "avatar_deft_shinobi", "avatar_elementals_challenger", "avatar_inquisitive_scientists", "avatar_mighty_shaman", "avatar_red_pan_dah", "avatar_wise_shogun", "sakura_flowers_trail", "shamanic_symbols_trail", "royale_car_village_keeper", "royale_car_floating_water_lily", "avatar_storms_overlord", "glider_ancient_dragon_spirit", "glider_loaded_koi", "super_bowl_player", "avatar_super_bowl_player", "royale_amphibian", "glider_executers_high_end", "glider_extraterrestrial_hoverboard", "glider_good_ol_plane", "avatar_aliens_executioner", "avatar_bubble_queen", "avatar_good_ol_bomber", "avatar_royal_confidant", "avatar_the_chosen_one", "avatar_the_empress", "executed_aliens_storage_trail", "pg_is_love_trail", "royale_car_all_terrain_tank", "avatar_goblin_rogue", "avatar_sun_elf", "glider_skyfly_3000", "avatar_troll_warchief", "deadly_plague_trail", "spheres_of_power_trail", "royale_car_flamy_racer", "avatar_moon_elf", "avatar_edelonne_the_undead", "glider_armored_flyer", "avatar_light_paladin", "glider_death_herald", "avatar_crystal_golem", "avatar_living_armor", "avatar_moon", "avatar_neptune", "avatar_earth", "glider_sunstrider", "earth_atmosphere_trail", "neptune_ices_trail", "glider_terra_board", "avatar_the_sun", "avatar_the_everything", "avatar_the_shadow", "avatar_junior_cult_member", "avatar_dark_woods_spirit", "avatar_exorcist", "royale_car_moonstrider", "royale_hat_lich_crown_preview", "glider_yellow_duck", "avatar_maniac_artist", "avatar_mr_monke", "avatar_sad_sally", "avatar_yellow_slicker", "dyed_hares_trail", "royale_car_vacations_minivan", "glider_ramen_noodle", "glider_mad_dyer", "tamagochi_trail", "avatar_the_empress" };
const char* skinList[] = { "Weapon1018_black_friday", "Weapon1018_climber", "Weapon1018_default", "Weapon1032_default", "Weapon1032_ice_serpent", "Weapon1032_orobro", "Weapon1041_default", "Weapon1041_yeti", "Weapon1044_default", "Weapon1044_pixel_cola", "Weapon1057_default", "Weapon1057_pumpking_rabbit", "Weapon1057_silver_wolf", "Weapon1063_default", "Weapon1063_hallorifle", "Weapon1063_mini_game", "Weapon1065_default", "Weapon1065_flammable_backpack", "Weapon1068_default", "Weapon1068_digital_pumpkin", "Weapon1069_crystal_cycler", "Weapon1069_default", "Weapon1069_demons_soul", "Weapon1077_default", "Weapon1077_pro_version", "Weapon1087_caution_wild_deer", "Weapon1087_default", "Weapon1089_default", "Weapon1089_north_pole_crusader", "Weapon1110_default", "Weapon1110_spooktober", "Weapon1111_default", "Weapon1111_poison_vein", "Weapon1112_default", "Weapon1112_zombie", "Weapon1125_default", "Weapon1125_ice_dragon_cult", "Weapon1135_default", "Weapon1135_three_eyed_pumpking", "Weapon1137_default", "Weapon1137_love_implant", "Weapon1144_default", "Weapon1144_santa_pew", "Weapon1146_default", "Weapon1146_monster_slimethrower", "Weapon1153_default", "Weapon1153_neon_christmas", "Weapon115_default", "Weapon115_Gold", "Weapon1167_default", "Weapon1167_soul_catcher", "Weapon1169_default", "Weapon1169_winter_fox", "Weapon1172_default", "Weapon1172_pumpkin_shogun", "Weapon1172_soul_of_winter", "Weapon1177_default", "Weapon1177_holy_punch", "Weapon1177_millitary_supply", "Weapon1195_default", "Weapon1195_icicle", "Weapon1195_living_specimen", "Weapon1203_broken_heart", "Weapon1203_default", "Weapon1203_ice_dragon_cult", "Weapon1208_default", "Weapon1208_happy_deers", "Weapon1209_default", "Weapon1209_mecha", "Weapon1210_black_dragon_bite", "Weapon1210_default", "Weapon1214_default", "Weapon1214_сold_breath", "Weapon1216_default", "Weapon1216_grizzly", "Weapon1224_default", "Weapon1224_mecha", "Weapon1231_default", "Weapon1231_jolly", "Weapon1232_santa_list", "Weapon1234_default", "Weapon1234_spooky_hunter", "Weapon1254_default", "Weapon1254_millitary_supply", "Weapon1264_default", "Weapon1264_irradiated", "Weapon1265_default", "Weapon1265_scary_news", "Weapon1269_default", "Weapon1269_mecha", "Weapon127_default", "Weapon127_Gold", "Weapon1284_default", "Weapon1284_night_tempest", "Weapon1286_cristmas_cook", "Weapon1286_default", "Weapon1287_default", "Weapon1287_goth", "Weapon1287_mecha", "Weapon1290_christmas_light", "Weapon1290_default", "Weapon1294_default", "Weapon1294_sckeleton", "Weapon1300_bloody_order", "Weapon1300_default", "Weapon1305_default", "Weapon1305_vummy_again", "Weapon1323_brain_eater", "Weapon1323_default", "Weapon1338_default", "Weapon1338_millitary_supply", "Weapon133_default", "Weapon133_Gold", "Weapon1341_default", "Weapon1341_demon_eye", "Weapon1355_default", "Weapon1355_irradiated", "Weapon1407_default", "Weapon1407_winter_holiday", "Weapon1411_default", "Weapon1411_nuclear_reactor", "Weapon1416_crystal_dislike", "Weapon1416_default", "Weapon1416_nuclear_reactor", "Weapon1422_default", "Weapon1422_santas_deer", "Weapon1428_default", "Weapon1428_сarousel", "Weapon1434_default", "Weapon1434_mummy", "Weapon1435_default", "Weapon1435_frozen_king", "Weapon1448_christmas_tree", "Weapon1448_default", "Weapon1449_default", "Weapon1449_frozen", "Weapon1455_default", "Weapon1455_train", "Weapon1462_antidote", "Weapon1462_default", "Weapon1478_default", "Weapon1478_rafflesia", "Weapon1490_default", "Weapon1490_one_aloner", "Weapon1492_default", "Weapon1492_surgeon", "Weapon1493_default", "Weapon1493_pixel_cola", "Weapon1505_default", "Weapon1505_eternal_night", "Weapon1506_default", "Weapon1506_princess", "Weapon1533_bloody_web", "Weapon1533_default", "Weapon1534_ancient_dragon", "Weapon1534_default", "Weapon1546_altered_ak_5_56", "Weapon1546_default", "Weapon1547_altered_disguised_weapon", "Weapon1547_default", "Weapon1555_default", "Weapon1555_destruction_preader", "Weapon1556_black_dragon_curse", "Weapon1556_default", "Weapon17_altered_fast_death", "Weapon17_default", "Weapon1_default", "Weapon1_halloween", "Weapon220_default", "Weapon220_fishbone", "Weapon220_Gold", "Weapon220_neon", "Weapon221_alligator", "Weapon221_default", "Weapon221_techno", "Weapon224_default", "Weapon224_Gold", "Weapon224_overhited", "Weapon224_transformer", "Weapon25_default", "Weapon25_mythical_power", "Weapon281_bone", "Weapon281_cyborg", "Weapon281_default", "Weapon281_green_dragon", "Weapon281_mythical_power", "Weapon293_blocks", "Weapon293_default", "Weapon293_portalium_blaster", "Weapon293_reactor", "Weapon308_default", "Weapon308_Gold", "Weapon339_cyber_necromancy", "Weapon339_default", "Weapon339_medieval", "Weapon339_paladin", "Weapon345_altered_space_rifle", "Weapon345_default", "Weapon374_default", "Weapon374_Gold", "Weapon386_default", "Weapon386_ice_dragon_cult", "Weapon392_default", "Weapon392_mythical_power", "Weapon399_default", "Weapon399_emperor", "Weapon399_modified", "Weapon399_runes", "Weapon411_black_friday", "Weapon411_default", "Weapon411_insidious", "Weapon413_default", "Weapon413_Gold", "Weapon421_default", "Weapon421_Gold", "Weapon438_default", "Weapon438_Gold", "Weapon444_Clown", "Weapon444_default", "Weapon444_Gold", "Weapon444_RedSkull", "Weapon533_default", "Weapon533_Resort", "Weapon544_default", "Weapon544_mythical_power", "Weapon552_default", "Weapon552_mythical_power", "Weapon564_Chrome", "Weapon564_default", "Weapon564_Predator", "Weapon564_Tropic", "Weapon613_default", "Weapon613_enchanted", "Weapon613_from_the_ruins", "Weapon613_Gold", "Weapon613_neon", "Weapon613_reactor", "Weapon615_default", "Weapon615_jack_the _umpkin", "Weapon625_chamomile", "Weapon625_default", "Weapon625_hype", "Weapon625_samurai", "Weapon640_default", "Weapon640_icicle", "Weapon640_Monster", "Weapon640_Techno", "Weapon654_default", "Weapon654_frozen_candy", "Weapon65_default", "Weapon65_one_dragon_story", "Weapon667_black_friday", "Weapon667_default", "Weapon679_default", "Weapon679_spooky_laser", "Weapon681_default", "Weapon681_military", "Weapon681_relic", "Weapon681_severe_caramel", "Weapon681_supercharged", "Weapon687_crystal_anti_champion", "Weapon687_decorated_pruce", "Weapon687_default", "Weapon687_Neon", "Weapon687_Steampunk", "Weapon687_Watermelon", "Weapon688_altered_champion_solar_cannon", "Weapon688_default", "Weapon688_electro_pine", "Weapon707_default", "Weapon707_jack_the_orac", "Weapon711_default", "Weapon711_santas_helper", "Weapon712_default", "Weapon712_old_clock", "Weapon714_bad_elf", "Weapon714_default", "Weapon720_default", "Weapon720_Gold", "Weapon721_black_dragon_revolver", "Weapon721_default", "Weapon725_default", "Weapon725_Gold", "Weapon725_laser", "Weapon725_lava_spirit", "Weapon725_necromancer", "Weapon727_default", "Weapon727_Gold", "Weapon737_burning", "Weapon737_default", "Weapon737_druid", "Weapon737_Scarab", "Weapon751_default", "Weapon751_ice_dragon_cult", "Weapon752_cosmus", "Weapon752_default", "Weapon752_prehistoric", "Weapon755_Bombardier", "Weapon755_default", "Weapon759_Code", "Weapon759_default", "Weapon759_Mutant", "Weapon764_default", "Weapon764_Gold", "Weapon769_default", "Weapon769_dragons_fury", "Weapon770_default", "Weapon770_ice_dragon_cult", "Weapon778_default", "Weapon778_packed", "Weapon77_default", "Weapon77_Gold", "Weapon77_mythical_power", "Weapon782_candy_factory", "Weapon782_default", "Weapon782_snowman", "Weapon790_default", "Weapon790_match_inviter", "Weapon791_default", "Weapon791_super_bowl_fan", "Weapon792_default", "Weapon792_super_bowl_cannon", "Weapon795_dead_eye", "Weapon795_default", "Weapon795_winter_rune", "Weapon809_default", "Weapon809_xmas_energy", "Weapon811_default", "Weapon811_diner", "Weapon811_snowman", "Weapon82_cheese", "Weapon82_chess", "Weapon82_default", "Weapon82_egypt", "Weapon834_christmas", "Weapon834_default", "Weapon834_nuclear_reactor", "Weapon834_pumpkin_destoyer", "Weapon865_default", "Weapon865_toy", "Weapon895_default", "Weapon895_krampus_soul", "Weapon895_pumpking_king", "Weapon895_robot_head", "Weapon898_black_armor_stitcher", "Weapon898_default", "Weapon906_default", "Weapon906_snow_blower", "Weapon916_asimov", "Weapon916_default", "Weapon916_frosty", "Weapon917_asimov", "Weapon917_default", "Weapon918_asimov", "Weapon918_default", "Weapon918_snow_camouflage", "Weapon925_black_friday", "Weapon925_default", "Weapon928_black_friday", "Weapon928_default", "Weapon949_default", "Weapon949_monstrous_harpy", "Weapon94_black_friday", "Weapon94_default", "Weapon94_giant_bell", "Weapon94_SpacePirate", "Weapon94_Volcano", "Weapon954_default", "Weapon954_fireplace", "Weapon954_hell_oven", "Weapon957_default", "Weapon957_fortune", "Weapon957_poison", "Weapon968_default", "Weapon968_ice_dragon_cult", "Weapon972_altered_rainbow_destroyer", "Weapon972_default", "Weapon977_default", "Weapon977_nothern_lights", "Weapon982_default", "Weapon982_scourge_of_the_king", "Weapon987_default", "Weapon987_mecha", "Weapon991_default", "Weapon991_fenfir", "Weapon999_default", "Weapon999_dual_xmas", "Weapon999_millitary_supply", "Weapon1564_default", "Weapon1564_gemini_constellation", "Weapon447_aries_constellation", "Weapon447_default", "Weapon1046_default", "Weapon1046_pisces_constellation", "Weapon222_default", "Weapon222_sagittarius_constellation", "Weapon1565_default", "Weapon1565_libra_constellation", "Weapon1440_default", "Weapon1440_ultrasounding_rifle", "Weapon1000_brutal_sounder", "Weapon1000_default", "Weapon706_default", "Weapon706_pband_beat", "Weapon189_default", "Weapon189_true_fan_shotgun", "Weapon669_pband_fans_sword", "Weapon669_default", "Weapon1069_legendary_bike", "Weapon1374_default", "Weapon1374_dragon_of_doom", "Weapon1544_default", "Weapon1544_fire_team_no1", "Weapon1356_default", "Weapon1356_sakura", "Weapon1580_default", "Weapon1580_dragons_wrath", "Weapon1588_corrupred_raven_staff", "Weapon1588_default", "Weapon1587_corrupted_sorting_pistol", "Weapon1587_default", "Weapon1021_default", "Weapon1021_enchanted_healing_staff", "Weapon1109_default", "Weapon1109_enchanted_snow_queen_wand", "Weapon132_default", "Weapon132_enchanted_dark_mage_wand", "Weapon1596_cosmic_seas_navigation_shotgun", "Weapon1596_default", "Weapon419_deepwater_dead_star", "Weapon419_default", "Weapon116_deepwater_scythe", "Weapon116_default", "Weapon1595_cosmic_seas_navigation_shotgun", "Weapon1595_default", "Weapon893_deepwater_comet", "Weapon893_default", "Weapon1606_default", "Weapon1606_iced_stars", "Weapon1338_christmas_ronin", "Weapon1607_default", "Weapon1607_winter_curse", "Weapon778_royal_christmas_revolver", "Weapon1259_bad_vibes_neutralizer", "Weapon1259_default", "Weapon1197_default", "Weapon1197_harsh_snowfall", "Weapon834_ultimative_snowfall", "Weapon1196_advanced_snowfall", "Weapon1196_default", "Weapon1200_bullet_snowfall", "Weapon1200_default", "Weapon1618_deadly_butterfly", "Weapon1618_default", "Weapon1619_ancient_warrior", "Weapon1619_default", "Weapon1628_default", "Weapon1628_spring_ritual", "Weapon1200_default" };
const char* capeList[] = { "cape_Custom", "cape_EliteCrafter", "StormTrooperCape_Up1", "StormTrooperCape_Up2", "cape_Archimage", "HitmanCape_Up1", "HitmanCape_Up2", "cape_BloodyDemon", "BerserkCape_Up1", "BerserkCape_Up2", "cape_Engineer", "cape_Engineer_Up1", "cape_Engineer_Up2", "cape_SkeletonLord", "SniperCape_Up1", "SniperCape_Up2", "cape_cloak_of_night", "cape_ultimate_defense", "cape_vaneuvering_module", "cape_RoyalKnight", "DemolitionCape_Up1", "DemolitionCape_Up2"};
const char* bootsList[] = { "boots_gray", "StormTrooperBoots_Up1", "StormTrooperBoots_Up2", "boots_red", "HitmanBoots_Up1", "HitmanBoots_Up2", "boots_black", "BerserkBoots_Up1", "BerserkBoots_Up2", "EngineerBoots", "EngineerBoots_Up1", "EngineerBoots_Up2", "boots_blue", "SniperBoots_Up1", "SniperBoots_Up2", "boots_green", "DemolitionBoots_Up1", "DemolitionBoots_Up2", "boots_tabi", "boots_cyber_santa", "boots_cyber_santa_Up1", "boots_cyber_santa_Up2", "boots_exoskeleton_pilot_avatar", "boots_exoskeleton_pilot_avatar_up1", "boots_exoskeleton_pilot_avatar_up2", "boots_black_dragon", "boots_halloween", "boots_halloween_up1", "boots_halloween_up2", "boots_roller_skates", "boots_stardust", "boots_ultimate_defense", "boots_piligrim", "boots_piligrim_up1", "boots_piligrim_up2"};
const char* hatList[] = { "league1_hat_hitman", "league2_hat_cowboyhat", "league3_hat_afro", "league4_hat_mushroom", "league5_hat_brain", "league6_hat_tiara", "hat_Adamant_3", "hat_Headphones", "hat_black_dragon", "hat_chest", "hat_KingsCrown", "hat_stardust", "hat_Samurai", "hat_DiamondHelmet", "hat_SeriousManHat", "hat_cyber_santa", "hat_cyber_santa_Up1", "hat_cyber_santa_Up2", "hat_exoskeleton_pilot_avatar", "hat_exoskeleton_pilot_avatar_up1", "hat_exoskeleton_pilot_avatar_up2", "hat_halloween", "hat_halloween_up1", "hat_halloween_up2", "hat_piligrim", "hat_piligrim_up1", "hat_piligrim_up2", "hat_high_end_earphones" };
const char* maskList[] = { "mask_trooper","mask_trooper_up1","mask_trooper_up2","mask_hitman_1","mask_hitman_1_up1","mask_hitman_1_up2","mask_berserk","mask_berserk_up1","mask_berserk_up2","mask_engineer","mask_engineer_up1","mask_engineer_up2","mask_sniper","mask_sniper_up1","mask_sniper_up2","mask_demolition","mask_demolition_up1","mask_demolition_up2","hat_ManiacMask","mask_cyber_santa","mask_cyber_santa_Up1","mask_cyber_santa_Up2","mask_exoskeleton_pilot_avatar","mask_exoskeleton_pilot_avatar_up1", "mask_exoskeleton_pilot_avatar_up2", "mask_halloween", "mask_halloween_up1", "mask_halloween_up2", "mask_black_dragon", "mask_doctor_plague", "mask_medical_mask", "mask_turkey_piligrim", "mask_turkey_piligrim_up1", "mask_turkey_piligrim_up2", "mask_ultimate_defense", "mask_anime_glasses", "mask_smart_bullet_bazooka", "mask_stop_posting_hepi"};
const char* armorList[] = { "Armor_Adamant_Const_2", "Armor_Almaz_3", "Armor_Army_1", "Armor_Army_3", "armor_black_dragon", "armor_clan_golden", "armor_clan_steel", "armor_clan_wooden", "armor_crystal_clan", "armor_dope_windcheater", "Armor_Halloween", "armor_inflatable", "Armor_Royal_3", "Armor_Rubin_3", "armor_stardust", "Armor_Steel_3", "armor_ultimate_defense", "cyber_santa_armor", "developer_armor", "exoskeleton_pilot_avatar_armor", "heavy_champion_armor", "pilgrim_armor", "starter_pack_armor" };
const char* wepList[] = { "FirstPistol", "FirstShotgun", "UziWeapon", "Revolver", "Machingun", "MinersWeapon", "AK47", "Knife", "m16", "Eagle 1", "SPAS", "FAMAS", "Glock", "Shovel", "Hammer", "Beretta", "IronSword", "SteelAxe", "WoodenBow", "Chainsaw 2", "SteelCrossbow", "Hammer 2", "Mace 2", "Staff 2", "DoubleShotgun", "AlienGun", "m16_2", "Tree", "Fire_Axe", "SVD", "Barrett_2", "Uzi2", "Hunter_Rifle", "Bazooka", "railgun", "tesla", "grenade_launcher", "grenade_launcher2", "Bazooka_3", "GravityGun", "AUG", "katana_3", "bigbuddy", "Mauser", "Shmaiser", "Tompson", "Tompson_2", "basscannon", "SparklyBlaster", "CherryGun", "AK74", "FreezeGun", "3pl_Shotgun_3", "flowerpower_3", "Revolver4", "Scythe_3", "plazma_3", "plazma_pistol_2", "plazma_pistol_3", "Razer_3", "Flamethrower_3", "FreezeGun_0", "Minigun_3", "Barret_3", "LightSword_3", "Sword_2_3", "Staff 3", "DragonGun", "Bow_3", "Bazooka_2_3", "m79_3", "Red_Stone_3", "XM8_1", "PumpkinGun_1", "TwoBolters", "RayMinigun", "SignalPistol", "AutoShotgun", "TwoRevolvers", "SnowballGun", "SnowballMachingun", "HeavyShotgun", "Solar_Ray", "Water_Pistol", "Solar_Power_Cannon", "Water_Rifle", "Needle_Throw", "Valentine_Shotgun", "Carrot_Sword", "Easter_Bazooka", "RailRevolverBuy", "Assault_Machine_GunBuy", "Impulse_Sniper_RifleBuy", "Autoaim_RocketlauncherBuy", "PX-3000", "Sunrise", "Bastion", "DualHawks", "StormHammer", "Badcode_gun", "ElectroBlastRifle", "PlasmaShotgun", "Devostator", "Dark_Matter_Generator", "Hydra", "TacticalBow", "FutureRifle", "RapidFireRifle", "DualUzi", "LaserDiscThower", "Tesla_Cannon", "Photon_Pistol", "Fire_orb", "Hand_dragon", "Alligator", "Hippo", "Alien_Cannon", "Alien_Laser_Pistol", "Dater_Flowers", "Dater_DJ", "Dater_Arms", "Dater_Bow", "FriendsUzi", "Alien_rifle", "VACUUMIZER", "Fireworks_Launcher", "Shotgun_Pistol", "Range_Rifle", "Pit_Bull", "Tiger_gun", "Balloon_Cannon", "Mech_heavy_rifle", "Shuriken_Thrower", "BASIC_FLAMETHROWER", "snowball", "MysticOreEmitter", "Laser_Crossbow", "Nutcracker", "SPACE_RIFLE", "Icicle_Generator", "PORTABLE_DEATH_MOON", "Candy_Baton", "Hockey_stick", "Space_blaster", "mp5_gold_gift", "Dynamite_Gun_1", "Dual_shotguns_1", "Antihero_Rifle_1" ,"HarpoonGun_1", "Red_twins_pistols_1", "Toxic_sniper_rifle_1", "NuclearRevolver_1", "NAIL_MINIGUN_1", "DUAL_MACHETE_1", "Fighter_1", "Gas_spreader", "LaserBouncer_1", "magicbook_fireball", "magicbook_frostbeam", "magicbook_thunder", "TurboPistols_1", "Laser_Bow_1", "loud_piggy", "Trapper_1", "chainsaw_sword_1", "dark_star", "toy_bomber", "zombie_head", "mr_squido", "RocketCrossbow", "spark_shark", "power_claw", "zombie_slayer", "AcidCannon", "frank_sheepone", "Ghost_Lantern", "Semiauto_sniper", "Chain_electro_cannon", "Barier_Generator", "Demoman", "charge_rifle", "minigun_pistol", "bad_doctor_1", "dual_laser_blasters", "toxic_bane", "Charge_Cannon", "Heavy_Shocker", "ruler_sword_1", "pencil_thrower_1", "napalm_cannon", "sword_of_shadows", "dracula", "xmas_destroyer", "santa_sword", "snow_storm", "heavy_gifter", "bell_revolver", "elfs_revenge", "photon_sniper_rifle", "subzero", "mercenary", "laser_spear", "nunchuks", "double_dragon", "magicbook_love", "casanova", "romeo_and_juliette", "Q_grenade_launcher", "Q_machine_gun", "Q_electrogun", "Q_railgun", "Q_rocket_launcher", "Q_shotgun", "Q_plasma_gun", "Q_chopper", "Q_BFG", "Blade_of_Justice", "lab_eight_barrel_shotgun", "Tactical_tomahawk", "Masterpiece_musket", "Executioner", "Serious_argument", "lab_piranha", "lab_oneshot", "lab_machinegun", "lab_combatyoyo", "lab_orbital_pistol", "Defender_of_the_law", "fidget_thrower", "festive_bazooka", "thunderer", "bomber_slingshot", "future_sniper_rifle", "beast", "invader", "core_sword", "hedgehog", "third_eye", "eraser", "excalibur", "poseidon_trident", "reaper", "void_ray_rifle", "black_mamba", "chip_sword", "vampire_hunter", "mountain_wolf", "hurricane", "dino_slayer", "soulstone", "electrosphere", "prototype_s", "frozen_dragon", "special_agent", "Power_Fists", "VerticalShotgun", "astral_bow", "zeus_chain_ sword", "music_lover", "veteran", "mini_alien_spaceship", "flag_keeper", "leaders_sword", "last_christmas", "laser_assistant", "dual_cryo_pistols", "automatic_decorator", "elder_force_saber", "Champion_Mercenary", "champion_peacemaker", "adamant_laser_cannon", "Adamant_Revolver", "adamant_spear", "champion_electric_arc", "adamant_sniper_rifle", "Anti-Champion_Rifle", "Champion_Solar_Cannon", "Adamant_Bomber", "sharp_snowflake", "little_cthulhu", "magicbook_curse_spell", "airblade_spell", "magicbook_poison_spell", "magicbook_earth_spikes_spell", "magicbook_shadow_spell", "Dragon_King", "angry_pumpkin", "Steam_Knuckes", "electro_blast_pistol", "wyvern_sniper", "hero", "sly_avenger", "bright_reflector", "sakura", "deadly_beat", "killer_whale", "renegade", "demon_sword", "bolters2", "Stormtrooper_Exoskeleton", "Sniper_Exoskeleton", "demolition_exosceleton", "hitman_exoskeleton", "berserk_exoskeleton", "engineer_exoskeleton", "death_claws", "Eva", "space_desinsector", "mega_gun", "laser_revolver", "Sword_and_Shield", "last_samurai", "manga_gun", "anime_schtye", "nitrogen_spreader", "future_police_rifle", "stakegun", "alive_blaster", "plasma_cutter", "good_doctor", "devastator", "supercharged_rifle", "archangel", "manticore", "soul_thief", "multitaskers", "yin_yang", "last_hope", "Jet_hammer", "energy_drill", "Spender", "graviton", "acid_pistol", "Rhino", "antique_pistol", "lava_thrower", "Inventors_Rifle", "vandals", "revolver_sniper_rifle", "captain", "void_energy_pistol", "destruction_system", "circus_cannon", "military_grenade_launcher", "cyborg_hands", "ritual_blade", "tactical_rifle", "sparkling_eel", "alien_sniper_rifle", "neutralizer", "bone_sniper_rifle", "prehistoric_shotgun", "shamans_bow", "poison_darts", "primal_beast", "eye_of_ra", "gym_rifle", "bone_crusher", "soda_launcher", "rocket_launcher", "royale_fighter", "royale_revolver", "royale_sniper_rifle", "royale_ashbringer", "dagger_shovels", "worker_shovel", "sniper_shovel", "gold_shovel", "jet_shovel", "shover_laser", "monster_shovel", "penilizer", "fan_revolver", "football_cannon", "shovel_axe", "cursed_pirate", "Viking", "gladiator", "heart_of_volcano", "shovel_not_a_shovel", "chainsaw_shovel", "clan_legend_shovel", "shovel_road_sign", "cactus_shovel", "toy_hammer", "igniter", "infiltrator", "warmaster", "combat_slinger", "paintball_rifle", "energy_rings_exoskeleton", "particle_accelerator", "RitualRevolvers", "magicbook_Spell_of_Bats", "deadmans_rifle", "Witchunter", "industrial_nailer", "trick_and_treat", "ghost_launcher", "poisonous_vine", "johnny_p", "witch_broom", "cyber_slicer", "ricochet_rifle", "cyber_revolver", "neon_lighting", "quadruple_death", "cops_shovel", "cyber_shovel", "rocket_hand", "cyber_laser", "scull_crossbow", "shovel_hell", "sniper_cyber_module", "cops_club", "ultimatum", "lamp_revolver", "drum_rifle", "bp_flamethrower", "hand_gatling", "steamer", "gold_digger", "bow_of_spirit", "tomahawk_shovel", "sherif_shovel", "miner_shovel", "corn_launcher", "pie_thrower", "triple_musket", "demolition_gun", "ecco_hunter", "like_throver", "photo_gun", "liquid_fire", "electric_vortex", "cursed_staff", "blinder", "peppermint_guardians", "freon_gun", "shovel_ice", "frostomat", "frosty_railgun", "winter_staff", "ice_spiker", "shovel_peppermint", "total_annihilator", "torpedo_launcher", "sawed_off_shotguns", "gauss_machine_gun", "ultra_beam", "sock_bomber", "energy_assault_rifle", "shovel_alien_ice", "shovel_ice_skate", "futuristic_minigun", "sniper_ice", "gloves_of_power", "space_redeemer", "snow_blaster", "alien_bounser", "alien_shuriken", "spruce_pistols", "christmas_spirit", "ice_lightning", "ice_generator", "icecream_pistol", "whip_with_cake", "festive_guitar", "evil_snowman", "gangerbread_homezooka", "portable_gift_factory", "saturn", "venus_n_mercury", "Asteroid", "comet_sniper_rifle", "meteor_shower", "neclaces_ice_king", "Ice_Paws", "frozen_lich_bow", "gift_stitcher", "ice_king_anger", "gifted_revolvers", "yetti_spirit", "bambo", "new_year_defender", "candy_axe", "cold_silence", "proton_freezer", "coldstarter", "cyber_machinegun", "simple_cybergun", "shock", "cyber_sniper", "simple_cyberlauncher", "simple_acid_spray", "spec_ops_backup", "pixel_cola_refresher", "warden", "instigator", "storm", "semi_auto_shotgun", "stinger_3000", "airstrike_laptop", "deadly_digital_system", "night_hunter", "spec_ops_pistol", "tactical_knife", "hand_minigun", "heavy_sniper_rifle", "armadillo", "police_zapper", "temple_guardian", "healing_carp", "moon_halberd", "magic_primary", "magic_backup", "magic_melee", "magic_special", "magic_sniper", "royale_alpenstock", "agent_valentine", "lovebirds", "Serenade", "heartbreakers", "Succubus", "postman", "harpy", "runic_hammer", "blade_rifle", "arahna_bite", "medusa", "bigb_oven", "mr_mixer", "cookie_minigun", "magic_blades", "alchemic_staff", "rune_shovel", "сyclops_shovel", "system_admin", "flash_drive", "disconnector", "network_streamer", "аntivirus", "big_data", "cyclops_siling", "rainbow_dispenser", "rainbow_destroyer", "mister_l", "adamant_claws", "hypersound_uzi", "double_gum", "black_hole", "space_shovel", "heroic_shovel", "protector_of_peace", "portalius_wrath", "chipping_whip", "portalium_rifle", "nuclear_rifle", "deadly_umbrella", "big_bang_revolver", "acid_shotgun", "batohawk", "combat_gautlent", "spider_sense", "werewolf", "sharp_flame", "charged_injector", "garbage_rifle", "firefighter", "fireman_shovel", "huge_boy","fatal_melter", "dual_anger", "brutal_thinner", "liquidator_z1", "eliminator_z2", "pursuer_z3", "pixelgun_old", "plastic_shovel", "balloon_shovel", "block_rifle", "block_shotgun", "block_flamethrower", "plastic_rifle", "plastic_bazooka", "plastic_carabin", "cloud_launcher", "sucker_gun", "witch_doctor", "portal_cannon", "healing_staff", "laser_ray_rifle", "clockwork_cannon", "transformed_blaster", "transformed_machinegun", "witch_cauldron", "combat_darts", "throwing_spears", "alloha_boom", "professional_bow", "artifact", "ouroboros", "keen_courier", "uncertainty_emitter", "camp_leader_rifle", "knife_and_compass", "super_loudspeaker", "raccoon_with_trumpet", "butterfly_net_shovel", "guitar_shovel", "werewolf_paws", "ice_cream_thrower", "water_juice_cannon", "pop_soda", "pyro_1116", "sparkling_hydra", "tiny_resizer", "pawn_cannon", "horse_axe", "battle_rook", "inevitability", "chess_board_shovel", "royal_shovel", "checkmate", "proud_eagle", "sly_wolf", "brave_lion", "offensive_catapult", "battle_horn", "sharp_ring", "ornate_bagpipes", "pew_pew_rifle", "pixel_stream_sword", "tube_backpack", "supershot_rifle", "hyper_mega_cannon", "digital_sunrise", "lasercycler", "wave_pulser", "arcade_rifle", "arcade_flamethrower", "fatality_hook", "fireball_gauntlets", "anubis", "duck_hunter", "disc_battle_station", "pixel_shovel", "holy_shotgun", "aspen_spear", "dual_hunter_crossbows", "plague_purifier", "lives_oppressor", "sword_of_silence", "gravestone_shovel", "hot_greetings", "cooking_totem", "spiritual_rifle", "storm_all_seas", "huge_anchor_shovel", "hard_corn", "chef_fork_shovel", "lancelot", "galahad", "percival_n_lamorak", "mordred", "morgana", "bediver", "neutron_pulsator", "clapper_gun", "royal_marryer", "snowqueen_wand", "heroic_epee", "ice_club", "last_squeak", "fierce_pack", "mouse_scepter", "ice_chaser", "sneaky_revolver", "coal_frightener", "enchanted_butler_shovel", "golden_bros", "gangsta_shield", "double_cashback", "luxury_beats", "sniper_dude", "gems_power", "spirit_staff", "offensive_fireworks", "sharp_fans", "river_dragon", "explosive_lemonade", "stick_gong_shovel", "fire_spirit", "thunder_storm", "water_blades", "deadly_stings", "dragon_bite", "ancient_scroll", "potato_implant", "ai_pistol", "super_drill", "meteorite_fighter", "gravitator", "predatory_bush_shovel", "zap_blaster", "pew_blaster", "boom_blaster", "psionic_spores", "poisonous_grasshopper", "shrimposaur", "", "egg_colorizer", "hocus_pocus", "bunny_smasher", "a_killer", "carrot_crossbow", "festive_frying_pan_shovel", "carrot_on_stick_shovel", "watchy_snipy", "spiny_shooty", "pully_blasty", "chickoboom", "battle_grainthrowers", "last_cry", "", "dragon_emitter", "fate_fist", "demon_catcher", "aboard_puncher", "deadly_tale", "ninja_shovel", "moon_shovel", "sword_soul_card", "lance_soul_card", "bow_soul_card", "vertical_grip_device", "siege_mode_glove", "one_punch_glove", "treasurer", "merchant_shovel", "shell_bubbler", "tide_bracers", "call_of_sirens", "deep_ambusher", "cthulhu_legacy", "sunken_hook_shovel", "atlantis_shovel", "mooring_anchor", "heavy_drill_rifle", "rivet_gun", "submarine_drone_station", "echo_scanner", "underwater_carbine", "project_dolphin", "project_rapid_flock", "project_marlin", "advanced_scout_rifle", "harsh_punisher", "manual_inferno", "offensive_rocket_system", "headhunter_pistols", "thunderbolt", "spear_of_war", "aphrodite_crossbow", "winged_revolver", "ferocious_poleaxe", "hermes_rod_shovel", "hydra_shovel", "cerberus", "souls_conductor", "triple_bite", "wraith_of_fire", "earth_trembler", "primal_cold", "boar_roar", "gorgon_head", "invincible_lion", "debugger", "likes_booster", "ban_hammer", "vr_blasters", "killer_chord", "invigorating_awaker", "total_planner", "money_rain", "render_station", "gun_force", "digital_pad", "graphic_shovel", "cheater_hunter", "super_fixer", "bugs_cleaner", "code_shovel", "good_code", "ambush_crocorifle", "bubble_splasher", "disguised_rifle", "surf_daggers_shovel", "sand_shovel", "enchanting_altar", "spirit_mask", "magical_boomerang", "foam_sword", "sand_tower_pistol", "watermans_rifle", "high_pressure_rifle", "sharkado", "shower_sprinkler", "star_shard", "solar_flare", "magnetic_storm", "MonsterHunterMechBody1", "ectoplasmic_grenade_launcher", "reversed_pistol", "activity_neutralizer", "pga_mirror_shovel", "dimerian_shovel", "goo_substance_rifle", "goo_substance_pistol", "goo_substance_scythe", "pga_tv", "pga_doll", "pga_gates", "pga_living_rifle", "pga_slender_touch", "pga_screaming_saw", "atomic_splitter", "detimerian_divider", "restructurer", "anomaly_concentrator", "MonsterHunterMechBody2", "MonsterHunterMechBody3", "bloq_plasma_module", "plasma_beast", "drone_station", "arma_shovel", "police_alarm_shovel", "max_cruelty", "represser", "fear_and_espair", "rude_bully", "laser_ninja", "rebel", "mobile_outpost", "outlaw_catcher", "fair_sheriff", "minigun_cyber_module", "med_surgery_module", "supply_airdrop", "battle_falcon", "super_mega_tool", "pneumatic_needle_rifle", "summon_dagger", "melter_shovel", "pg3d_flag_shovel", "frozen_heart", "eye_of_the_storm", "frosty_fangs", "combat_candy", "ho_ho_rocket", "expllosive_caramel", "candy_revolver", "elven_killer_rifle", "assassin_daggers", "farewell_volley", "mystic_potion", "siege_crossbow", "storm_poleaxes", "prophet_staff", "hammer_of_thor", "thor_shovel", "carved_paddle_shovel", "geri_n_freki", "sleipnir", "huginn", "deceitful_cheater", "cunning_flame", "harbinger_of_ragnarok", "sun_eater", "call_of_valhalla", "winged_bow", "war_horse", "glitch_machine_gun", "ideal_rocket_launcher", "noob_rifle", "evil_rubiks_cube", "super_pixel_gun", "ultimatum_shovel", "final_sentence", "piercer", "border_eraser", "deminic_crusher", "dual_beam", "ronin", "brutal_rainbow", "tomianom", "time_scythe", "cyber_cat_lantern", "laser_cat_rifle", "bomber_cat", "lab_laser_shovel", "evidence_hunter", "patrol_officer_747", "redneck_bazooka", "mayor_trophy_revolver", "pixel_cola_cans", "city_key_shovel", "police_baton_shovel", "large_machinegun", "tactical_pistol", "mammoth", "debts_collector", "godfathers_assistant", "black_label", "plan_b", "traitor", "incinerator", "saboteur", "rock_smelter", "tunnel_riveter", "complete_grinder", "drilling_railgun", "plasmatic_digger", "drill_shovel", "portalius_shovel", "giant_flytrap_shovel", "portal_rifle", "extracting_ray_pistol", "terramorphing_stone", "aerosaur_rifle", "bone_saur_sword", "heavy_spik_saur", "ritual_rifle", "tooth_of_fate", "staff_crystals_will", "spark_horn", "adventure_swords", "flat_rifle", "secret_burger", "charles", "ink_spreader", "eraser_crossbow", "pencil_spears", "bro_steamer", "sad_magnet", "anvil_pistol", "masterly_trumpet", "blot_axe", "blot_shotgun", "blot_cannon", "adventure_shovel", "flat_shovel", "mr_shovel", "veteran_rifle", "veteran_sniper_rifle", "veteran_rocket_system", "lives_harvester", "jet_dragon", "thermal_scythe", "simple_dimple", "fx_camera", "dance_rifle", "rifle_me_online", "virtual_orchestra", "sharp_chaser", "zap_katana", "cyber_lantern", "bro_rifle", "ninja_swordpad", "hype_yo_yo", "streaming_beast", "dislike", "likes_absorber", "bad_comment", "golden_button_shovel", "pop_it_shovel", "mysterious_obelisk", "ritual_embalmer", "sacred_cat_will", "sentinel", "howl_of_the_mummy", "punishing_light", "piercing_gaze_of_ra", "eternal_suns_wrath", "fair_judge", "black_obelisk", "plaguebringer", "scarab_shrine", "disturbed_sarcophagus", "royal_cobra_spirit", "pharaon_3_breath", "sentinel_shopeish_shovel", "wand_of_summoning_shovel", "hand_mummy_shovel", "block_pyramid_shovel", "ultrasonic_rifle", "electronics_burner", "hounds_rx03", "system_scorpion", "rumble", "huge_wrench_shovel", "shovel_spear_shovel", "experimental_rifle", "jet_spear", "tactical_scorcher", "siege_minigun", "thermal_mecha_knife", "disruptive_howitzer", "anti_aircraft_module", "engineering_saw_thrower", "corpuscular_revolver", "uranium_grenade_launcher", "golden_eagles", "wayfarer", "fate", "holographic_cards", "chronos_shovel", "mutation_cannon", "additional_hand_shovel", "hungry_flower_shovel", "c4_basket", "head_of_statue", "tank_turret", "space_parasite_dreadnought", "space_parasite_fighter", "alien_parasite_mothership", "bad_clown", "mutant_weapon_one", "mutant_weapon_two", "splutter_weapon_one", "splutter_weapon_two", "jumper_weapon_one", "jumper_weapon_two", "hound_weapon_one", "thorn_weapon_one", "thorn_weapon_two", "supermutant_weapon_one", "disturbing_balloons", "clown_chainsaw", "irradiated_smg", "mutant_fish", "spiky_limbs", "predatory_rifle", "automatic_injector", "antidote_missile_launcher", "purifying_shotgun", "mutant_slayer", "mutagen_neutralizer", "antidote_dropper", "power_gauntlet", "class_bullet_rifle", "class_knight_hammer", "class_knight_shield", "class_valk_rocket", "class_lifeline_gun", "class_sara_sniper", "class_sara_shotgun", "class_saber_disk", "class_saber_katana", "class_mex_flame", "class_viktor_grenade", "full_moon_rifle", "fireflies_blessing", "branch_of_wisdom", "eternal_light", "mirrorer", "otherworld_wanderer", "dark_deer_bow", "reflected_santa_sword", "combat_fireplace", "combat_candy_machine", "gift_stealer", "tricky_glance", "end_of_fairytale", "fairy_axe", "butterfly_eater", "realities_sign_shovel", "reflected_light_shovel", "class_lifeline_healgun", "handy_darts", "spirits_whisper", "makeshift_piercer", "battle_kusarigama", "temple_claws", "summoning_totem_shovel", "cloud_standard_shovel", "vortex_core", "storm_bringer", "clouds_disperser", "field_plasma_station", "still_working_windmill", "thunder_formula", "bubblegum_01", "tnt_thrower", "ak5_56", "disguised_weapon", "intelligence_swords", "controlled_drone_system", "deadly_beamer", "tnt_holder_shovel", "aliens_shredder_shovel", "rune_buster", "ice_wyvern_horns", "runic_sentinel", "crystal_touch", "crystal_bow", "enchanted_crystal_of_ancients", "elves_energy_crystal", "tricky_electrizer", "plague_spreader", "ancestors_curse", "moon_hunter_bow", "elf_heavy_crossbow", "lightbringer", "boom_bang_shovel", "plague_flame_shovel", "eco_rifle", "ice_giant_rifle", "moonblades", "solar_lens", "supernova_generator", "event_horizon", "eco_shovel", "blacklight_pistol", "crescent_moon_shovel", "unfriendly_bat", "tamagochi_revolver", "mad_colorer", "nachos_shotgun", "skateboard_shovel", "artists_tool_shovel", "crescent_power", "shockorizer", "sharingan_vengeance", "odm_special_gear", "grenade_quirk", "thunder_spear_shovel", "magic_moon_wand_shovel", "doubled_quirk", "guardian_spirit", "ghost_epee", "the_sorting_pistol", "raven_staff", "additional_biting_limb", "corrupted_speelbook", "ghost_candlestick_shovel", "pocket_potion_pot_shovel", "transforming_rifle", "navigation_shotgun", "holographic_hook_saber", "monkey_pirate", "living_specimen_shovel", "cosmo_underwater_mine_shovel", "ships_drowner", "three_barrels_cannon", "wardens_trident", "deadly_shark", "nets_thrower", "tick_tock_pistol", "heavy_cone_minigun", "starfall", "dreams_warden", "snowstorm_fury", "spirits_overseer", "s_q_dominion", "nature_guards_shovel", "starfall_shovel", "x_mas_vibes_setter", "fatal_decorator", "x_mas_spirits_defender", "armor_piercing_sniper_rifle", "harsh_dragon", "leaders_butterfly", "leaders_bestfriend", "reckoning_day_sniper_rifle", "reckoning_day_pistols", "flaming_reckoning_day", "spirit_exorcist", "festive_eliminator", "wealth_spreader", "bloody_terror_axe", "ritual_scissors", "ultimate_worship", "terror_spreader", "sacrificial_altar", "light_devourer", "dark_ritual", "hunting_signs", "spring_snatcher", "monster_hand", "", "terror_spreader" };
const char* sceneList[] = { "Fort", "Farm", "Hill", "Dust", "Mine", "Jail", "rust", "Gluk", "Cube", "City", "Pool", "Ants", "Maze", "Arena", "Train", "Day_D", "Space", "Pizza", "Barge", "Pool2", "Winter", "Area52", "Castle", "Arena2", "Sniper", "Day_D2", "Matrix", "Heaven", "office", "Portal", "Hungry", "Bridge", "Gluk_2", "knife2", "Estate", "Glider", "Utopia", "School", "Gluk_3", "spleef1", "Slender", "Loading", "temple4", "sawmill", "Parkour", "pg_gold", "olympus", "Stadium", "ClanWar", "shipped", "Coliseum", "GGDScene", "Paradise", "valhalla", "Assault2", "Training", "Speedrun", "Hospital", "Hungry_2", "mine_new", "LevelArt", "facility", "office_z", "Pumpkins2", "red_light", "BioHazard", "ChatScene", "impositor", "PromScene", "New_tutor", "Cementery", "AppCenter", "aqua_park", "Aztec_old", "ClanWarV2", "toy_story", "checkmate", "CustomInfo", "tokyo_3019", "new_hangar", "Pool_night", "china_town", "FortAttack", "Ghost_town", "Area52Labs", "Ice_Palace", "Arena_Mine", "SkinEditor", "North_Pole", "Ghost_town2", "Arena_Swamp", "ToyFactory3", "NuclearCity", "space_ships", "FortDefence", "Two_Castles", "Ships_Night", "RacingTrack", "Coliseum_MP", "Underwater2", "ChooseLevel", "Sky_islands", "Menu_Custom", "Secret_Base", "white_house", "ProfileShop", "Arena_Space", "Cube_portals", "ClosingScene", "Mars_Station", "Arena_Castle", "checkmate_22", "Hungry_Night", "Sky_islands2", "Death_Escape", "Arena_Hockey", "WinterIsland", "Dust_entering", "pizza_sandbox", "alien_planet2", "LevelComplete", "COLAPSED_CITY", "ClanTankBuild", "train_robbery", "space_updated", "AfterBanScene", "corporate_war", "ships_updated", "templ4_winter", "Pool_entering", "supermarket_2", "DuelArenaSpace", "LoadAnotherApp", "checkmate_22.0", "Paradise_Night", "Slender_Multy2", "Code_campaign3", "Spleef_Arena_1", "infernal_forge", "china_town_day", "islands_sniper", "FortFieldBuild", "monster_hunter", "paladin_castle", "Spleef_Arena_2", "Bota_campaign4", "CampaignLoading", "Developer_Scene", "christmas_train", "Space_campaign3", "Ice_Palace_Duel", "clan_fortress01", "Christmas_Town3", "orbital_station", "Duel_ghost_town", "Swamp_campaign3", "WalkingFortress", "office_christmas", "Spooky_Lunapark3", "knife3_christmas", "Portal_Campaign4", "Arena_Underwater", "emperors_palace2", "hurricane_shrine", "Castle_campaign3", "christmas_town_22", "CampaignChooseBox", "Christmas_Dinner2", "Dungeon_dead_city", "aqua_park_sandbox", "Stadium_deathmatch", "AuthorizationScene", "sky_islands_updated", "LevelToCompleteProm", "sky_islands_sandbox", "AuthenticationScene", "NuclearCity_entering", "DownloadAssetBundles", "red_light_team_fight", "freeplay_city_summer", "four_seasons_updated", "tokyo_3018_campaign4", "COLAPSED_CITY_sniper", "ice_palace_christmas", "LoveIsland_deathmatch", "cubic_arena_campaign4", "Christmas_Town_Night3", "toy_factory_christmas", "battle_royale_arcade_2", "Dungeon_magical_valley", "Death_Escape_campaign4", "battle_royale_arcade_3", "battle_royale_09_summer", "WalkingFortress_campaign4" };
const char* curList[] = { OBFUSCATE("GemsCurrency"), OBFUSCATE("Coins"), OBFUSCATE("ClanSilver"), OBFUSCATE("ClanLootBoxPoints"),  OBFUSCATE("Coupons"), OBFUSCATE("PixelPassCurrency"), OBFUSCATE("RouletteAdsCurrency"), OBFUSCATE("RouletteAdsSpin"), OBFUSCATE("PixelBucks"), OBFUSCATE("BattlePassCurrency"), OBFUSCATE("CurrencyCompetitionTier1"), OBFUSCATE("CurrencyCompetitionTier2") };
bool maxLevel, cWear, uWear, gadgetUnlock, isLoadScenePressed, modKeys, tgod,
        removedrone, god, ammo, collectibles, changeID, bypassBan,
        crithit, charm, fte,enemymarker, enableEditor, electric, daterweapon, grenade,
        doublejump, coindrop, isDiscordPressed, webLevel, blindness, kniferange, expbull,
        shotbull, railbull,jumpdisable, slowdown, headmagnify, destroy, recoilandspread, quickscope, speedup, speed,
        isAddCurPressed, isAddWeapons, isAddWeapons2, isAddWeapons3, isAddWeapons4, isAddWeapons5, isAddWeapons6,
        ninjaJump,spamchat,gadgetdisabler, xray, scopef,isBuyEasterSticker, gadgetsEnabled, xrayApplied, kniferangesex,
        portalBull, snowstormbull, polymorph, harpoonBull,spoofMe, reload,firerate,isAimbot,Telekill, modules,
        addAllArmors, gundmg,catspam, gadgetcd, addAllGadgets,
        showItems, gadgetduration, isAddWeapons7,isAddWeapons8,uncapFps, couponClicker, teamkill, noclip, pgod, pspeed, pdamage, prespawntime, addAllWepSkins,
        isAddWepPress, addAllPets, addAllRoyale1, addAllRoyale2, addAllRoyale3, addAllRoyale4, playerScore, gbullets, flamethrower, pnoclip, reflections,
        isAddGraffitis, showWepSkins, clanparts, buyall, shopnguitest, showInfo, unban, spoofMe2;

float damage, rimpulseme, rimpulse,fovModifier,snowstormbullval, jumpHeight;
int reflection, amountws, maxLevelam;

bool autolog = true;

Vector3 WorldToScreenPoint(void *transform, Vector3 pos) {
    if (!transform)return Vector3();
    Vector3 position;
    static const auto WorldToScreenPoint_Injected = reinterpret_cast<uint64_t(__fastcall *)(void *,Vector3, int, Vector3 &)>(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43EA580")));
    WorldToScreenPoint_Injected(transform, pos, 4, position);
    return position;
}

void *get_camera() {
    static const auto get_camera_injected = reinterpret_cast<uint64_t(__fastcall *)()>(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43EAA74")));
    return (void *) get_camera_injected();
}

monoString* CreateIl2cppString(const char* str)
{
    static monoString* (*CreateIl2cppString)(const char* str, int *startIndex, int *length) =
    (monoString* (*)(const char* str, int *startIndex, int *length))(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x44865B4")));
    int* startIndex = 0;
    int* length = (int *)strlen(str);
    return CreateIl2cppString(str, startIndex, length);
}

void* setActive(void* gameObject, bool* value)
{
    static void* (*setActive)(void* instance, bool* value) =
    (void* (*)(void* instance, bool* value))(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43EFEF0")));
    return setActive(gameObject, value);
}

int* getWearIndex(const char* str)
{
    static int* (*wearIndex)(monoString* str) =
    (int* (*)(monoString* str))(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16A85D4")));
    return wearIndex(CreateIl2cppString(str));
}

void* webInstance()
{
    static void*(*webInstance)() = (void* (*)())(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x1B35B08")));//Analyze LeprechauntManager.DropReward() and youll find it :)
    return webInstance();
}

void* graffitiInstance()
{
    static void*(*graffitiInstance)() = (void* (*)())(g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x15AF6A8")));//Analyze LeprechauntManager.DropReward() and youll find it :)
    return graffitiInstance();
}

std::vector<unsigned int> keys_pressed;
int refocus=0;
bool isValidAuth, hasRegistered;

void *MyPlayer;
void *enemyPlayer;
void *myCamera;


void (*LoadLevel) (monoString* key);
void (*OpenURL) (monoString* url);
void (*setSomething) (void* instance, monoString* currencyType, int* value, int* num);
void (*addWeapon) (void* instance, monoString* weaponName, int* num);
bool (*SetMasterClient) (void* masterClientPlayer);
void* (*get_LocalPlayer) ();
void (*DestroyPlayerObjects)(void *player);
monoArray<void**> *(*PhotonNetwork_playerListothers)();
// public static GameObject Instantiate(string prefabName, Vector3 position, Quaternion rotation, byte group = 0, object[] data = null)
void (*BuyStickerPack) (int* type);
void(*EnableJetpack)(void* obj, bool);
void(*JoinToRoomPhotonAfterCheckCustom)(void* obj);
void* (*GetComponent) (void* gameObject, void* type);
void (*SendChat) (void* obj, monoString* text, bool isClan, monoString* logoid);
void (*EnableXray) (void* obj, bool enable);
void (*CharacterController$set_radius)(void* player, float val);
void (*SetXrayShader) (void* obj, bool enable);
void (*JoinToRoomPhotonAfterCheck) (void* obj);
void (*provideRoyaleItem) (monoString* item, bool* idfk);

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

// string
bool (*string$StartsWith)(monoString* string, monoString* value);
monoString* (*string$Substring)(monoString* string, int startIndex);

// File
monoArray<monoString*> *(*File$ReadAllLines)(monoString* path);
bool (*File$Exists)(monoString* path);

// Application
monoString* (*Application$persistentDataPath)();

// Component
void* (*Component$get_gameObject)(void* component);
void* (*Component$get_transform)(void* component);
monoString* (*Component$get_tag)(void* component);

// Transform
Vector3 (*get_position)(void *_instance);
Vector3 (*Transform$get_forward)(void *_instance);

// Quaternion
void* (*Quaternion$get_identity)();

// GameObject
bool (*GameObject$get_active)(void* gameObject);
void (*GameObject$set_active)(void* gameObject, bool active);
void* (*GameObject$get_transform)(void* gameObject);
void* (*GameObject$Instantiate)(void* original);
void* (*GameObject$class)();

// Camera
void* (*Camera$get_Main)();

// PhotonView
void (*PhotonView$RPC)(void* ref, int rpc, int targets, void* args[]);

// FirstPersonControlSharp
void (FirstPersonControlSharp$set_ninjaJumpUsed)(void* ref, bool used) {
    if (ref != nullptr) {
        *(bool *) ((uint64_t) ref + 0x100) = used;
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



#ifdef BIGSEX
// DebugLogWindow

void* (*DebugLogWindow$get_debugLogWindow)();

// Resources

void* (*Resources$Load)(monoString* path);
#endif

void (*LookAt)(void* obj, Vector3);
void (*set_rotation)(void* obj, Quaternion value);
void (*get_rotation)(void* obj);
void (*set_position)(void* obj, Vector3 val);
void (*buyArmor) (void* instance, int* id, int* level, monoString* reason);
void (*addWear) (int* enumerator, monoString* item);
void (*targetFrameRate) (int* value);
void (*provideGadget) (monoString* name, int* level);
void (*providePet) (monoString* petName, int* level);
monoString* (*getDeviceUniqueIdentifier)();
void (*addGraffiti) (void* instance, monoString* graffiti);
void (*AddScoreOnEvent)(void* obj, int, float);
void (*buyButtonHandle)(void* obj, monoString* itemID);
void (*buyWeaponSkinButton)(void* obj);
monoString* (*getString) (monoString* key);
void (*setString) (monoString* key, monoString* value);
monoString* (*getID) ();
void (*setID) (monoString* value);
void (*setNetworkParams) (void* instance, monoString* name, monoString* ID, bool* something);

void Pointers() {
    setNetworkParams = (void(*)(void*, monoString*, monoString*, bool*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4838618")));

    getString = (monoString*(*)(monoString*)) (monoString*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x454245C")));
    setString = (void(*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4541E00")));
    getID = (monoString*(*)()) (monoString*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4363374")));
    setID = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43634A8")));

    buyWeaponSkinButton = (void(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2138664")));
    buyButtonHandle = (void(*)(void*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x38D2C88")));
    addGraffiti = (void(*)(void*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x15B0D2C")));
    File$ReadAllLines = (monoArray<monoString*> *(*)(monoString*)) (monoArray<monoString*>*) (g_il2cppBaseMap.startAddress + string2Offset("0x33CB964"));
    Application$persistentDataPath = (monoString*(*)()) (monoString*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x440FD34")));
    File$Exists = (bool(*)(monoString*)) (bool*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x33CA678")));
    provideRoyaleItem = (void(*)(monoString*, bool*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x3C729F4")));
    providePet = (void(*)(monoString*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x3D03A74")));
    buyArmor = (void(*)(void* instance, int*, int*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x1B2AFF0")));
    provideGadget = (void(*) (monoString*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x2CC93C4")));
    targetFrameRate = (void(*)(int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x440FE60")));//search for SetTargetFrameRate in field/strings
    addWear = (void(*)(int*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16AA328"))); // if this works fede add it yourself im not your slave (it did in fact work)
    //setState = (void(*)(int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x49B20B4"))); fede u told me this doesnt work so im not updating it :)
    LoadLevel = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4794D74")));//search LoadLevel
    OpenURL = (void(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x440FDFC")));//search OpenURL
    setSomething = (void(*) (void*, monoString*, int*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x1B3ED88")));//search up ClanLootBoxPoints and look for Progress
    SendChat = (void(*) (void*, monoString*, bool, monoString *)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x47BC280")));//search SendChat
    addWeapon = (void(*) (void*, monoString*, int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x1433F78")));//search AddW_eaponToInv rec == null , tag =  {0}
    SetMasterClient = (bool(*)(void*)) (bool*) (g_il2cppBaseMap.startAddress + string2Offset("0x443E100"));//search SetMasterClient in strings
    get_LocalPlayer = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x4436B24"));//find the photonplayer class name and just search it until you find something similiar to the old version
    DestroyPlayerObjects = (void (*)(void *)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x443E5BC"));//search DestroyPlayerObjects in strings
    PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) (monoArray<void**>*) (g_il2cppBaseMap.startAddress + string2Offset("0x4436E34"));//same steps as localplayer, its the 2nd array, so below the first array method you see
    EnableXray = (void(*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x47AE550"));//search "use_zoom"
    BuyStickerPack = (void(*)(int*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x413BA80")));//look in StickersController, compare and find the right function
    JoinToRoomPhotonAfterCheck = (void(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x39520EC")));//not obfuscated just search
    JoinToRoomPhotonAfterCheckCustom = (void(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x3500F0C")));//not obfuscated just search
    // UNITY FUNC
    Component$get_gameObject = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x44052B0")));
    Component$get_tag = (monoString*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x44055A0")));
    Component$get_transform = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4405274")));
    CharacterController$set_radius = (void(*)(void*, float)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x457B544")));
    Type$GetType = (void*(*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4DDCC58")));
    // cool misc stuff //


    // OK ITS FINE NOW //
    GameObject$get_active = (bool(*)(void*)) (bool) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43EFE70")));
    GameObject$set_active = (void(*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43EFEAC")));
    Transform$get_forward = (Vector3 (*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4410FFC")));
    string$StartsWith = (bool (*)(monoString*, monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4480420")));
    string$Substring = (monoString* (*)(monoString*, int)) (monoString*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x44848C4")));
    get_position = (Vector3 (*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4410A1C")));
    PhotonView$RPC = (void(*)(void*, int, int, void*[])) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4444180")));//go look for it in photonview you'll find it out using the args
    LookAt = (void (*)(void*, Vector3)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4382840")));
    set_rotation = (void (*)(void*, Quaternion)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4410D38")));
    get_rotation = (void (*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4410CB4")));
    set_position = (void (*)(void*, Vector3)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x4410ABC")));
    EnableJetpack = (void (*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x47C1934")));
 //   isDead = (bool (*)(void*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x143726C")));
    SetXrayShader = (void(*)(void*, bool)) (void*) (g_il2cppBaseMap.startAddress + string2Offset("0x47D58D8"));//analyze the xray field and you'll see it
    getDeviceUniqueIdentifier = (monoString*(*)()) (monoString*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x43E8660")));
    AddScoreOnEvent = (void(*)(void*, int, float)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x473B0FC")));
#ifdef BIGSEX
    Resources$Load = (void*(*)(monoString*)) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x437FCA0")));
    DebugLogWindow$get_debugLogWindow = (void*(*)()) (void*) (g_il2cppBaseMap.startAddress + string2Offset(OBFUSCATE("0x16766AC")));
#endif
}

#include "Auth.h"
// 0x435FA0C <- offset for gameobject.tag
// 0x434733C <- offset for object.name

void (*old_WeaponManager)(void *obj);
void WeaponManager(void *obj) {
    if (obj != nullptr) {
        if (isAddWeapons) {
            for (int i = 0; i < 150; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons = false;
        }
        if (isAddWeapons2) {
            for (int i = 150; i < 300; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons2 = false;
        }
        if (isAddWeapons3) {
            for (int i = 300; i < 450; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons3 = false;
        }
        if (isAddWeapons4) {
            for (int i = 450; i < 600; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons4 = false;
        }
        if (isAddWeapons5) {
            for (int i = 600; i < 750; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons5 = false;

        }
        if (isAddWeapons6) {
            for (int i = 750; i < 900; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons6 = false;
        }
        if (isAddWeapons7) {
            for (int i = 900; i < 1050; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons7 = false;
        }
        if (isAddWeapons8) {
            for (int i = 1050; i < 1193; i++) {
                addWeapon(obj, CreateIl2cppString(wepList[i]), (int *) (62));
            }
            isAddWeapons8 = false;
        }
        if (isAddWepPress)
        {
            addWeapon(obj, CreateIl2cppString(goofyWeps[selectedWeapon]), (int *) (62));
            isAddWepPress = false;
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
        }


        if(gundmg){
            *(float*)((uint64_t) obj + 0x3A4) = 10;//sectorsAOEDamageMultiplierFront
            *(float*)((uint64_t) obj + 0x3A8) = 10;//sectorsAOEDamageMultiplierSide
            *(float*)((uint64_t) obj + 0x3A8) = 10;//sectorsAOEDamageMultiplierBack
        }

        if(gadgetdisabler){
            *(bool*)((uint64_t) obj + 0x1F0) = true;//isGadgetDisabler
            *(float*)((uint64_t) obj + 0x1F4) = 99999;//gadgetDisableTime
        }

        if(catspam){
            *(bool*)((uint64_t) obj + 0x640) = false;//is3CatSpam
        }

        if(portalBull){
            *(bool*)((uint64_t) obj + 0x3B4) = true;//portalGun
            *(bool*)((uint64_t) obj + 0x3B5) = true;//isPortalExplosion
          //  *(int*)((uint64_t) obj + 0x3C0) = 1;//isPortalExplosion
        }

        if(charm){
            *(bool*)((uint64_t) obj + 0x260) = true;//isCharm
            *(float*)((uint64_t) obj + 0x264) = 99999;//charmTime
        }

        if(scopef){
            *(bool*)((uint64_t) obj + 0xC5) = true;//isZooming
        }

        if(fte){
            *(bool*)((uint64_t) obj + 0x234) = true;//fireImmunity
            *(bool*)((uint64_t) obj + 0x235) = true;//toxicImmunity
            *(bool*)((uint64_t) obj + 0x236) = true;//toxicImmunity
        }

        if(snowstormbull){
            *(bool*)((uint64_t) obj + 0x2E4) = true;//snowStorm
            *(float*)((uint64_t) obj + 0x2E8) = 6;//snowStormBonusMultiplier
            if(snowstormbullval != NULL && !kniferange){
                *(float*)((uint64_t) obj + 0xDC) = snowstormbullval; // shootDistance
                *(float*)((uint64_t) obj + 0x5F8) = snowstormbullval; // range
            }
        }

        if(playerScore){
            *(bool*)((uint64_t) obj + 0x368) = true;//isBuffPoints
            *(float*)((uint64_t) obj + 0x374) = 99999;//buffBonusPointsForKill
        }
        if(polymorph){
            *(bool*)((uint64_t) obj + 0x2D4) = true;//polymorpher
            *(float*)((uint64_t) obj + 0x2D8) = 999999999;//polymorphDuarationTime
            *(int*)((uint64_t) obj + 0x3B4) = 0;//polymorphType
            *(float*)((uint64_t) obj + 0x2E0) = 22340000;//polymorphMaxHealth
            harpoonBull = true;
        }

        if(enemymarker){
            *(bool*)((uint64_t) obj + 0x237) = true;//enemyMarker
            *(bool*)((uint64_t) obj + 0x238) = true;//enemyMarkerWhenShot
            *(bool*)((uint64_t) obj + 0x239) = true;//enemyMarkerWhenAiming
            *(float*)((uint64_t) obj + 0x23C) = 0;//enemyMarketChargeTime
        }

        if(electric){
            *(bool*)((uint64_t) obj + 0x404) = true;//isElectricShock
            *(float*)((uint64_t) obj + 0x408) = 99999;//electricShockCoeff
            *(float*)((uint64_t) obj + 0x40C) = 99999;//electricShockTime
        }


        //*(bool*)((uint64_t) obj + 0x428) = true;//isDaterWeapon

        if(gbullets){
            *(bool*)((uint64_t) obj + 0x42D) = true;//isGrenadeWeapon
        }

        if(reflections){
            *(bool*)((uint64_t) obj + 0x2F9) = true;//isDamageReflection
            *(bool*)((uint64_t) obj + 0x2FA) = false;//isDamageAbsorption
            *(float*)((uint64_t) obj + 0x2FC) = -99999;//damageReflectionValue
        }

        if(firerate){
            *(int*)((uint64_t) obj + 0x5B0) = 999;
        }

        if(flamethrower){
            *(bool*)((uint64_t) obj + 0x334) = true;//flamethrower
        }

        if(blindness){
            *(bool*)((uint64_t) obj + 0x254) = true;//isBlindEffect
            *(float*)((uint64_t) obj + 0x25C) = 99999;//blindness
        }

        if(crithit){
            *(float*)((uint64_t) obj + 0x438) = 9999;//criticalHitCoef
            *(float*)((uint64_t) obj + 0x434) = 9999;//criticalHitChance
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
            *(bool*)((uint64_t) obj + 0x1A6) = true; //isShotGun
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

        if(pnoclip){
            *(bool*)((uint64_t) obj + 0x156) = true;//IsGhost
        }

        if(quickscope){
            *(float*)((uint64_t) obj + 0xF8) = 9999;//scopeSpeed
        }

        //if(xray){ *(bool*)((uint64_t) obj + 0xC6) = true;}
        /*void* ItemRecord = *(void**)((uint64_t) obj + 0x648);
        if(ItemRecord != nullptr){
            if(spleef){
                *(bool*)((uint64_t) obj + 0x6D) = true;//isSpleef
            }
        }*/
    }
    oldWeaponSounds(obj);
}

float(*oldGadgetDuration)(void* obj);
float gadgetDuration(void* obj){
    if(obj != nullptr && gadgetduration){
        return 9999999;
    }
    return oldGadgetDuration(obj);
}

void (*oldFirstPersonControllerSharp)(void* obj);
void FirstPersonControllSharp(void* obj){
    if(obj != nullptr){
        if(jumpHeight != NULL){
            *(float*)((uint64_t) obj + 0x4A0) = jumpHeight;
        }
    }
    oldFirstPersonControllerSharp(obj);
}

Vector3 GetPlayerLocation(void *player) {
    return get_position(Component$get_transform(player));
}

void behaviour_teleport(void* obj, monoString* message, const char* prefix) {
    void* myTransform = Component$get_transform(Player_move_c$skinName(obj));
    std::string it = std::string(string$Substring(message, strlen(prefix))->getChars());
    int myOffset = std::stoi(it);
    set_position(myTransform, get_position(myTransform)+(Transform$get_forward(myTransform)*myOffset));
}

void (*old_SendChatHooked)(void* obj, monoString* message, bool isClan, monoString* clanIcon);
void SendChatHooked(void* obj, monoString* message, bool isClan, monoString* clanIcon){
    if(obj != nullptr){
        if (string$StartsWith(message, CreateIl2cppString(OBFUSCATE("!teleport ")))) {
            behaviour_teleport(obj, message, OBFUSCATE("!teleport "));
            return;
        }
        if (string$StartsWith(message, CreateIl2cppString(OBFUSCATE("!tp ")))) {
            behaviour_teleport(obj, message, OBFUSCATE("!tp "));
            return;
        }
    }
    old_SendChatHooked(obj, message, isClan, clanIcon);
}

float (*oldSpeeds)(void* obj);
float Speed(void* obj){
    if(obj != nullptr && speed){
        return 1000;
    }
    return oldSpeeds(obj);
}

float (*oldPetSpeeds)(void* obj);
float petSpeed(void* obj){
    if(obj != nullptr && pspeed){
        return 999999;
    }
    return oldPetSpeeds(obj);
}

float (*oldpetHealth)(void* obj);
float petHealth(void* obj){
    if(obj != nullptr && pgod){
        return 9999999;
    }
    return oldpetHealth(obj);
}

float (*oldpetAttack)(void* obj);
float petAttack(void* obj){
    if(obj != nullptr && pdamage){
        return 1000;
    }
    return oldpetAttack(obj);
}

bool isEnemy(void* player, void* enemy)
{
    bool (*isEnemy)(void *player, void *enemy) = (bool(*)(void *, void *))(g_il2cppBaseMap.startAddress + string2Offset("0x473BCB0")); // PlayerDamageable$$IsEnemyTo
    return isEnemy(player, enemy);
}

bool IsMine(void* SkinName){
    return *(bool*)((uint64_t) SkinName + 0xA8);
}

bool isDead(void* player)
{
    bool (*IsDead)(void *player) = (bool(*)(void *))(g_il2cppBaseMap.startAddress + string2Offset("0x473C00C")); // PlayerDamageable$$IsDead
    return IsDead(player);
}


float (*old_get_fieldOfView)(void *instance);
float get_fieldOfView(void *instance) {
    if (instance != nullptr && fovModifier != NULL) {
        return fovModifier;
    }
    return old_get_fieldOfView(instance);
}

void (*old_ShopNGUIController)(void *instance);
void ShopNGUIController(void *instance) {
    return old_ShopNGUIController(instance);
}

void (*old_updateSkinButtons)(void *instance);
void updateSkinButtons(void *instance) {
    if (instance != nullptr && buyall)
    {
        void* buySkinGameObj = (void*)((uint64_t) instance + 0x98);
        setActive(buySkinGameObj, (bool*)(true));
        return;
    }
    return old_updateSkinButtons(instance);
}

void* get_PlayerTransform(void* player)
{
    void *var = player;
    if (var)
    {
        void* var1 = *(void **)((uint64_t)player + 0x398);
        return var1;
    }
}

void Aimbot(void* players){
    {

    }
}


void(*oldPlayerMoveC)(void* obj);
void(PlayerMoveC)(void* obj){
    if(obj != nullptr) {
        if (spamchat) {
            SendChat(obj, CreateIl2cppString("BUY ZYGISKPG - https://discord.gg/DGtgZkk6sR"), false,CreateIl2cppString("0"));
        }

        if (xrayApplied) {
            EnableXray(obj, true);
        }


        if (xray) {
            SetXrayShader(obj, true);
            *(bool *) ((uint64_t) obj + 0x710) = true; // search ZombiManager then its above
        }

        if (ninjaJump) {
            EnableJetpack(obj, true);//search for jetpack in player_move_C
            ninjaJump = false;
        }

        if (spoofMe2) {
            void *argsForSetPixelBookID[] = {CreateIl2cppString(OBFUSCATE("zygiskPGonTop"))};
            void *argsForSetPlayerUniqID[] = {CreateIl2cppString(OBFUSCATE("zygiskPGonTop"))};
            void *argsForSetNickName[] = {CreateIl2cppString(OBFUSCATE("zygiskPGonTop"))};
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetPixelBookID,PhotonTargets::All, argsForSetPixelBookID);
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetPlayerUniqID,PhotonTargets::All, argsForSetPlayerUniqID);
            PhotonView$RPC(Player_move_c$photonView(obj), RPCList::SetNickName, PhotonTargets::All, argsForSetNickName);
            spoofMe2 = false;
        }

        if (gadgetsEnabled) {
            *(bool *) ((uint64_t) obj + 0xDAA) = false;//search for Action, the field should be under the gadget class
        }

        void *SkinName = *(void **) ((uint64_t) obj + 0x650);
        if (SkinName != nullptr) {
            if (IsMine(SkinName)) {
                MyPlayer = SkinName;
                enemyPlayer = obj;
            }

            if (isAimbot) {
                  //  Aimbot(enemyPlayer);
            }

            if (Telekill) {
                if(isDead(obj)){
                    Vector3 enemyPos = get_position(Component$get_transform(enemyPlayer));
                    set_position(Component$get_transform(MyPlayer),Vector3(enemyPos.X, enemyPos.Y, enemyPos.Z - 1));
                }
            }
            /* if (playstantiate) {
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
             }*/
        }
    }
    oldPlayerMoveC(obj);
}

void(*old_HandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args);
void(HandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args){
    if(obj != nullptr){
        JoinToRoomPhotonAfterCheck(obj);
        return;
    }
    old_HandleJoinRoomFromEnterPasswordBtnClicked(obj, sender, args);
}


void(*old_CustomHandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args);
void(CustomHandleJoinRoomFromEnterPasswordBtnClicked)(void* obj, void* sender, void* args){
    if(obj != nullptr){
        JoinToRoomPhotonAfterCheckCustom(obj);
        return;
    }
    old_CustomHandleJoinRoomFromEnterPasswordBtnClicked(obj, sender, args);
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
        targetFrameRate((int*)(999));
        if (isAddGraffitis) {
            for (int i = 0; i < 15; i++) {
                addGraffiti(graffitiInstance(), CreateIl2cppString(graffitiList[i]));
            }
            isAddGraffitis = false;
        }
        if (showInfo)
        {
            LOGE("AuthSecret: %s", getString(CreateIl2cppString("terceShtuA"))->getChars());
            LOGE("AccountID: %s", getID()->getChars());
            showInfo = false;
        }
        if (unban)
        {
            setString(CreateIl2cppString("banned-id"), CreateIl2cppString(""));
            setString(CreateIl2cppString("banned-hash"), CreateIl2cppString(""));
        }
        if (spoofMe)
        {
            setID(CreateIl2cppString("-69"));
            LoadLevel(CreateIl2cppString("Menu_Custom"));
            spoofMe = false;
        }
        if (addAllRoyale1)
        {
            for (int i = 0; i < 200; i++)
            {
                provideRoyaleItem(CreateIl2cppString(avatarList[i]), (bool*)(true));
            }
            addAllRoyale1 = false;
        }
        if (addAllRoyale2)
        {
            for (int i = 200; i < 400; i++)
            {
                provideRoyaleItem(CreateIl2cppString(avatarList[i]), (bool*)(true));
            }
            addAllRoyale2 = false;
        }
        if (addAllRoyale3)
        {
            for (int i = 400; i < 600; i++)
            {
                provideRoyaleItem(CreateIl2cppString(avatarList[i]), (bool*)(true));
            }
            addAllRoyale3 = false;
        }
        if (addAllRoyale4)
        {
            for (int i = 600; i < 721; i++)
            {
                provideRoyaleItem(CreateIl2cppString(avatarList[i]), (bool*)(true));
            }
            addAllRoyale4 = false;
        }
        if (addAllGadgets) {
            for (int i = 0; i < 68; i++)
            {
                provideGadget(CreateIl2cppString(gadgetList[i]), (int *)(65));
            }
            addAllGadgets = false;
        }
        if (addAllArmors) {
            for (int i = 0; i < 35; i++)
            {
                addWear((int*)(10), CreateIl2cppString(bootsList[i]));
            }
            for (int i = 0; i < 28; i++)
            {
                addWear((int*)(6), CreateIl2cppString(hatList[i]));
            }
            for (int i = 0; i < 22; i++)
            {
                addWear((int*)(9), CreateIl2cppString(capeList[i])); // hola
            }
            for (int i = 0; i < 38; i++)
            {
                addWear((int*)(12), CreateIl2cppString(maskList[i]));
            }
            for (int i = 0; i < 35; i++)
            {
                buyArmor(webInstance(), getWearIndex(armorList[i]), (int *)(65), CreateIl2cppString("migr")); // leave AS IS, works
            }
            addAllArmors = false;
        }
        if (addAllPets) {
            for (int i = 0; i < 94; i++) {
                providePet(CreateIl2cppString(petsList[i]), (int*)(260));
            }
            addAllPets = false;
        }
        if (isBuyEasterSticker) {
            isBuyEasterSticker = false;
            BuyStickerPack((int*)StickerType::easter);
        }
        if (isLoadScenePressed) {
            LoadLevel(CreateIl2cppString(sceneList[selectedScene]));
            isLoadScenePressed = false;
        }
        if (bypassBan) {
           // setState((int*)(15));
            //bypassBan = false;
        }
        if (isDiscordPressed) {
            OpenURL(CreateIl2cppString(OBFUSCATE("http://bit.ly/3yck7Bw")));
            isDiscordPressed = false;
        }
        if (isAddCurPressed) {
            setSomething(webInstance(), CreateIl2cppString(curList[selectedCur]), (int *)(amountws), (int *)(8));
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

int (*oldRewardedExp)(void* obj);
int RewardedExp(void* obj){
    if(obj != nullptr && maxLevel){
        return 999;
    }
    return oldRewardedExp(obj);
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


bool setupimg;

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

void Hooks() {
    // hooks
    HOOK("0x2135C9C", updateSkinButtons, old_updateSkinButtons);
    HOOK("0x3D3FEE0", PixelTime, old_PixelTime);
    HOOK("0x38C1638", ShopNGUIController, old_ShopNGUIController);
    HOOK("0x1B080B8", WeaponSounds, oldWeaponSounds);
    HOOK("0x142FE8C", WeaponManager, old_WeaponManager);
    HOOK("0x47E0A14", PlayerMoveC, oldPlayerMoveC);
    HOOK("0x3953680", HandleJoinRoomFromEnterPasswordBtnClicked, old_HandleJoinRoomFromEnterPasswordBtnClicked);
    HOOK("0x3500D28", CustomHandleJoinRoomFromEnterPasswordBtnClicked, old_CustomHandleJoinRoomFromEnterPasswordBtnClicked);
    HOOK("0x21E9C5C", Speed, oldSpeeds);
    HOOK("0x2342FB0", gadgetDuration, oldGadgetDuration);//compare inside the gadget class
    HOOK("0x17E4150", FirstPersonControllSharp, oldFirstPersonControllerSharp);
   // HOOK("0x47BC280", SendChatHooked, old_SendChatHooked);
    HOOK("0x40220F0", petSpeed, oldPetSpeeds);//PetInfo
    HOOK("0x4021E80", petHealth, oldpetHealth);
    HOOK("0x4021FB8", petAttack, oldpetAttack);
    HOOK("0x1C71944", RewardedExp, oldRewardedExp);
}

void Patches() {
    PATCH_SWITCH("0x38C5DAC", "20008052C0035FD6", shopnguitest);
    PATCH_SWITCH("0x4153B64", "20008052C0035FD6", clanparts); // 0x41529C4
    PATCH_SWITCH("0x4154658", "20008052C0035FD6", clanparts);
    PATCH_SWITCH("0x480525C", "1F2003D5C0035FD6", god); // search int viewID and you'll find it
    PATCH_SWITCH("0x3C48320", "1F2003D5C0035FD6", god); // search for SkinName skinName = this.mySkinName; and find a float in analyze (player_move_c for the one above too)
    PATCH_SWITCH("0x4FBDCF0", "1F2003D5C0035FD6", god);//OnTriggerEnter
    PATCH_SWITCH("0x4FBD460", "1F2003D5C0035FD6", god);//OnControllerColliderHit
    PATCH_SWITCH("0x1C71944", "A0F08FD2C0035FD6", maxLevel); // first class in analyze should be ExpController not ArmouryCell
    PATCH_SWITCH("0x248B0A8", "802580D2C0035FD6", cWear);//search for almanachmainui and find the refresh method then youll check analyze to find the class, then get both of the ints
    PATCH_SWITCH("0x2486960", "802580D2C0035FD6", cWear);
    PATCH_SWITCH("0x2CC71F0", "00008052C0035FD6", gadgetUnlock);
    PATCH_SWITCH("0x21E709C", "603E8012C0035FD6", modKeys);//go to AmmoButtonInGame and find the function called onclick, analyze and try to find the same class once you do just goodluck finding the exact method it shouldn't be hard
    PATCH_SWITCH("0x1DDDA84", "C0035FD6", tgod);//all the minus live
    PATCH_SWITCH("0x1DE2BDC", "C0035FD6", tgod);
    PATCH_SWITCH("0x38DFF10", "20008052C0035FD6", showWepSkins); //show all weapon skins (test)
    PATCH_SWITCH("0x1DE2E74", "C0035FD6", tgod);
    PATCH_SWITCH("0x47F6CB8", "C0035FD6", removedrone);//my old self lied just search for player_move_c and DroneController droneController; droneController.Deactivate();
    PATCH_SWITCH("0x47F6D70", "C0035FD6", removedrone);//search Dictionary<GadgetCategory in movec
    PATCH_SWITCH("0x1B08DE8", "200080D2C0035FD6", crithit);//nexthitcritical in weaponsounds
    PATCH_SWITCH("0x1E11928", "200080D2C0035FD6", couponClicker);//this requires abit of effort and luck, go to CollectButtonPressed the class is in the typeof handle then just try to match the bool to the one before by comparing it
    PATCH_SWITCH("0x4598D28", "00F0271EC0035FD6", reload);//mask_hitman_1_up1 in strings
    PATCH_SWITCH("0x2342F74", "000080D2C0035FD6", gadgetcd);//search Action in player_move_c find the 2 fields with action exactly then above some bools should be a classname which is GADGET
    PATCH_SWITCH("0x4021788", "00008052C0035FD6", prespawntime);
    //    PATCH_SWITCH("0x2F87D18", "00FA80D2C0035FD6", initParams); // do it 0x2F87D18 0x2F944C8 0x2F87D98 0x2F95CF8
    PATCH_SWITCH("0x2379F48", "80388152C0035FD6", collectibles); // 0x48EF240
    PATCH_SWITCH("0x14C80C0", "00008052C0035FD6", daterweapon); // compare isDaterRegim to 16.6.1  version goodluck
    PATCH_SWITCH("0x14C80C0", "00008052C0035FD6", daterweapon); // compare isSpeedrun to 16.6.1  version goodluck
    PATCH_SWITCH("0x14D7DA8", "00008052C0035FD6", daterweapon); // compare isMiniGame to 16.6.1  version goodluck (it should have the first thing in analyze GameConnect method )
    PATCH_SWITCH("0x14CC548", "200080D2C0035FD6", gadgetcd);//compare gadgetinfo cooldown to a deobfuscated version goodluck
    PATCH_SWITCH("0x14D7520", "00008052C0035FD6", teamkill);//compare isTeamMode to 16.6.1  version goodluck
    PATCH_SWITCH("0x14D8834", "00008052C0035FD6", teamkill);//look for Random in PlayerBotInstance, you'll find the bool at the end of the method
    //PATCH_SWITCH("0x4810EE8", "E923BB6D", firerate);//_Shot - search component["Shoot"].length; & _ShotPressed = search if ("WeaponGrenade" == null) and match it, set ShotPressed's first 4 bytes as Shot
    PATCH("0x3C484C0", "C0035FD6");//ANTIBAN
    PATCH("0x499903C", "000080D2C0035FD6");//Swear filter
    PATCH("0x3BE5458", "200080D2C0035FD6");//ValidateNickName
    PATCH("0x3BE5698", "200080D2C0035FD6");//ValidateNickNameNoAnalytics
    PATCH("0x37A2660", "C0035FD6");
    PATCH("0x4B2A1AC", "C0035FD6");//CheatDetectorBanner
    PATCH("0x1D4B3F2", "C0035FD6");//BannedOnServer
}

void DrawMenu(){
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGui::Begin(OBFUSCATE("ZygiskPG Premium 1.1b (23.1) - chr1s#4191 && networkCommand()#7611 && ohmyfajett#3500"));
        if (isValidAuth && isAuth()) {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyResizeDown;
            if (ImGui::BeginTabBar("Menu", tab_bar_flags)) {
                if (ImGui::BeginTabItem(OBFUSCATE("Account"))) {
                    ImGui::Checkbox(OBFUSCATE("Max Level"), &maxLevel);
                    ImGui::TextUnformatted((OBFUSCATE("Gives the player max level after a match ends (Recommended to use after level3)")));
                    ImGui::Checkbox(OBFUSCATE("Collectibles"), &collectibles);
                    ImGui::TextUnformatted(OBFUSCATE("Does what collectibles used to do"));
                    ImGui::Checkbox(OBFUSCATE("Free Clan Parts"), &clanparts);
                    ImGui::TextUnformatted(OBFUSCATE("Makes it so you can upgrade forts/tanks easily"));
                    ImGui::Checkbox(OBFUSCATE("ShopNGUI test"), &shopnguitest);
                    ImGui::Checkbox(OBFUSCATE("Show Items"), &showItems);
                    ImGui::Checkbox(OBFUSCATE("Free Lottery"), &modKeys);
                    ImGui::TextUnformatted(OBFUSCATE("Makes the keys a negative value. (Don't buy stuff from the Armoury while this is on)"));
                    if (ImGui::Button(OBFUSCATE("Buy Easter Sticker Pack"))) {
                        isBuyEasterSticker = true;
                    }
                    ImGui::Checkbox(OBFUSCATE("Force buy weapon skin"), &buyall);
                    if (ImGui::CollapsingHeader("Unlockables"))
                    {
                        ImGui::TextUnformatted((OBFUSCATE("Gives the player items you pick, Freezes are expected.")));
                        if (ImGui::Button(OBFUSCATE("Add All Wear"))) {
                            addAllArmors = true;
                        }
                        if (ImGui::Button(OBFUSCATE("Add All Gadgets"))) {
                            addAllGadgets = true;
                        }
                        if (ImGui::Button(OBFUSCATE("Add All Pets"))) {
                            addAllPets = true;
                        }
                        if (ImGui::Button(OBFUSCATE("Add All Graffities"))) {
                            isAddGraffitis = true;
                        }
                        if (ImGui::CollapsingHeader(OBFUSCATE("Royale Items Unlock")))
                        {
                            if (ImGui::Button(OBFUSCATE("Add All Royale 1/4"))) {
                                addAllRoyale1 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Royale 2/4"))) {
                                addAllRoyale2 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Royale 3/4"))) {
                                addAllRoyale3 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Royale 4/4"))) {
                                addAllRoyale4 = true;
                            }
                        }
                        if (ImGui::CollapsingHeader(OBFUSCATE("Weapon Unlock")))
                        {
                            ImGui::Checkbox("Show All Weapon Skins", &showWepSkins);
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 0-150"))) {
                                isAddWeapons = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 151-300"))) {
                                isAddWeapons2 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 301-450"))) {
                                isAddWeapons3 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 451-600"))) {
                                isAddWeapons4 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 601-750"))) {
                                isAddWeapons5 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 751-900"))) {
                                isAddWeapons6 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 901-1050"))) {
                                isAddWeapons7 = true;
                            }
                            if (ImGui::Button(OBFUSCATE("Add All Weapons 1051-1194"))) {
                                isAddWeapons8 = true;
                            }
                            ImGui::ListBox(OBFUSCATE("Weapon"), &selectedWeapon, goofyWeps,IM_ARRAYSIZE(goofyWeps), 4);
                            if (ImGui::Button(OBFUSCATE("Add Weapon"))) {
                                isAddWepPress = true;
                            }
                        }
                    }
                    if (ImGui::CollapsingHeader(OBFUSCATE("Currency Mods"))) {
                        ImGui::SliderInt(OBFUSCATE("Amount"), &amountws, 0, 5000);//dont fucking change it you cocksucker, make it how you want in a dev build but for users its 5000
                        ImGui::TextUnformatted(OBFUSCATE("Will be counted as the value that the game will use. (I reccommend not getting more than 50K)"));
                        ImGui::ListBox(OBFUSCATE("Currency"), &selectedCur, curList,IM_ARRAYSIZE(curList), 4);
                        if (ImGui::Button(OBFUSCATE("Add Currency"))) {
                            isAddCurPressed = true;
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(OBFUSCATE("Player"))) {
                    ImGui::Checkbox(OBFUSCATE("Godmode"), &god);
                    ImGui::TextUnformatted(OBFUSCATE("Makes you invincible (others can kill you but you won't die and just become invisible)"));
                    if (ImGui::Button("Spoof ID -69"))
                    {
                        spoofMe = true;
                    }
                    ImGui::TextUnformatted(OBFUSCATE("Makes your ID be -69, reverts after restart, has weird effects"));
                    ImGui::Checkbox(OBFUSCATE("Force Double Jump"), &doublejump);
                  //  ImGui::Checkbox(OBFUSCATE("Noclip"), &noclip);
                    if(ImGui::Button(OBFUSCATE("Get Jetpack/Fly"))){
                        ninjaJump = true;
                    }
                    ImGui::Checkbox(OBFUSCATE("Speed"), &speed);
                    ImGui::SliderFloat(OBFUSCATE("Jump/Jetpack Height"), &jumpHeight, 0.0f,2.5f);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(OBFUSCATE("Game"))) {
                  // ImGui::Checkbox(OBFUSCATE("Aimbot"), &isAimbot);
                 //   ImGui::Checkbox(OBFUSCATE("Telekill"), &Telekill);
                 //   ImGui::TextUnformatted(OBFUSCATE("Teleports you behind an enemy."));
                    ImGui::Checkbox(OBFUSCATE("Kill All"), &kniferange);
                    ImGui::TextUnformatted(OBFUSCATE("Kill everyone"));
                    ImGui::Checkbox(OBFUSCATE("Friendly-Fire"), &teamkill);
                    ImGui::TextUnformatted(OBFUSCATE("Allows you to kill your teammates."));
                    ImGui::Checkbox(OBFUSCATE("Spam Chat"), &spamchat);
                    ImGui::Checkbox(OBFUSCATE("Force Coin Drop"), &coindrop);
                    ImGui::TextUnformatted(OBFUSCATE("Always drops coins when someone dies."));
                    ImGui::Checkbox(OBFUSCATE("Glitch Everyone"), &xrayApplied);
                    ImGui::TextUnformatted(OBFUSCATE("Every weapon will have a scope."));
                    ImGui::Checkbox(OBFUSCATE("Infinite Game Score"), &playerScore);
                    ImGui::TextUnformatted(OBFUSCATE("Gives 2mil score when you kill someone."));
                    if (ImGui::Button(OBFUSCATE("Crash Everyone"))) {
                        destroy = true;
                    }
                    if (ImGui::CollapsingHeader(OBFUSCATE("Gadget Mods"))) {
                        ImGui::Checkbox(OBFUSCATE("Drone Godmode"), &removedrone);
                        ImGui::TextUnformatted(OBFUSCATE("The drone gadget will never despawn. (Don't get more than 1 drone)"));
                        ImGui::Checkbox(OBFUSCATE("No Gadget Cooldown"), &gadgetcd);
                        ImGui::TextUnformatted(OBFUSCATE("Do not change gagets while its enabled."));
                        ImGui::Checkbox(OBFUSCATE("Infinite Gadget Duration"), &gadgetduration);
                        ImGui::TextUnformatted(OBFUSCATE("Disable after placing down the timed gadget to get access to other gadgets"));
                        ImGui::Checkbox(OBFUSCATE("Turret Godmode"), &tgod);
                        ImGui::TextUnformatted(OBFUSCATE("Gives the Turret Gadget Infinite Health, others can destroy it, it will become invisible when it does."));
                    }
                    if (ImGui::CollapsingHeader(OBFUSCATE("Pet Mods"))) {
                        ImGui::Checkbox(OBFUSCATE("Godmode"), &pgod);
                        ImGui::Checkbox(OBFUSCATE("Speed"), &pspeed);
                        ImGui::Checkbox(OBFUSCATE("One Shot Kill"), &pdamage);
                        ImGui::Checkbox(OBFUSCATE("No Respawn Time"), &prespawntime);
                        ImGui::Checkbox(OBFUSCATE("Pet No-Clip"), &pnoclip);
                        ImGui::TextUnformatted(OBFUSCATE("Pet can move through walls."));
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(OBFUSCATE("Weapon"))) {
                    ImGui::Checkbox(OBFUSCATE("Force Critical Hits"), &crithit);
                    ImGui::TextUnformatted(OBFUSCATE("Forces Critical Shots each time you hit someone."));
                    ImGui::Checkbox(OBFUSCATE("Unlimited Ammo"), &ammo);
                  //  ImGui::Checkbox(OBFUSCATE("One Shot Kill"), &gundmg);
                 //  ImGui::Checkbox(OBFUSCATE("Fire-Rate"), &firerate);
                    ImGui::Checkbox(OBFUSCATE("No Reload Length"), &reload);
                    ImGui::TextUnformatted(OBFUSCATE("Reloads the gun almost instantly (Re-equip after enabling)"));
                    ImGui::SliderFloat(OBFUSCATE("Silent Aim Power"), &snowstormbullval, 0.0f,2000.0f);
                    ImGui::Checkbox(OBFUSCATE("Silent Aim"), &snowstormbull);
                    ImGui::TextUnformatted(OBFUSCATE("Shooting anywhere will hit others."));
                    ImGui::Checkbox(OBFUSCATE("No Recoil and Spread"), &recoilandspread);
                    ImGui::Checkbox(OBFUSCATE("Force Scope"), &scopef);
                    ImGui::Checkbox(OBFUSCATE("Infinite Knife/Flamethrower Range"),&kniferangesex);
                    ImGui::TextUnformatted(OBFUSCATE("Allows you to aim and hit people with a knife or a framethrower at any distance."));
                    if (ImGui::CollapsingHeader(OBFUSCATE("Bullet Mods"))) {
                        ImGui::Checkbox(OBFUSCATE("Force Explosive Bullets"), &expbull);
                        ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot explosive bullets."));
                        ImGui::Checkbox(OBFUSCATE("Force Railgun Bullets"), &railbull);
                        ImGui::TextUnformatted(OBFUSCATE("Forces any gun to shoot railgun bullets."));
                        ImGui::SliderInt(OBFUSCATE("Reflection Rays"), &reflection, 0, 1000);
                        ImGui::TextUnformatted(OBFUSCATE("Amplifys the reflection ray ricochet."));
                        ImGui::Checkbox(OBFUSCATE("Force Polymorph Bullets"), &polymorph);
                        ImGui::TextUnformatted(OBFUSCATE("Forces bullets to make players turn into sheep."));
                        ImGui::Checkbox(OBFUSCATE("Force Harpoon Bullets"), &harpoonBull);
                        ImGui::TextUnformatted(OBFUSCATE("Explosive Bullets that dont launch you."));
                        ImGui::Checkbox(OBFUSCATE("Force Flamethrower Shots"), &flamethrower);
                        ImGui::TextUnformatted(OBFUSCATE("Will make the gun shoot flames."));
                    }

                    if (ImGui::CollapsingHeader(OBFUSCATE("Effect Mods"))) {
                        ImGui::Checkbox(OBFUSCATE("Force Charm"), &charm);
                        ImGui::TextUnformatted(OBFUSCATE("Adds the charm effect (Used to reduce half of the enemy's weapon efficiency)"));
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
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(OBFUSCATE("Visual"))) {
                    ImGui::Checkbox(OBFUSCATE("Chams"), &xray);
                    ImGui::TextUnformatted(OBFUSCATE("Shows the enemy body through walls."));
                    ImGui::Checkbox(OBFUSCATE("Show marker"), &enemymarker);
                    ImGui::TextUnformatted(OBFUSCATE("Shows the enemy after you shoot them once."));
                    ImGui::Checkbox(OBFUSCATE("Quick-Scope"), &quickscope);
                    ImGui::TextUnformatted(OBFUSCATE("Opens the scope instantly."));
                //    ImGui::SliderFloat(OBFUSCATE("Field Of View"), &fovModifier, 0.0, 360.0);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(OBFUSCATE("Misc"))) {
                    ImGui::ListBox(OBFUSCATE("Select Scene"), &selectedScene, sceneList, IM_ARRAYSIZE(sceneList), 4);
                    if (ImGui::Button(OBFUSCATE("Load Scene"))) {
                        isLoadScenePressed = true;
                    }
                    /* if (ImGui::Button(OBFUSCATE("Playstantiate the playfab"))) {
                         playstantiate = true;
                     }
                     if (ImGui::Button(OBFUSCATE("Mask me"))) {
                         spoofMe = true;
                     }*/
                    if (ImGui::Button(OBFUSCATE("Join Discord"))) {
                        isDiscordPressed = true;
                    }
                    ImGui::TextUnformatted(OBFUSCATE("Its Recommended to join the discord server for mod updates etc."));
                    if (ImGui::Button(OBFUSCATE("Show Info"))) {
                        showInfo = true;
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            if(isAuth()){
                Patches();
            }
        }
    }
    if (!isValidAuth) {
        if (accessibleCode == 200 && results != CURLE_ABORTED_BY_CALLBACK) {
            json j = json::parse(readBuffer);
            jsonresult = j.dump(1);

            if (jsonresult.find("invalid_details") != std::string::npos) {
                ImGui::TextUnformatted(OBFUSCATE("Invalid Username or Password, check again."));
            }
            if (jsonresult.find("hwid_updated") != std::string::npos) {
                ImGui::TextUnformatted(OBFUSCATE("Your HWID has updated! Restart the application again to see the menu."));
            }
            if (jsonresult.find("invalid_hwid") != std::string::npos) {
                ImGui::TextUnformatted(OBFUSCATE("The license is set for a different HWID, if you have changed devices"));
                ImGui::TextUnformatted(OBFUSCATE("or reset your phone, go back to the zygiskPG portal and press the button Update HWID."));
            }
            if (jsonresult.find("time_expired") != std::string::npos) {
                ImGui::TextUnformatted(OBFUSCATE("Your license has expired, renew it to get access."));
            }
        }
        else{
            if(!fileExists){
                ImGui::TextUnformatted(OBFUSCATE("You forgot to add license.key, check if its inside the files folder."));
            }
            else{
                ImGui::TextUnformatted(OBFUSCATE("Failed to get a response from our auth."));
                ImGui::TextUnformatted(OBFUSCATE("We are down, or your network is unstable."));
            }
        }
    }
    ImGui::End();
}

void SetupImgui() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 100"));
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(7.0f);
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
    //LOGE("AccInfo %s", File("/sdcard/license.txt").read_content().c_str());

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
        g_il2cppBaseMap = KittyMemory::getLibraryBaseMap(OBFUSCATE("libil2cpp.so"));
    } while (!g_il2cppBaseMap.isValid());

    Pointers();
    sleep(5);
    auto eglhandle = dlopen(OBFUSCATE("libunity.so"), RTLD_LAZY);
    auto eglSwapBuffers = dlsym(eglhandle, OBFUSCATE("eglSwapBuffers"));
    if(autolog) {
        isValidAuth = tryAutoLogin();
        autolog = false;
    }
    Hooks();
    DobbyHook((void*)eglSwapBuffers,(void*)hook_eglSwapBuffers,
              (void**)&old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver((OBFUSCATE("/system/lib/libinput.so")), (OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE")));
    if (NULL != sym_input) {
        DobbyHook(sym_input,(void*)myInput,(void**)&origInput);
    }
    return nullptr;
}
