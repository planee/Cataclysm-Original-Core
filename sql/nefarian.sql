--- Nefarian

-- Orb of Culimation spawn
DELETE FROM `gameobject` WHERE `id`=203254;
INSERT INTO `gameobject` (`id`,`map`,`zone`,`area`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(203254, 669, 5094, 5094, 1, 65535, -27.8, -224.44, 63.34, 0.0813443, 0, 0, 0.040661, 0.999173, 300, 0, 1);

-- Orb of culmination gossip text
UPDATE `gameobject_template` SET `data3`='6001' WHERE  `entry`=203254;

--- Restore original data
DELETE FROM `gossip_menu_option` WHERE `menu_id`=6001 AND `id`=1;
INSERT INTO `gossip_menu_option` VALUES (6001, 1, 0, '<Put your hand on the sphere.>', 1, 1, 0, 0, 0, 0, NULL, NULL, NULL);
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(203254, 2, 4891, 'Orb of Culmination', '', '', '', 35, 32, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6001, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 'go_dragon_orb', 15595);

-- Onixya template data
DELETE FROM `creature_template` WHERE `entry` IN (41270,51116,51117,51118);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(41270, 51116, 51117, 51118, 0, 0, 32569, 0, 0, 0, 'Onyxia', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 3, 1225.5, 1544.7, 0, 1651, 35, 1500, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 1160.9, 1328.1, 225, 2, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 81.9983, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 0, 650854271, 0, 'boss_bwd_onyxia', 13623),
(51116, 0, 0, 0, 0, 0, 32569, 0, 0, 0, 'Onyxia (1)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 3, 548, 822, 0, 205, 125, 0, 0, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 288.036, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 0, 650854271, 0, '', 13623),
(51117, 0, 0, 0, 0, 0, 32569, 0, 0, 0, 'Onyxia (2)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 3, 13000, 16000, 0, 50750, 7.5, 0, 0, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 115, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 0, 650854271, 0, '', 13623),
(51118, 0, 0, 0, 0, 0, 32569, 0, 0, 0, 'Onyxia (3)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 3, 18000, 22000, 0, 70000, 7.5, 1500, 0, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 405, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 0, 650854271, 0, '', 13623);

-- Onyxia spawn
DELETE FROM `creature` WHERE `id` = 41270;
INSERT INTO `creature` (`id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(41270, 669, 5094, 5094, 15, 1, 0, 0, -107.324, -224.665, -7.33029, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0);

-- Onyxia chains
DELETE FROM `conditions` WHERE `SourceEntry` IN (81177,81175,81158,81176,81174,81159);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 81177, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', ''),
(13, 1, 81175, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', ''),
(13, 1, 81158, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', ''),
(13, 1, 81176, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', ''),
(13, 1, 81174, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', ''),
(13, 1, 81159, 0, 0, 31, 0, 3, 41270, 0, 0, 0, 0, '', '');

-- Nefarian Elevator
UPDATE `gameobject_template` SET `data1`='0' WHERE  `entry`=207834;

-- Nefarian Intro path
DELETE FROM `waypoint_data` WHERE `id` = 4137600;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`, `inverse_formation_angle`) VALUES 
(4137600, 5, -147.641, -224.222, 92.3193, 0, 0, 0, 0, 100, 0, 0),
(4137600, 4, -87.167, -190.667, 92.3193, 0, 0, 0, 0, 100, 0, 0),
(4137600, 3, -86.6406, -258.222, 92.3193, 0, 0, 0, 0, 100, 0, 0),
(4137600, 2, -169.394, -250.802, 91.7818, 0, 0, 0, 0, 100, 0, 0),
(4137600, 1, -162.076, -224.604, 57.9262, 0, 0, 0, 0, 100, 0, 0);

-- Pillar markers
DELETE FROM `creature` WHERE `id` = 320000;
INSERT INTO `creature` (`id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(320000, 669, 5094, 5094, 15, 1, 0, 0, -87.2247, -258.395, 3.1, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(320000, 669, 5094, 5094, 15, 1, 0, 0, -87.8527, -191.394, 3.1, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(320000, 669, 5094, 5094, 15, 1, 0, 0, -147.136, -224.881, 3.1, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0);

-- Jump to pillar markers
DELETE FROM `conditions` WHERE `SourceEntry` = 79205;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 79205, 0, 0, 31, 0, 3, 320000, 0, 0, 0, 0, '', '');

-- Hail of bones targeting
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=78682 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=42844 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES (13, 1, 78682, 0, 0, 31, 0, 3, 42844, 0, 0, 0, 0, '', '');

-- Cataslysm boss stalker 2 spawn
DELETE FROM `creature` WHERE `id` = 42844;
INSERT INTO `creature` (`id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(42844, 669, 5094, 5094, 15, 1, 0, 0, -107.237, -224.561, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -143.339, -199.598, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -143.685, -249.293, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -111.444, -268.628, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -67.2247, -244.395, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -66.8527, -205.394, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0),
(42844, 669, 5094, 5094, 15, 1, 0, 0, -108.136, -179.881, 8.6, 0.0174063, 300, 0, 0, 68714, 0, 0, 0, 0, 0);


DELETE FROM `creature_template` WHERE `entry`=320000;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(320000, 0, 0, 0, 0, 0, 1126, 0, 0, 0, 'Invisible Stalker (Cataclysm Boss, Ignore Combat)', '', '', 0, 88, 88, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 0, 1225.5, 1544.7, 0, 1651, 1, 2000, 2000, 1, 33587456, 2048, 0, 0, 0, 0, 0, 0, 1160.9, 1328.1, 225, 10, 1048652, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 0, 0.800005, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 128, '', 13623);

-- Chromatic Prototype
DELETE FROM `creature_template` WHERE `entry` IN (41948,51119,51120,51121);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
(41948, 51119, 51120, 51121, 0, 0, 33185, 0, 0, 0, 'Chromatic Prototype', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 1, 619.2, 835.2, 0, 976, 7.5, 2000, 2000, 2, 0, 134219776, 0, 0, 0, 0, 0, 0, 466.8, 654, 156, 2, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 0, 25.9389, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 617299839, 0, '', 13623),
(51119, 0, 0, 0, 0, 0, 33185, 0, 0, 0, 'Chromatic Prototype (1)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 1, 500, 750, 0, 187, 35, 0, 0, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 0, 89.0438, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 617299839, 0, '', 13623),
(51120, 0, 0, 0, 0, 0, 33185, 0, 0, 0, 'Chromatic Prototype (2)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 1, 500, 750, 0, 187, 35, 0, 0, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 0, 36, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 617299839, 0, '', 13623),
(51121, 0, 0, 0, 0, 0, 33185, 0, 0, 0, 'Chromatic Prototype (3)', '', '', 0, 85, 85, 3, 0, 16, 16, 0, 1, 1.14286, 1, 1.14286, 1, 1, 500, 750, 0, 187, 35, 0, 0, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 7, 0, 126, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 617299839, 0, '', 13623);

-- Disable animated bone warrior pathfinding.
DELETE FROM `disables` WHERE `sourceType`=9 AND `entry`=41918;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES (9, 41918, 0, '', '', 'Disable Pathfinding for Animated Bone Warrior');

-- Onyxia model
DELETE FROM `creature_model_info` WHERE `modelid`=32569;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES (32569, 0.558, 18, 1, 0);
