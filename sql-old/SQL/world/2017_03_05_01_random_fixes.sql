-- Professor Calvert, missing speech, thanks to dalaran-wow
-- Professor Calvert texts
DELETE FROM `creature_text` WHERE `entry` = 28266; 
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(28266,0,0,'All of Northrend is colder than a witch\'s heart; yet here we are...  There must be some explanation!  Quite possibly one related to the crystal pillars!',12,0,100,1,5000,0,27638,'Professor Calvert'),
(28266,0,1,'The similarities between this place and Un\'goro are uncanny.  Maybe the two are connected in some way.',12,0,100,1,5000,0,27640,'Professor Calvert'),
(28266,0,2,'How long until we can have another pilot airborne?  I need aerial surveys if my research is going to get anywhere.',12,0,100,1,5000,0,27641,'Professor Calvert');

-- Professor Calvert SAI
SET @ENTRY := 28266;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,1000,2000,180000,240000,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Professor Calvert - Out of Combat - Say Line 0");

-- Based on: https://github.com/dalaranwow/TrinityCore/blob/a140544d2f24f44ad692584c74c15ca2e6e20697/sql/updates/world/3.3.5/2017_02_16_01_world.sql - Thanks to Dalaranwow devs
SET @ENTRY := 28793;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,2,0,37,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Darmuk - On Initialize - Set Active On");
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES (28793, 0, 1, 0, 0, 0, 100, 0, 7000, 13000, 12000, 17000, 11, 54386, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darmuk - In Combat - Cast \'Darmuk\'s Vigilance\'');

-- Fix Darmuk and guards - add missing paths/random-run-movement - Big thanks to Dalaranwow for the idea. Now it'll work like retail; https://www.youtube.com/watch?v=g9XB4QXKfbc&feature=youtu.be

SET @NPC := 138040;
SET @PATH := 138040 * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=5957.316,`position_y`=-2082.866,`position_z`=248.3568 WHERE `guid`= 138040;
DELETE FROM `creature_addon` WHERE `guid`= 138040;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,5957.316,-2082.866,248.3568,0,15000,0,0,100,0), -- 10:21:03
(@PATH,2,5956.233,-2129.016,248.6345,0,0,0,0,100,0), -- 10:21:34
(@PATH,3,5970.171,-2131.559,248.8019,0,15000,0,0,100,0), -- 10:21:53
(@PATH,4,5966.756,-2134.273,248.7502,0,0,0,0,100,0), -- 10:22:16
(@PATH,5,5954.458,-2183.096,248.5561,0,15000,0,0,100,0), -- 10:22:22
(@PATH,6,5952.354,-2141.64,248.548,0,0,0,0,100,0), -- 10:23:04
(@PATH,7,5952.22,-2100.056,248.5795,0,0,0,0,100,0); -- 10:23:23

DELETE FROM `creature` WHERE `guid` IN (253561, 253560, 253556, 253555, 134530, 134514);
INSERT INTO `creature` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`) VALUES
(253561, 28750, 571, 0, 0, 1, 1, 0, 0, 5943.117, -2188.9858, 247.957, 1.922359, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1), -- 28750 (Area: -1)
(253560, 28750, 571, 0, 0, 1, 1, 0, 0, 5941.483, -2150.7951, 247.9784, 1.399843, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1), -- 28750 (Area: 4315)
(253556, 28750, 571, 0, 0, 1, 1, 0, 0, 5943.3120, -2129.4682, 248.2264, 4.124675, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1), -- 28750 (Area: 4315)
(253555, 28750, 571, 0, 0, 1, 1, 0, 0, 5959.199, -2162.388, 248.3089, 3.128839, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1), -- 28750 (Area: 4315)
(134530, 28750, 571, 0, 0, 1, 1, 0, 0, 5940.885, -2099.2729, 248.2018, 0.1183602, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1), -- 28750 (Area: 4315)
(134514, 28750, 571, 0, 0, 1, 1, 0, 0, 5960.793, -2101.225, 248.1623, 4.201819, 300, 15, 0, 1, 0, 1, 0, 0, 0, 1); -- 28750 (Area: 4315)
DELETE FROM `creature_addon` WHERE `guid` IN (253561, 253560, 253556, 253555, 134530, 134514);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(253561, 0, 0, 0, 1, 0, ''), -- 28750
(253560, 0, 0, 0, 1, 0, ''), -- 28750
(253556, 0, 0, 0, 1, 0, ''), -- 28750
(253555, 0, 0, 0, 1, 0, ''), -- 28750
(134530, 0, 0, 0, 1, 0, ''), -- 28750
(134514, 0, 0, 0, 1, 0, ''); -- 28750
UPDATE `creature_template` SET `speed_walk`=1, `speed_run`=1.14 WHERE  `entry`=28750;


-- [NPC][WotlK] Stormforged Saboteur, missing paths
-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC := 148796;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7693.506,`position_y`=-2324.018,`position_z`=1038.417 WHERE `guid`=148796;
DELETE FROM `creature_addon` WHERE `guid`=148796;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (148796,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7693.506,-2324.018,1038.417,0,0,0,0,100,0),
(@PATH,2,7681.481,-2302.699,1024.499,0,0,0,0,100,0),
(@PATH,3,7692.982,-2323.4,1038.048,0,0,0,0,100,0),
(@PATH,4,7698.107,-2330.618,1043.213,0,0,0,0,100,0),
(@PATH,5,7733.517,-2353.724,1068.687,0,0,0,0,100,0),
(@PATH,6,7717.036,-2345.113,1058.945,0,0,0,0,100,0),
(@PATH,7,7706.17,-2339.842,1050.38,0,0,0,0,100,0),
(@PATH,8,7694.688,-2325.7,1039.569,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D000008E709 .go 7693.506 -2324.018 1038.417


-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC := 148802;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7717.821,`position_y`=-2373.15,`position_z`=1076.397 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7717.821,-2373.15,1076.397,0,0,0,0,100,0),
(@PATH,2,7716.69,-2373.623,1076.546,0,0,0,0,100,0),
(@PATH,3,7713.94,-2375.873,1077.046,0,0,0,0,100,0),
(@PATH,4,7712.434,-2377.115,1077.223,0,0,0,0,100,0),
(@PATH,5,7708.74,-2383.643,1077.744,0,0,0,0,100,0),
(@PATH,6,7707.24,-2390.143,1078.244,0,0,0,0,100,0),
(@PATH,7,7708.308,-2384.875,1077.819,0,0,0,0,100,0),
(@PATH,8,7709.108,-2383.253,1077.683,0,0,0,0,100,0),
(@PATH,9,7712.659,-2376.858,1077.257,0,0,0,0,100,0),
(@PATH,10,7715.409,-2374.608,1076.757,0,0,0,0,100,0),
(@PATH,11,7716.896,-2373.736,1076.646,0,0,0,0,100,0),
(@PATH,12,7721.577,-2372.192,1075.534,0,0,0,0,100,0),
(@PATH,13,7723.327,-2371.942,1075.284,0,0,0,0,100,0),
(@PATH,14,7721.291,-2372.098,1075.588,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D000009FFF3 .go 7717.821 -2373.15 1076.397

-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC := 148797;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7716.721,`position_y`=-2417.521,`position_z`=1078.866 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7716.721,-2417.521,1078.866,0,0,0,0,100,0),
(@PATH,2,7721.32,-2423.333,1078.504,0,0,0,0,100,0),
(@PATH,3,7726.283,-2426.826,1078.622,0,0,0,0,100,0),
(@PATH,4,7734.296,-2428.488,1079.077,0,0,0,0,100,0),
(@PATH,5,7741.796,-2428.331,1079.079,0,0,0,0,100,0),
(@PATH,6,7734.083,-2428.462,1078.925,0,0,0,0,100,0),
(@PATH,7,7726.114,-2426.458,1078.75,0,0,0,0,100,0),
(@PATH,8,7721.104,-2423.094,1078.602,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D00000A6AE6 .go 7716.721 -2417.521 1078.866

-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC := 148803;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7708.882,`position_y`=-2383.844,`position_z`=1077.573 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7708.882,-2383.844,1077.573,0,0,0,0,100,0),
(@PATH,2,7712.606,-2376.904,1077.29,0,0,0,0,100,0),
(@PATH,3,7715.585,-2374.437,1076.769,0,0,0,0,100,0),
(@PATH,4,7717.085,-2373.687,1076.519,0,0,0,0,100,0),
(@PATH,5,7721.5,-2372.127,1075.651,0,0,0,0,100,0),
(@PATH,6,7723.25,-2371.877,1075.401,0,0,0,0,100,0),
(@PATH,7,7722.302,-2371.998,1075.731,0,0,0,0,100,0),
(@PATH,8,7716.878,-2373.573,1076.597,0,0,0,0,100,0),
(@PATH,9,7713.878,-2375.823,1077.097,0,0,0,0,100,0),
(@PATH,10,7712.221,-2377.236,1077.321,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D00008B704D .go 7708.882 -2383.844 1077.573

-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC := 148793;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7810.335,`position_y`=-2396.439,`position_z`=1093.944 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7810.335,-2396.439,1093.944,0,0,0,0,100,0),
(@PATH,2,7811.585,-2396.189,1094.694,0,0,0,0,100,0),
(@PATH,3,7817.335,-2396.189,1097.694,0,0,0,0,100,0),
(@PATH,4,7833.19,-2401.916,1106.79,0,0,0,0,100,0),
(@PATH,5,7830.69,-2400.916,1105.54,0,0,0,0,100,0),
(@PATH,6,7828.484,-2400.28,1103.903,0,0,0,0,100,0),
(@PATH,7,7825.484,-2399.28,1102.153,0,0,0,0,100,0),
(@PATH,8,7822.984,-2398.28,1100.653,0,0,0,0,100,0),
(@PATH,9,7820.234,-2397.28,1099.653,0,0,0,0,100,0),
(@PATH,10,7818.22,-2396.551,1098.187,0,0,0,0,100,0),
(@PATH,11,7811.454,-2396.305,1094.445,0,0,0,0,100,0),
(@PATH,12,7809.454,-2396.555,1093.445,0,0,0,0,100,0),
(@PATH,13,7806.704,-2396.805,1092.195,0,0,0,0,100,0),
(@PATH,14,7804.516,-2397.255,1090.906,0,0,0,0,100,0),
(@PATH,15,7802.516,-2397.755,1089.906,0,0,0,0,100,0),
(@PATH,16,7799.766,-2398.255,1088.656,0,0,0,0,100,0),
(@PATH,17,7802.344,-2397.896,1089.801,0,0,0,0,100,0),
(@PATH,18,7804.471,-2397.053,1090.839,0,0,0,0,100,0),
(@PATH,19,7808.221,-2396.803,1092.839,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D000288E709 .go 7810.335 -2396.439 1093.944

-- Pathing for  Entry: 31693 'TDB FORMAT' 
SET @NPC :=  148792;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7594.389,`position_y`=-2281.263,`position_z`=940.06 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7594.389,-2281.263,940.06,0,0,0,0,100,0),
(@PATH,2,7590.639,-2284.263,935.81,0,0,0,0,100,0),
(@PATH,3,7583.639,-2287.263,929.31,0,0,0,0,100,0),
(@PATH,4,7587.911,-2285.628,933.4109,0,0,0,0,100,0),
(@PATH,5,7594.338,-2281.126,940.1424,0,0,0,0,100,0),
(@PATH,6,7596.588,-2278.876,942.6424,0,0,0,0,100,0),
(@PATH,7,7602.338,-2272.376,949.6424,0,0,0,0,100,0),
(@PATH,8,7606.058,-2268.674,953.5016,0,0,0,0,100,0),
(@PATH,9,7607.808,-2266.674,955.2516,0,0,0,0,100,0),
(@PATH,10,7612.058,-2262.424,961.2516,0,0,0,0,100,0),
(@PATH,11,7612.923,-2261.475,962.8279,0,0,0,0,100,0),
(@PATH,12,7617.423,-2259.975,966.5779,0,0,0,0,100,0),
(@PATH,13,7622.441,-2258.586,971.277,0,0,0,0,100,0),
(@PATH,14,7623.191,-2258.336,972.277,0,0,0,0,100,0),
(@PATH,15,7628.944,-2257.969,976.8357,0,0,0,0,100,0),
(@PATH,16,7639.093,-2262.12,985.2325,0,0,0,0,100,0),
(@PATH,17,7645.936,-2263.47,991.0493,0,0,0,0,100,0),
(@PATH,18,7653.126,-2270.09,997.1328,0,0,0,0,100,0),
(@PATH,19,7649.956,-2266.439,994.4049,0,0,0,0,100,0),
(@PATH,20,7645.541,-2263.432,990.6445,0,0,0,0,100,0),
(@PATH,21,7636.538,-2260.767,982.4916,0,0,0,0,100,0),
(@PATH,22,7633.74,-2259.331,980.5753,0,0,0,0,100,0),
(@PATH,23,7628.049,-2257.727,975.717,0,0,0,0,100,0),
(@PATH,24,7614.933,-2261.062,964.6306,0,0,0,0,100,0),
(@PATH,25,7608.933,-2265.812,956.8806,0,0,0,0,100,0),
(@PATH,26,7596.662,-2278.802,942.7743,0,0,0,0,100,0);
-- 0x203CD047601EF340000C1D000408E709 .go 7594.389 -2281.263 940.06

-- Fix https://github.com/TrinityCore/TrinityCore/issues/16915 , thanks to Dalaranwow!

-- Saving Sharpbeak
-- Sharpbeak's Cage SAI
SET @ENTRY := 144070;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,70,0,100,0,2,0,0,0,45,1,0,0,0,0,0,19,8023,20,0,0,0,0,0,"Sharpbeak's Cage - On Gameobject State Changed - Set Data 1 0");
UPDATE `creature` SET `spawntimesecs`=60 WHERE  `guid`=92997;

-- Sharpbeak SAI
SET @ENTRY := 8023;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,1,0,0,0,91,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak - On Data Set 1 0 - Remove Flag Standstate Dead"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,53,0,8023,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak - On Data Set 1 0 - Start Waypoint"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak - On Data Set 1 0 - Set Active On"),
(@ENTRY,0,3,0,58,0,100,0,1,8023,0,0,12,8024,3,120000,0,0,0,8,0,0,0,-150.362167,-3511.225830,274.172699,4.062795,"Sharpbeak - On Waypoint 1 Reached - Summon Creature 'Sharpbeak's Father'"),
(@ENTRY,0,4,0,38,0,100,0,2,0,0,0,29,2,270,0,0,0,0,19,8024,10,0,0,0,0,0,"Sharpbeak - On Data Set 2 0 - Start Follow Closest Creature 'Sharpbeak's Father'"),
(@ENTRY,0,5,0,38,0,100,0,3,0,0,0,1,0,0,0,0,0,0,21,50,0,0,0,0,0,0,"Sharpbeak - On Data Set 3 0 - Say Line 0"),
(@ENTRY,0,6,0,38,0,100,0,4,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak - On Data Set 4 0 - Despawn Instant");

-- waypoints
DELETE FROM `waypoints` WHERE `entry` IN (8023, 8024);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(8023, 1, -229.494385, -3623.708740, 237.462433, 'Sharpbeak'),
(8024, 1, -230.034515, -3627.350098, 237.616638, 'Sharpbeak father'),
(8024, 2, -244.359818, -3638.187500, 236.659546, 'Sharpbeak father'),
(8024, 3, -227.883606, -3624.622314, 237.235733, 'Sharpbeak father'),
(8024, 4, -122.637520, -3502.211914, 256.078064, 'Sharpbeak father');

UPDATE `creature_template` SET `InhabitType`=4, `speed_walk`=2 WHERE  `entry` IN (8024, 8025, 8023);

-- Sharpbeak's Father SAI
SET @ENTRY := 8024;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,54,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Father - On Just Summoned - Set Active On"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,53,0,8024,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Father - On Just Summoned - Start Waypoint"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,12,8025,3,120000,0,0,0,8,0,0,0,-168.569000,-3509.420166,276.238770,4.320847,"Sharpbeak's Father - On Just Summoned - Summon Creature 'Sharpbeak's Mother'"),
(@ENTRY,0,3,0,40,0,100,0,1,8024,0,0,45,2,0,0,0,0,0,19,8023,20,0,0,0,0,0,"Sharpbeak's Father - On Waypoint 1 Reached - Set Data 2 0"),
(@ENTRY,0,4,0,40,0,100,0,2,8024,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Father - On Waypoint 2 Reached - Pause Waypoint"),
(@ENTRY,0,5,0,40,0,100,0,3,8024,0,0,45,3,0,0,0,0,0,19,8023,20,0,0,0,0,0,"Sharpbeak's Father - On Waypoint 3 Reached - Set Data 3 0"),
(@ENTRY,0,6,7,40,0,100,0,4,8024,0,0,45,4,0,0,0,0,0,9,0,0,100,0,0,0,0,"Sharpbeak's Father - On Waypoint 4 Reached - Set Data 4 0"),
(@ENTRY,0,7,0,61,0,100,0,4,8024,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Father - On Waypoint 4 Reached - Despawn Instant");

-- Sharpbeak's Mother SAI
SET @ENTRY := 8025;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,54,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Mother - On Just Summoned - Set Active On"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,29,2,90,0,0,0,0,19,8024,20,0,0,0,0,0,"Sharpbeak's Mother - On Just Summoned - Start Follow Closest Creature 'Sharpbeak's Father'"),
(@ENTRY,0,2,0,38,0,100,0,4,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sharpbeak's Mother - On Data Set 4 0 - Despawn Instant");

DELETE FROM `creature_text` WHERE `entry`= 8023;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`BroadcastTextId`, `comment`) VALUES
(8023, 0, 0, '%s chirps a goodbye to $n.', 16, 0, 100, 0, 0, 0, 4138, 'Sharpbeak');

-- Thanks to dalaranwow, https://github.com/dalaranwow/TrinityCore/commit/3b9d59ffe167f286435ccc4243c4555192bf6dd5
-- [NPC][WotlK] Library Guardian and Databank, missing spell, wrong movement 
UPDATE `creature_template` SET `InhabitType`=4 WHERE  `entry`=29746;
UPDATE `creature` SET `spawndist`=18 WHERE  `id`=29724 AND `spawndist` >0;
UPDATE `creature` SET `spawndist`=0, `MovementType`=0 WHERE  `id`=29746;

-- Library Guardian SAI
SET @ENTRY := 29724;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,2000,45000,60000,60000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - Out of Combat - Run Script"),
(@ENTRY,0,1,0,4,0,100,0,0,0,0,0,103,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Aggro - Set Rooted Off");

-- Actionlist SAI
SET @ENTRY := 2972400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Script - Set Active On"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,1,0,0,0,0,0,19,29746,25,0,0,0,0,0,"Library Guardian - On Script - Set Orientation Closest Creature 'Databank'"),
(@ENTRY,9,2,0,0,0,100,0,100,100,0,0,103,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Script - Set Rooted On"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,1300,1300,0,0,11,55134,0,0,0,0,0,19,29746,25,0,0,0,0,0,"Library Guardian - On Script - Cast 'Data Stream'"),
(@ENTRY,9,5,0,0,0,100,0,12000,12000,0,0,103,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Script - Set Rooted Off"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Library Guardian - On Script - Set Active Off");

-- Conditions for spellcast and sai execution
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=1 AND `SourceEntry`=29724;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,29724,0,0,29,1,29746,25,0,0,0,0,"","Library Guardian - Run SAI near Creature 'Databank'");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND  `SourceGroup`=1 AND `SourceEntry`=55134;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 55134, 0, 0, 31, 0, 3, 29746, 0, 0, 0, 0, '', 'Data Strea only targets Creature Databank');

DELETE FROM `creature_text` WHERE `entry`=29724;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(29724, 0, 0, 'Defragmenting swap file ...', 12, 0, 100, 1, 0, 0, 30375, 'Library Guardian'),
(29724, 0, 1, 'Running a level three diagnostic ...', 12, 0, 100, 1, 0, 0, 30374, 'Library Guardian'),
(29724, 0, 2, 'Attempting to restore lost data ...', 12, 0, 100, 1, 0, 0, 30377, 'Library Guardian'),
(29724, 0, 3, 'Implementing new security protocols ...', 12, 0, 100, 1, 0, 0, 30378, 'Library Guardian'),
(29724, 0, 4, 'Verifying encryption key ...', 12, 0, 100, 1, 0, 0, 30376, 'Library Guardian'),
(29724, 0, 5, 'Assessing database integrity ...', 12, 0, 100, 1, 0, 0, 30373, 'Library Guardian');

-- https://github.com/dalaranwow/TrinityCore/commit/d5ab68250c8e883a622f437579f428c9d29ef45c
-- [NPC][WotlK] Iron Sentinel <Servant of Loken>, missing spawn 
SET @GUID := 256300;
DELETE FROM `creature` WHERE `guid`=@GUID;
INSERT INTO `creature` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`) VALUES 
(@GUID, 29984, 571, 67, 4446, 1, 1, 27092, 0, 8474.9, -1601.15, 1037.59, 0.126558, 300, 0, 0, 50400, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_addon` WHERE `guid` = @GUID;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(@GUID, 0, 0, 0, 1, 333, "");

-- [NPC][WotlK] Stormforged Iron Giant, missing paths, emote, 1 spawn locs, mini event
-- increase rndmmovement for Stormforged Iron Giant to 26 yards
UPDATE `creature` SET `spawndist`=26 WHERE `id`=29375 AND `spawndist` > 0;

-- spawn two missing Stormforged Iron Giant (one static spawn, one eventspawn with waypoints)
SET @CGUID := 250500; -- SET by Zealot
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+0 AND @CGUID+1;
INSERT INTO `creature` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`) VALUES
(@CGUID+0, 29375, 571, 67, 4495, 1, 1, 27092, 1, 7164.41, -3565.155, 827.6359, 0.5585054, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0), -- Stormforged Iron Giant (Area: Fjorn's Anvil)
(@CGUID+1, 29375, 571, 67, 4495, 1, 1, 27092, 1, 7164.41, -3565.155, 827.6359, 0.5585054, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0); -- Stormforged Iron Giant (Area: Fjorn's Anvil)

-- add roar emotescript for Stormforged Iron Giant every 13 - 16 seconds
SET @MAXGUID := 942; 
SET @SCRIPTID := 11393600; 
DELETE FROM `waypoint_scripts` WHERE `id` IN (@SCRIPTID+0);
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
-- EMOTE (oneshot)
(@SCRIPTID+0, 13, 1, 53, 0, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1));

-- add roar emote for Fjorn on waypoint 7
UPDATE `waypoint_data` SET `action`=@SCRIPTID WHERE  `id`=883080 AND `point`=7;

-- we use wp scripts here to avoid -guid sai scripts
SET @NPC := 143136;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `orientation`, `delay`, `action`) VALUES
(@PATH, 1, 7233.65, -3437.45, 838.956, 2.6529, 15000, @SCRIPTID);

SET @NPC := 143137;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `orientation`, `delay`, `action`) VALUES
(@PATH, 1, 7181, -3349.72, 848.463, 1.09956, 4000, 0),
(@PATH, 2, 7181, -3349.72, 848.463, 1.09956, 15000, @SCRIPTID);

SET @NPC := @CGUID+0;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `orientation`, `delay`, `action`) VALUES
(@PATH, 1, 7164.41, -3565.16, 827.636, 0.558505, 15000, @SCRIPTID);

UPDATE `waypoint_data` SET `action_chance`=100 WHERE `action` IN (@SCRIPTID);

-- wps for eventspawn of Stormforged Iron Giant
-- Pathing for  Entry: Entry: 'TDB FORMAT' 
SET @NPC := @CGUID+1;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=7199.968,`position_y`=-3600.629,`position_z`=827.002 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7199.968,-3600.629,827.002,0,0,0,0,100,0), -- 10:38:27
(@PATH,2,7189.104,-3563.368,827.3765,0,0,0,0,100,0), -- 10:38:31
(@PATH,3,7188.668,-3539.213,827.3962,0,0,0,0,100,0), -- 10:38:38
(@PATH,4,7195.016,-3509.103,828.8126,0,0,0,0,100,0), -- 10:38:43
(@PATH,5,7214.329,-3441.1,837.9941,0,0,0,0,100,0), -- 10:38:53
(@PATH,6,7208.163,-3399.303,842.7413,0,0,0,0,100,0), -- 10:39:01
(@PATH,7,7204.88,-3331.804,843.1924,0,0,0,0,100,0), -- 10:39:06
(@PATH,8,7207.137,-3299.045,837.8273,0,0,0,0,100,0), -- 10:39:16
(@PATH,9,7231.24,-3264.245,837.8268,0,0,0,0,100,0), -- 10:39:22
(@PATH,10,7253.341,-3234.825,837.8268,0,0,0,0,100,0), -- 10:39:27
(@PATH,11,7269.063,-3209.878,837.8268,0,0,0,0,100,0), -- 10:39:31
(@PATH,12,7296.788,-3181.641,837.8268,0,0,0,0,100,0), -- 10:39:36
(@PATH,13,7337.208,-3163.068,837.8268,0,0,0,0,100,0), -- 10:39:44
(@PATH,14,7364.17,-3148.588,837.8268,0,0,0,0,100,0), -- 10:39:47
(@PATH,15,7395.288,-3140.528,837.8268,0,0,0,0,100,0), -- 10:39:52
(@PATH,16,7421.594,-3121.072,837.8268,0,5000,0,1188,100,0); -- 10:39:57
-- 0x20197C47601CAFC00000360000087213 .go 7199.968 -3600.629 827.002

-- https://github.com/dalaranwow/TrinityCore/blob/cca6bfee7b1649bd53b06d25f5658602df789ba1/sql/updates/world/3.3.5/2017_01_26_08_world.sql
-- [Quest][WotlK] The Last of Her Kind
-- Harnessed Icemaw Matriarch SAI
SET @ENTRY := 30468;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0 AND `id`=4;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,4,0,28,0,100,0,0,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Harnessed Icemaw Matriarch - On Passenger Removed - Despawn Instant");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=29563 AND `SourceEntry`=56795;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(18, 29563, 56795, 0, 0, 9, 0, 12983, 0, 0, 0, 0, 0, '', 'Required quest \'The Last of Her Kind\' active for spellclick');

-- Son of Hodir SAI -- Add OOC Emote
SET @ENTRY := 29809;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,3000,30000,30000,30000,11,55524,0,0,0,0,0,1,0,0,0,0,0,0,0,"Son of Hodir - Out of Combat - Cast 'Hurl Ice Club2'");

-- https://github.com/dalaranwow/TrinityCore/blob/cb466b7543e689151b67d41c795b321c7dbdcaf3/sql/updates/world/3.3.5/2017_01_26_06_world.sql

-- Plains Mammoth - Increase Randommove + Add script for emote
UPDATE `creature` SET `spawndist`=10 WHERE `id`=30448 AND `spawndist`=5;
UPDATE `creature` SET `spawndist`=10, `MovementType`=1 WHERE  `guid`=146606 and id = 30448;

-- Plains Mammoth SAI
SET @ENTRY := 30448;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0 AND `id`>=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,1,0,1,0,100,0,4000,45000,45000,45000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - Out of Combat - Run Script"),
(@ENTRY,0,2,0,4,0,100,0,0,0,0,0,103,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Aggro - Set Rooted Off");

-- Actionlist SAI
SET @ENTRY := 3044800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Script - Set Active On"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,103,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Script - Set Rooted On"),
(@ENTRY,9,2,0,0,0,100,0,100,100,0,0,5,377,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Script - Play Emote 377"),
(@ENTRY,9,3,0,0,0,100,0,5000,5000,0,0,103,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Script - Set Rooted Off"),
(@ENTRY,9,4,0,0,0,100,0,0,0,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plains Mammoth - On Script - Set Active Off");

-- https://github.com/dalaranwow/TrinityCore/commit/6485a4afd88fd2097d8f930802e75270fc855d0d
UPDATE `creature` SET `spawndist`=10, `MovementType`=1 WHERE  `id`=30160;

-- Fix https://github.com/TrinityCore/TrinityCore/issues/18939
-- Thrallmar Peon Worker Script 
SET @MAXGUID := 938; 
SET @SCRIPTID := 5757600; 
DELETE FROM `waypoint_scripts` WHERE `id` IN (@SCRIPTID+0,@SCRIPTID+1);
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
(@SCRIPTID+0, 3, 1, 233, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+0, 57, 1, 0, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+1, 3, 1, 69, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+1, 57, 1, 0, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1));

SET @NPC := 102042;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `orientation`, `delay`, `action`) VALUES
(@PATH, 1, 23.7, 2677.07, 76.7947, 4.65552, 0, 0),
(@PATH, 2, 29.5186, 2662.79, 75.5889, 5.97498, 0, 0),
(@PATH, 3, 36.7549, 2663.57, 76.6482, 0.389553, 0, 0),
(@PATH, 4, 57.1088, 2670.26, 79.1019, 5.93525, 0, 0),
(@PATH, 5, 65.1478, 2663.53, 79.9572, 5.34777, 0, 0),
(@PATH, 6, 68.2604, 2653.86, 80.6543, 4.51761, 0, 0),
(@PATH, 7, 66.6964, 2647.89, 79.1051, 4.01889, 60000, @SCRIPTID+0),
(@PATH, 8, 69.3452, 2654.17, 80.9377, 1.86925, 0, 0),
(@PATH, 9, 65.8566, 2661.81, 80.4231, 2.45123, 0, 0),
(@PATH, 10, 54.4738, 2667.41, 78.6493, 3.32538, 0, 0),
(@PATH, 11, 41.6966, 2665.8, 77.3675, 3.71022, 0, 0),
(@PATH, 12, 31.8775, 2658.08, 75.2857, 4.39666, 0, 0),
(@PATH, 13, 32.2505, 2643.01, 74.3166, 5.25981, 0, 0),
(@PATH, 14, 35.6496, 2635.57, 75.9404, 5.43495, 0, 0),
(@PATH, 15, 41.5567, 2626.67, 74.4366, 6.22431, 0, 0),
(@PATH, 16, 45.3727, 2626.16, 73.861, 0.135119, 60000, @SCRIPTID+1),
(@PATH, 17, 42.0584, 2627.08, 74.4471, 2.53294, 0, 0),
(@PATH, 18, 33.2927, 2635.99, 75.411, 2.20307, 0, 0),
(@PATH, 19, 32.4661, 2641.63, 74.3467, 1.65329, 0, 0),
(@PATH, 20, 27.458, 2663.08, 75.3224, 1.86142, 0, 0),
(@PATH, 21, 24.2932, 2669.6, 76.2387, 1.78838, 0, 0),
(@PATH, 22, 23.1339, 2680.03, 76.8103, 0.750085, 0, 0),
(@PATH, 23, 26.3861, 2681.9, 77.2808, 0.227795, 60000, @SCRIPTID+1);

UPDATE `waypoint_data` SET `action_chance`=100 WHERE `action` IN (@SCRIPTID+0,@SCRIPTID+1);

-- https://github.com/dalaranwow/TrinityCore/commit/f43a940b544906ddbede269e99be68bf0ec05f43
DELETE FROM `disables` WHERE `entry`= 45425;
INSERT INTO `disables` (`sourceType`,`entry`,`flags`,`params_0`,`params_1`,`comment`) VALUES
(0, 45425, 64, '', '', 'Ignore LOS for Shoot (Dummy)');

-- https://github.com/dalaranwow/TrinityCore/commit/533749bfa271bdb681b3cbb2c6706ef6f7bf7da2
-- Civilian Recruit Scripts in Building
DELETE FROM `creature_addon` WHERE `guid`=145988;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(117789, 0, 0, 3, 0, 0, NULL);

-- Civilian Recruit SAI
SET @GUID := -145993;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=25317;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GUID AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GUID,0,0,0,1,0,100,0,8000,8000,12000,12000,5,36,0,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - Out of Combat - Play Emote 36"),
(@GUID,0,1,0,1,0,100,0,35000,40000,35000,40000,80,11779400,0,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - Out of Combat - Run Script");

-- Actionlist SAI
SET @ENTRY := 11779400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 1"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,10,117790,25317,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 2"),
(@ENTRY,9,2,0,0,0,100,0,6000,6000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 3");

-- Civilian Recruit SAI
SET @GUID := -145987;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=25317;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GUID AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GUID,0,0,0,1,0,100,0,8000,8000,12000,12000,5,36,0,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - Out of Combat - Play Emote 36"),
(@GUID,0,1,0,1,0,100,0,35000,40000,35000,40000,80,11778800,0,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - Out of Combat - Run Script");

-- Actionlist SAI
SET @ENTRY := 11778800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 1"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,10,117789,25317,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 2"),
(@ENTRY,9,2,0,0,0,100,0,6000,6000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Civilian Recruit - On Script - Say Line 3");


DELETE FROM `creature_text` WHERE `entry`=25317 AND `groupid` >=1;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- First text
(25317, 1, 0, 'This is harder than it looks.', 12, 7, 100, 0, 0, 0, 26488, 'Civilian Recruit'),
(25317, 1, 1, 'Did that seem okay?', 12, 7, 100, 0, 0, 0, 26487, 'Civilian Recruit'),
(25317, 1, 2, 'My arm is getting sore.', 12, 7, 100, 0, 0, 0, 26491, 'Civilian Recruit'),
(25317, 1, 3, 'Hmm....', 12, 7, 100, 0, 0, 0, 26492, 'Civilian Recruit'),
(25317, 1, 4, 'I think I\'m starting to get the hang of this.', 12, 7, 100, 0, 0, 0, 26490, 'Civilian Recruit'),
(25317, 1, 5, 'This isn\'t easy.', 12, 7, 100, 0, 0, 0, 26489, 'Civilian Recruit'),
-- reaction
(25317, 2, 0, 'You aren\'t going to let me sleep are you?', 12, 7, 100, 0, 0, 0, 26497, 'Civilian Recruit'),
(25317, 2, 1, 'Are you going to be doing this for much longer?', 12, 7, 100, 0, 0, 0, 26502, 'Civilian Recruit'),
(25317, 2, 2, 'Could you try to be a little quieter?', 12, 7, 100, 0, 0, 0, 26500, 'Civilian Recruit'),
-- reaction 2
(25317, 3, 0, 'I just don\'t want to let anyone down.', 12, 7, 100, 0, 0, 0, 26494, 'Civilian Recruit'),
(25317, 3, 1, 'I need to do something to keep myself busy.', 12, 7, 100, 0, 0, 0, 26495, 'Civilian Recruit'),
(25317, 3, 2, 'I\'m just nervous.  Sorry.', 12, 7, 100, 0, 0, 0, 26493, 'Civilian Recruit');

-- https://github.com/dalaranwow/TrinityCore/commit/31d77d277367aec8e8ec7ef869b67fb1e1474997
-- [NPC][WotlK] Icetip Crawler, missing wps 
UPDATE `creature` SET `spawntimesecs`=30 WHERE `id`=29461;

-- Pathing for  Entry: Entry: 'TDB FORMAT' 
SET @NPC := 130984;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=6814.004,`position_y`=-1511.774,`position_z`=357.4496 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,6814.004,-1511.774,357.4496,0,0,0,0,100,0), -- 10:52:21
(@PATH,2,6810.2,-1512.353,357.7509,0,0,0,0,100,0), -- 10:52:21
(@PATH,3,6806.395,-1512.932,358.0522,0,0,0,0,100,0), -- 10:52:21
(@PATH,4,6802.339,-1513.548,358.3836,0,0,0,0,100,0), -- 10:52:21
(@PATH,5,6801.33,-1513.702,358.5244,0,0,0,0,100,0), -- 10:52:21
(@PATH,6,6789.786,-1515.457,360.9578,0,0,0,0,100,0), -- 10:52:21
(@PATH,7,6784.221,-1516.303,362.3386,0,0,0,0,100,0), -- 10:52:21
(@PATH,8,6781.355,-1516.739,362.8942,0,0,0,0,100,0), -- 10:52:21
(@PATH,9,6781.355,-1516.739,362.8942,0,0,0,0,100,0), -- 10:52:21
(@PATH,10,6793.878,-1503.023,360.585,0,0,0,0,100,0), -- 10:52:22
(@PATH,11,6808.696,-1469.133,360.8632,0,0,0,0,100,0), -- 10:52:25
(@PATH,12,6826.229,-1482.206,358.68,0,0,0,0,100,0), -- 10:52:30
(@PATH,13,6838.727,-1490.121,357.8129,0,0,0,0,100,0), -- 10:52:33
(@PATH,14,6827.854,-1522.164,356.5045,0,0,0,0,100,0), -- 10:52:35
(@PATH,15,6795.745,-1523.78,359.4385,0,0,0,0,100,0), -- 10:52:40
(@PATH,16,6784.725,-1496.301,362.6828,0,0,0,0,100,0), -- 10:52:44
(@PATH,17,6790.426,-1482.139,362.364,0,0,0,0,100,0), -- 10:52:49
(@PATH,18,6815.653,-1470.522,360.2863,0,0,0,0,100,0), -- 10:52:50
(@PATH,19,6797.456,-1477.746,361.3162,0,0,0,0,100,0), -- 10:52:54
(@PATH,20,6785.462,-1486.006,363.1354,0,0,0,0,100,0), -- 10:52:56
(@PATH,21,6793.262,-1522.423,360.3156,0,0,0,0,100,0), -- 10:52:58
(@PATH,22,6823.55,-1524.131,356.6426,0,0,0,0,100,0), -- 10:53:02
(@PATH,23,6843.175,-1494.858,357.0524,0,0,0,0,100,0), -- 10:53:07
(@PATH,24,6833.277,-1487.401,358.1684,0,0,0,0,100,0), -- 10:53:10
(@PATH,25,6812.998,-1465.577,360.3898,0,0,0,0,100,0), -- 10:53:13
(@PATH,26,6801.987,-1490.344,359.7208,0,0,0,0,100,0), -- 10:53:17
(@PATH,27,6782.202,-1516.059,363.032,0,0,0,0,100,0), -- 10:53:20
(@PATH,28,6806.502,-1512.667,358.5165,0,0,0,0,100,0), -- 10:53:24
(@PATH,29,6814.063,-1530.013,356.8398,0,0,0,0,100,0), -- 10:53:27
(@PATH,30,6824.634,-1510.613,356.9213,0,0,0,0,100,0), -- 10:53:30
(@PATH,31,6836.296,-1503.345,356.6489,0,0,0,0,100,0), -- 10:53:32
(@PATH,32,6836.358,-1477.138,359.5229,0,0,0,0,100,0), -- 10:53:35
(@PATH,33,6808.336,-1490.938,358.9387,0,0,0,0,100,0), -- 10:53:38
(@PATH,34,6832.787,-1476.135,359.6523,0,0,0,0,100,0), -- 10:53:44
(@PATH,35,6838.889,-1497.83,357.3196,0,0,0,0,100,0), -- 10:53:47
(@PATH,36,6829.207,-1507.01,356.8817,0,0,0,0,100,0), -- 10:53:49
(@PATH,37,6814.724,-1529.365,356.7092,0,0,0,0,100,0), -- 10:53:52
(@PATH,38,6812.575,-1522.472,357.1874,0,0,0,0,100,0), -- 10:53:54
(@PATH,39,6784.434,-1516.24,362.6053,0,0,0,0,100,0), -- 10:53:57
(@PATH,40,6794.039,-1502.998,360.5512,0,0,0,0,100,0), -- 10:54:00
(@PATH,41,6808.574,-1469.268,360.8869,0,0,0,0,100,0), -- 10:54:04
(@PATH,42,6808.625,-1460.67,362.2088,0,0,0,0,100,0), -- 10:54:11
(@PATH,43,6810.634,-1463.169,361.2263,0,0,0,0,100,0), -- 10:54:11
(@PATH,44,6812.643,-1465.669,360.2439,0,0,0,0,100,0), -- 10:54:11
(@PATH,45,6816.484,-1470.447,359.8347,0,0,0,0,100,0), -- 10:54:11
(@PATH,46,6819.701,-1474.449,359.0368,0,0,0,0,100,0), -- 10:54:11
(@PATH,47,6823.676,-1479.394,358.4186,0,0,0,0,100,0), -- 10:54:11
(@PATH,48,6826.224,-1482.563,358.3096,0,0,0,0,100,0), -- 10:54:11
(@PATH,49,6827.373,-1483.991,358.2897,0,0,0,0,100,0), -- 10:54:11
(@PATH,50,6827.373,-1483.991,358.2897,0,0,0,0,100,0), -- 10:54:11
(@PATH,51,6838.721,-1490.242,357.9254,0,0,0,0,100,0), -- 10:54:11
(@PATH,52,6827.905,-1522.265,356.5711,0,0,0,0,100,0), -- 10:54:14
(@PATH,53,6795.894,-1523.824,359.4495,0,0,0,0,100,0), -- 10:54:18
(@PATH,54,6784.802,-1496.306,362.7766,0,0,0,0,100,0), -- 10:54:22
(@PATH,55,6778.99,-1491.735,364.0555,0,0,0,0,100,0), -- 10:54:31
(@PATH,56,6782.148,-1489.045,363.4904,0,0,0,0,100,0), -- 10:54:31
(@PATH,57,6785.307,-1486.355,362.9253,0,0,0,0,100,0), -- 10:54:31
(@PATH,58,6790.332,-1482.075,362.1711,0,0,0,0,100,0), -- 10:54:31
(@PATH,59,6793.181,-1479.649,361.8699,0,0,0,0,100,0), -- 10:54:31
(@PATH,60,6797.31,-1477.95,361.2349,0,0,0,0,100,0), -- 10:54:31
(@PATH,61,6805.801,-1474.457,360.2778,0,0,0,0,100,0), -- 10:54:31
(@PATH,62,6809.209,-1473.054,359.9501,0,0,0,0,100,0), -- 10:54:31
(@PATH,63,6815.562,-1470.441,359.8851,0,0,0,0,100,0), -- 10:54:31
(@PATH,64,6818.231,-1469.342,359.7625,0,0,0,0,100,0), -- 10:54:31
(@PATH,65,6818.231,-1469.342,359.7625,0,0,0,0,100,0), -- 10:54:31
(@PATH,66,6797.456,-1477.746,361.3162,0,0,0,0,100,0), -- 10:54:33
(@PATH,67,6785.66,-1486.012,362.9827,0,0,0,0,100,0), -- 10:54:35
(@PATH,68,6793.356,-1522.528,360.3792,0,0,0,0,100,0), -- 10:54:37
(@PATH,69,6791.57,-1481.02,362.0597,0,0,0,0,100,0), -- 10:54:41
(@PATH,70,6790.951,-1481.548,362.1327,0,0,0,0,100,0), -- 10:54:41
(@PATH,71,6790.332,-1482.075,362.2057,0,0,0,0,100,0), -- 10:54:41
(@PATH,72,6785.307,-1486.355,362.9334,0,0,0,0,100,0), -- 10:54:41
(@PATH,73,6782.148,-1489.045,363.4904,0,0,0,0,100,0), -- 10:54:41
(@PATH,74,6784.519,-1496.181,362.5059,0,0,0,0,100,0), -- 10:54:41
(@PATH,75,6786.498,-1502.14,361.9723,0,0,0,0,100,0), -- 10:54:41
(@PATH,76,6786.958,-1503.523,361.7775,0,0,0,0,100,0), -- 10:54:41
(@PATH,77,6789.31,-1510.604,361.1386,0,0,0,0,100,0), -- 10:54:41
(@PATH,78,6792.167,-1519.207,360.3965,0,0,0,0,100,0), -- 10:54:41
(@PATH,79,6793.314,-1522.66,359.89,0,0,0,0,100,0), -- 10:54:41
(@PATH,80,6793.762,-1524.008,359.6256,0,0,0,0,100,0), -- 10:54:41
(@PATH,81,6793.762,-1524.008,359.6256,0,0,0,0,100,0), -- 10:54:41
(@PATH,82,6823.603,-1524.051,356.6179,0,0,0,0,100,0), -- 10:54:41
(@PATH,83,6843.219,-1494.841,357.1965,0,0,0,0,100,0), -- 10:54:45
(@PATH,84,6833.68,-1487.476,358.3132,0,0,0,0,100,0), -- 10:54:50
(@PATH,85,6812.612,-1457.098,361.9977,0,0,0,0,100,0), -- 10:55:01
(@PATH,86,6810.634,-1463.169,361.2263,0,0,0,0,100,0), -- 10:55:01
(@PATH,87,6808.656,-1469.241,360.455,0,0,0,0,100,0), -- 10:55:01
(@PATH,88,6808.28,-1470.396,360.3689,0,0,0,0,100,0), -- 10:55:01
(@PATH,89,6803.619,-1484.702,359.5985,0,0,0,0,100,0), -- 10:55:01
(@PATH,90,6803.18,-1486.049,359.6079,0,0,0,0,100,0), -- 10:55:01
(@PATH,91,6802.833,-1487.115,359.5579,0,0,0,0,100,0), -- 10:55:01
(@PATH,92,6801.731,-1490.498,359.5443,0,0,0,0,100,0), -- 10:55:01
(@PATH,93,6799.87,-1496.212,359.5768,0,0,0,0,100,0), -- 10:55:01
(@PATH,94,6799.87,-1496.212,359.5768,0,0,0,0,100,0), -- 10:55:01
(@PATH,95,6782.041,-1515.926,362.9824,0,0,0,0,100,0), -- 10:55:01
(@PATH,96,6806.451,-1512.805,358.4553,0,0,0,0,100,0), -- 10:55:03
(@PATH,97,6813.875,-1529.851,356.8897,0,0,0,0,100,0), -- 10:55:07
(@PATH,98,6824.628,-1510.521,356.8281,0,0,0,0,100,0), -- 10:55:09
(@PATH,99,6836.209,-1503.343,356.6615,0,0,0,0,100,0), -- 10:55:12
(@PATH,100,6836.404,-1476.961,359.5291,0,0,0,0,100,0), -- 10:55:14
(@PATH,101,6808.285,-1490.99,358.8646,0,0,0,0,100,0), -- 10:55:17
(@PATH,102,6842.629,-1500.061,356.8884,0,0,0,0,100,0), -- 10:55:32
(@PATH,103,6839.421,-1501.737,356.7122,0,0,0,0,100,0), -- 10:55:32
(@PATH,104,6836.213,-1503.412,356.5361,0,0,0,0,100,0), -- 10:55:32
(@PATH,105,6828.959,-1507.202,356.5681,0,0,0,0,100,0), -- 10:55:32
(@PATH,106,6825.497,-1509.011,356.7193,0,0,0,0,100,0), -- 10:55:32
(@PATH,107,6825.497,-1509.011,356.7193,0,0,0,0,100,0), -- 10:55:32
(@PATH,108,6814.79,-1529.54,356.5034,0,0,0,0,100,0), -- 10:55:33
(@PATH,109,6812.681,-1522.55,357.1689,0,0,0,0,100,0), -- 10:55:35
(@PATH,110,6784.553,-1516.297,362.4128,0,0,0,0,100,0); -- 10:55:37

-- Pathing for  Entry: Entry: 'TDB FORMAT' 
SET @NPC := 149895;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=6684.895,`position_y`=-1454.917,`position_z`=383.6346 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,6684.895,-1454.917,383.6346,0,0,0,0,100,0), -- 10:53:05
(@PATH,2,6686.636,-1464.036,382.3718,0,0,0,0,100,0), -- 10:53:05
(@PATH,3,6688.377,-1473.154,381.109,0,0,0,0,100,0), -- 10:53:05
(@PATH,4,6688.782,-1475.274,380.6954,0,0,0,0,100,0), -- 10:53:05
(@PATH,5,6690.852,-1486.113,378.6342,0,0,0,0,100,0), -- 10:53:05
(@PATH,6,6691.787,-1491.011,377.8044,0,0,0,0,100,0), -- 10:53:05
(@PATH,7,6691.913,-1491.667,377.6761,0,0,0,0,100,0), -- 10:53:05
(@PATH,8,6691.913,-1491.667,377.6761,0,0,0,0,100,0), -- 10:53:05
(@PATH,9,6696.476,-1509.763,374.3917,0,0,0,0,100,0), -- 10:53:06
(@PATH,10,6692.973,-1497.22,376.7173,0,0,0,0,100,0), -- 10:53:28
(@PATH,11,6691.913,-1491.667,377.6761,0,0,0,0,100,0), -- 10:53:28
(@PATH,12,6690.852,-1486.113,378.6348,0,0,0,0,100,0), -- 10:53:28
(@PATH,13,6688.782,-1475.27,380.6958,0,0,0,0,100,0), -- 10:53:28
(@PATH,14,6688.546,-1474.038,380.9565,0,0,0,0,100,0), -- 10:53:28
(@PATH,15,6686.636,-1464.036,382.3718,0,0,0,0,100,0); -- 10:53:28
-- 0x203AF447601CB90000279D000005D0B1 .go 6684.895 -1454.917 383.6346

-- Pathing for  Entry: Entry: 'TDB FORMAT' 
SET @NPC := 130983;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=6820.013,`position_y`=-1599.169,`position_z`=352.3645 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,6820.013,-1599.169,352.3645,0,0,0,0,100,0), -- 10:52:27
(@PATH,2,6820.104,-1594.376,352.2,0,0,0,0,100,0), -- 10:52:27
(@PATH,3,6820.195,-1589.583,352.0355,0,0,0,0,100,0), -- 10:52:27
(@PATH,4,6820.216,-1588.511,351.9038,0,0,0,0,100,0), -- 10:52:27
(@PATH,5,6820.249,-1586.775,352.0171,0,0,0,0,100,0), -- 10:52:27
(@PATH,6,6820.273,-1585.472,352.0441,0,0,0,0,100,0), -- 10:52:27
(@PATH,7,6820.512,-1572.989,352.7346,0,0,0,0,100,0), -- 10:52:27
(@PATH,8,6820.563,-1570.255,353.1082,0,0,0,0,100,0), -- 10:52:27
(@PATH,9,6820.779,-1558.941,353.6869,0,0,0,0,100,0), -- 10:52:27
(@PATH,10,6820.805,-1557.595,353.732,0,0,0,0,100,0), -- 10:52:27
(@PATH,11,6820.805,-1557.595,353.732,0,0,0,0,100,0), -- 10:52:27
(@PATH,12,6817.6,-1573.382,353.1295,0,0,0,0,100,0), -- 10:52:30
(@PATH,13,6793.767,-1575.178,355.5763,0,0,0,0,100,0), -- 10:52:31
(@PATH,14,6804.454,-1558.247,354.0426,0,0,0,0,100,0), -- 10:52:34
(@PATH,15,6823.396,-1567.967,353.5638,0,0,0,0,100,0), -- 10:52:38
(@PATH,16,6823.622,-1597.669,352.4767,0,0,0,0,100,0), -- 10:52:42
(@PATH,17,6808.817,-1618.017,351.1837,0,0,0,0,100,0), -- 10:52:44
(@PATH,18,6822.837,-1599.973,352.402,0,0,0,0,100,0), -- 10:52:50
(@PATH,19,6830.896,-1576.437,352.2047,0,0,0,0,100,0), -- 10:52:57
(@PATH,20,6827.16,-1572.276,352.8013,0,0,0,0,100,0), -- 10:52:57
(@PATH,21,6823.424,-1568.115,353.3978,0,0,0,0,100,0), -- 10:52:57
(@PATH,22,6819.097,-1563.295,353.3689,0,0,0,0,100,0), -- 10:52:57
(@PATH,23,6815.957,-1559.798,353.5005,0,0,0,0,100,0), -- 10:52:57
(@PATH,24,6812.121,-1555.526,353.9822,0,0,0,0,100,0), -- 10:52:57
(@PATH,25,6809.171,-1552.24,354.3131,0,0,0,0,100,0), -- 10:52:57
(@PATH,26,6808.941,-1551.984,354.3262,0,0,0,0,100,0), -- 10:52:57
(@PATH,27,6808.941,-1551.984,354.3262,0,0,0,0,100,0), -- 10:52:57
(@PATH,28,6793.424,-1572.007,355.5611,0,0,0,0,100,0), -- 10:52:59
(@PATH,29,6816.494,-1575.348,353.2957,0,0,0,0,100,0), -- 10:53:02
(@PATH,30,6820.627,-1558.608,353.9123,0,0,0,0,100,0), -- 10:53:06
(@PATH,31,6820.068,-1589.351,352.332,0,0,0,0,100,0), -- 10:53:07
(@PATH,32,6819.462,-1617.606,351.7835,0,0,0,0,100,0), -- 10:53:12
(@PATH,33,6797.732,-1635.693,349.3093,0,0,0,0,100,0), -- 10:53:15
(@PATH,34,6820.013,-1599.169,352.3645,0,0,0,0,100,0); -- 10:53:28
UPDATE `waypoint_data` SET `move_flag`=1 WHERE `id` IN (980680, 980690, 1520580);

-- https://github.com/dalaranwow/TrinityCore/commit/e2400c6d78605c721ddbeebf51c74a754c04a04d
-- Jimmy the Stable Boy
UPDATE `creature` SET `position_x`=2260.2114, `position_y`=5205.3139, `position_z`=11.4189, `orientation`=5.8917 WHERE  `guid`=145947;
UPDATE `creature` SET `spawndist`=0, `MovementType`=0, `position_x`=2255.84643, `position_y`=5207.1347, `position_z`=11.2519, `orientation`=5.8878 WHERE  `guid`=145992;

-- Jimmy the Stable Boy SAI
SET @ENTRY := 27364;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,34,0,100,0,2,1,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Reached Point 1 - Run Script"),
(@ENTRY,0,1,0,34,0,100,0,2,9,0,0,80,@ENTRY*100+01,0,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Reached Point 9 - Run Script"),
(@ENTRY,0,2,0,34,0,100,0,2,23,0,0,1,4,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Reached Point 23 - Say Line 4"),
(@ENTRY,0,3,0,34,0,100,0,2,27,0,0,28,48478,0,0,0,0,0,10,117793,27365,0,0,0,0,0,"Jimmy the Stable Boy - On Reached Point 27 - Remove Aura 'Transform - Stabled Pet Appearance'");

-- Actionlist SAI
SET @ENTRY := 2736400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,0,6000,0,0,0,0,10,107742,27010,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,75,48478,0,0,0,0,0,10,117793,27365,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Add Aura 'Transform - Stabled Pet Appearance'"),
(@ENTRY,9,3,0,0,0,100,0,6000,6000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Say Line 1");

-- Actionlist SAI
SET @ENTRY := 2736401;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Say Line 2"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Jimmy the Stable Boy - On Script - Say Line 3");

DELETE FROM `waypoint_data` WHERE `id`=1177440;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES 
(1177440, 1, 2260.21, 5205.31, 11.4189, 0, 0, 0, 0, 100, 0),
(1177440, 2, 2260.21, 5205.31, 11.4189, 0, 20000, 0, 0, 100, 0),
(1177440, 3, 2258.42, 5206.38, 11.3041, 0, 0, 0, 0, 100, 0),
(1177440, 4, 2260.78, 5212.97, 11.2323, 0, 0, 0, 0, 100, 0),
(1177440, 5, 2265.26, 5222.43, 11.285, 0, 0, 0, 0, 100, 0),
(1177440, 6, 2275.49, 5231.85, 11.3558, 0, 0, 0, 0, 100, 0),
(1177440, 7, 2284.1, 5249.56, 11.2543, 0, 0, 0, 0, 100, 0),
(1177440, 8, 2285.7, 5265.13, 11.3355, 0, 0, 0, 0, 100, 0),
(1177440, 9, 2285.82, 5269.38, 11.3778, 0, 0, 0, 0, 100, 0),
(1177440, 10, 2288.5, 5271.9, 11.4001, 0, 15000, 0, 0, 100, 0),
(1177440, 11, 2282.43, 5249.42, 11.2138, 0, 0, 0, 0, 100, 0),
(1177440, 12, 2275.66, 5250.47, 11.2624, 0, 0, 0, 0, 100, 0),
(1177440, 13, 2265.76, 5253.98, 7.16312, 0, 0, 0, 0, 100, 0),
(1177440, 14, 2252.91, 5259.53, 10.2728, 0, 0, 0, 0, 100, 0),
(1177440, 15, 2246.47, 5262.26, 11.7963, 0, 0, 0, 0, 100, 0),
(1177440, 16, 2236.65, 5265.97, 9.08925, 0, 0, 0, 0, 100, 0),
(1177440, 17, 2226.8, 5269.61, 7.16322, 0, 0, 0, 0, 100, 0),
(1177440, 18, 2209.85, 5275.66, 10.8121, 0, 0, 0, 0, 100, 0),
(1177440, 19, 2208.18, 5268.86, 10.7009, 0, 0, 0, 0, 100, 0),
(1177440, 20, 2203.12, 5252.14, 10.5775, 0, 0, 0, 0, 100, 0),
(1177440, 21, 2192.52, 5236.56, 10.5738, 0, 0, 0, 0, 100, 0),
(1177440, 22, 2176.46, 5233.64, 11.5804, 0, 0, 0, 0, 100, 0),
(1177440, 23, 2160.99, 5234.54, 17.9753, 0, 0, 0, 0, 100, 0),
(1177440, 24, 2137.26, 5245.75, 19.4465, 0, 0, 0, 0, 100, 0),
(1177440, 25, 2111.31, 5256.28, 19.4465, 0, 0, 0, 0, 100, 0),
(1177440, 26, 2087.92, 5269.58, 19.4465, 0, 0, 0, 0, 100, 0),
(1177440, 27, 2087.94, 5290.85, 19.4495, 0, 0, 0, 0, 100, 0),
(1177440, 28, 2092.85, 5313.56, 19.4495, 0, 5000, 0, 0, 100, 0),
(1177440, 29, 2091.17, 5300.7, 19.4495, 0, 22000, 0, 0, 100, 0),
(1177440, 30, 2086.89, 5273.04, 19.4482, 0, 0, 0, 0, 100, 0),
(1177440, 31, 2108.96, 5257.42, 19.4467, 0, 0, 0, 0, 100, 0),
(1177440, 32, 2138.17, 5245.65, 19.4467, 0, 0, 0, 0, 100, 0),
(1177440, 33, 2157.61, 5237.7, 19.0772, 0, 0, 0, 0, 100, 0),
(1177440, 34, 2177, 5234.21, 11.426, 0, 0, 0, 0, 100, 0),
(1177440, 35, 2194.04, 5236.1, 10.5729, 0, 0, 0, 0, 100, 0),
(1177440, 36, 2202.59, 5250.43, 10.5729, 0, 0, 0, 0, 100, 0),
(1177440, 37, 2210.88, 5273.46, 10.814, 0, 0, 0, 0, 100, 0),
(1177440, 38, 2228.14, 5268.23, 7.1635, 0, 0, 0, 0, 100, 0),
(1177440, 39, 2245.5, 5263.03, 11.7909, 0, 0, 0, 0, 100, 0),
(1177440, 40, 2264.42, 5255.25, 7.16331, 0, 0, 0, 0, 100, 0),
(1177440, 41, 2281.83, 5245.22, 11.317, 0, 0, 0, 0, 100, 0),
(1177440, 42, 2280.49, 5237.07, 11.4029, 0, 0, 0, 0, 100, 0),
(1177440, 43, 2269.62, 5227.42, 11.2536, 0, 0, 0, 0, 100, 0),
(1177440, 44, 2261.16, 5215.02, 11.2106, 0, 0, 0, 0, 100, 0),
(1177440, 45, 2256.64, 5205.68, 11.2277, 0, 0, 0, 0, 100, 0);
DELETE FROM `waypoint_scripts` WHERE `id` IN (1080, 1081);

DELETE FROM `creature_formations` WHERE `leaderGUID`=117744;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES 
(117744, 117744, 0, 0, 7),
(117744, 117793, 7, 359, 7);

DELETE FROM `creature_text` WHERE `entry` IN (27010, 27364);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(27010, 0, 0, 'All these folks are keeping me busy.  Take this horse over to your stables.', 12, 7, 100, 396, 0, 0, 26522, 'Celidh Aletracker to Jimmy the Stable Boy'),
(27364, 0, 0, 'Hey Celidh!  Do you have any pets you want me to take over to the stable?', 12, 7, 100, 0, 0, 0, 26428, 'Jimmy the Stable Boy'),
(27364, 1, 0, 'Let''s go boy.', 12, 7, 100, 0, 0, 0, 26432, 'Jimmy the Stable Boy'),
(27364, 2, 0, 'If I were king, I''d have a horse like you.  I''d make sure you didn''t get lost, and I bet you''d make sure I never got lost.', 12, 7, 100, 0, 0, 0, 26478, 'Jimmy the Stable Boy'),
(27364, 3, 0, 'We''d be best friends.  I bet me and Anduin would also be best friends, too.  Friends are important.  ', 12, 7, 100, 0, 0, 0, 26479, 'Jimmy the Stable Boy'),
(27364, 4, 0, 'I''ll take you to some other horses.  You probably miss being around other horses.  They can be your friend too.', 12, 7, 100, 0, 0, 0, 26486, 'Jimmy the Stable Boy');

DELETE FROM `creature_text` WHERE `entry` = 20088;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(20088,0,0,'%s calls for a nearby slave to assist him!',16,0,100,0,0,0,0,'');
-- Bloodscale Overseer SAI
SET @ENTRY := 20088;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,2000,2000,5000,5000,11,9080,0,0,0,0,0,2,0,0,0,0,0,0,0,"Bloodscale Overseer - In Combat - Cast 'Hamstring'"),
(@ENTRY,0,1,0,0,0,100,0,5000,5000,10000,10000,11,34812,0,0,0,0,0,2,0,0,0,0,0,0,0,"Bloodscale Overseer - In Combat - Cast 'Curse of Nazjatar'"),
(@ENTRY,0,2,3,4,0,100,0,0,0,0,0,39,20,0,0,0,0,0,10,0,18123,0,0,0,0,0,"Bloodscale Overseer - On Aggro - Call For Help"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bloodscale Overseer - On Aggro - Say Line 0");

DELETE FROM `creature_text` WHERE `entry` = 20090;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(20090,0,0,'You are spotted by the %s!',16,0,100,0,0,0,0,'');

-- Bloodscale Sentry SAI
SET @ENTRY := 20090;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,2,0,100,0,0,25,20000,20000,39,50,1,0,0,0,0,11,0,30,0,0,0,0,0,"Bloodscale Sentry - Between 0-25% Health - Call For Help"),
(@ENTRY,0,1,0,4,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bloodscale Sentry - On Aggro - Say Line 0");

-- https://github.com/dalaranwow/dalaran-wow/issues/4359
UPDATE `creature` SET `position_x` = 7313.96 , `position_y` = -1611.88 , `position_z` = 944.59 , `orientation` = 1.798 WHERE `guid` = 149960;
DELETE FROM `gameobject` WHERE `guid`=252555;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(252555, 191726, 571, 0, 0, 1, 1, 7312.415, -1610.487, 944.2941, 4.991644, 0, 0, -0.6018143, 0.7986361, 120, 255, 1); 

-- https://github.com/TrinityCore/TrinityCore/issues/18770
-- Footman Rob + Footman Chuck + Footman George + Footman Mitch - Path and Textevent

-- Footman Rob SAI
SET @ENTRY := 25258;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,34,0,100,1,2,5,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Reached Point 5 - Run Script (No Repeat)"),
(@ENTRY,0,1,0,34,0,100,0,2,9,0,0,80,@ENTRY*100+01,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Reached Point 9 - Run Script"),
(@ENTRY,0,2,0,34,0,100,0,2,10,0,0,80,@ENTRY*100+02,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Reached Point 10 - Run Script"),
(@ENTRY,0,3,0,34,0,100,1,2,15,0,0,12,25260,3,180000,0,0,0,8,0,0,0,2255.77,5186.26,11.4391,1.47479,"Footman Rob - On Reached Point 15 - Summon Creature 'Footman Mitch' (No Repeat)"),
(@ENTRY,0,4,0,34,0,100,0,2,17,0,0,80,@ENTRY*100+03,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Reached Point 17 - Run Script"),
(@ENTRY,0,5,0,17,0,100,0,25260,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Footman Rob - On Summoned Unit - Store Targetlist"),
(@ENTRY,0,6,0,1,0,100,1,100,100,0,0,71,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - Out of Combat - Change Equipment (No Repeat)"),
(@ENTRY,0,7,0,1,0,100,1,100,100,0,0,71,0,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - Out of Combat - Change Equipment (No Repeat)"),
(@ENTRY,0,8,0,1,0,100,1,100,100,0,0,71,0,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - Out of Combat - Change Equipment (No Repeat)");

-- Actionlist SAI -- Stop in Front of house
SET @ENTRY := 2525800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,3000,3000,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,0,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Say Line 0");

-- Actionlist SAI - stop in front of barkeeper
SET @ENTRY := 2525801;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,4000,4000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Say Line 1"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,0,6000,0,0,0,0,10,110613,25245,0,0,0,0,0,"Footman Rob - On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Say Line 2"),
(@ENTRY,9,3,0,0,0,100,0,6000,6000,0,0,1,1,6000,0,0,0,0,10,110613,25245,0,0,0,0,0,"Footman Rob - On Script - Say Line 1"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,5,397,0,0,0,0,0,10,110613,25245,0,0,0,0,0,"Footman Rob - On Script - Play Emote 397"),
(@ENTRY,9,5,0,0,0,100,0,7000,7000,0,0,17,423,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 423"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,7,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,8,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,9,0,0,0,100,0,5000,5000,0,0,17,423,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 423"),
(@ENTRY,9,10,0,0,0,100,0,0,0,0,0,17,423,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 423"),
(@ENTRY,9,11,0,0,0,100,0,22000,22000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 0"),
(@ENTRY,9,12,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 0"),
(@ENTRY,9,13,0,0,0,100,0,0,0,0,0,17,0,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Set Emote State 0"),
(@ENTRY,9,14,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,15,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,16,0,0,0,100,0,0,0,0,0,71,1,0,2703,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,17,0,0,0,100,0,0,0,0,0,71,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,18,0,0,0,100,0,0,0,0,0,71,0,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Change Equipment"),
(@ENTRY,9,19,0,0,0,100,0,0,0,0,0,71,0,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Change Equipment");

-- Actionlist SAI - prepare leave building
SET @ENTRY := 2525802; 
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Say Line 3"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,45,1,1,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Set Data 1 1"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,45,1,1,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Set Data 1 1"),
(@ENTRY,9,3,0,0,0,100,0,3000,3000,0,0,1,0,6000,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,6000,6000,0,0,5,11,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Play Emote 11"),
(@ENTRY,9,5,0,0,0,100,0,0,0,0,0,5,11,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Play Emote 11"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,5,11,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Play Emote 11"),
(@ENTRY,9,7,0,0,0,100,0,1000,1000,0,0,5,274,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Play Emote 274"),
(@ENTRY,9,8,0,0,0,100,0,0,0,0,0,5,274,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Play Emote 274"),
(@ENTRY,9,9,0,0,0,100,0,0,0,0,0,5,274,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Play Emote 274");

-- Actionlist SAI - despawn all member
SET @ENTRY := 2525803;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,41,1,0,0,0,0,0,10,85222,25259,0,0,0,0,0,"Footman Rob - On Script - Despawn Instant"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,41,1,0,0,0,0,0,10,85226,25261,0,0,0,0,0,"Footman Rob - On Script - Despawn Instant"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,41,1,0,0,0,0,0,12,1,0,0,0,0,0,0,"Footman Rob - On Script - Despawn Instant"),
(@ENTRY,9,3,0,0,0,100,0,100,100,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Rob - On Script - Despawn Instant");
UPDATE `creature_template` SET `flags_extra`=2, `InhabitType`=3 WHERE  `entry` IN (25259, 25258, 25261);

-- Footman George SAI
SET @ENTRY := 25259;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.282578,"Footman George - On Data Set 1 1 - Set Orientation 4,282578");

-- Footman Chuck SAI
SET @ENTRY := 25261;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.28258,"Footman Chuck - On Data Set 1 1 - Set Orientation 4.28258");

-- Footman Mitch SAI
SET @ENTRY := 25260;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,63,0,100,0,0,0,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Mitch - On Just Created - Say Line 0"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Footman Mitch - On Just Created - Set Run Off"),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,2264.93652,5221.9863,11.2882,4.3713,"Footman Mitch - On Just Created - Move To Position");

-- texts
DELETE FROM `creature_text` WHERE `entry` IN (25259, 25258, 25261, 25260, 25245);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
-- Footman Rob
(25258, 0, 0, 'One last drink before we head to the front?', 12, 7, 100, 1, 0, 0, 24475, 'Footman Rob'),
(25258, 1, 0, 'Barkeep!  Bring out your strongest ale.  We''re off to the front.', 12, 7, 100, 1, 0, 0, 24478, 'Footman Rob'),
(25258, 2, 0, 'You''ll surely make an exception for us, eh?  We might not come back.', 12, 7, 100, 1, 0, 0, 24479, 'Footman Rob'),
(25258, 3, 0, 'I guess that''s it then.  Off we go.', 12, 7, 100, 1, 0, 0, 24482, 'Footman Rob'),
-- Footman George
(25259, 0, 0, 'Sure, why not?', 12, 7, 100, 6, 0, 0, 24476, 'Footman George'),
-- Footman Chuck
(25261, 0, 0, 'That was the worst beer I''ve ever had.', 12, 7, 100, 0, 0, 0, 24483, 'Footman Chuck'),
-- Footman Mitch
(25260, 0, 0, 'Hey, guys!  Wait up!', 12, 7, 100, 0, 0, 0, 24761, 'Footman Mitch'),
-- James Deacon
(25245, 0, 0, 'You lads know the drill.  No alcohol to servicemen on duty.  You want the general to kill me himself?', 12, 7, 100, 274, 0, 0, 24744, 'James Deacon'),
(25245, 1, 0, 'All right, all right... but you didn''t get these from me!', 12, 7, 100, 6, 0, 0, 24745, 'James Deacon');

-- Formation
DELETE FROM `creature_formations` WHERE `leaderGUID`= 85221;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES 
(85221, 85221, 0, 0, 4),   -- Rob
(85221, 85222, 2, 305, 4), -- George
(85221, 85226, 2, 55, 4);  -- Chuck

-- Waypoints for Rob
SET @NPC := 156168;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES 
(@PATH, 1, 2277.23, 5177.21, 11.34, 0, 0, 0, 0, 100, 0),
(@PATH, 2, 2268.97, 5175.55, 11.1665, 0, 0, 0, 0, 100, 0),
(@PATH, 3, 2254.59, 5188.89, 11.384, 0, 0, 0, 0, 100, 0),
(@PATH, 4, 2254.27, 5196.42, 11.3837, 0, 0, 0, 0, 100, 0),
(@PATH, 5, 2260.67, 5200.36, 11.3711, 0, 0, 0, 0, 100, 0),
(@PATH, 6, 2264.38, 5199.09, 11.3676, 0, 13000, 0, 0, 100, 0), -- stop in front
(@PATH, 7, 2270.18, 5196.94, 12.4959, 0, 0, 0, 0, 100, 0),
(@PATH, 8, 2275.23, 5194.94, 12.49, 0, 0, 0, 0, 100, 0),
(@PATH, 9, 2276.23, 5197.58, 12.4896, 0, 0, 0, 0, 100, 0),
(@PATH, 10, 2279.08, 5206.5, 12.494, 0, 60000, 0, 0, 100, 0), -- stop beer
(@PATH, 11, 2278.6, 5205.3, 12.4926, 0, 16000, 0, 0, 100, 0), -- stop go
(@PATH, 12, 2275.55, 5197.84, 12.4897, 0, 0, 0, 0, 100, 0),
(@PATH, 13, 2275.60, 5194.96, 12.489, 0, 0, 0, 0, 100, 0),
(@PATH, 14, 2265.21, 5198.54, 11.3533, 0, 0, 0, 0, 100, 0),
(@PATH, 15, 2258.23, 5201.82, 11.3182, 0, 0, 0, 0, 100, 0),
(@PATH, 16, 2258.98, 5205.83, 11.322, 0, 0, 0, 0, 100, 0), -- summon friend
(@PATH, 17, 2261, 5212.02, 11.2373, 0, 0, 0, 0, 100, 0),
(@PATH, 18, 2268.17, 5226.07, 11.2421, 0, 5000, 0, 0, 100, 0); -- despawn

-- Gavin Gnarltree SAI
-- https://github.com/dalaranwow/TrinityCore/commit/1367a27b64ef1f5780828ffe0a9acaa852c555df
SET @ENTRY := 225;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0 AND `id`=7;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,7,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Gavin Gnarltree - On Gossip Hello - Pause Waypoint");

-- https://github.com/dalaranwow/TrinityCore/commit/b9bd4cd19856ca902cef1ea70b016ae312a07b52
-- Bailey + Bellinger Event
UPDATE `creature` SET `spawndist`=0, `MovementType`=0 WHERE  `guid` IN (143968, 143989);
DELETE FROM `creature_formations` WHERE `leaderGUID`=114937;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES 
(114937, 114937, 0, 0, 4),
(114937, 114958, 2, 270, 4);

-- Bailey SAI
SET @ENTRY := 29643;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,1000,1000,900000,900000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - Out of Combat - Run Script"),
(@ENTRY,0,1,0,40,0,100,0,4,29643,0,0,80,@ENTRY*100+01,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Waypoint 4 Reached - Run Script"),
(@ENTRY,0,2,0,40,0,100,0,8,29643,0,0,80,@ENTRY*100+02,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Waypoint 8 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,15,29643,0,0,80,@ENTRY*100+03,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Waypoint 15 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 2964300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,1,0,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,6000,6000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 1"),
(@ENTRY,9,3,0,0,0,100,0,6000,6000,0,0,1,1,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 1"),
(@ENTRY,9,4,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 2"),
(@ENTRY,9,5,0,0,0,100,0,6000,6000,0,0,1,2,0,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 2"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Orientation Closest Creature 'Bellinger'"),
(@ENTRY,9,7,0,0,0,100,0,6000,6000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 3"),
(@ENTRY,9,8,0,0,0,100,0,3000,3000,0,0,5,60,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Play Emote 60"),
(@ENTRY,9,9,0,0,0,100,0,0,0,0,0,86,54899,0,10,114958,29644,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Cross Cast 'Knockback 15'"),
(@ENTRY,9,10,0,0,0,100,0,5000,5000,0,0,1,3,5000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 3"),
(@ENTRY,9,11,0,0,0,100,0,0,0,0,0,45,4,4,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Data 4 4"),
(@ENTRY,9,12,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,1.58825,"Bailey - On Script - Set Orientation 1.58825"),
(@ENTRY,9,13,0,0,0,100,0,3000,3000,0,0,1,4,3000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 4"),
(@ENTRY,9,14,0,0,0,100,0,5000,5000,0,0,45,1,1,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Data 1 1"),
(@ENTRY,9,15,0,0,0,100,0,300000,300000,0,0,1,5,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 5"), -- 5min
(@ENTRY,9,16,0,0,0,100,0,6000,6000,0,0,1,4,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 4"),
(@ENTRY,9,17,0,0,0,100,0,6000,6000,0,0,1,6,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 6"),
(@ENTRY,9,18,0,0,0,100,0,6000,6000,0,0,1,7,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 7"),
(@ENTRY,9,19,0,0,0,100,0,6000,6000,0,0,1,8,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 8"),
(@ENTRY,9,20,0,0,0,100,0,6000,6000,0,0,1,5,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 5"),
(@ENTRY,9,21,0,0,0,100,0,6000,6000,0,0,1,6,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 6"),
(@ENTRY,9,22,0,0,0,100,0,300000,300000,0,0,53,0,29643,0,0,0,1,1,0,0,0,0,0,0,0,"Bailey - On Script - Start Waypoint"); -- 5min 300000

-- Actionlist SAI
SET @ENTRY := 2964301;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,13000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.085985,"Bailey - On Script - Set Orientation 2,085985"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,45,2,2,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Data 2 2"),
(@ENTRY,9,3,0,0,0,100,0,1000,1000,0,0,1,7,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 7"),
(@ENTRY,9,4,0,0,0,100,0,6000,6000,0,0,1,9,5000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 9");

-- Actionlist SAI
SET @ENTRY := 2964302;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,33000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.50084,"Bailey - On Script - Set Orientation 2.50084"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,45,3,3,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Data 3 3"),
(@ENTRY,9,3,0,0,0,100,0,3000,3000,0,0,1,8,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 8"),
(@ENTRY,9,4,0,0,0,100,0,6000,6000,0,0,1,10,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 10"),
(@ENTRY,9,5,0,0,0,100,0,6000,6000,0,0,1,9,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 9"),
(@ENTRY,9,6,0,0,0,100,0,6000,6000,0,0,1,11,6000,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Say Line 11"),
(@ENTRY,9,7,0,0,0,100,0,6000,6000,0,0,1,10,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Say Line 10");

-- Actionlist SAI
SET @ENTRY := 2964303;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,4000,4000,0,0,45,5,5,0,0,0,0,10,114958,29644,0,0,0,0,0,"Bailey - On Script - Set Data 5 5"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Bailey - On Script - Set Active Off");

-- Bellinger SAI
SET @ENTRY := 29644;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,1.50098,"Bellinger - On Data Set 1 1 - Set Orientation 1.50098"),
(@ENTRY,0,1,0,38,0,100,0,2,2,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.08598,"Bellinger - On Data Set 2 2 - Set Orientation 2.08598"),
(@ENTRY,0,2,0,38,0,100,0,3,3,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.50084,"Bellinger - On Data Set 3 3 - Set Orientation 2.50084"),
(@ENTRY,0,3,0,38,0,100,0,4,4,0,0,69,0,0,0,0,0,0,8,0,0,0,6034.820,-750.8344,369.8008,1.4146,"Bellinger - On Data Set 4 4 - Move To Position"),
(@ENTRY,0,4,0,38,0,100,0,5,5,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,1.269333,"Bellinger - On Data Set 3 3 - Set Orientation 1.269333");

DELETE FROM `creature_text` WHERE `entry` IN (29643, 29644);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
-- Bailey
(29643, 0, 0, 'This doesn''t look pretty, lad.', 12, 0, 100, 1, 0, 0, 30286, 'Bailey'),
(29643, 1, 0, 'That right? Well why don''t you go prance your merry little self over and grab them, eh?', 12, 0, 100, 6, 0, 0, 30289, 'Bailey'),
(29643, 2, 0, 'That''s strange... for a moment there, I could have swore you called me short.', 12, 0, 100, 1, 0, 0, 30288, 'Bailey'),
(29643, 3, 0, 'Toss me?! Okay, that''s it punk.', 12, 0, 100, 5, 0, 0, 30290, 'Bailey'),
(29643, 4, 0, 'Keep it to yourself.', 12, 0, 100, 1, 0, 0, 30291, 'Bailey'),
(29643, 5, 0, 'You better hope they don''t survive it, lad.', 12, 0, 100, 1, 0, 0, 30292, 'Bailey'),
(29643, 6, 0, 'Cause I''m just going to watch and laugh as they toss you in.', 12, 0, 100, 1, 0, 0, 30293, 'Bailey'),
(29643, 7, 0, 'There''s got to be a way in somewhere.', 12, 0, 100, 1, 0, 0, 30294, 'Bailey'),
(29643, 8, 0, 'How''d the goblin leave his tools there then?', 12, 0, 100, 1, 0, 0, 30295, 'Bailey'),
(29643, 9, 0, 'That''s it, lad! It looks totally safe. Go for it, I''ll be right behind ya!', 12, 0, 100, 5, 0, 0, 30296, 'Bailey'),
(29643, 10, 0, 'Pansy.', 12, 0, 100, 5, 0, 0, 30312, 'Bailey'),
-- Bellinger
(29644, 0, 0, 'I don''t know, Bailey... it''s not so bad. We could throw a rock at the tools from here.', 12, 0, 100, 1, 0, 0, 30287, 'Bellinger'),
(29644, 1, 0, 'Well... I was just thinking... you''re a bit smaller, so you''re less likely to set off any of those mines.', 12, 0, 100, 1, 0, 0, 30297, 'Bellinger'),
(29644, 2, 0, 'No, no, no! Not short... it''s just... well... maybe I could toss you across most of it?', 12, 0, 100, 1, 0, 0, 30298, 'Bellinger'),
(29644, 3, 0, 'Ahhh!', 12, 0, 100, 0, 0, 0, 30306, 'Bellinger'),
(29644, 4, 0, 'That was close! You scared me.', 12, 0, 100, 5, 0, 0, 30311, 'Bellinger'),
(29644, 5, 0, 'I''ve got an idea, Bailey...', 12, 0, 100, 5, 0, 0, 30299, 'Bellinger'),
(29644, 6, 0, 'No, no - this might work!', 12, 0, 100, 5, 0, 0, 30300, 'Bellinger'),
(29644, 7, 0, 'There''s a path right there... only a few mines at the end in between us and the tools.', 12, 0, 100, 1, 0, 0, 30301, 'Bellinger'),
(29644, 8, 0, 'We wait for the next adventurer to come through and we tell him that''s the way to go... then after they set off the mines, we just walk in and grab the tools!', 12, 0, 100, 1, 0, 0, 30302, 'Bellinger'),
(29644, 9, 0, 'It really doesn''t look like it, Bailey.', 12, 0, 100, 1, 0, 0, 30303, 'Bellinger'),
(29644, 10, 0, 'Oh... good point. How about right there? That looks clear... kind of.', 12, 0, 100, 1, 0, 0, 30304, 'Bellinger'),
(29644, 11, 0, 'Uh... that''s okay. It still kind of hurts from last time.', 12, 0, 100, 1, 0, 0, 30305, 'Bellinger');

DELETE FROM `waypoints` WHERE `entry`=29643;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(29643, 1, 6043.27, -752.358, 371.289, 'Bailey'),                                                          
(29643, 2, 6057.13, -750.684, 374.297, 'Bailey'),                                                          
(29643, 3, 6075.11, -746.512, 378.785, 'Bailey'),                                                          
(29643, 4, 6074.88, -746.098, 378.744, 'Bailey'),                                                          
(29643, 5, 6082.66, -744.527, 380.612, 'Bailey'),                                                          
(29643, 6, 6094.52, -739.821, 383.156, 'Bailey'),                                                          
(29643, 7, 6108.06, -728.91, 385.165, 'Bailey'),                                                          
(29643, 8, 6106.12, -727.415, 384.716, 'Bailey'),                                                          
(29643, 9, 6103.38, -731.758, 384.439, 'Bailey'),                                                          
(29643, 10, 6094.78, -740.08, 383.232, 'Bailey'),                                                          
(29643, 11, 6081.35, -743.988, 380.304, 'Bailey'),                                                          
(29643, 12, 6064.46, -748.543, 376.139, 'Bailey'),                                                          
(29643, 13, 6047.42, -752.5, 372.131, 'Bailey'),                                                          
(29643, 14, 6036.15, -753.071, 369.981, 'Bailey'),                                                          
(29643, 15, 6036.69, -751.232, 370.083, 'Bailey'); 

-- https://raw.githubusercontent.com/dalaranwow/TrinityCore/7b578cb14281eaeb165477662a23c24546e3a7fd/sql/updates/world/3.3.5/2017_01_22_08_world.sql
-- Reduce respawntime for landmine
UPDATE `gameobject` SET `spawntimesecs`=10 WHERE  `id`=191502;

-- add fake death emote for Savage Hill Scavengers
UPDATE `creature` SET `spawndist`=0, `MovementType`=0, `unit_flags`=`unit_flags`|536870912|33554432|32770, `dynamicflags`=32 WHERE  `guid` IN (145342, 145341, 145343, 145344);
DELETE FROM `creature_addon` WHERE `guid` IN (145342, 145341, 145343, 145344);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES 
(116957,0,0,7,1,0, '29266'),
(116956,0,0,7,1,0, '29266'),
(116958,0,0,7,1,0, '29266'),
(116959,0,0,7,1,0, '29266');

-- add textevent for Janks 
-- Janks SAI
SET @ENTRY := 29429;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,90,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Reset - Set Flag Standstate Sit Down"),
(@ENTRY,0,1,0,10,0,100,0,1,10,55000,55000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - Within 0-10 Range Out of Combat LoS - Run Script");

-- Actionlist SAI
SET @ENTRY := 2942900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Set Active On"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,91,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Remove Flag Standstate Sit Down"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,5000,5000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Say Line 1"),
(@ENTRY,9,4,0,0,0,100,0,5000,5000,0,0,1,2,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Say Line 2"),
(@ENTRY,9,5,0,0,0,100,0,5000,5000,0,0,1,3,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Say Line 3"),
(@ENTRY,9,6,0,0,0,100,0,3000,3000,0,0,90,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Set Flag Standstate Sit Down"),
(@ENTRY,9,7,0,0,0,100,0,0,0,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Janks - On Script - Set Active Off");

DELETE FROM `creature_text` WHERE `entry`=29429;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(29429, 0, 0, 'Is it true?! Has someone finally come to get me out of this place?', 12, 0, 100, 0, 0, 0, 30198, 'Janks'),
(29429, 1, 0, 'No... no... that''s not it, is it? That heartless lout probably sent you after the tools, didn''t he?', 12, 0, 100, 0, 0, 0, 30199, 'Janks'),
(29429, 2, 0, 'Well... I guess I''ll just keep an eye on the turrets... or something.', 12, 0, 100, 0, 0, 0, 30200, 'Janks'),
(29429, 3, 0, '%s sighs.', 16, 0, 100, 0, 0, 0, 30201, 'Janks');

-- Torseg the Exiled should be sleeping, https://github.com/TrinityCore/TrinityCore/issues/18902
-- Picture, http://www.wowhead.com/npc=29350/torseg-the-exiled#comments + comments for proof
-- Torseg the Exiled SAI
SET @ENTRY := 29350;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0 AND `id`>=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,2,0,25,0,100,0,0,0,0,0,90,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Torseg the Exiled - On Reset - Set Flag Standstate Sleep"),
(@ENTRY,0,3,0,4,0,100,0,0,0,0,0,91,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Torseg the Exiled - On Aggro - Remove Flag Standstate Sleep");

-- Fix: https://github.com/TrinityCore/TrinityCore/issues/18866
-- Initiate Park
SET @NPC := 141419;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `orientation`, `delay`) VALUES
(@PATH, 1, 3621.02, 5897.4, 174.577, 2.35131, 0),
(@PATH, 2, 3619.69, 5898.14, 174.577, 2.92465, 16000),
(@PATH, 3, 3619.09, 5890.59, 174.577, 3.63152, 0),
(@PATH, 4, 3618.11, 5890.39, 174.577, 2.89325, 16000);

-- Initiate Park SAI
SET @ENTRY := 27302;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,34,0,100,0,2,1,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Park - On Reached Point 1 - Run Script"),
(@ENTRY,0,1,0,34,0,100,0,2,3,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Park - On Reached Point 3 - Run Script");

-- Actionlist SAI
SET @ENTRY := 2730200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 69"),
(@ENTRY,9,1,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 0");

-- https://github.com/dalaranwow/TrinityCore/commit/439654d1e88dfc20ed437ac66e3b5a80833da773
-- Warden Nork Bloodfrenzy SAI
SET @ENTRY := 25379;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0 AND `id`=4;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,4,0,1,0,100,0,10000,10000,470000,470000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - Out of Combat - Run Script");

-- Actionlist SAI
SET @ENTRY := 2537900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Set Active On"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,1,1,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,1,0,10000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,8000,8000,0,0,5,11,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Play Emote 11"),
(@ENTRY,9,4,0,0,0,100,0,2000,2000,0,0,1,2,1000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 2"),
(@ENTRY,9,5,0,0,0,100,0,1000,1000,0,0,1,3,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 3"),
(@ENTRY,9,6,0,0,0,100,0,5000,5000,0,0,1,1,6000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 1"),
(@ENTRY,9,7,0,0,0,100,0,6000,6000,0,0,1,4,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 4"),
(@ENTRY,9,8,0,0,0,100,0,3000,3000,0,0,1,5,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 5"),
(@ENTRY,9,9,0,0,0,100,0,5000,5000,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Play Emote 1"),
(@ENTRY,9,10,0,0,0,100,0,5000,5000,0,0,1,2,8000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 2"),
(@ENTRY,9,11,0,0,0,100,0,8000,8000,0,0,1,6,9000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 6"),
(@ENTRY,9,12,0,0,0,100,0,5000,5000,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Play Emote 1"),
(@ENTRY,9,13,0,0,0,100,0,4000,4000,0,0,1,7,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 7"),
(@ENTRY,9,14,0,0,0,100,0,5000,5000,0,0,1,8,7000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 8"),
(@ENTRY,9,15,0,0,0,100,0,7000,7000,0,0,1,3,3000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 3"),
(@ENTRY,9,16,0,0,0,100,0,3000,3000,0,0,1,4,5000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 4"),
(@ENTRY,9,17,0,0,0,100,0,5000,5000,0,0,1,5,3000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 5"),
(@ENTRY,9,18,0,0,0,100,0,3000,3000,0,0,1,6,5000,0,0,0,0,10,125421,25426,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 6"),
(@ENTRY,9,19,0,0,0,100,0,5000,5000,0,0,1,9,2000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 9"),
(@ENTRY,9,20,0,0,0,100,0,2000,2000,0,0,1,10,7000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 10"),
(@ENTRY,9,21,0,0,0,100,0,4000,4000,0,0,5,15,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Play Emote 15"),
(@ENTRY,9,22,0,0,0,100,0,3000,3000,0,0,1,11,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Say Line 11"),
(@ENTRY,9,23,0,0,0,100,0,3000,3000,0,0,5,5,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Play Emote 5"),
(@ENTRY,9,24,0,0,0,100,0,2000,2000,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Warden Nork Bloodfrenzy - On Script - Set Active Off");

DELETE FROM `creature_text` WHERE `entry`=25379 AND `groupid` >=1;
DELETE FROM `creature_text` WHERE `entry`=25426;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- Warden Nork Bloodfrenzy
(25379, 1, 0, 'Look at \'em, Ug. You know what those are?', 12, 1, 100, 25, 0, 0, 24631, 'Warden Nork Bloodfrenzy'),
(25379, 2, 0, 'Those...', 12, 1, 100, 25, 0, 0, 24633, 'Warden Nork Bloodfrenzy'),
(25379, 3, 0, 'Those are cowards.', 12, 1, 100, 1, 0, 0, 24634, 'Warden Nork Bloodfrenzy'),
(25379, 4, 0, '%s shakes his head.', 16, 1, 100, 274, 0, 0, 24636, 'Warden Nork Bloodfrenzy'),
(25379, 5, 0, 'No, blood of my blood, they are soldiers of the Alliance. Our sworn enemies.', 12, 1, 100, 1, 0, 0, 24637, 'Warden Nork Bloodfrenzy'),
(25379, 6, 0, 'They are here because they do not wish to defend their homes... their families...', 12, 1, 100, 1, 0, 0, 24639, 'Warden Nork Bloodfrenzy'),
(25379, 7, 0, 'They have given up and surrendered - willingly - to us.', 12, 1, 100, 1, 0, 0, 24640, 'Warden Nork Bloodfrenzy'),
(25379, 8, 0, 'Ug\'thor, they are afraid to die.', 12, 1, 100, 1, 0, 0, 24641, 'Warden Nork Bloodfrenzy'),
(25379, 9, 0, '%s laughs.', 16, 1, 100, 11, 0, 0, 24646, 'Warden Nork Bloodfrenzy'),
(25379, 10, 0, 'That\'s right, Ug. Victory or death!', 12, 1, 100, 273, 0, 0, 24647, 'Warden Nork Bloodfrenzy'),
(25379, 11, 0, '[In Broken Common] Hear boy, cowards? Boy die! Boy die for freedom!', 12, 0, 100, 273, 0, 0, 24648, 'Warden Nork Bloodfrenzy'),
-- Ug''thor Bloodfrenzy
(25426, 0, 0, 'No, popo. They look kind of like the pigs. Are they pigs?', 12, 1, 100, 1, 0, 0, 24632, 'Ug''thor Bloodfrenzy'),
(25426, 1, 0, 'Are they cowardly pigs, popo?', 12, 1, 100, 1, 0, 0, 24635, 'Ug''thor Bloodfrenzy'),
(25426, 2, 0, 'But popo, what are they afraid of? Why are they here?', 12, 1, 100, 1, 0, 0, 24638, 'Ug''thor Bloodfrenzy'),
(25426, 3, 0, '%s scratches his head.', 16, 1, 100, 0, 0, 0, 24642, 'Ug''thor Bloodfrenzy'),
(25426, 4, 0, 'I... I don\'t understand, popo.', 12, 1, 100, 1, 0, 0, 24643, 'Ug''thor Bloodfrenzy'),
(25426, 5, 0, '%s puffs out his chest and stands on his tip-toes.', 16, 1, 100, 0, 0, 0, 24644, 'Ug''thor Bloodfrenzy'),
(25426, 6, 0, 'Lok\'tar ogar, popo!', 12, 1, 100, 1, 0, 0, 24645, 'Ug''thor Bloodfrenzy');

-- https://github.com/dalaranwow/TrinityCore/commit/97c9dd24d2d8df4794b6cf99abf7af57f5074cf2
-- Apprentice Trotter -- http://www.wowhead.com/npc=27301
-- Apprentice Trotter SAI
SET @ENTRY := 27301;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,27301,1,0,0,1,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,3,40,0,100,0,3,27301,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Waypoint 3 Reached - Run Script"),
(@ENTRY,0,3,0,61,0,100,0,3,27301,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.20907,"Apprentice Trotter - On Waypoint 3 Reached - Set Orientation 5,20907"),
(@ENTRY,0,4,5,40,0,100,0,5,27301,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Waypoint 5 Reached - Run Script"),
(@ENTRY,0,5,0,61,0,100,0,5,27301,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,6.1162,"Apprentice Trotter - On Waypoint 5 Reached - Set Orientation 6,1162"),
(@ENTRY,0,6,7,40,0,100,0,8,27301,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Waypoint 8 Reached - Run Script"),
(@ENTRY,0,7,0,61,0,100,0,8,27301,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,6.10835,"Apprentice Trotter - On Waypoint 8 Reached - Set Orientation 6,10835");

-- Actionlist SAI
SET @ENTRY := 2730100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,16000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Script - Set Emote State 133"),
(@ENTRY,9,2,0,0,0,100,0,8000,8000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Apprentice Trotter - On Script - Set Emote State 0");

UPDATE `creature` SET `spawndist`=0, `MovementType`=0 WHERE  `guid`=141381;
DELETE FROM `waypoints` WHERE `entry`=27301;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(27301, 1, 3655.39, 5880.89, 174.576, 'Apprentice Trotter'),
(27301, 2, 3648.21, 5875.72, 174.554, 'Apprentice Trotter'),
(27301, 3, 3649.17, 5874.43, 174.578, 'Apprentice Trotter'),
(27301, 4, 3656.08, 5881.3, 174.577, 'Apprentice Trotter'),
(27301, 5, 3659.29, 5880.93, 174.577, 'Apprentice Trotter'),
(27301, 6, 3658.57, 5882.95, 174.577, 'Apprentice Trotter'),
(27301, 7, 3659.6, 5887.87, 174.577, 'Apprentice Trotter'),
(27301, 8, 3661.47, 5888.53, 174.577, 'Apprentice Trotter'),
(27301, 9, 3659.61, 5887.48, 174.576, 'Apprentice Trotter');

-- https://github.com/dalaranwow/TrinityCore/blob/fd1620a4e20cdf94df122edbac4599a2a5374e98/sql/updates/world/3.3.5/2017_01_16_04_world.sql
-- Initiate Vernon SAI
SET @ENTRY := 27300;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,1,1,0,0,103,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Vernon - On Data Set 1 1 - Set Rooted On"),
(@ENTRY,0,1,0,61,0,100,0,1,1,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Vernon - On Data Set 1 1 - Set Emote State 0"),
(@ENTRY,0,2,0,38,0,100,0,2,2,0,0,66,0,0,0,0,0,0,10,111281,27299,0,0,0,0,0,"Initiate Vernon - On Data Set 2 2 - Set Orientation Closest Creature 'Initiate Greer'"),
(@ENTRY,0,3,0,38,0,100,0,3,3,0,0,11,48254,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Vernon - On Data Set 3 3 - Cast 'Prototype Neural Needle Impact'"),
(@ENTRY,0,4,0,38,0,100,0,4,4,0,0,103,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Vernon - On Data Set 4 4 - Set Rooted Off");

SET @MAXGUID := 915; 
SET @SCRIPTID := 14132500; 
DELETE FROM `waypoint_scripts` WHERE `id` IN (@SCRIPTID+0);
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
(@SCRIPTID+0, 2, 1, 69, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+0, 17, 1, 0, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1));

SET @NPC := 141325;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `delay`, `action`, `orientation`) VALUES
(@PATH, 1, 3647.87, 5909.61, 174.578, 21000, @SCRIPTID, 1.26067),
(@PATH, 2, 3640.9, 5911.28, 174.579, 21000, @SCRIPTID, 1.26067);

UPDATE `waypoint_data` SET `action_chance`=100 WHERE `action` IN (@SCRIPTID);

-- Initiate Greer SAI
SET @ENTRY := 27299;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,27299,1,0,0,1,1,0,0,0,0,0,0,0,"Initiate Greer - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,3,40,0,100,0,5,27299,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Waypoint 5 Reached - Run Script"),
(@ENTRY,0,3,0,61,0,100,0,5,27299,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.76692,"Initiate Greer - On Waypoint 5 Reached - Set Orientation 2,76692"),
(@ENTRY,0,4,5,40,0,100,0,11,27299,0,0,80,@ENTRY*100+01,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Waypoint 11 Reached - Run Script"),
(@ENTRY,0,5,0,61,0,100,0,11,27299,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.84897,"Initiate Greer - On Waypoint 11 Reached - Set Orientation 5,84897");

-- Actionlist SAI
SET @ENTRY := 2729900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,55000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Active On"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,5000,5000,0,0,45,3,3,0,0,0,0,10,111307,27300,0,0,0,0,0,"Initiate Greer - On Script - Set Data 3 3"),
(@ENTRY,9,6,0,0,0,100,0,1000,1000,0,0,45,1,1,0,0,0,0,10,111307,27300,0,0,0,0,0,"Initiate Greer - On Script - Set Data 1 1"),
(@ENTRY,9,7,0,0,0,100,0,6000,6000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Say Line 1"),
(@ENTRY,9,8,0,0,0,100,0,4000,4000,0,0,45,2,2,0,0,0,0,10,111307,27300,0,0,0,0,0,"Initiate Greer - On Script - Set Data 2 2"),
(@ENTRY,9,9,0,0,0,100,0,2000,2000,0,0,1,0,6000,0,0,0,0,10,111307,27300,0,0,0,0,0,"Initiate Greer - On Script - Say Line 0"),
(@ENTRY,9,10,0,0,0,100,0,14000,14000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Say Line 2"),
(@ENTRY,9,11,0,0,0,100,0,0,0,0,0,45,4,4,0,0,0,0,10,111307,27300,0,0,0,0,0,"Initiate Greer - On Script - Set Data 4 4"),
(@ENTRY,9,12,0,0,0,100,0,2000,2000,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Active Off");

-- Actionlist SAI
SET @ENTRY := 2729901;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,70000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Active On"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,64000,64000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,0,0,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Initiate Greer - On Script - Set Active Off");

DELETE FROM `creature_text` WHERE `entry` IN (27299, 27300);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(27299, 0, 0, 'Tell me if this hurts.', 12, 0, 100, 0, 0, 0, 26296, 'Initiate Greer'),
(27299, 0, 1, 'Let''s try this one out...', 12, 0, 100, 0, 0, 0, 26298, 'Initiate Greer'),
(27299, 0, 2, 'I think I''ve got it this time..', 12, 0, 100, 0, 0, 0, 26297, 'Initiate Greer'),
(27299, 1, 0, 'How''d that feel?', 12, 0, 100, 0, 0, 0, 26299, 'Initiate Greer'),
(27299, 1, 1, 'Was that potent enough?', 12, 0, 100, 0, 0, 0, 26302, 'Initiate Greer'),
(27299, 1, 2, 'Do you think that will get some answers?', 12, 0, 100, 0, 0, 0, 26300, 'Initiate Greer'),
(27299, 2, 0, 'Hmm...', 12, 0, 100, 0, 0, 0, 26304, 'Initiate Greer'),
(27299, 2, 1, 'True, our enemies would show us no mercy.', 12, 0, 100, 0, 0, 0, 26320, 'Initiate Greer'),
(27299, 2, 2, 'True, but how far should we push?  At what point are we no better than our enemies?', 12, 0, 100, 0, 0, 0, 26319, 'Initiate Greer'),
(27300, 0, 0, 'You need more bite.  Do not show our enemies any mercy.', 12, 0, 100, 0, 0, 0, 26305, 'Initiate Vernon'),
(27300, 0, 1, 'The more pain we can inflict, the quicker we can get our answers.', 12, 0, 100, 0, 0, 0, 26306, 'Initiate Vernon');

UPDATE `creature` SET `spawndist`=0, `MovementType`=0 WHERE  `guid`=141301;
DELETE FROM `waypoints` WHERE `entry`=27299;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(27299, 1, 3640.38, 5908, 174.576, 'Initiate Greer'),
(27299, 2, 3642.54, 5905.63, 174.572, 'Initiate Greer'),
(27299, 3, 3645.93, 5904.91, 174.571, 'Initiate Greer'),
(27299, 4, 3647.52, 5906.78, 174.576, 'Initiate Greer'),
(27299, 5, 3646.7, 5907.29, 174.578, 'Initiate Greer'),
(27299, 6, 3647.49, 5906.69, 174.578, 'Initiate Greer'),
(27299, 7, 3646.49, 5905.07, 174.576, 'Initiate Greer'),
(27299, 8, 3643.7, 5905.04, 174.568, 'Initiate Greer'),
(27299, 9, 3641.21, 5906.38, 174.577, 'Initiate Greer'),
(27299, 10, 3640.11, 5908.16, 174.577, 'Initiate Greer'),
(27299, 11, 3641.55, 5908.3, 174.577, 'Initiate Greer');

-- Librarian Hamilton -- http://www.wowhead.com/npc=27141/librarian-hamilton
-- Librarian Hamilton SAI
SET @ENTRY := 27141;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,27141,1,0,0,1,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,27141,0,0,54,60000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,0,40,0,100,0,2,27141,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Waypoint 2 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,3,27141,0,0,54,60000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Waypoint 3 Reached - Pause Waypoint"),
(@ENTRY,0,5,6,40,0,100,0,4,27141,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Waypoint 4 Reached - Run Script"),
(@ENTRY,0,6,0,61,0,100,0,4,27141,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.15815,"Librarian Hamilton - On Waypoint 4 Reached - Set Orientation 3,15815");

-- Actionlist SAI
SET @ENTRY := 2714100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,17000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Script - Set Flag Standstate Kneel"),
(@ENTRY,9,2,0,0,0,100,0,11000,11000,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Librarian Hamilton - On Script - Remove Flag Standstate Kneel");

DELETE FROM `waypoints` WHERE `entry`=27141;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(27141, 1, 3595.1, 5959.01, 136.328, 'Librarian Hamilton'),
(27141, 2, 3590.25, 5953.57, 136.203, 'Librarian Hamilton'),
(27141, 3, 3595.1, 5959.01, 136.328, 'Librarian Hamilton'),
(27141, 4, 3593.01, 5963.17, 136.328, 'Librarian Hamilton');

-- Fruits of Our Labor - http://wotlk.openwow.com/quest=11904
-- Cart Release SAI
SET @ENTRY := 188104;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,70,0,100,0,2,0,0,0,45,1,1,0,0,0,0,19,25986,20,0,0,0,0,0,"Cart Release - On Gameobject State Changed - Set Data 1 1");

DELETE FROM `creature_text` WHERE `entry`=25986;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(25986, 0, 0, 'The Ore Cart squeaks against the rails as it is released.', 16, 0, 100, 0, 0, 0, 25416, 'Ore Cart');

UPDATE `creature` SET `spawntimesecs`=1 WHERE  `guid`=154487;

-- Ore Cart SAI
SET @ENTRY := 25986;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,1,1,1,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Ore Cart - On Data Set 1 1 - Set Active On (No Repeat)"),
(@ENTRY,0,1,3,61,0,100,0,1,1,0,0,53,0,25986,0,0,0,0,1,0,0,0,0,0,0,0,"Ore Cart - On Data Set 1 1 - Start Waypoint (No Repeat)"),
(@ENTRY,0,2,4,40,0,100,0,15,25986,0,0,41,100,0,0,0,0,0,1,0,0,0,0,0,0,0,"Ore Cart - On Waypoint 15 Reached - Despawn In 100 ms"),
(@ENTRY,0,3,0,61,0,100,0,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Ore Cart - On Data Set 1 1 - Say Line 0 (No Repeat)"),
(@ENTRY,0,4,0,61,0,100,0,15,25986,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Ore Cart - On Waypoint 15 Reached - Set Active Off");

DELETE FROM `waypoints` WHERE `entry`=25986;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(25986,1,2797.49,5277.37,22.0435, 'Ore Cart'),
(25986,2,2794.32,5281.92,22.4205, 'Ore Cart'),
(25986,3,2790.66,5285.51,21.9443, 'Ore Cart'),
(25986,4,2783.22,5291.11,22.7502, 'Ore Cart'),
(25986,5,2776.37,5297,24.6788, 'Ore Cart'),
(25986,6,2772.41,5299.31,25.5129, 'Ore Cart'),
(25986,7,2767.82,5300.13,26.1103, 'Ore Cart'),
(25986,8,2764.16,5299.4,26.2145, 'Ore Cart'),
(25986,9,2760.57,5297.91,26.187, 'Ore Cart'),
(25986,10,2757.37,5295.31,25.8549, 'Ore Cart'),
(25986,11,2755.49,5291.84,25.142, 'Ore Cart'),
(25986,12,2752.95,5288.89,24.7977, 'Ore Cart'),
(25986,13,2748.91,5286.94,24.7359, 'Ore Cart'),
(25986,14,2746.13,5285.9,24.8515, 'Ore Cart'),
(25986,15,2733.06,5284.92,27.2962, 'Ore Cart');

-- Mordenai -- http://wotlk.openwow.com/npc=22113
-- Mordenai SAI
SET @ENTRY := 22113;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,22113,1,0,0,2,1,0,0,0,0,0,0,0,"Mordenai - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordenai - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,3,40,0,100,0,1,22113,0,0,54,12000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordenai - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,0,61,0,100,0,1,22113,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.59702,"Mordenai - On Waypoint 1 Reached - Set Orientation 4.59702"),
(@ENTRY,0,4,0,40,0,100,0,2,22113,0,0,49,0,0,0,0,0,0,19,21478,80,0,0,0,0,0,"Mordenai - On Waypoint 2 Reached - Start Attacking"),
(@ENTRY,0,5,0,40,0,100,0,12,22113,0,0,49,0,0,0,0,0,0,19,21478,80,0,0,0,0,0,"Mordenai - On Waypoint 12 Reached - Start Attacking"),
(@ENTRY,0,6,0,0,0,100,0,0,0,2300,3900,11,41440,64,0,0,0,0,2,0,0,0,0,0,0,0,"Mordenai - In Combat - Cast 'Shoot'"),
(@ENTRY,0,7,0,0,0,100,0,6000,9000,27000,28000,11,41448,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mordenai - In Combat - Cast 'Multi-Shot'"),
(@ENTRY,0,8,0,0,0,100,0,5000,5000,18000,18000,11,38370,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mordenai - In Combat - Cast 'Aimed Shot'");

DELETE FROM `waypoints` WHERE `entry`=22113;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(22113, 1, -4100.05, 969.276,  24.9571, 'Mordenai'),
(22113, 2, -4104.06, 931.187,  18.8469, 'Mordenai'),
(22113, 3, -4105.65, 911.224,  16.1353, 'Mordenai'),
(22113, 4, -4106.05, 889.833,  14.4649, 'Mordenai'),
(22113, 5, -4118.47, 857.928,  8.29486, 'Mordenai'),
(22113, 6, -4122.4, 839.518,   10.4151, 'Mordenai'),
(22113, 7, -4143.45, 803.2,    9.22043, 'Mordenai'),
(22113, 8, -4150.96, 769.769,  6.31908, 'Mordenai'),
(22113, 9, -4153.1, 760.191,   3.47747, 'Mordenai'),
(22113, 10, -4155.08, 747.368, 1.8985,  'Mordenai'),
(22113, 11, -4151.37, 769.929, 6.42453, 'Mordenai'),
(22113, 12, -4132.84, 797.355, 8.16607, 'Mordenai'),
(22113, 13, -4137.32, 823.672, 9.68296, 'Mordenai'),
(22113, 14, -4119.16, 842.164, 9.89524, 'Mordenai'),
(22113, 15, -4115.26, 851.276, 7.68533, 'Mordenai'),
(22113, 16, -4109.66, 883.088, 13.8876, 'Mordenai'),
(22113, 17, -4110.74, 914.769, 17.723,  'Mordenai'),
(22113, 18, -4106.89, 948.026, 23.0078, 'Mordenai');

-- Chancellor Bloodleaf -- http://wotlk.openwow.com/npc=22012
-- Chancellor Bloodleaf SAI
SET @ENTRY := 22012;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,15000,15000,130000,130000,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - Out of Combat - Run Script"),
(@ENTRY,0,1,0,0,0,100,0,5000,6000,5000,6000,11,34517,0,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - In Combat - Cast 'Arcane Explosion'");

-- Actionlist SAI
SET @ENTRY := 2201200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,8000,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Play Emote 1"),
(@ENTRY,9,2,0,0,0,100,0,9000,9000,0,0,1,0,6000,0,0,0,0,10,77086,22011,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,6000,6000,0,0,1,1,7000,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Say Line 1"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Play Emote 1"),
(@ENTRY,9,5,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Chancellor Bloodleaf - On Script - Say Line 2");

DELETE FROM `creature_text` WHERE `entry` IN (22011, 22012);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- Corok the Mighty 
(22011, 0, 0, 'And... if we do not?', 12, 0, 100, 0, 0, 0, 19695, 'Corok the Mighty'),
-- Chancellor Bloodleaf
(22012, 0, 0, 'Lend us the strength of the giants of Shadowmoon, Corok. The master is a just ruler. He will be sure to take care of those that assisted in subduing the dissidents of this world.', 12, 0, 100, 0, 0, 0, 19693, 'Chancellor Bloodleaf'),
(22012, 1, 0, 'Then you become enemies of my lord, Illidan. Enemies of Illidan are destroyed. Are your giants able to withstand an attack from the forces of Eclipse Point?', 12, 0, 100, 0, 0, 0, 19696, 'Chancellor Bloodleaf'),
(22012, 2, 0, 'No... I did not think so...', 12, 0, 100, 0, 0, 0, 19697, 'Chancellor Bloodleaf');

-- Painmistress Gabrissa -- http://wotlk.openwow.com/npc=21309
-- Painmistress Gabrissa SAI
SET @ENTRY := 21309;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,5000,7000,20000,25000,11,38048,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - In Combat - Cast 'Curse of Pain'"),
(@ENTRY,0,1,0,0,0,100,0,6000,7000,17000,20000,11,38169,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - In Combat - Cast 'Subservience'"),
(@ENTRY,0,2,0,25,0,100,0,0,0,0,0,53,0,21309,1,0,0,2,0,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Reset - Start Waypoint"),
(@ENTRY,0,3,0,40,0,100,0,5,21309,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Waypoint 5 Reached - Run Script"),
(@ENTRY,0,4,5,40,0,100,0,9,21309,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Waypoint 9 Reached - Pause Waypoint"),
(@ENTRY,0,5,0,61,0,100,0,9,21309,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.37533,"Painmistress Gabrissa - On Waypoint 9 Reached - Set Orientation 3.37533"),
(@ENTRY,0,6,0,4,0,100,0,0,0,0,0,49,0,0,0,0,0,0,2,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Aggro - Start Attacking");

-- Actionlist SAI
SET @ENTRY := 2130900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.60702,"Painmistress Gabrissa - On Script - Set Orientation 3,60702"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,11,36578,0,0,0,0,0,10,74619,21310,0,0,0,0,0,"Painmistress Gabrissa - On Script - Cast 'Fool's Fortune'"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,92,0,36578,1,0,0,0,1,0,0,0,0,0,0,0,"Painmistress Gabrissa - On Script - Interrupt Spell 'Fool's Fortune'");

DELETE FROM `waypoints` WHERE `entry`=21309;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(21309, 1, -3697.1, 2669.77,  107.186, 'Painmistress Gabrissa'),
(21309, 2, -3697.68, 2673.34, 108.569, 'Painmistress Gabrissa'),
(21309, 3, -3692.22, 2678.74, 108.66,  'Painmistress Gabrissa'),
(21309, 4, -3690.04, 2682.79, 108.752, 'Painmistress Gabrissa'),
(21309, 5, -3692.89, 2684.18, 108.485, 'Painmistress Gabrissa'),
(21309, 6, -3690.92, 2682.03, 108.744, 'Painmistress Gabrissa'),
(21309, 7, -3694.07, 2677.51, 108.537, 'Painmistress Gabrissa'),
(21309, 8, -3697.53, 2673.77, 108.581, 'Painmistress Gabrissa'),
(21309, 9, -3693.46, 2668.44, 107.174, 'Painmistress Gabrissa');

-- Levixus SAI
SET @ENTRY := 19847;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,11,0,100,1,0,0,0,0,11,24313,0,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - On Respawn - Cast 'Shade' (No Repeat)"),
(@ENTRY,0,1,0,0,0,100,0,3000,7000,13000,17000,11,11443,0,0,0,0,0,2,0,0,0,0,0,0,0,"Levixus - In Combat - Cast 'Cripple'"),
(@ENTRY,0,2,0,0,0,100,0,9000,14000,13000,17000,11,7645,1,0,0,0,0,6,0,0,0,0,0,0,0,"Levixus - In Combat - Cast 'Dominate Mind'"),
(@ENTRY,0,3,0,0,0,100,0,10000,13000,18000,22000,11,38939,1,0,0,0,0,2,0,0,0,0,0,0,0,"Levixus - In Combat - Cast 'Inferno'"),
(@ENTRY,0,4,0,1,0,100,0,0,0,20000,20000,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - Out of Combat - Run Script");

-- Actionlist SAI
SET @ENTRY := 1984700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,1000,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,1,1,8000,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,8000,8000,0,0,1,0,1000,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,1000,1000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Levixus - On Script - Say Line 1");

DELETE FROM `creature_text` WHERE `entry`=19847;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(19847, 0, 0, '%s reads from the book of the dead.', 16, 0, 100, 0, 0, 0, 17446, 'Levixus'),
-- 
(19847, 1, 0, 'Shane Dabiri, Lee, Alex T., Rob F., J. A. B.,  A. Mayberry!', 12, 8, 100, 15, 0, 0, 17454, 'Levixus'),
(19847, 1, 1, 'Jeffrey Kaplan! Tom Chilton! Rob Pardo!', 12, 8, 100, 15, 0, 0, 17447, 'Levixus'),
(19847, 1, 2, 'Justin Thavirat! Roman Kenney! Our entire awesome art and animation team!', 12, 8, 100, 15, 0, 0, 17448, 'Levixus'),
(19847, 1, 3, 'Kevin Jordan and Jonathan LeCraft!', 12, 8, 100, 15, 0, 0, 17453, 'Levixus'),
(19847, 1, 4, 'CHRIS METZEN!', 12, 8, 100, 15, 0, 0, 17449, 'Levixus'),
(19847, 1, 5, 'Pat Nagle! World design team! Cory Stockton, Paul C., Jim Chadwick, Staats, Ed Hanes, Morris, Gotcher, Jesse!, Sarah B., Victor C., Dave A.! All of our amazing exterior artists!', 12, 8, 100, 15, 0, 0, 17450, 'Levixus'),
(19847, 1, 6, 'Scott Mercer! Geoff, Wyatt, Joe, Travis, Kris, Steven! Brazie!', 12, 8, 100, 15, 0, 0, 17451, 'Levixus'),
(19847, 1, 7, 'Luis, Brianna and Eric! Blizzard QA team! Of course our fans!', 12, 8, 100, 15, 0, 0, 17452, 'Levixus');

-- Experimental Pilot -- http://wotlk.openwow.com/npc=19776

-- Experimental Pilot SAI
SET @ENTRY := 19776;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,19776,1,0,0,0,1,0,0,0,0,0,0,0,"Experimental Pilot - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Experimental Pilot - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,8,19776,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Experimental Pilot - On Waypoint 8 Reached - Run Script"),
(@ENTRY,0,3,4,40,0,100,0,15,19776,0,0,54,300000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Experimental Pilot - On Waypoint 15 Reached - Pause Waypoint"),
(@ENTRY,0,4,0,61,0,100,0,15,19776,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.0689,"Experimental Pilot - On Waypoint 15 Reached - Set Orientation 5,0689");

-- Actionlist SAI
SET @ENTRY := 1977600;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,140000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.111339,"On Script - Set Orientation 2.111339"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,45,1,1,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Set Data 1 1"),
(@ENTRY,9,3,0,0,0,100,0,1000,1000,0,0,1,0,15000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,15000,15000,0,0,1,0,20000,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,20000,20000,0,0,1,1,20000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,5,6,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 6"),
(@ENTRY,9,7,0,0,0,100,0,20000,20000,0,0,1,1,15000,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,8,0,0,0,100,0,15000,15000,0,0,5,6,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 6"),
(@ENTRY,9,9,0,0,0,100,0,0,0,0,0,1,2,15000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 2"),
(@ENTRY,9,10,0,0,0,100,0,15000,15000,0,0,1,2,10000,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Say Line 2"),
(@ENTRY,9,11,0,0,0,100,0,10000,10000,0,0,1,3,15000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 3"),
(@ENTRY,9,12,0,0,0,100,0,0,0,0,0,5,6,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 6"),
(@ENTRY,9,13,0,0,0,100,0,15000,15000,0,0,1,3,10000,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Say Line 3"),
(@ENTRY,9,14,0,0,0,100,0,10000,10000,0,0,1,4,15000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 4"),
(@ENTRY,9,15,0,0,0,100,0,15000,15000,0,0,1,4,0,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Say Line 4"),
(@ENTRY,9,16,0,0,0,100,0,0,0,0,0,45,2,2,0,0,0,0,10,70614,19737,0,0,0,0,0,"On Script - Set Data 2 2");

-- Buddy AI
-- Engineering Crewmember SAI
SET @GUID := -125002;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=19737;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GUID AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GUID,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,10,70731,19776,0,0,0,0,0,"Engineering Crewmember - On Data Set 1 1 - Set Orientation Closest Creature 'Experimental Pilot'"),
(@GUID,0,1,0,38,0,100,0,2,2,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.199120,"Engineering Crewmember - On Data Set 2 2 - Set Orientation 2,199120");

DELETE FROM `creature_text` WHERE `entry` IN (19737, 19776);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(19776, 0, 0, 'All my bags are packed, I''m ready to go.', 12, 0, 100, 0, 0, 0, 17413, 'Experimental Pilot'),
(19776, 1, 0, 'You need to do what? All this science, I don''t understand... Look, this is just my job, five days a week.', 12, 0, 100, 0, 0, 0, 17416, 'Experimental Pilot'),
(19776, 2, 0, 'I can''t. I''m not the man they think I am at home.  Besides, I didn''t bring them out here.', 12, 0, 100, 0, 0, 0, 17419, 'Experimental Pilot'),
(19776, 3, 0, 'This isn''t the kind of place to raise your kids. It''s cold, and there''d be no one to raise them.', 12, 0, 100, 0, 0, 0, 17421, 'Experimental Pilot'),
(19776, 4, 0, 'Oh no, no, no... I''m a rocket man.', 12, 0, 100, 0, 0, 0, 17423, 'Experimental Pilot'),
-- 
(19737, 0, 0, 'The ship''s not ready yet.  We still need to calibrate the fuse length to make sure that it doesn''t burn out and leave you up there alone.', 12, 0, 100, 0, 0, 0, 17414, 'Engineering Crewmember'),
(19737, 1, 0, 'Essentially, it''s going to be a long, long time till we are ready to launch.  Maybe you should just head back home to your family.', 12, 0, 100, 0, 0, 0, 17417, 'Engineering Crewmember'),
(19737, 2, 0, 'Why not? A family can give you strength.', 12, 0, 100, 0, 0, 0, 17420, 'Engineering Crewmember'),
(19737, 3, 0, 'Couldn''t you raise them?', 12, 0, 100, 0, 0, 0, 17422, 'Engineering Crewmember'),
(19737, 4, 0, 'Well, it''s like I said; it''s going to be a while before we get this ship ready for flight.  We''ll keep you informed.', 12, 0, 100, 0, 0, 0, 17424, 'Engineering Crewmember');

DELETE FROM `waypoints` WHERE `entry`=19776;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(19776, 1, 2976.31, 3709.67,  142.398, 'Experimental Pilot'),
(19776, 2, 2987.82, 3686.2,   142.681, 'Experimental Pilot'),
(19776, 3, 2987.82, 3652.62,  143.244, 'Experimental Pilot'),
(19776, 4, 2999.17, 3645.03,  143.849, 'Experimental Pilot'),
(19776, 5, 3006.7, 3648.63,   143.771, 'Experimental Pilot'),
(19776, 6, 3042.1, 3645.08,   142.808, 'Experimental Pilot'),
(19776, 7, 3047.12, 3648.91,  142.832, 'Experimental Pilot'),
(19776, 8, 3043.87, 3652.34,  142.88,  'Experimental Pilot'),
(19776, 9, 3026.43, 3641.43,  143.507, 'Experimental Pilot'),
(19776, 10, 2989.67, 3650.38, 143.486, 'Experimental Pilot'),
(19776, 11, 2988.99, 3685.32, 142.73,  'Experimental Pilot'),
(19776, 12, 2975.27, 3712.82, 142.309, 'Experimental Pilot'),
(19776, 13, 2975.12, 3725.84, 142.688, 'Experimental Pilot'),
(19776, 14, 2965.31, 3733.57, 143.503, 'Experimental Pilot'),
(19776, 15, 2966.14, 3733,    143.472, 'Experimental Pilot');

-- Consortium Nether Runner -- http://wotlk.openwow.com/npc=19667
UPDATE `creature` SET `position_x`=-2057.45, `position_y`=8528.71, `position_z`=24.6556, `orientation`=2.05449, `spawntimesecs`=120 WHERE  `guid`=124705;

-- Consortium Nether Runner SAI
SET @ENTRY := 19667;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,19667,0,0,0,1,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,19667,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Waypoint 1 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,5,19667,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Waypoint 5 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,8,19667,0,0,80,@ENTRY*100+02,2,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Waypoint 8 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 1966700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,3000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.05449,"Consortium Nether Runner - On Script - Set Orientation 2.05449"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,11,34427,0,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Script - Cast 'Ethereal Teleport'");

-- Actionlist SAI
SET @ENTRY := 1966701;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,11000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.57111,"Consortium Nether Runner - On Script - Set Orientation 3.57111"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,1,0,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,3000,3000,0,0,45,1,1,0,0,0,0,10,65554,18265,0,0,0,0,0,"Consortium Nether Runner - On Script - Set Data 1 1"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,1,0,5000,0,0,0,0,10,65554,18265,0,0,0,0,0,"Consortium Nether Runner - On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,5000,5000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Consortium Nether Runner - On Script - Say Line 1"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,45,2,2,0,0,0,0,10,65554,18265,0,0,0,0,0,"Consortium Nether Runner - On Script - Set Data 1 1");

-- Actionlist SAI
SET @ENTRY := 1966702;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,3000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.573574,"On Script - Set Orientation 0,573574"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,11,34427,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Cast 'Ethereal Teleport'"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Despawn Instant");

-- Buddy AI
-- Gezhe SAI
SET @ENTRY := 18265;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,10,70166,19667,0,0,0,0,0,"Gezhe - On Data Set 1 1 - Set Orientation Closest Creature 'Consortium Nether Runner'"),
(@ENTRY,0,1,0,38,0,100,0,2,2,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.513270,"Gezhe - On Data Set 2 2 - Set Orientation 2,513270");

DELETE FROM `creature_text` WHERE `entry` IN (19667, 18265);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- Consortium Nether Runner
(19667, 0, 0, 'Hail, Gezhe!  Seventeen more orders have been placed in the Hara''samid Sector.  The markets of heavy veldarite have all but collapsed, as you predicted.', 12, 0, 100, 0, 0, 0, 17184, 'Consortium Nether Runner'),
(19667, 0, 1, 'Heraazi exports are on the rise, sir.  We''ve already bribed the appropriate taxation officers and contraband will begin to arrive within days.', 12, 0, 100, 0, 0, 0, 17186, 'Consortium Nether Runner'),
(19667, 1, 0, 'By your leave, sir.', 12, 0, 100, 0, 0, 0, 17185, 'Consortium Nether Runner'),
-- Gezhe
(18265, 0, 0, 'Nothing surprising there.  Come back with some real news, will you?', 12, 0, 100, 0, 0, 0, 17191, 'Gezhe'),
(18265, 0, 1, 'As expected.  Very well, dismissed.', 12, 0, 100, 0, 0, 0, 17190, 'Gezhe');

DELETE FROM `waypoints` WHERE `entry`=19667;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(19667, 1, -2057.45, 8528.71, 24.6556, 'Consortium Nether Runner'),
(19667, 2, -2058.23, 8547.79, 24.0203, 'Consortium Nether Runner'),
(19667, 3, -2064.09, 8554.57, 23.9054, 'Consortium Nether Runner'),
(19667, 4, -2075.08, 8562.74, 22.9801, 'Consortium Nether Runner'),
(19667, 5, -2083.14, 8565.28, 22.0791, 'Consortium Nether Runner'),
(19667, 6, -2062.94, 8574.57, 23.1942, 'Consortium Nether Runner'),
(19667, 7, -2056.25, 8597.59, 21.3663, 'Consortium Nether Runner'),
(19667, 8, -2049.12, 8603.37, 22.7377, 'Consortium Nether Runner');

-- Nurse Judith -- http://wotlk.openwow.com/npc=19455
UPDATE `creature_template_addon` SET `bytes1`=0 WHERE  `entry`=19455;

-- Nurse Judith SAI
SET @ENTRY := 19455;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,19455,1,0,0,1,0,0,0,0,0,0,0,0,"Nurse Judith - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,3,40,0,100,0,2,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 2 Reached - Run Script"),
(@ENTRY,0,3,0,61,0,100,0,2,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.76273,"Nurse Judith - On Waypoint 2 Reached - Set Orientation 2,76273"),
(@ENTRY,0,4,5,40,0,100,0,5,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 5 Reached - Run Script"),
(@ENTRY,0,5,0,61,0,100,0,5,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.1529,"Nurse Judith - On Waypoint 5 Reached - Set Orientation 4,1529"),
(@ENTRY,0,5,6,40,0,100,0,7,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 7 Reached - Run Script"),
(@ENTRY,0,6,0,61,0,100,0,7,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.25502,"Nurse Judith - On Waypoint 7 Reached - Set Orientation 4.25502"),
(@ENTRY,0,7,8,40,0,100,0,9,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 9 Reached - Run Script"),
(@ENTRY,0,8,0,61,0,100,0,9,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,2.969,"Nurse Judith - On Waypoint 9 Reached - Set Orientation 2.969"),
(@ENTRY,0,9,10,40,0,100,0,12,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 12 Reached - Run Script"),
(@ENTRY,0,10,0,61,0,100,0,12,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.17224,"Nurse Judith - On Waypoint 12 Reached - Set Orientation 4.17224"),
(@ENTRY,0,11,12,40,0,100,0,15,19455,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Waypoint 15 Reached - Run Script"),
(@ENTRY,0,12,0,61,0,100,0,15,19455,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.1529,"Nurse Judith - On Waypoint 15 Reached - Set Orientation 2.41624");

-- Actionlist SAI
SET @ENTRY := 1945500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,12000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,4000,4000,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Script - Set Flag Standstate Kneel"),
(@ENTRY,9,2,0,0,0,100,0,6000,6000,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nurse Judith - On Script - Remove Flag Standstate Kneel");

DELETE FROM `waypoints` WHERE `entry`=19455;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(19455, 1, -365.789, 981.419,   54.195, 'Nurse Judith'),
(19455, 2, -368.858, 982.56,   54.1727, 'Nurse Judith'),
(19455, 3, -363.252, 980.47,   54.2127, 'Nurse Judith'),
(19455, 4, -357.494, 978.322,  54.2425, 'Nurse Judith'),
(19455, 5, -358.948, 975.922,  54.243,  'Nurse Judith'),
(19455, 6, -357.512, 973.939,  54.2493, 'Nurse Judith'),
(19455, 7, -360.045, 968.102,  54.2344, 'Nurse Judith'),
(19455, 8, -358.571, 965.26,   54.2447, 'Nurse Judith'),
(19455, 9, -362.99, 962.005,   54.2135, 'Nurse Judith'),
(19455, 10, -352.415, 973.809, 54.2617, 'Nurse Judith'),
(19455, 11, -349.296, 971.591, 54.2724, 'Nurse Judith'),
(19455, 12, -349.964, 970.224, 54.2733, 'Nurse Judith'),
(19455, 13, -350.597, 974.797, 54.2635, 'Nurse Judith'),
(19455, 14, -362.665, 981.665, 54.2171, 'Nurse Judith'),
(19455, 15, -364.29, 984.89,   54.2066, 'Nurse Judith');

-- Jessera of Mac'Aree SAI
SET @ENTRY := 17663;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,60000,60000,60000,60000,1,0,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Jessera of Mac'Aree - Out of Combat - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=17663;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17663, 0, 0, 'If only I could get back to the mushroom thicket. I have not seen such a mature cluster in my lifetime!', 12, 7, 100, 0, 0, 0, 14269, 'Jessera of Mac''Aree');

-- Messenger Hermesius SAI
SET @ENTRY := 17703;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,20000,25000,20000,25000,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Messenger Hermesius - Out of Combat - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=17703;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17703, 0, 0, 'Don\'t you people ever check the mail?', 12, 7, 100, 0, 0, 0, 14328, 'Messenger Hermesius'),
(17703, 0, 1, 'How am I expected to deliver all of these messages?', 12, 7, 100, 0, 0, 0, 14329, 'Messenger Hermesius'),
(17703, 0, 2, 'I need to be re-assigned. This job is pushing me over the edge...', 12, 7, 100, 0, 0, 0, 14330, 'Messenger Hermesius'),
(17703, 0, 3, 'He thinks he''s so damned important. Always pushing me around... I''ll show him. I''ll show all of them!', 12, 7, 100, 0, 0, 0, 14332, 'Messenger Hermesius'),
(17703, 0, 4, 'One of these days I''m going to bust out the crystalline doomhammer and go positively orc on this town. Then they''ll be sorry...', 12, 7, 100, 0, 0, 0, 14331, 'Messenger Hermesius');

-- Tuluun <Shaman Trainer> + Draenei Artificer Hate Conversation -- http://wotlk.openwow.com/npc=17212 + http://wotlk.openwow.com/npc=17228

UPDATE `creature` SET `position_x`=-4149.24, `position_y`=-12465, `position_z`=45.3466, `orientation`=5.67608 WHERE  `guid`=106052;

-- Draenei Artificer SAI
SET @ENTRY := 17228;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,20000,20000,176000,176000,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - Out of Combat - Run Script"),
(@ENTRY,0,1,0,1,0,100,0,65000,65000,176000,176000,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - Out of Combat - Run Script"),
(@ENTRY,0,2,0,1,0,100,0,118000,118000,176000,176000,80,@ENTRY*100+02,2,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - Out of Combat - Run Script");

-- Actionlist SAI
SET @ENTRY := 1722800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 0"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,66,0,0,0,0,0,0,10,61721,17212,0,0,0,0,0,"Draenei Artificer - On Script - Set Orientation Closest Creature 'Tuluun'"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,1,0,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Say Line 1"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.67608,"Draenei Artificer - On Script - Set Orientation 5.67608"),
(@ENTRY,9,5,0,0,0,100,0,2000,2000,0,0,17,234,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 234");

-- Actionlist SAI
SET @ENTRY := 1722801;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 0"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,10,61721,17212,0,0,0,0,0,"Draenei Artificer - On Script - Set Orientation Closest Creature 'Tuluun'"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,1,2,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Say Line 2"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.67608,"Draenei Artificer - On Script - Set Orientation 5.67608"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,17,234,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 234");

-- Actionlist SAI
SET @ENTRY := 1722802;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 0"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Run Off"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,69,0,0,0,0,0,0,8,0,0,0,-4160.31,-12458.7,44.5919,2.08552,"Draenei Artificer - On Script - Move To Position"),
(@ENTRY,9,3,0,0,0,100,0,8000,8000,0,0,66,0,0,0,0,0,0,10,61721,17212,0,0,0,0,0,"Draenei Artificer - On Script - Set Orientation Closest Creature 'Tuluun'"),
(@ENTRY,9,4,0,0,0,100,0,2000,2000,0,0,1,3,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Say Line 3"),
(@ENTRY,9,5,0,0,0,100,0,3000,3000,0,0,69,0,0,0,0,0,0,8,0,0,0,-4149.24,-12465,45.3466,5.67608,"Draenei Artificer - On Script - Move To Position"),
(@ENTRY,9,6,0,0,0,100,0,7000,7000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.67608,"Draenei Artificer - On Script - Set Orientation 5.67608"),
(@ENTRY,9,7,0,0,0,100,0,2000,2000,0,0,17,234,0,0,0,0,0,1,0,0,0,0,0,0,0,"Draenei Artificer - On Script - Set Emote State 234");

DELETE FROM `creature_text` WHERE `entry`=17228;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17228, 0, 0, 'How can I be expected to get any work done with that thing spreading its heresy?', 12, 0, 100, 0, 0, 0, 14383, 'Draenei Artificer'),
(17228, 1, 0, '%s sighs disgustedly as he turns back to his work.', 16, 0, 100, 0, 0, 0, 14384, 'Draenei Artificer'),
(17228, 2, 0, 'Turning to face Tuluun, the %s sneers contemptuously.', 16, 0, 100, 0, 0, 0, 14381, 'Draenei Artificer'),
(17228, 3, 0, 'We do not need your kind here.', 12, 0, 100, 0, 0, 0, 14382, 'Draenei Artificer');

-- The Sparklematic 5200 SAI
SET @ENTRY := 142487;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,20,0,100,0,0,0,0,0,93,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"The Sparklematic 5200 - On Quest Finished - Send Custom Animation 0");

-- The Sparklematic 5200 SAI
SET @ENTRY := 175084;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,20,0,100,0,0,0,0,0,93,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"The Sparklematic 5200 - On Quest Finished - Send Custom Animation 0");

-- The Sparklematic 5200 SAI
SET @ENTRY := 175085;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,20,0,100,0,0,0,0,0,93,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"The Sparklematic 5200 - On Quest Finished - Send Custom Animation 0");

-- The Demon Seed -- http://wotlk.openwow.com/quest=924
-- Ak'Zeloth SAI
SET @ENTRY := 3521;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,924,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Ak'Zeloth - On Quest 'The Demon Seed' Taken - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,924,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Ak'Zeloth - On Quest 'The Demon Seed' Taken - Store Targetlist");

-- Actionlist SAI
SET @ENTRY := 352100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.115413,"On Script - Set Orientation 0,115413"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,1,0,6000,0,0,0,0,12,1,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,10000,10000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.305800,"On Script - Set Orientation 5,305800");

DELETE FROM `creature_text` WHERE `entry`=3521;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(3521, 0, 0, 'Take a stone from the table, $n.  You will need it for your quest.', 12, 1, 100, 25, 0, 0, 1115, 'Ak''Zeloth');

-- Padaar -- http://wotlk.openwow.com/npc=16727
UPDATE `creature` SET `orientation` = 0.02, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 117605;
UPDATE `creature_template` SET `MovementType` = 0 WHERE `Entry` = 16727;

-- Padaar SAI
SET @ENTRY := 16727;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,16727,1,0,0,0,1,0,0,0,0,0,0,0,"Padaar - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Padaar - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,16727,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Padaar - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,0,40,0,100,0,2,16727,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Padaar - On Waypoint 2 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,3,16727,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Padaar - On Waypoint 3 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 1672700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,120000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,17,233,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 233"),
(@ENTRY,9,2,0,0,0,100,0,85000,85000,0,17,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 0");

-- Actionlist SAI
SET @ENTRY := 1672701;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,120000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 69"),
(@ENTRY,9,2,0,0,0,100,0,85000,85000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 0");

DELETE FROM `waypoints` WHERE `entry`=16727;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(16727,1,-3783.15,-11546.8,-134.781, 'Paddar'),
(16727,2,-3783.15,-11546.8,-134.781, 'Paddar'),
(16727,3,-3783.15,-11546.8,-134.781, 'Paddar');

-- Dekin -- http://wotlk.openwow.com/npc=16708
UPDATE `creature` SET `position_x` = -3751.045898, `position_y` = -11377.740234, `position_z` = -137.525208, `orientation` = 3.976810, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 102132;

-- Dekin SAI
SET @ENTRY := 16708;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,16708,1,0,0,0,1,0,0,0,0,0,0,0,"Dekin - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,6,16708,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Dekin - On Waypoint 6 Reached - Run Script"),
(@ENTRY,0,2,0,40,0,100,0,12,16708,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Dekin - On Waypoint 12 Reached - Run Script"),
(@ENTRY,0,3,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Dekin - On Gossip Hello - Pause Waypoint");

-- Actionlist SAI
SET @ENTRY := 1670800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,25000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Flag Standstate Kneel"),
(@ENTRY,9,2,0,0,0,100,0,19000,19000,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Remove Flag Standstate Kneel");

-- Actionlist SAI
SET @ENTRY := 1670801;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,245000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,17,379,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 379"),
(@ENTRY,9,2,0,0,0,100,0,235000,235000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 0"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,5,18,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 18");

DELETE FROM `waypoints` WHERE `entry`=16708;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(16708,1,-3746.39,-11372,-135.298, 'Dekin'),
(16708,2,-3743.14,-11371,-135.256, 'Dekin'),
(16708,3,-3734.39,-11370,-135.152, 'Dekin'),
(16708,4,-3728.64,-11371.5,-135.092, 'Dekin'),
(16708,5,-3722.89,-11376,-135.043, 'Dekin'),
(16708,6,-3721.73,-11378.3,-135.039, 'Dekin'),
(16708,7,-3722.61,-11376.3,-135.042, 'Dekin'),
(16708,8,-3728.57,-11371.6,-135.092, 'Dekin'),
(16708,9,-3734.09,-11369.9,-135.149, 'Dekin'),
(16708,10,-3742.64,-11370.7,-135.249, 'Dekin'),
(16708,11,-3746.09,-11372.2,-135.295, 'Dekin'),
(16708,12,-3751.04,-11377.7,-137.525, 'Dekin');

-- Netherologist Coppernickels -- http://wotlk.openwow.com/npc=19569
DELETE FROM `creature_template_addon` WHERE  `entry`=19569;

-- Netherologist Coppernickels SAI
SET @ENTRY := 19569;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,1,0,25,0,100,0,0,0,0,0,53,0,19569,1,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Reset - Start Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,19569,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Waypoint 1 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,7,19569,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Waypoint 7 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,11,19569,0,0,80,@ENTRY*100+02,2,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Waypoint 11 Reached - Run Script"),
(@ENTRY,0,5,0,40,0,100,0,14,19569,0,0,80,@ENTRY*100+03,2,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Waypoint 14 Reached - Run Script"),
(@ENTRY,0,6,0,40,0,100,0,22,19569,0,0,80,@ENTRY*100+04,2,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Waypoint 22 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 1956900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.74424,"Netherologist Coppernickels - On Script - Set Orientation 0.74424"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,54,45000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Pause Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,20000,20000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,19000,19000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,0,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Say Line 0");

-- Actionlist SAI
SET @ENTRY := 1956901;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.159118,"Netherologist Coppernickels - On Script - Set Orientation 0.159118"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,54,45000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Pause Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,20000,20000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,19000,19000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Say Line 1");

-- Actionlist SAI
SET @ENTRY := 1956902;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.81776,"Netherologist Coppernickels - On Script - Set Orientation 3.81776"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,54,45000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Pause Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,20000,20000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,19000,19000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,2,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Say Line 2");

-- Actionlist SAI
SET @ENTRY := 1956903;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.200999,"Netherologist Coppernickels - On Script - Set Orientation 0.200999"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,54,45000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Pause Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,20000,20000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,19000,19000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,2,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Say Line 2");

-- Actionlist SAI
SET @ENTRY := 1956904;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.118525,"Netherologist Coppernickels - On Script - Set Orientation 0.118525"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,54,45000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Pause Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,20000,20000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 133"),
(@ENTRY,9,3,0,0,0,100,0,19000,19000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Set Emote State 0"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,3,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Netherologist Coppernickels - On Script - Say Line 3");

DELETE FROM `creature_text` WHERE `entry`=19569;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(19569, 0, 0, 'I hope those blood elves across the way don''t think that I''m spying on them!', 12, 0, 100, 5, 0, 0, 16952, 'Netherologist Coppernickels'),
(19569, 1, 0, 'If I were smart, I''d be selling this research for a seat on the rocket ship.  I have a feeling we''re going to need to get out of here soon!', 12, 0, 100, 5, 0, 0, 16955, 'Netherologist Coppernickels'),
(19569, 2, 0, 'If this is right, we don''t have long! Oh well.', 12, 0, 100, 5, 0, 0, 16953, 'Netherologist Coppernickels'),
(19569, 3, 0, 'This is the last time that I buy second-hand equipment from an ethereal!', 12, 0, 100, 5, 0, 0, 16951, 'Netherologist Coppernickels');

DELETE FROM `waypoints` WHERE `entry`=19569;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(19569, 1, 3389.06, 4296.1, 120.848,   'Netherologist Coppernickels'),
(19569, 2, 3381.91, 4300.33, 120.185,  'Netherologist Coppernickels'),
(19569, 3, 3379.54, 4317.5, 121.136,   'Netherologist Coppernickels'),
(19569, 4, 3379.26, 4319.73, 122.64,   'Netherologist Coppernickels'),
(19569, 5, 3378.79, 4328.07, 122.64,   'Netherologist Coppernickels'),
(19569, 6, 3380.94, 4335.49, 122.64,   'Netherologist Coppernickels'),
(19569, 7, 3386.13, 4336.3, 122.64,    'Netherologist Coppernickels'),
(19569, 8, 3374.05, 4341.09, 122.684,  'Netherologist Coppernickels'),
(19569, 9, 3371.34, 4343.16, 122.712,  'Netherologist Coppernickels'),
(19569, 10, 3367.23, 4343.4, 122.719,  'Netherologist Coppernickels'),
(19569, 11, 3363.59, 4340.95, 122.697, 'Netherologist Coppernickels'),
(19569, 12, 3367.61, 4344.06, 122.727, 'Netherologist Coppernickels'),
(19569, 13, 3374.61, 4356.84, 123.634, 'Netherologist Coppernickels'),
(19569, 14, 3379.82, 4357.93, 123.631, 'Netherologist Coppernickels'),
(19569, 15, 3375.75, 4344.93, 122.725, 'Netherologist Coppernickels'),
(19569, 16, 3377.95, 4319.35, 122.641, 'Netherologist Coppernickels'),
(19569, 17, 3378.48, 4315.87, 121.045, 'Netherologist Coppernickels'),
(19569, 18, 3382.31, 4281.61, 121.106, 'Netherologist Coppernickels'),
(19569, 19, 3382.72, 4279, 122.64,     'Netherologist Coppernickels'),
(19569, 20, 3383.64, 4271.15, 122.64,  'Netherologist Coppernickels'),
(19569, 21, 3388.75, 4267.11, 122.64,  'Netherologist Coppernickels'),
(19569, 22, 3392.29, 4267.63, 122.64,  'Netherologist Coppernickels'),
(19569, 23, 3382.1, 4279.11, 122.64,   'Netherologist Coppernickels'),
(19569, 24, 3381.78, 4281.36, 121.115, 'Netherologist Coppernickels'),
(19569, 25, 3381.14, 4290.22, 120.607, 'Netherologist Coppernickels'),
(19569, 26, 3383.74, 4292.46, 120.963, 'Netherologist Coppernickels'),
(19569, 27, 3389.09, 4296.38, 120.831, 'Netherologist Coppernickels');

-- Swiftmane -- http://wotlk.openwow.com/npc=5831
SET @NPC := 69893;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `move_flag`) VALUES
(@PATH, 1, -702.322, -3593.87, 93.8325, 1),
(@PATH, 2, -685.513, -3594.81, 91.8542, 1),
(@PATH, 3, -664.382, -3573.64, 91.789, 1),
(@PATH, 4, -649.281, -3550.45, 91.789, 1),
(@PATH, 5, -631.25, -3524.16, 93.164, 1),
(@PATH, 6, -608.157, -3497.97, 94.3281, 1),
(@PATH, 7, -598.635, -3474.48, 92.5627, 1),
(@PATH, 8, -592.714, -3457.25, 91.789, 1),
(@PATH, 9, -568.124, -3434.6, 91.789, 1),
(@PATH, 10, -559.095, -3427.52, 91.789, 1),
(@PATH, 11, -532.917, -3435.41, 93.4966, 1),
(@PATH, 12, -521.856, -3444.02, 94.6973, 1),
(@PATH, 13, -508.566, -3457.7, 93.4341, 1),
(@PATH, 14, -493.592, -3469.16, 94.167, 1),
(@PATH, 15, -474.164, -3490.81, 93.2984, 1),
(@PATH, 16, -455.237, -3516.19, 95.3331, 1),
(@PATH, 17, -455.548, -3552.11, 91.914, 1),
(@PATH, 18, -464.342, -3564.53, 91.789, 1),
(@PATH, 19, -496.266, -3591.78, 93.5876, 1),
(@PATH, 20, -519.625, -3594.8, 93.7065, 1),
(@PATH, 21, -537.209, -3579.71, 93.4125, 1),
(@PATH, 22, -540.395, -3560.1, 93.164, 1),
(@PATH, 23, -546.26, -3538.08, 91.789, 1),
(@PATH, 24, -541.695, -3517.06, 91.789, 1),
(@PATH, 25, -544.309, -3489.59, 92.9094, 1),
(@PATH, 26, -554.641, -3469.96, 92.9621, 1),
(@PATH, 27, -567.257, -3454.55, 92.2805, 1),
(@PATH, 28, -583.176, -3440.61, 91.789, 1),
(@PATH, 29, -601.774, -3430.94, 91.789, 1),
(@PATH, 30, -623.082, -3433.78, 91.789, 1),
(@PATH, 31, -639.502, -3449.7, 93.8133, 1),
(@PATH, 32, -652.568, -3476.71, 93.8199, 1),
(@PATH, 33, -650.802, -3498.19, 91.8199, 1),
(@PATH, 34, -671.141, -3522.95, 91.8839, 1),
(@PATH, 35, -698.236, -3537.26, 92.664, 1),
(@PATH, 36, -714.682, -3548.67, 93.914, 1),
(@PATH, 37, -718.461, -3563.25, 92.414, 1),
(@PATH, 38, -715.575, -3582.03, 92.3202, 1);

-- Nekthar -- http://wotlk.openwow.com/npc=18248
-- Nekthar SAI
SET @ENTRY := 18248;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,18248,1,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,1,18248,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Waypoint 1 Reached - Say Line 0"),
(@ENTRY,0,2,0,40,0,100,0,4,18248,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Waypoint 4 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,8,18248,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Waypoint 8 Reached - Run Script"),
(@ENTRY,0,4,0,64,0,100,0,0,0,0,0,54,180000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Gossip Hello - Pause Waypoint");

-- Actionlist SAI
SET @ENTRY := 1824800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,240000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.63,"Nekthar - On Script - Set Orientation 5.63"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,90,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Script - Set Flag Standstate Sit Down"),
(@ENTRY,9,3,0,0,0,100,0,235000,235000,0,0,91,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Script - Remove Flag Standstate Sit Down");

-- Actionlist SAI
SET @ENTRY := 1824801;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,240000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.88,"Nekthar - On Script - Set Orientation 3,88"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Nekthar - On Script - Say Line 1");

DELETE FROM `creature_text` WHERE `entry`=18248;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(18248, 0, 0, 'Grunting, %s turns away from the wanted poster, returning to his warm spot by the fire.', 16, 0, 100, 0, 0, 0, 18782, 'Nekthar'),
(18248, 1, 0, '%s begins to read the wanted poster.', 16, 0, 100, 0, 0, 0, 18781, 'Nekthar');

SET @PATH := 18248;
DELETE FROM `waypoints` WHERE `entry`=@PATH;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(@PATH, 1, 2317.72, 6030.27, 142.57, 'Nekthar'),
(@PATH, 2, 2307.82, 6024.44, 142.56, 'Nekthar'),
(@PATH, 3, 2304.89, 6025.77, 142.96, 'Nekthar'),
(@PATH, 4, 2304.89, 6025.77, 142.96, 'Nekthar'), 
(@PATH, 5, 2306.93, 6023.86, 142.56, 'Nekthar'), 
(@PATH, 6, 2317.38, 6030, 142.57, 'Nekthar'),
(@PATH, 7, 2319.51, 6029.16, 142.62, 'Nekthar'),
(@PATH, 8, 2319.51, 6029.16, 142.62, 'Nekthar');

-- Consortium Overseer -- http://wotlk.openwow.com/npc=18274
SET @MAXGUID := 914; 
SET @SCRIPTID := 12066200;
DELETE FROM `waypoint_scripts` WHERE `id` IN (@SCRIPTID+0);
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
(@SCRIPTID+0, 15, 0, 0, 0, 16960, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1));

SET @NPC := 120662;
SET @POINT := 0;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `delay`, `action`, `orientation`) VALUES
(@PATH, 1, -2086, 8569.04, 21.5988, 0, 0, 2.25497),
(@PATH, 2, -2104.1, 8587.69, 18.2267, 0, 0, 2.44975),
(@PATH, 3, -2076.87, 8558.08, 22.9817, 0, 0, 5.41856),
(@PATH, 4, -2065.22, 8536.39, 23.9165, 60000, @SCRIPTID, 5.10126),
(@PATH, 5, -2074.9, 8545.01, 22.9726, 0, 0, 1.88819),
(@PATH, 6, -2078.75, 8557.38, 22.8488, 0, 0, 2.1081),
(@PATH, 7, -2086.85, 8569.92, 21.4582, 0, 0, 2.22591),
(@PATH, 8, -2103.09, 8588.28, 18.2584, 0, 0, 0.444629),
(@PATH, 9, -2091.81, 8599.37, 20.2861, 0, 0, 0.0809899),
(@PATH, 10, -2068.57, 8594.29, 21.5667, 0, 0, 5.19236),
(@PATH, 11, -2064.07, 8583.91, 21.4222, 0, 0, 4.72034),
(@PATH, 12, -2063.53, 8571.92, 23.4418, 0, 0, 4.72539),
(@PATH, 13, -2063.37, 8546.69, 23.9351, 60000,0, 4.61544);

UPDATE `waypoint_data` SET `action_chance`=100 WHERE `action` IN (@SCRIPTID);

-- Durkot Wolfbrother -- http://wotlk.openwow.com/npc=26044
SET @MAXGUID := 911; 
SET @SCRIPTID := 15362300;
DELETE FROM `waypoint_scripts` WHERE `id` IN (@SCRIPTID+0);
INSERT INTO `waypoint_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`, `guid`) VALUES 
(@SCRIPTID+0, 2, 15, 68442, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+0, 10, 1, 1, 0, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1)),
(@SCRIPTID+0, 18, 14, 68442, 1, 0, 0, 0, 0, 0, (@MAXGUID := @MAXGUID + 1));

SET @NPC := 153623;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `delay`, `action`, `orientation`) VALUES
(@PATH, 1, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 2, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 3, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 4, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 5, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 6, 2791.75, 6244.01, 84.6092, 22000, @SCRIPTID, 0.717889),
(@PATH, 7, 2786.42, 6245.35, 84.3805, 0, 0, 2.43222),
(@PATH, 8, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 9, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 10, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 11, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 12, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 13, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 14, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 15, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 16, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 17, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 18, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 19, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 20, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 21, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 22, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 23, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 24, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 25, 2775.46, 6252.13, 83.9431, 22000, @SCRIPTID, 3.87735),
(@PATH, 26, 2780.02, 6251.43, 84.0246, 0, 0, 5.66806),
(@PATH, 27, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 28, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 29, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 30, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 31, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 32, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 33, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 34, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 35, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 36, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 37, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 38, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 39, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 40, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 41, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 42, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 43, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 44, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026),
(@PATH, 45, 2788.16, 6243.06, 84.4932, 5000, 0, 5.38473),
(@PATH, 46, 2773.72, 6258.83, 83.6357, 5000, 0, 2.29026);

UPDATE `waypoint_data` SET `action_chance`=100 WHERE `action` IN (@SCRIPTID);

-- DB/Creature: Taleris Dawngazer - Emoteevent
DELETE FROM `waypoints` WHERE `entry`=17015;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`) VALUES 
(17015,1,-650.434021,4136.919922,64.620346),
(17015,2,-649.897,4149.45,64.13),
(17015,3,-650.945,4153.25,64.3724),
(17015,4,-653.408,4159.69,65.9067),
(17015,5,-649.211,4169.54,67.9885),
(17015,6,-647.255,4172.52,68.3677),
(17015,7,-646.951,4176.908,68.51362),
(17015,8,-653.285,4163.94,67.01),
(17015,9,-651.272,4154.21,64.553),
(17015,10,-649.733,4147.03,64.231),
(17015,11,-650.434021,4136.919922,64.620346);

-- Taleris Dawngazer SAI
SET @ENTRY := 17015;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,17015,1,0,0,0,1,0,0,0,0,0,0,0,"Taleris Dawngazer - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,7,17015,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Taleris Dawngazer - On Waypoint 7 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 1701500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,30000,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,"On Script - Play Emote 2"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,10,11,7,6,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Random Emote (11, 7, 6)"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,10,7,0,11,6,0,0,19,17046,20,0,0,0,0,0,"On Script - Play Random Emote (7, 0, 11, 6)"),
(@ENTRY,9,4,0,0,0,100,0,4000,4000,0,0,10,11,6,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Random Emote (11, 6)"),
(@ENTRY,9,5,0,0,0,100,0,6000,6000,0,0,10,7,11,6,0,0,0,19,17046,20,0,0,0,0,0,"On Script - Play Random Emote (7, 11, 6)");

-- some inhabittypefixes for flying creatures
UPDATE `creature_template` SET `InhabitType`=4 WHERE  `entry` IN (6141,4015,2830);

-- Elwyn forrest improvements, from mangos & tc
UPDATE `creature` SET `SpawnDist`=8, `MovementType`=1 WHERE `id` IN (798,800,802,801);
UPDATE `creature` SET `SpawnDist`=15, `MovementType`=1 WHERE `id` IN (30,113,118,299,524,525,822,1922,883,1933,2442,721,1412) AND `MovementType` IN (0,1);
-- =============================
-- General Fix NPC Respawn Times - Existing Times Are Too Short For Almost All NPC
-- =============================
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 30;   -- Forest Spider (30)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 40;   -- Kobold Miner (40)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 43;   -- Mine Spider (43)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 46;   -- Murloc Forager (46)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 94;   -- Defias Catpurse (94)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 97;   -- Riverpaw Runt (97)
UPDATE `creature` SET `spawntimesecs`=180 WHERE `id`= 113;  -- Stonetusk Boar (113)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 116;  -- Defias Bandit (116)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 118;  -- Prowler (118)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 285;  -- Murloc (285)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 299;  -- Young Wolf (299)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 327;  -- Goldtooth (327)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 448;  -- Hogger (448)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 474;  -- Defias Rogue Wizard (474)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 475;  -- Kobold Tunneler (475)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 476;  -- Kobold Geomancer (476)
UPDATE `creature` SET `spawntimesecs`=360 WHERE `id`= 478;  -- Riverpaw Outrunner (478)
UPDATE `creature` SET `spawntimesecs`=180 WHERE `id`= 524;  -- Rockh`id`e Boar (524)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 525;  -- Mangy Wolf (525)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 732;  -- Murloc Lurker (732)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 822;  -- Young Forest Bear (822)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 1922; -- Grey Forest Wolf (1922)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 6093; -- Dead-Tooth Jack (6093)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 6846; -- Defias Dockmaster (6846)
UPDATE `creature` SET `spawntimesecs`=300 WHERE `id`= 6927; -- Defias Dockworker (6927)

-- 
UPDATE `gameobject_template` SET `Data0`=1505 WHERE `entry`=188112;
DELETE FROM `gameobject_template` WHERE `entry`=300184;
DELETE FROM `gameobject` WHERE `id`=300184;
SET @ENTRY := 26161;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY*100 AND `source_type`=9;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,1,46574,0,0,0,80,@ENTRY*100,0,0,0,0,0,1,0,0,0,0,0,0,0,"Farshire Grain Credit - On Spellhit 'Burn Grain' - Action list"),
(@ENTRY*100,9,0,0,0,0,100,0,0,0,0,0,33,26161,0,0,0,0,0,7,0,0,0,0,0,0,0,"Farshire Grain Credit - Action list - Quest Credit 'Take No Chances'"),
(@ENTRY*100,9,1,0,0,0,100,0,5000,5000,0,0,99,3,0,0,0,0,0,20,188112,10,0,0,0,0,0,"Farshire Grain Credit - Action list - Set loot state to Despawn gob"),
(@ENTRY*100,9,2,0,0,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Farshire Grain Credit - Action list - Despawn");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=46574;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 3, 46574, 0, 0, 31, 0, 3, 26161, 0, 0, 0, 0, '', 'Spell effect on Farshire Grain Credit');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=21060 AND  `SourceEntry`= 23612;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=21061 AND  `SourceEntry`= 23612;

-- Craftsman's Writ
UPDATE `quest_template` SET `SpecialFlags`=1 WHERE  `ID`=9142;

-- Brother Daniels SAI -- http://www.wowhead.com/npc=19447/brother-daniels
SET @ENTRY := 19447;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,19447,1,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,2,19447,0,0,54,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Waypoint 2 Reached - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,3,19447,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Waypoint 3 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,5,19447,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Waypoint 5 Reached - Run Script");

-- Actionlist SAI
SET @ENTRY := 1944700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,155000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.47099,"Brother Daniels - On Script - Set Orientation 4.47099"),
(@ENTRY,9,2,0,0,0,100,0,15000,15000,0,0,1,0,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,15000,15000,0,0,75,68442,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Set Flag Standstate Kneel"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,1,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Say Line 1"),
(@ENTRY,9,5,0,0,0,100,0,119000,119000,0,0,28,68442,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Remove Flag Standstate Stand Up");

-- Actionlist SAI
SET @ENTRY := 1944701;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,155000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.00368,"Brother Daniels - On Script - Set Orientation 4.00368"),
(@ENTRY,9,2,0,0,0,100,0,15000,15000,0,0,1,2,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Say Line 2"),
(@ENTRY,9,3,0,0,0,100,0,15000,15000,0,0,75,68442,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Set Flag Standstate Kneel"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,3,10000,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Say Line 3"),
(@ENTRY,9,5,0,0,0,100,0,119000,119000,0,0,28,68442,0,0,0,0,0,1,0,0,0,0,0,0,0,"Brother Daniels - On Script - Remove Flag Standstate Stand Up");

DELETE FROM `creature_text` WHERE `entry`=19447;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(19447, 0, 0, 'Light, lead the fallen from untruth to truth, from darkness to Light and from death to immortality.', 12, 0, 100, 0, 0, 0, 16769, 'Brother Daniels'),
(19447, 1, 0, 'Light be with you now and forever.', 12, 0, 100, 0, 0, 0, 16770, 'Brother Daniels'),
(19447, 2, 0, 'Light, give me strength.', 12, 0, 100, 0, 0, 0, 16767, 'Brother Daniels'),
(19447, 3, 0, 'The priest kneels in silent prayer.', 16, 0, 100, 0, 0, 0, 16772, 'Brother Daniels');

DELETE FROM `waypoints` WHERE `entry`=19447;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(19447, 1, -365.8, 980.679, 54.1955,   'Brother Daniels'),
(19447, 2, -354.904, 978.248, 54.2496, 'Brother Daniels'),
(19447, 3, -354.904, 978.248, 54.2496, 'Brother Daniels'),
(19447, 4, -366.477, 980.557, 54.1895, 'Brother Daniels'),
(19447, 5, -369.43, 977.106, 54.1679,  'Brother Daniels');

-- Razia SAI -- http://www.wowhead.com/npc=16643/razia
SET @ENTRY := 16643;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,16643,1,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,1,16643,0,0,54,30000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,3,16643,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Waypoint 3 Reached - Run Script"),
(@ENTRY,0,3,0,40,0,100,0,6,16643,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Waypoint 6 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,9,16643,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Waypoint 9 Reached - Pause Waypoint"),
(@ENTRY,0,5,0,64,0,100,0,0,0,0,0,54,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Gossip Hello - Pause Waypoint");

-- Actionlist SAI
SET @ENTRY := 1664300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,5,25,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Script - Play Emote 25");

-- Actionlist SAI
SET @ENTRY := 1664301;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,5,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Razia - On Script - Play Emote 1");

DELETE FROM `waypoints` WHERE `entry`=16643;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(16643, 1, 10013.3, -7214.86, 32.0748, 'Razia'),
(16643, 2, 10013.5, -7217.56, 32.0748, 'Razia'),
(16643, 3, 10016.2, -7218.02, 32.0748, 'Razia'),
(16643, 4, 10014.7, -7217.3, 32.0764,  'Razia'),
(16643, 5, 10017.5, -7213.64, 32.0764, 'Razia'),
(16643, 6, 10017.7, -7210.42, 32.0756, 'Razia'),
(16643, 7, 10017.5, -7213.48, 32.0764, 'Razia'),
(16643, 8, 10013.3, -7214.86, 32.0748, 'Razia'),
(16643, 9, 10013.3, -7214.86, 32.0748, 'Razia');

UPDATE `creature` SET `Spawndist` = 0, `MovementType` = 0 WHERE `guid` IN (102129, 102068);

-- Technician Dyvuun SAI -- http://www.wowhead.com/npc=16551
SET @ENTRY := 16551;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,16551,1,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,64,0,100,0,0,0,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Gossip Hello - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,16551,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,0,40,0,100,0,2,16551,0,0,54,46000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 2 Reached - Pause Waypoint"),
(@ENTRY,0,4,0,40,0,100,0,3,16551,0,0,54,90000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 3 Reached - Pause Waypoint"),
(@ENTRY,0,5,6,40,0,100,0,4,16551,0,0,54,65000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 4 Reached - Pause Waypoint"),
(@ENTRY,0,6,0,61,0,100,0,4,16551,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 4 Reached - Run Script"),
(@ENTRY,0,7,0,40,0,100,0,5,16551,0,0,54,100000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Technician Dyvuun - On Waypoint 5 Reached - Pause Waypoint");

DELETE FROM `waypoints` WHERE `entry`=@ENTRY;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(@ENTRY, 1, -4158.32, -12483.1, 45.4554, 'Technician Dyvuun'),
(@ENTRY, 2, -4157.08, -12480.4, 45.2039, 'Technician Dyvuun'),
(@ENTRY, 3, -4158.32, -12483.1, 45.4554, 'Technician Dyvuun'),
(@ENTRY, 4, -4157.08, -12480.4, 45.2039, 'Technician Dyvuun'),
(@ENTRY, 5, -4158.32, -12483.1, 45.4554, 'Technician Dyvuun');

-- Actionlist SAI
SET @ENTRY := 1655100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 69"),
(@ENTRY,9,1,0,0,0,100,0,7000,7000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,9,0,0,0,0,0,0,20,181805,100,0,0,0,0,0,"On Script - Activate Gameobject"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,5,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 4"),
(@ENTRY,9,4,0,0,0,100,0,0,0,0,0,12,17268,3,27000,0,0,0,8,0,0,0,-4156.35,-12477.5,46.1839,3.12414,"On Script - Summon Creature 'Image of Technician Zhanaa'"),
(@ENTRY,9,5,0,0,0,100,0,5000,5000,0,0,1,0,0,0,0,0,0,19,17268,100,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,6,0,0,0,100,0,4000,4000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,7,0,0,0,100,0,9000,9000,0,0,1,1,0,0,0,0,0,19,17268,100,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,8,0,0,0,100,0,9000,9000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,9,0,0,0,100,0,6000,6000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 69"),
(@ENTRY,9,10,0,0,0,100,0,15000,15000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 0");

-- must be summned by script
DELETE FROM `creature` WHERE guid = 106077;

DELETE FROM `creature_text` WHERE `entry`=16551;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(16551, 0, 0, 'Yes, Zhanna, it''s me! Your signal is weak.', 12, 0, 100, 0, 0, 0, 13687, 'Technician Dyvuun'),
(16551, 1, 0, 'Zhanna! Gah, I have to get this thing fixed!', 12, 0, 100, 0, 0, 0, 13692, 'Technician Dyvuun');

DELETE FROM `creature_text` WHERE `entry`=17268;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17268, 0, 0, 'Dyvuun, is that you?', 12, 0, 100, 0, 0, 0, 13686, 'Image of Technician Zhanaa'),
(17268, 1, 0, 'I can barely hear you... <static> aking up!', 12, 0, 100, 0, 0, 0, 13688, 'Image of Technician Zhanaa');

-- Kayneth Stillwind -- http://wotlk.openwow.com/npc=3848
DELETE FROM `waypoints` WHERE `entry`=3848;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(3848, 1, 2954.25,-3215.41,169.205, 'Kayneth Stillwind '),
(3848, 2, 2966.65,-3213.95,168.914, 'Kayneth Stillwind '),
(3848, 3, 2954.23,-3215.45,169.206, 'Kayneth Stillwind ');

-- Kayneth Stillwind SAI
SET @ENTRY := 3848;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kayneth Stillwind - Between 0-15% Health - Flee For Assist (No Repeat)"),
(@ENTRY,0,1,0,25,0,100,0,0,0,0,0,53,0,3848,1,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Reset - Start Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,1,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,4,40,0,100,0,2,3848,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Waypoint 2 Reached - Pause Waypoint"),
(@ENTRY,0,4,0,61,0,100,0,2,3848,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Waypoint 2 Reached - Run Script"),
(@ENTRY,0,5,0,40,0,100,0,3,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Waypoint 3 Reached - Pause Waypoint");

-- Actionlist SAI
SET @ENTRY := 384800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.965562,"Kayneth Stillwind - On Script - Set Orientation 0.965562"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 69"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 0"),
(@ENTRY,9,3,0,0,0,100,0,3000,3000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=3848;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextId`) VALUES 
(3848, 0, 0, 'Putting the bottles away, %s sighs.', 16, 0, 100, 0, 0, 0, 'Kayneth Stillwind', 14108);


-- 
-- Erin -- http://www.wowhead.com/npc=850/erin#comments
SET @NPC := 41732;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,4097,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`, `delay`, `move_flag`) VALUES
(@PATH, 1, -9174.33, -2109.71, 88.95, 20000, 1),
(@PATH, 2, -9192.52, -2098.69, 87.7837, 0, 1),
(@PATH, 3, -9208.23, -2090.51, 85.3921, 0, 1),
(@PATH, 4, -9220.96, -2089.61, 81.1544, 0, 1),
(@PATH, 5, -9234.93, -2085.46, 76.4945, 0, 1),
(@PATH, 6, -9242.56, -2096.16, 73.6174, 0, 1),
(@PATH, 7, -9249.78, -2122.28, 65.0417, 0, 1),
(@PATH, 8, -9256.07, -2139.82, 64.0223, 0, 1),
(@PATH, 9, -9254.31, -2147.89, 64.0695, 0, 1),
(@PATH, 10, -9238.9, -2148.92, 64.3411, 0, 1),
(@PATH, 11, -9237.57, -2152.63, 64.3492, 0, 1),
(@PATH, 12, -9218.89, -2155.42, 64.3544, 0, 1),
(@PATH, 13, -9216.46, -2151.99, 64.3544, 1000, 0),
(@PATH, 14, -9216.22, -2147.52, 64.3544, 0, 0),
(@PATH, 15, -9216.22, -2147.52, 64.3544, 60000, 1),
(@PATH, 16, -9218.43, -2154.78, 64.3544, 0, 1),
(@PATH, 17, -9231.99, -2155.23, 64.3576, 0, 1),
(@PATH, 18, -9237.01, -2157.84, 64.3576, 0, 1),
(@PATH, 19, -9237.87, -2149.49, 64.3426, 0, 1),
(@PATH, 20, -9248.67, -2148.45, 63.9334, 0, 1),
(@PATH, 21, -9256.32, -2138.6, 63.8953, 0, 1),
(@PATH, 22, -9251.08, -2121.47, 65.2993, 0, 1),
(@PATH, 23, -9253.26, -2112.42, 66.5815, 0, 1),
(@PATH, 24, -9261.04, -2106.96, 66.8522, 15000, 0),
(@PATH, 25, -9251.76, -2113.45, 66.6836, 0, 0),
(@PATH, 26, -9246.46, -2127.76, 64.1511, 0, 0),
(@PATH, 27, -9248.61, -2135.95, 63.9343, 0, 0),
(@PATH, 28, -9254.32, -2148.24, 64.056, 0, 0),
(@PATH, 29, -9271.21, -2158.97, 61.1872, 0, 0),
(@PATH, 30, -9277.96, -2157.76, 59.2388, 0, 0),
(@PATH, 31, -9310.03, -2148.39, 63.4841, 60000, 1),
(@PATH, 32, -9300.57, -2151.61, 63.3088, 0, 1),
(@PATH, 33, -9278.2, -2158.05, 59.202, 0, 1),
(@PATH, 34, -9261.5, -2157.36, 64.0706, 0, 1),
(@PATH, 35, -9252.47, -2139.73, 64.0216, 0, 1),
(@PATH, 36, -9244.6, -2120.1, 65.1008, 0, 1),
(@PATH, 37, -9242.08, -2095.47, 73.8924, 0, 1),
(@PATH, 38, -9233.67, -2085.3, 76.8089, 0, 1),
(@PATH, 39, -9219.75, -2090.06, 81.5803, 0, 1),
(@PATH, 40, -9207.11, -2090.25, 85.8603, 0, 1),
(@PATH, 41, -9198.75, -2092.32, 87.6703, 0, 1);

DELETE FROM `creature_template_addon` WHERE `entry`=16408;
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES 
(16408,0,0,0,0,0, '18950');

UPDATE `creature_text` SET `sound`=6209 WHERE  `entry`=11218 AND `groupid`=0 AND `id`=0;

-- 
-- [Q] The Fel and the Furious -- http://wotlk.openwow.com/?quest=10613
-- Nakansi SAI
SET @ENTRY := 21789;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,10613,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Nakansi - On Quest 'The Fel and the Furious' Taken - Run Script");

-- Actionlist SAI
SET @ENTRY := 2178900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.989615,"Nakansi - On Script - Set Oriebtation 5.989615"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,5,25,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nakansi - On Script - Play Emote 25"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,1,0,7000,0,0,0,0,1,0,0,0,0,0,0,0,"Nakansi - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,5000,5000,0,0,66,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Nakansi - On Script - ReSet Oriebtation");


DELETE FROM `creature_text` WHERE `entry`=21789;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(21789, 0, 0, 'I''ve rigged a few control consoles down the slope. Get one quickly and take control of the fel reaver! You''ll only have a limited amount of time.', 12, 0, 100, 0, 0, 0, 19666, 'Nakansi');

SET @ENTRY := 2179000;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9 AND `id`=3;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,3,0,0,0,100,0,5000,5000,0,0,66,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Plexi - On Script - ReSet Oriebtation");

-- [Q] Galaen's Fate -- http://wotlk.openwow.com/quest=9579
-- Galaen's Corpse SAI
SET @ENTRY := 17508;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,9579,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Galaen's Corpse - On Quest 'Galaen's Fate' Taken - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,9579,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Galaen's Corpse - On Quest 'Galaen's Fate' Taken - Store Targetlist"),
(@ENTRY,0,2,0,17,0,100,0,17426,0,0,0,64,2,0,0,0,0,0,7,0,0,0,0,0,0,0,"Galaen's Corpse - On Summoned Unit - Store Targetlist");

-- Conditions for summon
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceGroup`=1 AND `SourceEntry`=17508;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,17508,0,0,29,0,17426,20,0,1,0,0,"","Galaen's Corpse - Do not summon Galaen if creature is in 20 yards range");

-- Actionlist SAI
SET @ENTRY := 1750800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,12,17426,1,15000,0,0,0,8,0,0,0,-2090.03,-11297.5,63.5025,3.56455,"Galaen's Corpse - On Script - Summon Creature 'Galaen'"),
(@ENTRY,9,1,0,0,0,100,0,100,100,0,0,100,1,0,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Send Target 1");

-- Actionlist SAI
SET @ENTRY := 1750800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,12,17426,1,15000,0,0,0,8,0,0,0,-2090.03,-11297.5,63.5025,3.56455,"Galaen's Corpse - On Script - Summon Creature 'Galaen'"),
(@ENTRY,9,1,0,0,0,100,0,100,100,0,0,100,1,0,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Send Target 1"),
(@ENTRY,9,2,0,0,0,100,0,100,100,0,0,45,1,1,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Set Data 1 1"),
(@ENTRY,9,3,0,0,0,100,0,1000,1000,0,0,1,0,3000,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,1,1,4000,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Say Line 1"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,1,2,4000,0,0,0,0,12,2,0,0,0,0,0,0,"Galaen's Corpse - On Script - Say Line 2");

-- Galaen SAI
SET @ENTRY := 17426;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,0,1,1,0,0,66,0,0,0,0,0,0,12,1,0,0,0,0,0,0,"Galaen - On Data Set 1 1 - Set Orientation Stored Payer");

DELETE FROM `creature_text` WHERE `entry`=17426;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17426, 0, 0, 'Morae... Is that you?', 12, 0, 100, 6, 0, 0, 14262, 'Galaen'),
(17426, 1, 0, 'No... you''re not Morae, but I sense that you have met. When you see her, tell her that I survived the crash, only to be done in by the infiltrators.', 12, 0, 100, 274, 0, 0, 14263, 'Galaen'),
(17426, 2, 0, 'Let her know... my last thought was of her...', 12, 0, 100, 1, 0, 0, 14264, 'Galaen');

-- [Q] Vyletongue Corruption -- http://wotlk.openwow.com/?quest=7029
-- Vark Battlescar SAI
SET @ENTRY := 11823;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,7029,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Vark Battlescar - On Quest 'Vyletongue Corruption' Taken - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,7029,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Vark Battlescar - On Quest 'Vyletongue Corruption' Taken - Store Targetlist");

-- Actionlist SAI
SET @ENTRY := 1182300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,1,0,5000,0,0,0,0,12,1,0,0,0,0,0,0,"Vark Battlescar - On Script - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=11823;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(11823, 0, 0, '$n, please take a look at the sample of the Vylestem vine that I took from the caverns, so that you know what to look for...', 12, 0, 100, 0, 0, 0, 8923, 'Vark Battlescar');

-- [Q] World of Shadows -- http://wotlk.openwow.com/quest=11004
-- Severin SAI
SET @ENTRY := 23042;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,11004,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Severin - On Quest 'World of Shadows' Taken - Run Script");

-- Actionlist SAI
SET @ENTRY := 2304200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,7000,0,0,0,0,19,23038,10,0,0,0,0,0,"Severin - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,6000,6000,0,0,5,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"Severin - On Script - Play Emote 16"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Severin - On Script - Say Line 1");

DELETE FROM `creature_text` WHERE `entry` IN (23038, 23042);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(23038, 0, 0, 'The arakkoa are hidden... everywhere!', 12, 0, 100, 0, 0, 0, 21917, 'Sky Commander Adaris'),
(23042, 0, 0, 'Rest now, Adaris. You need to recover your strength.', 12, 0, 100, 0, 0, 0, 21918, 'Severin');

-- Overlord Runthak SAI
SET @ENTRY := 14392;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,20,0,100,0,7784,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Overlord Runthak - On Quest 'The Lord of Blackrock' Finished - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,7784,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Overlord Runthak - On Quest 'The Lord of Blackrock' Finished - Store Targetlist");

-- Actionlist SAI
SET @ENTRY := 1439200;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Overlord Runthak - On Script - Remove Npc Flag Questgiver"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Overlord Runthak - On Script - Set Run Off"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,1544,-4425.87,10.9056,3.323,"Overlord Runthak - On Script - Move To Position"),
(@ENTRY,9,3,0,0,0,100,0,14000,14000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.6367,"Overlord Runthak - On Script - Set Orientation 0,6367"),
(@ENTRY,9,4,0,0,0,100,0,3000,3000,0,0,1,0,17000,0,0,0,0,12,1,0,0,0,0,0,0,"Overlord Runthak - On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,17000,17000,0,0,1,1,10000,0,0,0,0,12,1,0,0,0,0,0,0,"Overlord Runthak - On Script - Say Line 1"),
(@ENTRY,9,6,0,0,0,100,0,3000,3000,0,0,50,179881,21600,0,0,0,0,8,0,0,0,1540.28,-4422.19,7.0051,5.22833,"Overlord Runthak - On Script - Summon Gameobject 'The Severed Head of Nefarian'"),
(@ENTRY,9,7,0,0,0,100,0,5000,5000,0,0,9,0,0,0,0,0,0,20,179881,100,0,0,0,0,0,"Overlord Runthak - On Script - Activate Gameobject"),
(@ENTRY,9,8,0,0,0,100,0,5000,5000,0,0,11,22888,0,0,0,0,0,1,0,0,0,0,0,0,0,"Overlord Runthak - On Script - Cast 'Rallying Cry of the Dragonslayer'"),
(@ENTRY,9,9,0,0,0,100,0,10000,10000,0,0,69,0,0,0,0,0,0,8,0,0,0,1568,-4405.87,8.13371,0.3434,"Overlord Runthak - On Script - Move To Position"),
(@ENTRY,9,10,0,0,0,100,0,15000,15000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.31613,"Overlord Runthak - On Script - Set Orientation 3,31613"),
(@ENTRY,9,11,0,0,0,100,0,1000,1000,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Overlord Runthak - On Script - Add Npc Flag Questgiver");

DELETE FROM `creature_text` WHERE `entry` IN (14392);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`, `BroadcastTextId`) VALUES 
(14392, 0, 0, 'NEFARIAN IS SLAIN! people of Orgrimmar, bow down before the might of $N and his allies for they have laid a blow against the Black Dragonflight that is sure to stir the Aspects from their malaise! This defeat shall surely be felt by the father of the Black Flight: Deathwing reels in pain and anguish this day!', 14, 0, 100, 0, 0, 0, 'Overlord Runthak', 9867),
(14392, 1, 0, 'Be lifted by $N accomplishment! Revel in his rallying cry!', 14, 0, 100, 0, 0, 0, 'Overlord Runthak', 9868);

-- 
-- Highlord Bolvar Fordragon SAI
SET @ENTRY := 27872;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,12499,0,0,0,68,14,0,0,0,0,0,7,0,0,0,0,0,0,0,"Highlord Bolvar Fordragon - On Quest 'Return To Angrathar' Finished - Play Movie 14");

-- 
-- [Q] Strange Brew -- http://wotlk.openwow.com/quest=10511
-- Borgrim Stouthammer SAI
SET @ENTRY := 21151;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,10511,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Borgrim Stouthammer - On Quest 'Strange Brew' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 2115100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,5000,5000,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Borgrim Stouthammer - On Script - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=21151;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(21151, 0, 0, 'ACK! That''s the worst thing I''ve ever tasted! I wouldn''t let my ram drink that!', 12, 0, 100, 1, 0, 0, 18830, 'Borgrim Stouthammer');

-- 
-- [Q] Gamel the Cruel -- http://wotlk.openwow.com/quest=11619
-- Karuk SAI
SET @ENTRY := 25435;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,11619,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Karuk - On Quest 'Gamel the Cruel' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 2543500;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,12,26450,1,15000,0,0,0,8,0,0,0,2003.83,5851.17,36.2061,2.75762,"Karuk - On Script - Summon Creature 'Ariut'"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,1,0,10000,0,0,0,0,19,26450,20,0,0,0,0,0,"Karuk - On Script - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=26450;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(26450, 0, 0, 'The fallen of Pal\'ea will not rest, son.  Find the naga priestess!', 12, 0, 100, 1, 0, 0, 25684, 'Ariut');

-- [Q] Know Thine Enemy -- http://wotlk.openwow.com/quest=9567
-- Vindicator Aalesia SAI
SET @ENTRY := 17433;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,20,0,100,0,9567,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Quest 'Know Thine Enemy' Finished - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Vindicator Aalesia - On Quest 'Know Thine Enemy' Finished - Store Targetlist");

-- Actionlist SAI
SET @ENTRY := 1743300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Remove Npc Flag Questgiver"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Set Emote State 69"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,17,26,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Set Emote State 26"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,1,0,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,4000,4000,0,0,1,1,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Say Line 1"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,1,2,2000,0,0,0,0,12,1,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Say Line 2"),
(@ENTRY,9,6,0,0,0,100,0,2000,2000,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vindicator Aalesia - On Script - Add Npc Flag Questgiver");

DELETE FROM `creature_text` WHERE `entry`=17433;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17433, 0, 0, 'These pictographs tell a story. This figure at the beginning, almost looks like one of the creatures we encountered at Azure Watch...', 12, 0, 100, 0, 0, 0, 14030, 'Vindicator Aalesia'),
(17433, 1, 0, 'No, this can\'t be... It says this creature willingly became a servant of the Legion. He transforms into a satyr and receives the Legion\'s "blessing.', 12, 0, 100, 274, 0, 0, 14031, 'Vindicator Aalesia'),
(17433, 2, 0, 'We must defeat these satyr, $n. We will not suffer the Legion\'s presence in these lands.', 12, 0, 100, 53, 0, 0, 14032, 'Vindicator Aalesia');

-- 
-- [Q] All Clear! -- http://wotlk.openwow.com/quest=10436
-- Tashar SAI
SET @ENTRY := 20913;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,10436,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Tashar - On Quest 'All Clear!' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 2091300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Remove Npc Flag Questgiver"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Run Off"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,69,0,0,0,0,0,0,8,0,0,0,4922.18,2991.38,93.9847,5.88582,"On Script - Move To Position"),
(@ENTRY,9,4,0,0,0,100,0,5000,5000,0,0,17,173,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 173"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,17,26,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Emote State 26"),
(@ENTRY,9,6,0,0,0,100,0,0,0,0,0,11,28730,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Cast 'Arcane Torrent'"),
(@ENTRY,9,7,0,0,0,100,0,2000,2000,0,0,50,184614,15,0,0,0,0,8,0,0,0,4923.56,2990.45,93.9413,0.813699,"On Script - Summon Gameobject 'Ethereal Repair Kit'"),
(@ENTRY,9,8,0,0,0,100,0,0,0,0,0,50,184614,15,0,0,0,0,8,0,0,0,4923.45,2991.21,93.9248,5.49404,"On Script - Summon Gameobject 'Ethereal Repair Kit'"),
(@ENTRY,9,9,0,0,0,100,0,0,0,0,0,50,184614,15,0,0,0,0,8,0,0,0,4924.47,2990.35,93.8867,2.25067,"On Script - Summon Gameobject 'Ethereal Repair Kit'"),
(@ENTRY,9,10,0,0,0,100,0,0,0,0,0,50,184614,15,0,0,0,0,8,0,0,0,4924.32,2991.21,93.8808,3.88003,"On Script - Summon Gameobject 'Ethereal Repair Kit'"),
(@ENTRY,9,11,0,0,0,100,0,2000,2000,0,0,1,1,4000,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,12,0,0,0,100,0,4000,4000,0,0,69,0,0,0,0,0,0,8,0,0,0,4914.74,2991.92,94.1138,0,"On Script - Move To Position"),
(@ENTRY,9,13,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.15708,"On Script - Set Orientation 0,15708"),
(@ENTRY,9,14,0,0,0,100,0,0,0,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Add Npc Flag Questgiver");

DELETE FROM `creature_text` WHERE `entry`=20913;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(20913, 0, 0, 'Now to begin the repairs.', 12, 0, 100, 0, 0, 0, 18651, 'Tashar'),
(20913, 1, 0, 'It''s working! Ghabar will be pleased to hear the news.', 12, 0, 100, 0, 0, 0, 18652, 'Tashar');

-- 
-- [Q] Into the Churning Gulch -- http://wotlk.openwow.com/quest=10715
-- Baron Sablemane SAI
SET @ENTRY := 22103;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,10715,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Quest 'Into the Churning Gulch' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 2210300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Remove Npc Flag Questgiver"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Set Run Off"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,2915.57,5957.42,3.27419,2.97874,"Baron Sablemane - On Script - Move To Position"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Set Emote State 69"),
(@ENTRY,9,4,0,0,0,100,0,7000,7000,0,0,17,26,0,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Set Emote State 26"),
(@ENTRY,9,5,0,0,0,100,0,2000,2000,0,0,69,0,0,0,0,0,0,8,0,0,0,2919.11,5956.93,3.16149,1.03933,"Baron Sablemane - On Script - Move To Position"),
(@ENTRY,9,6,0,0,0,100,0,2000,2000,0,0,69,0,0,0,0,0,0,8,0,0,0,2919.11,5956.93,3.16149,1.58825,"Baron Sablemane - On Script - Move To Position"),
(@ENTRY,9,7,0,0,0,100,0,1000,1000,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Add Npc Flag Questgiver"),
(@ENTRY,9,8,0,0,0,100,0,0,0,0,0,1,0,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Baron Sablemane - On Script - Say Line 0");

DELETE FROM `creature_text` WHERE `entry`=22103;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(22103, 0, 0, 'There, it is completed.', 12, 0, 100, 0, 0, 0, 19808, 'Baron Sablemane');

-- 
-- [Q] Deciphering the Book -- http://wotlk.openwow.com/quest=9557
-- Anchorite Paetheus SAI
SET @ENTRY := 17424;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,9557,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Quest 'Deciphering the Book' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 1742400;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Remove Npc Flag Questgiver"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,1,0,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Say Line 0"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,1,1,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Say Line 1"),
(@ENTRY,9,3,0,0,0,100,0,6000,6000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Say Line 2"),
(@ENTRY,9,4,0,0,0,100,0,6000,6000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Say Line 3"),
(@ENTRY,9,5,0,0,0,100,0,0,0,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Anchorite Paetheus - On Script - Add Npc Flag Questgiver");

DELETE FROM `creature_text` WHERE `entry`=17424;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(17424, 0, 0, 'Now, let\'s see....', 12, 0, 100, 69, 0, 0, 14281, 'Anchorite Paetheus'),
(17424, 1, 0, 'In the process of translating this, I believe I\'ve learned more about the drunken exploits of a certain \'Andrew Clementine\' than about the journal\'s author.', 12, 0, 100, 1, 0, 0, 14282, 'Anchorite Paetheus'),
(17424, 2, 0, 'But, no matter. Mister Clementine\'s... err, \'contribution\' notwithstanding, this seems to be the journal of a man named Nolkai. The entries toward the end describe a brutal battle.', 12, 0, 100, 11, 0, 0, 14283, 'Anchorite Paetheus'),
(17424, 3, 0, 'Take a look at this passage...', 12, 0, 100, 0, 0, 0, 14285, 'Anchorite Paetheus');

-- 
-- [Q] Birds of a Feather -- http://wotlk.openwow.com/quest=9397
UPDATE `creature` SET `MovementType`=0 WHERE  `guid`=102219;
DELETE FROM `creature_addon` WHERE  `guid`=102219;
DELETE FROM `waypoint_data` WHERE `id`=1022190;
-- Falconer Drenna Riverwind SAI
SET @ENTRY := 16790;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,20,0,100,0,9397,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Falconer Drenna Riverwind - On Quest 'Birds of a Feather' Finished - Run Script"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,64,1,0,0,0,0,0,7,0,0,0,0,0,0,0,"Falconer Drenna Riverwind - On Quest 'Birds of a Feather' Finished - Store Targetlist");

-- Actionlist SAI
SET @ENTRY := 1679000;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Set Run Off"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,69,0,0,0,0,0,0,8,0,0,0,-597.806,4110.68,90.848,2.02116,"On Script - Move To Position"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,5,16,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Play Emote 16"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,12,17262,1,20000,0,0,0,8,0,0,0,-597.244,4111.88,90.7789,0.805268,"On Script - Summon Creature 'Captive Female Kaliri'"),
(@ENTRY,9,4,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,12,1,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,69,0,0,0,0,0,0,8,0,0,0,-596.347,4108.47,91.0061,5.15176,"On Script - Move To Position"),
(@ENTRY,9,6,0,0,0,100,0,2000,2000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.10152,"On Script - Set Orientation 4,10152");

DELETE FROM `creature_text` WHERE `entry`=16790;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(16790, 0, 0, 'Thank you, $N. It looks like they''re going to get along just fine.', 12, 0, 100, 0, 0, 0, 13666, 'Falconer Drenna Riverwind');

--
-- [Q] Altered Beings - http://wotlk.openwow.com/quest=880 
-- Tonga Runetotem SAI - delay textevent
SET @ENTRY := 3448;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,20,0,100,0,880,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Tonga Runetotem - On Quest 'Altered Beings' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 344800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Tonga Runetotem - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,5000,5000,0,0,1,1,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Tonga Runetotem - On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,1,2,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Tonga Runetotem - On Script - Say Line 2"),
(@ENTRY,9,3,0,0,0,100,0,4000,4000,0,0,1,3,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Tonga Runetotem - On Script - Say Line 3");

-- 
-- [Q] Return to Onu -- http://wotlk.openwow.com/quest=950
-- Onu SAI
SET @ENTRY := 3616;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,20,0,100,0,950,0,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Onu - On Quest 'Return to Onu' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 361600;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Onu - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,1,1,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Onu - On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,1,2,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Onu - On Script - Say Line 2");

UPDATE `creature_template` SET `minlevel`=20, `maxlevel`=20, `faction_A`=29, `faction_H`=29, `npcflag`=2, `speed_walk`=1.142857, `speed_run`=1, `baseattacktime`=2000, `rangeattacktime`=2000, `unit_flags`=768, `HoverHeight`=1 WHERE `entry`=10176; -- Kaltunk

--
UPDATE `creature_template` SET `AIName` = 'SmartAI' where `entry` = 27056;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (27056, 2705600, 2705601, 2705602);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(27056, 0, 0, 0, 1, 0, 100, 0, 10000, 20000/*60000*/, 3600000, 3600000, 87, 2705600, 2705601, 2705602, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - Out of Combat - Call random script"),
(2705600, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 0"),
(2705600, 9, 1, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 27055, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 0 (Sentinel Amberline)"),
(2705601, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 1"),
(2705601, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 2"),
(2705601, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 27052, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 0 (Naohain)"),
(2705602, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 1"),
(2705602, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 3"),
(2705602, 9, 2, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 27054, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 0 (Modoru)"),
(2705602, 9, 3, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 27054, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 1 (Modoru)"),
(2705602, 9, 4, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 4"),
(2705602, 9, 5, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 5"),
(2705602, 9, 6, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 27119, 0, 0, 0, 0, 0, 0, "Sentinel Sweetspring - On Script - Talk 0 (Snowpaw)");

DELETE FROM `creature_text` WHERE `entry` IN (27056, 27055, 27052, 27054, 27119);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(27056, 0, 0, "%s grins at Sentinel Amberline across the camp, waggling her long eyebrows suggestively.", 16, 0, 100, 0, 0, 0, 29555, "Sentinel Sweetspring"),
(27056, 1, 0, "%s peers at Modoru's Draenic robes.", 16, 0, 100, 0, 0, 0, 27085, "Sentinel Sweetspring"),
(27056, 2, 0, "Pssst. Naohain. Why does Modoru still wear those ridiculous robes? He looks like some interdimensional bumpkin who stumbled out of Ammen Vale not six hours past.", 12, 2, 100, 0, 0, 0, 27086, "Sentinel Sweetspring"),
(27056, 3, 0, "Say, Modoru. Where did you get those... ah... \"wonderful\" robes of yours, hmmm?", 12, 7, 100, 0, 0, 0, 29546, "Sentinel Sweetspring"),
(27056, 4, 0, "Oh. Oh... I see. How nice!", 12, 7, 100, 0, 0, 0, 29547, "Sentinel Sweetspring"),
(27056, 5, 0, "%s bites back a laugh.", 16, 0, 100, 0, 0, 0, 29548, "Sentinel Sweetspring"),
(27055, 0, 0, "%s huffs with annoyance, glaring briefly at Sentinel Sweetspring before turning away with her nose in the air.", 16, 0, 100, 0, 0, 0, 29557, "Sentinel Amberline"),
(27052, 0, 0, "I... I do not know.", 12, 2, 100, 0, 0, 0, 29544, "Naohain"),
(27054, 0, 0, "%s beams, unaware that she is teasing him, answering in his thickly-accented Common.", 16, 0, 100, 0, 0, 0, 29550, "Modoru"),
(27054, 1, 0, "Well! You see, was gift from Grandmother Oluba. She hand-weave from finest silk on Draenor! Am quite proud of, yes.", 12, 7, 100, 0, 0, 0, 29552,"Modoru"),
(27119, 0, 0, "%s snickers, nuzzling into Sentinel Sweetspring's ankles.", 16, 0, 100, 0, 0, 0, 29553, "Snowpaw");

--
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=18810;
DELETE FROM `smart_scripts` WHERE `entryorguid`=18810;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(18810, 0, 0, 0, 1, 0, 100, 0, 120000, 240000, 300000, 420000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Otonambusi - Out of Combat - Talk 0");

DELETE FROM `creature_text` WHERE `entry`=18810;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(18810, 0, 0, "How can you have any pudding if you don't eat your fish?", 12, 7, 100, 5, 0, 0, 15941, "Otonambusi"),
(18810, 0, 1, "If you don't eat your fish, you can't have any pudding!", 12, 7, 100, 5, 0, 0, 15942, "Otonambusi");

-- 
-- "Swamp Eye" Jarl - Creepy Event -- https://ironyca.wordpress.com/2010/09/15/the-10-most-creepy-things-in-wow/ 
-- Mordant Grimsby SAI
SET @ENTRY := 23843;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,25,0,100,0,0,0,0,0,53,0,23843,1,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Reset - Start Waypoint"),
(@ENTRY,0,1,0,40,0,100,0,1,23843,0,0,54,15000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,2,0,40,0,100,0,2,23843,0,0,83,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Waypoint 2 Reached - Remove Npc Flag Questgiver"),
(@ENTRY,0,3,0,40,0,100,0,6,23843,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Waypoint 6 Reached - Run Script"),
(@ENTRY,0,4,0,40,0,100,0,10,23843,0,0,82,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Waypoint 10 Reached - Add Npc Flag Questgiver"),
(@ENTRY,0,5,0,40,0,100,0,11,23843,0,0,54,300000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Waypoint 11 Reached - Pause Waypoint");

-- Actionlist SAI
SET @ENTRY := 2384300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,54000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,5,381,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 381"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,50,186418,45,0,0,0,0,8,0,0,0,-2954.89,-3883.94,32.9969,2.60053,"Mordant Grimsby - On Script - Summon Gameobject 'Mordant's Gift'"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 0"),
(@ENTRY,9,4,0,0,0,100,0,2000,2000,0,0,1,0,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 1"),
(@ENTRY,9,6,0,0,0,100,0,5000,5000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 2"),
(@ENTRY,9,7,0,0,0,100,0,6000,6000,0,0,1,0,5000,0,0,0,0,19,4792,20,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 0"),
(@ENTRY,9,8,0,0,0,100,0,5000,5000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 3"),
(@ENTRY,9,9,0,0,0,100,0,6000,6000,0,0,1,1,5000,0,0,0,0,19,4792,20,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 1"),
(@ENTRY,9,10,0,0,0,100,0,5000,5000,0,0,1,4,8000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 4"),
(@ENTRY,9,11,0,0,0,100,0,8000,8000,0,0,5,381,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 381");

DELETE FROM `waypoints` WHERE `entry`=23843;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(23843, 1, -2948.36, -3893.17, 35.0324,  'Mordant Grimsby'),
(23843, 2, -2954, -3888.35, 34.991,      'Mordant Grimsby'),
(23843, 3, -2955.8, -3887.22, 33.729,    'Mordant Grimsby'),
(23843, 4, -2957.36, -3885.06, 33.0995,  'Mordant Grimsby'),
(23843, 5, -2957.23, -3882.92, 32.7325,  'Mordant Grimsby'),
(23843, 6, -2955.98, -3882.95, 32.8433,  'Mordant Grimsby'),
(23843, 7, -2955.55, -3885.08, 33.1672,  'Mordant Grimsby'),
(23843, 8, -2953.84, -3885.88, 33.9242,  'Mordant Grimsby'),
(23843, 9, -2952.99, -3887.91, 35.0223,  'Mordant Grimsby'),
(23843, 10, -2947.74, -3893.74, 35.0308, 'Mordant Grimsby'),
(23843, 11, -2948.36, -3893.17, 35.0324, 'Mordant Grimsby');

-- Texts
DELETE FROM `creature_text` WHERE `entry` IN(23843, 4792);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- Mordant Grimsby
(23843, 0, 0, 'I brought you a housewarming gift, Jarl.', 12, 0, 100, 1, 0, 0, 22348, 'Mordant Grimsby'),
(23843, 1, 0, 'Go on, open it. I think you\'ll like it.', 12, 0, 100, 25, 0, 0, 22349, 'Mordant Grimsby'),
(23843, 2, 0, 'Jarl?', 12, 0, 100, 0, 0, 0, 22350, 'Mordant Grimsby'),
(23843, 3, 0, 'Jarl, you haven\'t been yourself ever since I arrived. What\'s going on?', 12, 0, 100, 1, 0, 0, 22352, 'Mordant Grimsby'),
(23843, 4, 0, 'I came out here to visit you, Jarl! And I find you living in an empty cabin in the middle of nowhere, babbling about frogs? You\'re hopeless!', 12, 0, 100, 5, 0, 0, 22354, 'Mordant Grimsby'),
-- "Swamp Eye" Jarl
(4792, 0, 0, 'Unless it\'s something I can eat, I don\'t want it.', 12, 0, 100, 6, 0, 0, 22351, '"Swamp Eye" Jarl'),
(4792, 1, 0, 'Nothing. Wanna help me whip up some frog leg stew?', 12, 0, 100, 1, 0, 0, 22353, '"Swamp Eye" Jarl');
-- 
-- Actionlist SAI
SET @ENTRY := 2384300;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,54,54000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Pause Waypoint"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,5,381,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 381"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,50,186418,45,0,0,0,0,8,0,0,0,-2954.89,-3883.94,32.9969,2.60053,"Mordant Grimsby - On Script - Summon Gameobject Mordant's Gift"),
(@ENTRY,9,3,0,0,0,100,0,0,0,0,0,66,0,0,0,0,0,0,19,4792,20,0,0,0,0,0,"Mordant Grimsby - On Script - Set Orientation Closest Creature Swamp Eye Jarl"),
(@ENTRY,9,4,0,0,0,100,0,2000,2000,0,0,5,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 0"),
(@ENTRY,9,5,0,0,0,100,0,2000,2000,0,0,1,0,4000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 0"),
(@ENTRY,9,6,0,0,0,100,0,4000,4000,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 1"),
(@ENTRY,9,7,0,0,0,100,0,5000,5000,0,0,1,2,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 2"),
(@ENTRY,9,8,0,0,0,100,0,6000,6000,0,0,1,0,5000,0,0,0,0,19,4792,20,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 0"),
(@ENTRY,9,9,0,0,0,100,0,5000,5000,0,0,1,3,6000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 3"),
(@ENTRY,9,10,0,0,0,100,0,6000,6000,0,0,1,1,5000,0,0,0,0,19,4792,20,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 1"),
(@ENTRY,9,11,0,0,0,100,0,5000,5000,0,0,1,4,8000,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Say Line 4"),
(@ENTRY,9,12,0,0,0,100,0,8000,8000,0,0,5,381,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mordant Grimsby - On Script - Play Emote 381");

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (32682, 27932);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (32682, 27932) AND `source_type`=0;
UPDATE `creature_template` SET `InhabitType`=4, `speed_run`=2.5 WHERE `entry` IN (32682, 27932);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(32682, 0, 0, 0, 27, 0, 100, 1, 0, 0, 0, 0, 53, 1, 32682, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0,'Harry''s Bomber- On passeger rided - Start wp'),
(27932, 0, 0, 0, 27, 0, 100, 1, 0, 0, 0, 0, 53, 1, 27932, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0,'Harry''s Bomber- On passeger rided - Start wp'),
(32682, 0, 1, 0, 58, 0, 100, 1, 9, 32682, 0, 0, 11, 50630, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0,'Harry''s Bomber- On waypoint ended - Eject passengers'),
(27932, 0, 1, 0, 58, 0, 100, 1, 9, 27932, 0, 0, 11, 50630, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0,'Harry''s Bomber- On passeger ended - Eject passengers');

UPDATE `smart_scripts` SET `action_type`=85 WHERE `entryorguid`=28277 AND `source_type`=0 AND `id`=0;
DELETE FROM `waypoints` WHERE `entry` IN (32682, 27932);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(32682, 1, 61.83957, -6228.992, 19.07242, 'Harry''s Bomber'),
(32682, 2, -64.6518, -5908.95, 29.8502, 'Harry''s Bomber'),
(32682, 3, -158.0859, -5631.193, 29.8502, 'Harry''s Bomber'),
(32682, 4, -436.5865, -5223.862, 32.1002, 'Harry''s Bomber'),
(32682, 5, -412.989, -5012.779, 45.7391, 'Harry''s Bomber'),
(32682, 6, -75.09814, -4341.288, 64.29379, 'Harry''s Bomber'),
(32682, 7, -51.86116, -4023.003, 70.12712, 'Harry''s Bomber'),
(32682, 8, -136.1673, -3716.832, 59.96046, 'Harry''s Bomber'),
(32682, 9, -170.8945, -3578.378, 4.988236, 'Harry''s Bomber'),
(27932, 1, -136.1673, -3716.832, 59.96046, 'Harry''s Bomber'),
(27932, 2, -51.86116, -4023.003, 70.12712, 'Harry''s Bomber'),
(27932, 3, -75.09814, -4341.288, 64.29379, 'Harry''s Bomber'),
(27932, 4, -412.989, -5012.779, 45.7391, 'Harry''s Bomber'),
(27932, 5, -436.5865, -5223.862, 32.1002, 'Harry''s Bomber'),
(27932, 6, -158.0859, -5631.193, 29.8502, 'Harry''s Bomber'),
(27932, 7, -64.6518, -5908.95, 29.8502, 'Harry''s Bomber'),
(27932, 8, 61.83957, -6228.992, 19.07242, 'Harry''s Bomber'),
(27932, 9, 89.63259, -6287.430, -0.739605, 'Harry''s Bomber');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=10218;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`Scriptname`,`Comment`) VALUES
(15,10218,0,0,0,23,0,495,0,0,0,0 ,'' , 'Harry''s Bomber - Show gossip option if the npc is in the area 495'),
(15,10218,0,0,0,9,0,11567,0,0,0,0 ,'' , 'Harry''s Bomber - Show gossip option if player has taken quest 11567'),
(15,10218,0,0,1,28,0,11567,0,0,0,0 ,'' , 'Harry''s Bomber - Show gossip option if player has quest 11567 completed');

--
SET @CGUID := 255565;
DELETE FROM `creature` WHERE `guid`=@GUID;
INSERT INTO `creature` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `walkmode`) VALUES
(@CGUID, 24900, 571, 0, 0, 1, 1, 0, 0, 529.2123, -3530.377, 12.97082, 0, 60, 0, 0, 1, 0, 2, 0, 0, 0, 0);

DELETE FROM `creature_template_addon` WHERE `entry`=24900;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(24900, 24900, 0, 0, 0, 0, "");

DELETE FROM `waypoint_data` WHERE `id`=24900;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(24900, 1, 529.2123, -3530.377, 12.97082, 0, 0, 0, 0, 100, 0),
(24900, 2, 520.7833, -3528.544, 12.92207, 0, 0, 0, 0, 100, 0),
(24900, 3, 508.5591, -3526.409, 17.6906, 0, 0, 0, 0, 100, 0),
(24900, 4, 493.6954, -3522.943, 19.64948, 0, 0, 0, 0, 100, 0),
(24900, 5, 491.2342, -3526.773, 19.97676, 0, 0, 0, 0, 100, 0),
(24900, 6, 491.6969, -3530.882, 19.5777, 0, 0, 0, 0, 100, 0),
(24900, 7, 506.2397, -3536.302, 17.54548, 0, 0, 0, 0, 100, 0),
(24900, 8, 508.6644, -3526.426, 17.6785, 0, 0, 0, 0, 100, 0),
(24900, 9, 515.7313, -3527.45, 12.96216, 0, 0, 0, 0, 100, 0),
(24900, 10, 516.2963, -3534.765, 13.07965, 0, 0, 0, 0, 100, 0),
(24900, 11, 515.4622, -3539.267, 12.93611, 0, 0, 0, 0, 100, 0),
(24900, 12, 525.4114, -3542.12, 13.04245, 0, 0, 0, 0, 100, 0),
(24900, 13, 534.911, -3545.435, 13.23027, 0, 0, 0, 0, 100, 0),
(24900, 14, 538.83, -3540.197, 13.44585, 0, 0, 0, 0, 100, 0),
(24900, 15, 536.6614, -3533.317, 13.13511, 0, 0, 0, 0, 100, 0);

DELETE FROM `creature_text` WHERE `entry`=24900;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES
(24900, 0, 0, "Its power courses through me! I AM A GOD!", 12, 0, 100, 0, 0, 0, 23977, "Abdul the Insane"),
(24900, 0, 1, "ABDUL IS STRONG LIKE BULL!", 12, 0, 100, 0, 0, 0, 23978, "Abdul the Insane"),
(24900, 1, 0, "The fury of the Vrykul will consume you!", 12, 0, 100, 0, 0, 0, 23976, "Abdul the Insane"),
(24900, 1, 1, "ARE YOU CHALLENGING ME? I HAVE THE STRENGTH OF TEN MEN!", 12, 0, 100, 0, 0, 0, 23979, "Abdul the Insane");

UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=24900;
DELETE FROM `smart_scripts` WHERE entryorguid=24900;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(24900, 0, 0, 0, 1, 0, 100, 0, 2000, 2000, 10000, 10000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Abdul the Insane - Out of Combat - Talk 0"),
(24900, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Abdul the Insane - On Aggro - Talk 1");

--
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (19, 20) AND `SourceEntry` IN (13538, 13539);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 13538, 0, 0, 48, 0, 0, 0, 0, 1, 0, 0, "", "Alliance quest 'Southern Sabotage' is available if Wintergrasp is held by Alliance"),
(20, 0, 13538, 0, 0, 48, 0, 0, 0, 0, 1, 0, 0, "", "Quest mark for Alliance quest 'Southern Sabotage' is available if Wintergrasp is held by Alliance"),
(19, 0, 13539, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, "", "Horde quest 'Toppling the Towers' is available if Wintergrasp is held by Horde"),
(20, 0, 13539, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, "", "Quest mark for Horde quest 'Toppling the Towers' is available if Wintergrasp is held by Horde");

--
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (22432);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceEntry`=39371 AND `ElseGroup`>0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `Comment`) VALUES
(17, 0, 39371, 1, 31, 1, 3, 22507,0, "Ritual Prayer Beads"),
(17, 0, 39371, 2, 31, 1, 3, 22506,0, "Ritual Prayer Beads");

DELETE FROM `gossip_menu` WHERE `entry`=8554 AND `text_id`=10707;
INSERT INTO `gossip_menu` (`entry`,`text_id`) VALUES (8554,10707);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=14 AND `SourceGroup`=8554;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`, `ErrorTextId`,`ScriptName`,`Comment`, `NegativeCondition`) VALUES
(14,8554,10706,0,28,10935,0,0,0,'','Show gossip text if player  Exorcism of Colonel Jules is completed',0),
(14,8554,10706,1,8,10935,0,0,0,'','Show gossip text if player  Exorcism of Colonel Jules is rewarded',0),
(14,8554,10707,1,28,10935,0,0,0,'','Show gossip text if player  Exorcism of Colonel Jules is not completed',1),
(14,8554,10707,1,8,10935,0,0,0,'','Show gossip text if player  Exorcism of Colonel Jules is not rewarded',1);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 39371;
INSERT INTO `spell_linked_spell` VALUES (39371, 39322, 0, 0, 'Heal Barada');

UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry` IN (22506, 22507);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (22506, 22507);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(22506, 0, 0, 0, 8, 0, 100, 0, 39371, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Foul Purge - On Spellhit - Die"),
(22507, 0, 0, 0, 8, 0, 100, 0, 39371, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Darkness Released - On Spellhit - Die");

UPDATE `creature_template` SET `unit_flags`=`unit_flags`|33554432 WHERE `entry`=27632;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=1 AND `SourceEntry` IN (36742, 36746) AND `ElseGroup`=1;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(1, 26349, 36742, 0, 1, 8, 0, 12005, 0, 0, 0, 0, 0, "", "Item 'Goramosh's Strange Device' can drop if quest 'Prevent the Accord' is rewarded"),
(1, 26349, 36746, 0, 1, 8, 0, 12005, 0, 0, 0, 0, 0, "", "Item 'Goramosh's Strange Device' can drop if quest 'Prevent the Accord' is rewarded");


DELETE FROM `creature_text` WHERE `entry`=24371;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES (24371, 0, 0, 'I have fallen! Fear not, brethren! Let nothing keep you from the Stoneforge!', 14, 0, 100, 0, 0, 0, 23530, 'Megalith');

UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=24371;
DELETE FROM `smart_scripts` WHERE `entryorguid`=24371;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(24371, 0, 0, 0, 0, 0, 100, 0, 3000, 5000, 15000, 20000, 11, 50084, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Megalith - In Combat - Cast 'Hulking Uppercut'"),
(24371, 0, 1, 0, 0, 0, 100, 0, 0, 1000, 5000, 5000, 11, 50086, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Megalith - In Combat - Cast 'Boulder'"),
(24371, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Megalith - On Death - Say Line 1");

UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `item` = 24486;
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-92 WHERE `item` = 31814;

UPDATE `conditions` SET `ConditionValue2`=33844 WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62709 AND `ElseGroup`=0;
-- 
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN(17) AND `SourceEntry`=13982;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17,0,13982,0,0,29,0,9016,10,1,0,0,0,'',"Spell 'Bael'Gar's Fiery Essence' requires Bael'Gar's corpse");

-- Fix hp, force to do it through here
UPDATE `creature_template` SET `health_mod` = 10572 WHERE `entry` = 1572;

-- Terrorspark SAI
SET @ENTRY := 10078;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,0,0,3000,3400,11,9053,64,0,0,0,0,2,0,0,0,0,0,0,0,"Sifreldar Runekeeper - Combat - Cast Fireball"),
(@ENTRY,0,1,0,0,0,100,0,4800,29300,2300,9700,11,11968,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sifreldar Runekeeper - Combat - Cast Fire Shield"),
(@ENTRY,0,2,3,0,0,100,0,10900,23300,33000,53400,11,15710,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sifreldar Runekeeper - Combat - Cast Summon Flamekin Torcher"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,11,15711,0,0,0,0,0,1,0,0,0,0,0,0,0,"Sifreldar Runekeeper - Combat - Cast Summon Summon Flamekin Rager");

--
DELETE FROM `gossip_menu` WHERE `entry`=10144 AND `text_id`=14088;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (10144, 14088);

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 10144;
INSERT INTO `gossip_menu_option` (`menu_id`, id, option_icon, `option_text`, `OptionBroadcastTextID`, `option_id`, `npc_option_npcflag`, `action_menu_id`) VALUES 
(10144, 0, 0, 'I''m interested.', 32566, 1, 1, 0),
(10144, 1, 0, 'I''m interested.', 32566, 1, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=14 AND `SourceGroup`=10144;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=10144;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`,`NegativeCondition`) VALUES
(15,10144,0,0,27,77,3,0,0,'','The player must be level 77 or higher', 0),
(15,10144,0,0,25,54197,0,0,0,'','Show the gossip if the player doesn''t learn Cold Weather Flying', 1),
(15,10144,0,0,2,44221,1,1,0,'','Show the gossip if the player doesn''t have the item', 1),
(15,10144,0,0,16,2098253,0,0,0,'','Show the gossip if the player is from the alliance', 0),
(15,10144,1,0,27,77,3,0,0,'','The player must be level 77 or higher', 0),
(15,10144,1,0,25,54197,0,0,0,'','Show the gossip if the player doesn''t learn Cold Weather Flying', 1),
(15,10144,1,0,2,44229,1,1,0,'','Show the gossip if the player doesn''t have the item', 1),
(15,10144,1,0,16,946,0,0,0,'','Show the gossip if the player is from the horde', 0),
(14,10144,14088,0,27,77,3,0,0,'','Show the text if the player is level 77 or higher', 0),
(14,10144,14088,0,25,54197,0,0,0,'','Show the text if the player doesn''t learn Cold Weather Flying', 1),
(14,10144,14088,0,2,44221,1,1,0,'','Show the text if the player doesn''t have the item', 1),
(14,10144,14088,0,2,44229,1,1,0,'','Show the text if the player doesn''t have the item', 1);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=30464;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 30464  AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(30464, 0, 0, 1, 62, 0, 100, 0, 10144, 0, 0, 0, 85, 60126, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Honest" Max  - On Gossip select - additem'),
(30464, 0, 1, 0, 61, 0, 100, 0, 10144, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Honest" Max  - On Gossip select - close gossip'),
(30464, 0, 2, 3, 62, 0, 100, 0, 10144, 1, 0, 0, 85, 60128, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Honest" Max  - On Gossip select - additem'),
(30464, 0, 3, 0, 61, 0, 100, 0, 10144, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Honest" Max  - On Gossip select - close gossip');

