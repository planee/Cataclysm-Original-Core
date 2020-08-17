/*Stonecore*/
UPDATE `creature_template` SET `ScriptName`= 'npc_sc_millhouse_manastorm' WHERE `entry`= 43391;
UPDATE `creature_template` SET `ScriptName`= 'npc_crystalspawn_giant' WHERE `entry`= 42810;
UPDATE `creature_template` SET `ScriptName`= 'npc_impp' WHERE `entry`= 43014;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_instance_portal' WHERE `entry` IN (51396, 51397);
UPDATE `creature_template` SET `ScriptName`= 'npc_rock_borer' WHERE `entry`= 43917;
UPDATE `creature_template` SET `ScriptName`= 'npc_crystal_shard' WHERE `entry`= 49267;
UPDATE `creature_template` SET `ScriptName`= 'npc_stalactite' WHERE `entry` IN (43159, 43357);
UPDATE `creature_template` SET `ScriptName`= 'npc_lava_fissure' WHERE `entry`= 43242;
UPDATE `creature_template` SET `ScriptName`= 'npc_gravity_well' WHERE `entry`= 42499;

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (6076, 6070);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6076, 'AreaTrigger_at_behind_corborus'),
(6070, 'AreaTrigger_at_behind_slabhide');

DELETE FROM `spell_script_names` WHERE `spell_id`= 79251;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79251, 'spell_high_priestess_azil_gravity_well_script');