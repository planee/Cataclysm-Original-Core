/*vortex pinnacle*/
UPDATE `creature` SET `position_x`= -467.97, `position_y`= -115.13 WHERE `guid`= 170617;
UPDATE `creature` SET `MovementType`= 2 WHERE `guid` IN (170617, 170618, 170586, 170585);
UPDATE `creature` SET `MovementType`= 1, `spawndist`= 5 WHERE `guid` IN (170614, 170613, 170612, 170615, 170611, 170610, 170609, 170608, 170605, 170604, 170606, 170607);
UPDATE `creature` SET `position_x`= -951.67, `position_y`= -71.50 WHERE `guid`= 170585;
UPDATE `creature_template_addon` SET `auras`= 85734 WHERE `entry`= 45991;
UPDATE `creature_template` SET `flags_extra`= 128 WHERE `entry`= 45991;
UPDATE `creature_template` SET `ScriptName`= 'npc_starfall_rotation_manager' WHERE `entry`= 80044;

DELETE FROM `conditions` WHERE `SourceEntry`= 88201 AND `SourceTypeOrReferenceId`= 13;

DELETE FROM `creature_addon` WHERE `guid` IN (170617, 170618, 170586, 170585);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(170617, 170617, 0, 0, 1, 0, NULL),
(170618, 170618, 0, 0, 1, 0, NULL),
(170586, 170586, 0, 0, 1, 0, NULL),
(170585, 170585, 0, 0, 1, 0, NULL);

DELETE FROM `creature` WHERE `guid` IN (533506, 533507, 533508, 172776, 533509);
INSERT INTO `creature` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`) VALUES 
(533506, 45572, 657, 5035, 5035, 3, 1, 0, 0, -1052.99, -56.2396, 700.569, 0, 7200, 0, 0, 1, 0, 0, 0, 0, 0, 0),
(533507, 45572, 657, 5035, 5035, 3, 1, 0, 0, -1073.38, -35.9635, 700.569, 0, 7200, 0, 0, 1, 0, 0, 0, 0, 0, 0),
(533508, 45991, 657, 5035, 5035, 3, 1, 0, 0, -780.93, 487.144, 697.983, 0, 7200, 0, 0, 1, 0, 0, 0, 0, 0, 0),
(533509, 80044, 657, 5035, 5035, 3, 1, 0, 0, -694.541, 504.014, 647.953, 5.12079, 7200, 0, 0, 1, 0, 0, 0, 1, 0, 0);

DELETE FROM `waypoint_data` WHERE `id` IN (170617, 170618, 170586, 170585);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`, `move_flag`, `action`, `action_chance`, `inverse_formation_angle`, `wpguid`) VALUES
(170617, 1, -450.08, -97.26, 638.82, 0, 0, 0, 100, 0, 0),
(170617, 2, -467.97, -115.12, 638.82, 0, 0, 0, 100, 0, 0),
(170617, 3, -487.88, -135.05, 638.82, 0, 0, 0, 100, 0, 0),
(170618, 1, -450.08, -97.26, 638.82, 0, 0, 0, 100, 0, 0),
(170618, 2, -467.97, -115.12, 638.82, 0, 0, 0, 100, 0, 0),
(170618, 3, -487.88, -135.05, 638.82, 0, 0, 0, 100, 0, 0),
(170585, 1, -961.69, -64.93, 694.91, 0, 0, 0, 100, 0, 0),
(170585, 2, -977.81, -58.87, 694.91, 0, 0, 0, 100, 0, 0),
(170585, 3, -1002.54, -58.60, 694.91, 0, 0, 0, 100, 0, 0),
(170585, 4, -977.81, -58.87, 694.91, 0, 0, 0, 100, 0, 0),
(170585, 5, -961.69, -64.93, 694.91, 0, 0, 0, 100, 0, 0),
(170585, 6, -951.67, -71.50, 694.91, 0, 0, 0, 100, 0, 0),
(170586, 1, -963.98, -70.57, 694.91, 750, 0, 0, 100, 0, 0),
(170586, 2, -975.97, -66.72, 694.91, 750, 0, 0, 100, 0, 0),
(170586, 3, -1001.66, -64.93, 694.91, 1100, 0, 0, 100, 0, 0),
(170586, 4, -975.97, -66.72, 694.91, 750, 0, 0, 100, 0, 0),
(170586, 5, -963.98, -70.57, 694.91, 750, 0, 0, 100, 0, 0),
(170586, 6, -955.13, -75.47, 694.91, 1100, 0, 0, 100, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (85021, 88201, 85159);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(85021, 'spell_slipstream_aura'),
(88201, 'spell_vortex_pinnacle_healing_well'),
(85159, 'spell_vortex_pinnacle_howling_gale_eff');
