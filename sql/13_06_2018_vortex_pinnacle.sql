/*Vortex Pinnacle*/
UPDATE `creature_template` SET `ScriptName`= 'npc_ertan_vortex' WHERE `entry`= 46007;
UPDATE `creature_template` SET `ScriptName`= 'npc_air_current' WHERE `entry`= 47305;
UPDATE `creature_template` SET `ScriptName`= 'npc_unstable_grounding_field' WHERE `entry`= 46492;
UPDATE `creature_template` SET `ScriptName`= 'npc_vortex_pinnacle_slipsteam' WHERE `entry`= 45455;
UPDATE `creature_template` SET `ScriptName`= 'npc_skyfall_star' WHERE `entry` IN (45932, 52019);
UPDATE `creature_template` SET `ScriptName`= 'npc_cloud_prince' WHERE `entry`= 45917;
UPDATE `creature_template` SET `ScriptName`= 'npc_whipping_wind' WHERE `entry`= 47238;
UPDATE `creature_template` SET `ScriptName`= 'npc_young_storm_dragon' WHERE `entry`= 45919;
UPDATE `creature_template` SET `ScriptName`= 'npc_armored_mistral' WHERE `entry`= 45915;
UPDATE `creature_template` SET `ScriptName`= 'npc_empyrean_assassin' WHERE `entry`= 45922;
UPDATE `creature_template` SET `ScriptName`= 'npc_executor_of_the_caliph' WHERE `entry`= 45928;
UPDATE `creature_template` SET `ScriptName`= 'npc_gust_soldier' WHERE `entry`= 45477;
UPDATE `creature_template` SET `ScriptName`= 'npc_howling_gale' WHERE `entry`= 45572;
UPDATE `creature_template` SET `ScriptName`= 'npc_lurking_tempest' WHERE `entry`= 45704;
UPDATE `creature_template` SET `ScriptName`= 'npc_minister_of_air' WHERE `entry`= 45930;
UPDATE `creature_template` SET `ScriptName`= 'npc_servant_of_asaad' WHERE `entry`= 45926;
UPDATE `creature_template` SET `ScriptName`= 'npc_temple_adept' WHERE `entry`= 45935;
UPDATE `creature_template` SET `ScriptName`= 'npc_turbulent_squall' WHERE `entry`= 45924;
UPDATE `creature_template` SET `ScriptName`= 'npc_wild_vortex' WHERE `entry`= 45912;
UPDATE `creature_template` SET `ScriptName`= 'npc_golden_orb' WHERE `entry`= 51157;
UPDATE `creature_template` SET `unit_flags`= 33554952, `VehicleId`= 1307, `InhabitType`= 4, `ScriptName`= 'npc_fall_catcher_5' WHERE `entry`= 48165;
UPDATE `creature_template` SET `AIName`= '' WHERE `entry`= 43875;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (87553, 85467, 87765);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(87553, 'spell_asaad_supremacy_of_the_storm'),
(85467, 'spell_vp_lurk'),
(87765, 'spell_minister_of_air_lightning_lash');

DELETE FROM `creature` WHERE `guid` IN (170565, 170566, 170567, 170568);

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (6477, 6651, 6652, 6660, 6661, 6663, 6665, 6666, 6667);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6477, 'at_catch_fall_5'),
(6651, 'at_catch_fall_5'),
(6652, 'at_catch_fall_5'),
(6660, 'at_catch_fall_5'),
(6661, 'at_catch_fall_5'),
(6663, 'at_catch_fall_5'),
(6665, 'at_catch_fall_5'),
(6666, 'at_catch_fall_5'),
(6667, 'at_catch_fall_5');