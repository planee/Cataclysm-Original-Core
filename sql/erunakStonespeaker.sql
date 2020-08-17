-- No more accessory, just handle mindbender as if it was an ordinary creatur spawn
DELETE FROM `vehicle_template_accessory` WHERE `entry`=40825 AND `seat_id`=0;

DELETE FROM `disables` WHERE `sourceType`=9 AND `entry`=40788;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES (9, 40788, 0, '', '', 'Disable Pathfinding for mindbender gursha');

DELETE FROM `creature` WHERE id = 40788;
INSERT INTO `creature` (`id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`, `saiscriptflag`) VALUES 
(40788, 643, 5004, 5004, 3, 65535, 0, 0, -141.272, 635.088, 286.805, 3.0649, 604800, 0, 0, 670185, 0, 0, 0, 0, 0, 0, 0);