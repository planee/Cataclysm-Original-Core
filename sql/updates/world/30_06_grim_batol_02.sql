/*Grim Batol*/
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 131072 WHERE `entry`= 42570;
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 512, `AIName`= '', `ScriptName`= 'npc_battered_red_drake_flyer' WHERE `entry`= 39294;
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'npc_battered_red_drake_net' WHERE `entry`= 42570;
UPDATE `creature` SET `MovementType`= 2 WHERE `guid`= 169165;

DELETE FROM `creature_addon` WHERE `guid`= 169165;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(169165, 169165, 0, 0, 1, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`= 169165;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`, `move_flag`, `action`, `action_chance`, `inverse_formation_angle`, `wpguid`) VALUES
(169165, 1, -499.39, -479.55, 272.11, 0, 0, 0, 100, 0, 0),
(169165, 2, -505.24, -475.47, 276.65, 0, 0, 0, 100, 0, 0),
(169165, 3, -534.98, -469.27, 276.59, 0, 0, 0, 100, 0, 0),
(169165, 4, -570.17, -481.43, 276.59, 0, 0, 0, 100, 0, 0),
(169165, 5, -534.98, -469.27, 276.59, 0, 0, 0, 100, 0, 0),
(169165, 6, -505.24, -475.47, 276.65, 0, 0, 0, 100, 0, 0),
(169165, 7, -499.39, -479.55, 272.11, 0, 0, 0, 100, 0, 0),
(169165, 8, -481.04, -497.77, 271.91, 0, 0, 0, 100, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (79377);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79377, 'spell_grim_batol_net');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (39294, 42570);