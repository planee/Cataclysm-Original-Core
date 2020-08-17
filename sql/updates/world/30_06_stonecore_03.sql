/*Stonecore issue*/
UPDATE `creature_template` SET `InhabitType`= 4, `VehicleId`= 1307, `ScriptName`= 'npc_stonecore_camera_bunny' WHERE `entry`= 42565;
UPDATE `creature_template` SET `flags_extra`= 128, `InhabitType`= 4, `ScriptName`= 'npc_stonecore_siesmic_shard' WHERE `entry`= 42355;
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'boss_high_priestess_azil' WHERE `entry`= 42333;
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 33554432 WHERE `entry`= 42499;
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 131072 WHERE `entry`= 42565;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (79050, 79351, 79009, 92667);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79050, 'spell_stonecore_energy_shield'),
(79351, 'spell_stonecore_force_grip'),
(79009, 'spell_stonecore_siesmic_shard'),
(92667, 'spell_stonecore_energy_shield');

DELETE FROM `spell_target_position` WHERE `id` IN (86860, 86858, 86856, 92667);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(86860, 725, 1331.23, 934.06, 234.34, 1.81),
(86858, 725, 1345.99, 937.66, 234.34, 1.81),
(86856, 725, 1360.84, 940.86, 234.34, 1.81);