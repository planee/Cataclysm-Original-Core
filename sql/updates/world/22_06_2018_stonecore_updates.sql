/*Stonecore*/
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 131072, `flags_extra`= 128 WHERE `entry` IN (43159, 43357);
UPDATE `creature_template` SET `faction_A`= 14, `faction_H`= 14, `unit_flags`= `unit_flags` | 2 | 131072 | 33554432  WHERE `entry`= 51423;
UPDATE `creature_template` SET `faction_A`= 14, `faction_H`= 14, `flags_extra`= 128, `ScriptName`= 'npc_stonecore_rapture_controller' WHERE `entry`= 49597;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (81838, 81828, 92651, 81801, 92393, 92426);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(81838, 'spell_stonecore_thrashing_charge_selector'),
(81801, 'spell_stonecore_thrashing_charge_path'),
(92393, 'spell_stonecore_rupture_trigger'),
(92426, 'spell_stonecore_paralyze');