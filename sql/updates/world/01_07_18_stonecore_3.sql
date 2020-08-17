/*Stonecore*/
UPDATE `creature_template_addon` SET `auras`= NULL WHERE `entry`= 43159;
UPDATE `creature_template` SET `minlevel`= 86, `maxlevel`= 86, `faction_A`= 16, `faction_H`= 16, `unit_flags`= 33554436, `flags_extra`= 128 WHERE `entry` IN (42499, 49650);
UPDATE `creature_template` SET `faction_A`= 14, `faction_H`= 14, `flags_extra`= 0, `unit_flags`= `unit_flags` | 131072, `ScriptName`= 'npc_stonecore_siesmic_shard' WHERE `entry` IN (42355, 49657);

DELETE FROM `creature` WHERE `guid` IN (175366, 175367, 175368);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (80647, 92309, 79332, 79249);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80647, 'spell_stonecore_stalaktite'),
(92309, 'spell_stonecore_stalaktite'),
(79332, 'spell_high_priestess_azil_gravity_well_knockback'),
(79249, 'spell_azil_gravity_well_eff');