/*grim batol*/
UPDATE `creature_template` SET `ScriptName`= 'npc_ascended_flameseeker' WHERE `entry`= 39415;
UPDATE `creature_template` SET `ScriptName`= 'npc_ascended_rockbreaker' WHERE `entry`= 40272;
UPDATE `creature_template` SET `ScriptName`= 'npc_ascended_waterlasher' WHERE `entry`= 40273;
UPDATE `creature_template` SET `ScriptName`= 'npc_ascended_windwalker' WHERE `entry`= 39414;
UPDATE `creature_template` SET `ScriptName`= 'npc_azureborne_guardian' WHERE `entry`= 39854;
UPDATE `creature_template` SET `ScriptName`= 'npc_azureborne_seer' WHERE `entry` IN (39855, 40291);
UPDATE `creature_template` SET `ScriptName`= 'npc_azureborne_warlord' WHERE `entry`= 39909;
UPDATE `creature_template` SET `ScriptName`= 'npc_crimsonborne_guardian' WHERE `entry`= 39381;
UPDATE `creature_template` SET `ScriptName`= 'npc_crimsonborne_seer' WHERE `entry` IN (39405, 40290);
UPDATE `creature_template` SET `ScriptName`= 'npc_crimsonborne_warlord' WHERE `entry`= 39626;
UPDATE `creature_template` SET `ScriptName`= 'npc_enslaved_burning_ember' WHERE `entry`= 39892;
UPDATE `creature_template` SET `ScriptName`= 'npc_enslaved_rock_elemental' WHERE `entry`= 39900;
UPDATE `creature_template` SET `ScriptName`= 'npc_enslaved_gronn_brute' WHERE `entry`= 40166;
UPDATE `creature_template` SET `ScriptName`= 'npc_faceless_corruptor' WHERE `entry`= 39392;
UPDATE `creature_template` SET `ScriptName`= 'npc_trogg_dweller' WHERE `entry`= 39450;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_armsmaster' WHERE `entry` IN (40306, 41073);
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_beguiler' WHERE `entry`= 40167;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_drake' WHERE `entry`= 41095;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_earthshaper' WHERE `entry`= 39890;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_enforcer' WHERE `entry` IN (39956, 40448);
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_firecatcher' WHERE `entry`= 39870;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_shadow_weaver' WHERE `entry`= 39954;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_stormbreaker' WHERE `entry`= 39962;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_thundercaller' WHERE `entry`= 40270;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_war_mage' WHERE `entry`= 40268;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_wyrmcaller' WHERE `entry`= 39873;
UPDATE `creature_template` SET `ScriptName`= 'npc_ascended_rockbreaker_fissure' WHERE `entry`= 41091;
UPDATE `creature_template` SET `ScriptName`= 'npc_crimsonborne_warlord_empowering_flames' WHERE `entry`= 41045;
UPDATE `creature_template` SET `ScriptName`= 'npc_battered_red_drake' WHERE `entry`= 42571;
UPDATE `creature_template` SET `ScriptName`= 'boss_general_umbriss' WHERE `entry`= 39625;
UPDATE `creature_template` SET `ScriptName`= 'npc_malignant_trogg' WHERE `entry`= 39984;
UPDATE `creature_template` SET `ScriptName`= 'npc_umbriss_trogg_dweller' WHERE `entry`= 45467;
UPDATE `creature_template` SET `ScriptName`= 'boss_forgemaster_throngus' WHERE `entry`= 40177;
UPDATE `creature_template` SET `ScriptName`= 'npc_throngus_fire_patch' WHERE `entry`= 48711;
UPDATE `creature_template` SET `ScriptName`= 'boss_drahga_shadowburner' WHERE `entry`= 40319;
UPDATE `creature_template` SET `ScriptName`= 'npc_invocation_of_flame_stalker' WHERE `entry`= 40355;
UPDATE `creature_template` SET `ScriptName`= 'npc_invoked_flaming_spirit' WHERE `entry`= 40357;
UPDATE `creature_template` SET `ScriptName`= 'npc_drahga_valiona' WHERE `entry`= 40320;
UPDATE `creature_template` SET `ScriptName`= 'npc_seeping_twilight' WHERE `entry`= 40365;
UPDATE `creature_template` SET `ScriptName`= 'boss_erudax' WHERE `entry`= 40484;
UPDATE `creature_template` SET `ScriptName`= 'npc_erudax_faceless_corruptor' WHERE `entry` IN (40600, 48844);
UPDATE `creature_template` SET `ScriptName`= 'npc_alexstrasza_egg' WHERE `entry`= 40486;
UPDATE `creature_template` SET `ScriptName`= 'npc_shadow_gale_stalker' WHERE `entry`= 44314;
UPDATE `creature_template` SET `ScriptName`= 'npc_erudax_twilight_hatchling' WHERE `entry`= 39388;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (76414, 76579, 90673, 75000, 75238, 75692);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(76414, 'spell_twilight_enforcer_meat_grinder'),
(76579, 'spell_twilight_thundercaller_electric_blast'),
(90673, 'spell_twilight_shadow_weaver_shadow_weave'),
(75000, 'spell_throngus_pick_weapon'),
(75238, 'spell_drahga_supernova'),
(75692, 'spell_erudax_shadow_gale');