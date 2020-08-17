-- Taking Part Quest Fix
-- Fertile Mound Bunny SAI
SET @ENTRY := 36123;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,68355,0,0,0,33,36123,0,0,0,0,0,7,20,0,0,0,0,0,0,"Fertile Mound Bunny - On Spellhit 'Plant Seeds' - Quest Credit 'Taking Part'"),
(@ENTRY,0,1,2,61,0,100,0,68355,0,0,0,45,1,0,0,0,0,0,20,195687,10,0,0,0,0,0,"Fertile Mound Bunny - On Spellhit 'Plant Seeds' - Set Data 1 0"),
(@ENTRY,0,2,0,61,0,100,0,68355,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fertile Mound Bunny - On Spellhit 'Plant Seeds' - Despawn Instant");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND  `SourceGroup`=1 AND `SourceEntry`=68355;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(13, 1, 68355, 0, 0, 31, 0, 3, 36123, 0, 0, 0, 0, '', 'Plant Seeds only targets Fertile Mound Bunny');

-- Fertile Mound SAI
SET @ENTRY := 195687;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,38,0,100,0,1,0,0,0,70,180,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fertile Mound - On Data Set 1 0 - Respawn Self");

SET @OGUID := 195687;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID+0 AND @OGUID+13;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES 
(@OGUID+0, 195687, 1, 0, 0, 1, 1, -1303.52, 1627.32, 64.5241, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+1, 195687, 1, 0, 0, 1, 1, -1303.77, 1593.97, 62.9723, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+2, 195687, 1, 0, 0, 1, 1, -1068.2, 1887.38, 60.2275, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+3, 195687, 1, 0, 0, 1, 1, -1231.54, 1473.07, 63.6275, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+4, 195687, 1, 0, 0, 1, 1, -1294.44, 1496.29, 62.857, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+5, 195687, 1, 0, 0, 1, 1,-1340.84, 1389.97, 63.9532, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+6, 195687, 1, 0, 0, 1, 1, -1266.05, 1404.07, 63.38, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+7, 195687, 1, 0, 0, 1, 1, -1308.6, 1384.82, 62.7546, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+8, 195687, 1, 0, 0, 1, 1, -1103.98, 1303.44, 90.5823, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+9, 195687, 1, 0, 0, 1, 1, -1084.24, 1416.61, 61.7715, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+10, 195687, 1, 0, 0, 1, 1, -1090.97, 1385.4, 61.9833, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+11, 195687, 1, 0, 0, 1, 1, -872.094, 1828.09, 62.0108, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+12, 195687, 1, 0, 0, 1, 1, -922.672, 1926.03, 60.8396, 0, 0, 0, 0, 1, 120, 255, 1),
(@OGUID+13, 195687, 1, 0, 0, 1, 1, -1049.99, 1358.8, 65.7721, 0, 0, 0, 0, 1, 120, 255, 1);

-- Calming the Kodo Quest Fix
-- Enraged Kodo SAI
SET @ENTRY := 36094;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,4000,5000,18000,22000,11,5568,0,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - In Combat - Cast 'Trample'"),
(@ENTRY,0,1,0,2,0,100,1,0,30,0,0,11,18501,0,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - Between 0-30% Health - Cast 'Enrage' (No Repeat)"),
(@ENTRY,0,2,4,38,0,100,0,1,0,0,0,29,1,0,36079,1,1,0,19,36079,30,0,0,0,0,0,"Enraged Kodo - On Data Set 1 0 - Start Follow Closest Creature 'Enraged Kodo KC Bunny'"),
(@ENTRY,0,3,5,65,0,100,0,0,0,0,0,86,68331,0,18,35,0,0,1,20,0,0,0,0,0,0,"Enraged Kodo - On Follow Complete - Cross Cast 'Kill Credit'"),
(@ENTRY,0,4,0,61,0,100,0,1,0,0,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - On Data Set 1 0 - Set Reactstate Passive"),
(@ENTRY,0,5,6,61,0,100,0,0,0,0,0,5,33,0,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - On Follow Complete - Play Emote 33"),
(@ENTRY,0,6,7,61,0,100,0,0,0,0,0,12,36113,3,120000,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - On Follow Complete - Summon Creature 'Pacified Kodo'"),
(@ENTRY,0,7,0,61,0,100,0,0,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Kodo - On Follow Complete - Despawn In 1 ms");
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES (36094, 0, 8, 0, 2, 0, 100, 1, 0, 30, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Say Text at 30% HP');


-- Pacified Kodo SAI
SET @ENTRY := 36113;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,54,0,100,0,0,0,0,0,46,30,0,0,0,0,0,1,0,0,0,0,0,0,0,"Pacified Kodo - On Just Summoned - Move Forward 30 Yards"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,41,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Pacified Kodo - On Just Summoned - Despawn In 5000 ms");

-- Enraged Kodo KC Bunny SAI
SET @ENTRY := 36079;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,45,1,0,0,0,0,0,19,36094,30,0,0,0,0,0,"Enraged Kodo KC Bunny - On Just Summoned - Set Data 1 0");

-- Get Me Out of Here! Quest Fix
-- Melizza Brimbuzzle SAI
SET @ENTRY := 12277;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,19,0,100,0,6132,0,0,0,53,0,@ENTRY*100+00,0,0,0,1,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Quest 'Get Me Out of Here!' Taken - Start Waypoint"),
(@ENTRY,0,1,0,61,0,100,0,6132,0,0,0,81,0,0,0,0,0,0,10,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Quest 'Get Me Out of Here!' Taken - Set Npc Flag "),
(@ENTRY,0,2,0,40,0,100,0,3,@ENTRY*100+00,0,0,1,0,0,0,0,0,0,21,20,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 3 Reached - Say Line 0"),
(@ENTRY,0,3,4,40,0,100,0,58,@ENTRY*100+00,0,0,15,6132,0,0,0,0,0,17,0,30,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 58 Reached - Quest Credit 'Get Me Out of Here!'"),
(@ENTRY,0,4,5,61,0,100,0,58,@ENTRY*100+00,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 58 Reached - Say Line 1"),
(@ENTRY,0,5,6,61,0,100,0,58,@ENTRY*100+00,0,0,53,1,@ENTRY*100+01,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 58 Reached - Start Waypoint"),
(@ENTRY,0,6,0,61,0,100,0,58,@ENTRY*100+00,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 58 Reached - Set Active On"),
(@ENTRY,0,7,8,40,0,100,0,69,@ENTRY*100+01,0,0,54,10000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 69 Reached - Pause Waypoint"),
(@ENTRY,0,8,0,61,0,100,0,69,@ENTRY*100+01,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 69 Reached - Run Script"),
(@ENTRY,0,9,0,40,0,100,0,93,@ENTRY*100+01,0,0,41,3000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 93 Reached - Despawn In 3000 ms"),
(@ENTRY,0,10,0,11,0,100,0,0,0,0,0,81,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Respawn - Set Npc Flag Questgiver"),
(@ENTRY,0,11,0,40,0,100,1,57,@ENTRY*100+00,0,0,80,@ENTRY*100+01,0,0,0,0,0,1,0,0,0,0,0,0,0,"Melizza Brimbuzzle - On Waypoint 57 Reached - Run Script (No Repeat)");

-- Actionlist SAI
SET @ENTRY := 1227700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,3000,3000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 2"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 3"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 4");

-- Actionlist SAI
SET @ENTRY := 1227701;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,12,4655,7,0,0,0,0,8,0,0,0,-1382.56,2417.06,88.9714,2.46091,"Melizza Brimbuzzle - On Script - Summon Creature 'Maraudine Wrangler'"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,12,4655,7,0,0,0,0,8,0,0,0,-1406.39,2423.48,88.5732,0.20944,"Melizza Brimbuzzle - On Script - Summon Creature 'Maraudine Wrangler'"),
(@ENTRY,9,2,0,0,0,100,0,0,0,0,0,12,4655,7,0,0,0,0,8,0,0,0,-1394.38,2417.12,88.7675,0.959931,"Melizza Brimbuzzle - On Script - Summon Creature 'Maraudine Wrangler'");

DELETE FROM `creature_text` WHERE `entry`=12277;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextID`, `comment`) VALUES
(12277, 0, 0, 'Thanks $n.  Now let''s get out of here!', 12, 0, 100, 0, 0, 0, 7540, 'Melizza Brimbuzzle'),
(12277, 1, 0, 'We made it!  Thanks again!  I''m going to run ahead!', 12, 0, 100, 0, 0, 0, 7544, 'Melizza Brimbuzzle'),
(12277, 2, 0, 'Hey Hornizz!  I''m back!  And there are some people behind me who helped me out of a jam.', 12, 0, 100, 0, 0, 0, 7550, 'Melizza Brimbuzzle'),
(12277, 3, 0, 'We''re going to have to scratch the Maraudines off our list.  Too hard to work with...', 12, 0, 100, 0, 0, 0, 7551, 'Melizza Brimbuzzle'),
(12277, 4, 0, 'Well, I''m off to the Gelkis.  They''re not as dumb as the Maraudines, but they''re more reasonable.', 12, 0, 100, 0, 0, 0, 7552, 'Melizza Brimbuzzle');

SET @PATH := 1227700;
SET @PATH_TWO := 1227701;
DELETE FROM `waypoints` WHERE `entry` IN (@PATH, @PATH_TWO);
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(@PATH,1,-1155.146,2709.409,111.2361,'Melizza Brimbuzzle'),
(@PATH,2,-1155.146,2709.409,111.2361,'Melizza Brimbuzzle'),
(@PATH,3,-1162.785,2712.464,111.5828,'Melizza Brimbuzzle'),
(@PATH,4,-1162.752,2712.397,111.6417,'Melizza Brimbuzzle'),
(@PATH,5,-1169.502,2711.397,112.3917,'Melizza Brimbuzzle'),
(@PATH,6,-1177.48,2710.258,112.1255,'Melizza Brimbuzzle'),
(@PATH,7,-1199.98,2702.508,111.6255,'Melizza Brimbuzzle'),
(@PATH,8,-1206.389,2700.128,111.496,'Melizza Brimbuzzle'),
(@PATH,9,-1248.919,2680.271,111.8919,'Melizza Brimbuzzle'),
(@PATH,10,-1272.489,2666.548,111.8956,'Melizza Brimbuzzle'),
(@PATH,11,-1288.82,2646.104,111.7111,'Melizza Brimbuzzle'),
(@PATH,12,-1290.413,2644.388,111.6542,'Melizza Brimbuzzle'),
(@PATH,13,-1290.413,2644.388,111.6542,'Melizza Brimbuzzle'),
(@PATH,14,-1290.413,2644.388,111.6542,'Melizza Brimbuzzle'),
(@PATH,15,-1309.874,2622.18,111.7776,'Melizza Brimbuzzle'),
(@PATH,16,-1309.908,2622.186,111.9743,'Melizza Brimbuzzle'),
(@PATH,17,-1323.898,2605.008,111.8986,'Melizza Brimbuzzle'),
(@PATH,18,-1336.769,2591.199,111.8607,'Melizza Brimbuzzle'),
(@PATH,19,-1349.322,2578.486,111.7647,'Melizza Brimbuzzle'),
(@PATH,20,-1359.602,2564.017,111.4906,'Melizza Brimbuzzle'),
(@PATH,21,-1361.852,2559.517,110.7406,'Melizza Brimbuzzle'),
(@PATH,22,-1362.352,2557.767,109.9906,'Melizza Brimbuzzle'),
(@PATH,23,-1363.102,2556.267,109.4906,'Melizza Brimbuzzle'),
(@PATH,24,-1364.602,2553.517,108.9906,'Melizza Brimbuzzle'),
(@PATH,25,-1365.352,2551.767,108.2406,'Melizza Brimbuzzle'),
(@PATH,26,-1366.102,2550.017,107.7406,'Melizza Brimbuzzle'),
(@PATH,27,-1366.477,2549.685,107.5719,'Melizza Brimbuzzle'),
(@PATH,28,-1366.727,2549.185,107.0719,'Melizza Brimbuzzle'),
(@PATH,29,-1367.227,2547.185,106.8219,'Melizza Brimbuzzle'),
(@PATH,30,-1367.727,2545.185,106.0719,'Melizza Brimbuzzle'),
(@PATH,31,-1368.477,2543.435,105.3219,'Melizza Brimbuzzle'),
(@PATH,32,-1368.977,2541.435,104.5719,'Melizza Brimbuzzle'),
(@PATH,33,-1369.477,2539.435,103.8219,'Melizza Brimbuzzle'),
(@PATH,34,-1370.227,2536.685,103.3219,'Melizza Brimbuzzle'),
(@PATH,35,-1370.977,2534.685,102.5719,'Melizza Brimbuzzle'),
(@PATH,36,-1371.227,2532.935,102.0719,'Melizza Brimbuzzle'),
(@PATH,37,-1371.727,2531.185,101.3219,'Melizza Brimbuzzle'),
(@PATH,38,-1372.227,2529.185,100.8219,'Melizza Brimbuzzle'),
(@PATH,39,-1372.977,2527.185,100.0719,'Melizza Brimbuzzle'),
(@PATH,40,-1373.727,2524.435,99.32188,'Melizza Brimbuzzle'),
(@PATH,41,-1374.227,2522.435,98.57188,'Melizza Brimbuzzle'),
(@PATH,42,-1374.727,2520.685,97.82188,'Melizza Brimbuzzle'),
(@PATH,43,-1375.477,2518.685,97.32188,'Melizza Brimbuzzle'),
(@PATH,44,-1375.639,2518.402,97.00177,'Melizza Brimbuzzle'),
(@PATH,45,-1375.889,2517.152,96.75177,'Melizza Brimbuzzle'),
(@PATH,46,-1376.389,2515.152,96.00177,'Melizza Brimbuzzle'),
(@PATH,47,-1376.889,2513.402,95.25177,'Melizza Brimbuzzle'),
(@PATH,48,-1378.139,2509.402,93.75177,'Melizza Brimbuzzle'),
(@PATH,49,-1378.389,2507.652,93.00177,'Melizza Brimbuzzle'),
(@PATH,50,-1379.139,2504.902,92.50177,'Melizza Brimbuzzle'),
(@PATH,51,-1379.889,2501.902,91.75177,'Melizza Brimbuzzle'),
(@PATH,52,-1380.389,2500.152,91.25177,'Melizza Brimbuzzle'),
(@PATH,53,-1381.389,2496.152,90.50177,'Melizza Brimbuzzle'),
(@PATH,54,-1381.559,2495.931,90.14986,'Melizza Brimbuzzle'),
(@PATH,55,-1381.809,2495.431,90.14986,'Melizza Brimbuzzle'),
(@PATH,56,-1382.309,2491.431,89.64986,'Melizza Brimbuzzle'),
(@PATH,57,-1385.493,2471.513,89.11208,'Melizza Brimbuzzle'),
(@PATH,58,-1386.618,2405.71,89.767,'Melizza Brimbuzzle'),
(@PATH_TWO,1,-1385.368,2401.21,90.267,'Melizza Brimbuzzle'),
(@PATH_TWO,2,-1383.618,2395.46,90.767,'Melizza Brimbuzzle'),
(@PATH_TWO,3,-1382.118,2390.71,91.517,'Melizza Brimbuzzle'),
(@PATH_TWO,4,-1377.252,2375.137,91.91622,'Melizza Brimbuzzle'),
(@PATH_TWO,5,-1373.759,2355.208,92.06256,'Melizza Brimbuzzle'),
(@PATH_TWO,6,-1366.36,2323.406,91.9967,'Melizza Brimbuzzle'),
(@PATH_TWO,7,-1362.618,2277.267,91.79288,'Melizza Brimbuzzle'),
(@PATH_TWO,8,-1358.118,2257.017,91.54288,'Melizza Brimbuzzle'),
(@PATH_TWO,9,-1354.14,2239.229,91.62033,'Melizza Brimbuzzle'),
(@PATH_TWO,10,-1352.89,2229.729,92.12033,'Melizza Brimbuzzle'),
(@PATH_TWO,11,-1350.869,2214.848,91.63843,'Melizza Brimbuzzle'),
(@PATH_TWO,12,-1351.869,2210.848,90.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,13,-1352.119,2209.848,90.13843,'Melizza Brimbuzzle'),
(@PATH_TWO,14,-1352.619,2208.098,89.13843,'Melizza Brimbuzzle'),
(@PATH_TWO,15,-1352.869,2207.098,87.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,16,-1353.119,2206.098,86.38843,'Melizza Brimbuzzle'),
(@PATH_TWO,17,-1353.369,2205.348,85.38843,'Melizza Brimbuzzle'),
(@PATH_TWO,18,-1353.619,2204.348,84.38843,'Melizza Brimbuzzle'),
(@PATH_TWO,19,-1354.119,2202.348,83.63843,'Melizza Brimbuzzle'),
(@PATH_TWO,20,-1354.369,2201.348,82.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,21,-1354.619,2200.598,82.13843,'Melizza Brimbuzzle'),
(@PATH_TWO,22,-1354.869,2199.598,81.38843,'Melizza Brimbuzzle'),
(@PATH_TWO,23,-1355.119,2198.598,80.63843,'Melizza Brimbuzzle'),
(@PATH_TWO,24,-1355.119,2197.598,79.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,25,-1355.619,2195.598,78.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,26,-1356.119,2193.848,77.88843,'Melizza Brimbuzzle'),
(@PATH_TWO,27,-1356.619,2191.848,77.13843,'Melizza Brimbuzzle'),
(@PATH_TWO,28,-1357.619,2188.848,76.38843,'Melizza Brimbuzzle'),
(@PATH_TWO,29,-1355.239,2198.246,80.43513,'Melizza Brimbuzzle'),
(@PATH_TWO,30,-1355.489,2197.246,79.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,31,-1355.989,2195.496,78.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,32,-1356.489,2193.496,77.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,33,-1356.989,2191.496,76.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,34,-1357.739,2188.746,76.18513,'Melizza Brimbuzzle'),
(@PATH_TWO,35,-1357.989,2187.496,75.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,36,-1358.739,2185.746,75.18513,'Melizza Brimbuzzle'),
(@PATH_TWO,37,-1359.239,2183.746,74.43513,'Melizza Brimbuzzle'),
(@PATH_TWO,38,-1359.989,2181.996,73.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,39,-1360.489,2179.996,72.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,40,-1361.489,2177.246,72.18513,'Melizza Brimbuzzle'),
(@PATH_TWO,41,-1362.239,2175.246,71.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,42,-1362.739,2173.496,70.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,43,-1363.239,2171.746,70.43513,'Melizza Brimbuzzle'),
(@PATH_TWO,44,-1363.739,2169.746,69.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,45,-1364.739,2166.996,68.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,46,-1365.489,2164.996,67.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,47,-1365.989,2163.246,66.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,48,-1366.739,2161.246,65.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,49,-1367.489,2159.496,64.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,50,-1367.989,2157.496,63.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,51,-1368.989,2154.746,63.18513,'Melizza Brimbuzzle'),
(@PATH_TWO,52,-1369.739,2152.746,62.43513,'Melizza Brimbuzzle'),
(@PATH_TWO,53,-1370.239,2150.996,61.68513,'Melizza Brimbuzzle'),
(@PATH_TWO,54,-1370.989,2148.996,60.93513,'Melizza Brimbuzzle'),
(@PATH_TWO,55,-1369.163,2154.362,62.88633,'Melizza Brimbuzzle'),
(@PATH_TWO,56,-1369.913,2152.612,62.63633,'Melizza Brimbuzzle'),
(@PATH_TWO,57,-1370.413,2150.612,61.63633,'Melizza Brimbuzzle'),
(@PATH_TWO,58,-1371.163,2148.862,61.13633,'Melizza Brimbuzzle'),
(@PATH_TWO,59,-1371.663,2147.612,60.63633,'Melizza Brimbuzzle'),
(@PATH_TWO,60,-1374.663,2142.362,60.13633,'Melizza Brimbuzzle'),
(@PATH_TWO,61,-1381.413,2131.612,60.88633,'Melizza Brimbuzzle'),
(@PATH_TWO,62,-1387.413,2121.362,61.38633,'Melizza Brimbuzzle'),
(@PATH_TWO,63,-1391.413,2114.362,62.13633,'Melizza Brimbuzzle'),
(@PATH_TWO,64,-1395.163,2108.362,62.63633,'Melizza Brimbuzzle'),
(@PATH_TWO,65,-1391.807,2114.246,61.76419,'Melizza Brimbuzzle'),
(@PATH_TWO,66,-1395.307,2108.246,62.26419,'Melizza Brimbuzzle'),
(@PATH_TWO,67,-1396.307,2106.496,62.51419,'Melizza Brimbuzzle'),
(@PATH_TWO,68,-1404.391,2088.967,62.0089,'Melizza Brimbuzzle'),
(@PATH_TWO,69,-1405.748,2087.39,62.09642,'Melizza Brimbuzzle'),
(@PATH_TWO,70,-1407.248,2086.89,62.09642,'Melizza Brimbuzzle'),
(@PATH_TWO,71,-1408.248,2086.89,62.34642,'Melizza Brimbuzzle'),
(@PATH_TWO,72,-1410.498,2086.39,62.34642,'Melizza Brimbuzzle'),
(@PATH_TWO,73,-1412.498,2086.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,74,-1413.998,2085.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,75,-1415.498,2083.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,76,-1416.498,2082.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,77,-1417.498,2079.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,78,-1417.998,2077.39,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,79,-1419.998,2075.14,62.59642,'Melizza Brimbuzzle'),
(@PATH_TWO,80,-1424.748,2073.14,62.34642,'Melizza Brimbuzzle'),
(@PATH_TWO,81,-1418.114,2077.1,62.59175,'Melizza Brimbuzzle'),
(@PATH_TWO,82,-1420.364,2075.35,62.59175,'Melizza Brimbuzzle'),
(@PATH_TWO,83,-1425.114,2073.1,62.34175,'Melizza Brimbuzzle'),
(@PATH_TWO,84,-1425.114,2075.1,62.34175,'Melizza Brimbuzzle'),
(@PATH_TWO,85,-1436.864,2074.35,62.84175,'Melizza Brimbuzzle'),
(@PATH_TWO,86,-1451.277,2073.542,62.70998,'Melizza Brimbuzzle'),
(@PATH_TWO,87,-1464.777,2075.542,62.95998,'Melizza Brimbuzzle'),
(@PATH_TWO,88,-1465.031,2075.661,62.89689,'Melizza Brimbuzzle'),
(@PATH_TWO,89,-1473.031,2077.161,63.14689,'Melizza Brimbuzzle'),
(@PATH_TWO,90,-1480.031,2086.411,62.39689,'Melizza Brimbuzzle'),
(@PATH_TWO,91,-1485.531,2093.411,61.89689,'Melizza Brimbuzzle'),
(@PATH_TWO,92,-1485.639,2093.875,61.82396,'Melizza Brimbuzzle'),
(@PATH_TWO,93,-1487.889,2096.375,61.82396,'Melizza Brimbuzzle');

-- The almighty Scoripion shouldn't have a weapon! 
-- https://gyazo.com/82eb32c468e8e13e7c91362b108a313d Lol
UPDATE `creature_template` SET `equipment_id` = 0 WHERE `entry` = 4697;

-- Feralas, paths & emotes, sai etc:
UPDATE `creature` SET `spawndist`=15,`MovementType`=1 WHERE `id` IN (39677);
UPDATE `creature_template` SET `InhabitType`=4 WHERE  `entry`=39677;
UPDATE `creature` SET `spawndist`=0,`MovementType`=0 WHERE `id` IN (7939);
UPDATE `creature` SET `spawndist`=5,`MovementType`=1 WHERE `id` IN (40224,44400,39728,5234,5236,5240,11440,11442,11443,5258,5247,5245,5246,5253,5251,5254,5255);
UPDATE `creature` SET `spawndist`=10,`MovementType`=1 WHERE `id` IN (5366,5362,39384,39395,40194,39733,7939,5359,5462,39896,5300,40193,40059);
-- Moonclaw Warrior SAI
SET @ENTRY := 40362;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,3000,3000,3000,3000,5,35,0,0,0,0,0,1,0,0,0,0,0,0,0,"Moonclaw Warrior - Out of Combat - Play Emote 35");
-- Feathermoon Archer SAI
SET @GUID := -50553;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=40360;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GUID AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GUID,0,0,0,1,0,100,0,3000,3000,6000,6000,11,74279,0,0,0,0,0,19,37162,30,0,0,0,0,0,"Feathermoon Archer - Out of Combat - Cast 'Shoot'");
-- Feathermoon Archer SAI
SET @GUID := -68085;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=40360;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@GUID AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@GUID,0,0,0,1,0,100,0,3000,3000,6000,6000,11,74279,0,0,0,0,0,19,37162,30,0,0,0,0,0,"Feathermoon Archer - Out of Combat - Cast 'Shoot'");

-- Blood Theory Quest Fix
-- Rejuvenated Thunder Lizard SAI
SET @ENTRY := 35412;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,2000,3500,12000,14000,11,15611,0,0,0,0,0,2,0,0,0,0,0,0,0,"Rejuvenated Thunder Lizard - In Combat - Cast 'Lizard Bolt'"),
(@ENTRY,0,1,0,0,0,100,0,6000,8000,24000,27000,11,77601,0,0,0,0,0,1,0,0,0,0,0,0,0,"Rejuvenated Thunder Lizard - In Combat - Cast 'Lightning Nova'"),
(@ENTRY,0,2,0,6,0,100,0,0,0,0,0,11,68292,2,0,0,0,0,1,0,0,0,0,0,0,0,"Rejuvenated Thunder Lizard - On Just Died - Cast 'Summon Blood-filled Leech'");

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=35412;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(22, 3, 35412, 0, 0, 1, 1, 68290, 0, 0, 0, 0, 0, '', 'SAI triggers only if Creature has Aura "Leech Infestation"');

-- Blood-filled Leech SAI
SET @ENTRY := 36059;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,89,10,0,0,0,0,0,1,0,0,0,0,0,0,0,"Blood-filled Leech - On Just Summoned - Start Random Movement"),
(@ENTRY,0,1,0,8,0,100,0,68293,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Blood-filled Leech - On Spellhit 'Bloody Leech Cover' - Despawn Instant");
UPDATE `creature_template` SET `npcflag`=16777216 WHERE  `entry`=36059;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=36059;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(36059, 68291, 3, 0),
(36059, 68293, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=36059 AND `SourceEntry` IN (68291, 68293);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(18, 36059, 68291, 0, 0, 8, 0, 14304, 0, 0, 1, 0, 0, '', 'Forbidden rewarded quest for spellclick'),
(18, 36059, 68291, 0, 0, 9, 0, 14304, 0, 0, 0, 0, 0, '', 'Required quest active for spellclick'),
(18, 36059, 68293, 0, 0, 8, 0, 14304, 0, 0, 1, 0, 0, '', 'Forbidden rewarded quest for spellclick'),
(18, 36059, 68293, 0, 0, 9, 0, 14304, 0, 0, 0, 0, 0, '', 'Required quest active for spellclick');

-- Related to core commit, Grizzly Hills:
--  assign spellscript for Spell: 'Infected Worgen Bite'
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=53095;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(53095,-53094,2,'Infected Worgen Bite - Worgen\'s Call Immunity');
DELETE FROM `spell_script_names` WHERE `spell_id`=53094;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53094,'spell_infected_worgen_bite');

-- Fix Quest: Navarax's Gambit
DELETE FROM `creature_text` WHERE `entry` IN (47842, 48487);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextID`, `comment`) VALUES
(47842, 0, 0, 'Your death will tell a different story!', 14, 0, 100, 0, 0, 0, 48370, 'Arch Druid Navarax to Player'),
(48487, 0, 0, 'What is going on in here?!', 12, 0, 100, 5, 0, 0, 48691, 'Whisperwind Druid to Player'),
(48487, 1, 0, 'The Arch Druid... how in the world...?', 12, 0, 100, 274, 0, 0, 48692, 'Whisperwind Druid to Player'),
(48487, 2, 0, 'I... I must tell the others. You should find Huntress Selura right away.', 12, 0, 100, 1, 0, 0, 48693, 'Whisperwind Druid to Player');

DELETE FROM `gossip_menu` WHERE (`entry`=12353 AND `text_id`=17477);
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(12353, 17477); -- 47923

DELETE FROM `broadcast_text` WHERE `ID`=48373;
INSERT INTO `broadcast_text` (`ID`, `Language`, `MaleText`, `FemaleText`, `EmoteID0`, `EmoteID1`, `EmoteID2`, `EmoteDelay0`, `EmoteDelay1`, `EmoteDelay2`, `SoundId`, `Unk1`, `Unk2`) VALUES
(48373, 0, 'Yes, my child?', '', 0, 0, 0, 0, 0, 0, 0, 0, 1); -- 48373

DELETE FROM `gossip_menu_option` WHERE (`menu_id`=12398 AND `id`=0);
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`, `BoxBroadcastTextID`) VALUES
(12398, 0, 0, 'You''re accused of being a demon in disguise, Navarax... if that''s even your real name!', 1, 1, 0, 0, 0, 0, '', 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=12398 AND `SourceEntry`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12398, 0, 0, 0, 9, 0, 28264, 0, 0, 0, 0, 0, '', 'Gossip Option requires quest not complete');

-- Arch Druid Navarax SAI
SET @ENTRY := 47842;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,12398,0,0,0,2,14,0,0,0,0,0,1,0,0,0,0,0,0,0,"Arch Druid Navarax - On Gossip Option 0 Selected - Set Faction 14"),
(@ENTRY,0,1,4,61,0,100,0,12398,0,0,0,49,0,0,0,0,0,0,21,20,0,0,0,0,0,0,"Arch Druid Navarax - On Gossip Option 0 Selected - Start Attacking"),
(@ENTRY,0,2,0,25,0,100,0,0,0,0,0,2,35,0,0,0,0,0,1,0,0,0,0,0,0,0,"Arch Druid Navarax - On Reset - Set Faction 35"),
(@ENTRY,0,3,0,4,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Arch Druid Navarax - On Aggro - Say Line 0"),
(@ENTRY,0,4,7,61,0,100,0,12398,0,0,0,11,89515,2,0,0,0,0,1,0,0,0,0,0,0,0,"Arch Druid Navarax - On Gossip Option 0 Selected - Cast 'Xaravan's Transformation'"),
(@ENTRY,0,5,0,0,0,100,0,1000,1000,5000,5000,11,36996,0,0,0,0,0,2,0,0,0,0,0,0,0,"Arch Druid Navarax - In Combat - Cast 'Claw Swipe'"),
(@ENTRY,0,6,0,6,0,100,0,0,0,0,0,12,48487,6,0,0,0,0,8,0,0,0,6050.23,-934.345,455.853,1.15377,"Arch Druid Navarax - On Just Died - Summon Creature 'Whisperwind Druid'"),
(@ENTRY,0,7,0,61,0,100,0,12398,0,0,0,3,48160,0,0,0,0,0,0,0,0,0,0,0,0,0,"Arch Druid Navarax - On Gossip Option 0 Selected - Morph To Model 10189"),
(@ENTRY,0,8,0,0,0,100,1,1000,1000,0,0,3,48160,0,0,0,0,0,1,0,0,0,0,0,0,0,"Arch Druid Navarax - In Combat - Morph To Creature Xaravan");

-- Whisperwind Druid SAI
SET @ENTRY := 48487;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,53,1,48487,0,0,0,0,1,0,0,0,0,0,0,0,"Whisperwind Druid - On Just Summoned - Start Waypoint"),
(@ENTRY,0,1,2,40,0,100,0,1,48487,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Whisperwind Druid - On Waypoint 1 Reached - Run Script"),
(@ENTRY,0,2,0,61,0,100,0,1,48487,0,0,54,12000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Whisperwind Druid - On Waypoint 1 Reached - Pause Waypoint"),
(@ENTRY,0,3,0,40,0,100,0,3,48487,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Whisperwind Druid - On Waypoint 3 Reached - Despawn In 2000 ms");

-- Actionlist SAI
SET @ENTRY := 4848700;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,3000,3000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 2");

DELETE FROM `waypoints` WHERE `entry`=48487;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(48487, 1, 6053.084473, -927.829712, 455.742828, 'Whisperwind Druid'),
(48487, 2, 6050.680664, -933.536255, 455.892639, 'Whisperwind Druid'),
(48487, 3, 6054.942383, -934.681335, 455.893829, 'Whisperwind Druid');

-- 
-- Shade of the Spiritspeaker SAI
SET @ENTRY := 48678;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,0,2000,2000,1500,1500,11,90700,1,0,0,0,0,1,0,0,0,0,0,0,0,"Shade of the Spiritspeaker - Out of Combat - Cast 'Summon Arcane Sphere'"),
(@ENTRY,0,1,0,0,0,100,0,2000,5000,5000,5000,11,85424,2,0,0,0,0,2,0,0,0,0,0,0,0,"Shade of the Spiritspeaker - In Combat - Cast 'Spirit Burst'");

DELETE FROM `creature_template_addon` WHERE `entry`=48681;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(48681,0,0,33554432,1,0,'90698');

-- Arcane Roller SAI
SET @ENTRY := 48681;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,87,@ENTRY*100+00,@ENTRY*100+01,@ENTRY*100+02,0,0,0,1,0,0,0,0,0,0,0,"Arcane Roller - On Just Summoned - Run Random Script"),
(@ENTRY,0,1,0,4,0,100,0,0,0,0,0,41,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Arcane Roller - On Aggro - Despawn Instant");

-- Actionlist SAI
SET @ENTRY := 4868100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,100,100,0,0,53,1,4868100,0,0,1000,0,1,0,0,0,0,0,0,0,"On Script - Start Waypoint");

-- Actionlist SAI
SET @ENTRY := 4868101;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,100,100,0,0,53,1,4868101,0,0,1000,0,1,0,0,0,0,0,0,0,"On Script - Start Waypoint");

-- Actionlist SAI
SET @ENTRY := 4868102;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,100,100,0,0,53,1,4868102,0,0,1000,0,1,0,0,0,0,0,0,0,"On Script - Start Waypoint");

UPDATE `creature_template` SET `InhabitType`=4 WHERE  `entry`=48681;
UPDATE `creature_template` SET `speed_walk`=2.5, `speed_run`=2.5 WHERE  `entry`=48681;

DELETE FROM `waypoints` WHERE `entry`=4868101;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(4868101, 1, 6195.35, -4111.85, 744.147, 'Arcane Roller'),
(4868101, 2, 6203.05, -4111.92, 741.04, 'Arcane Roller'),
(4868101, 3, 6212.6, -4112.66, 735.066, 'Arcane Roller'),
(4868101, 4, 6228.86, -4114.14, 723.562, 'Arcane Roller'),
(4868101, 5, 6240.95, -4113.68, 721.984, 'Arcane Roller'),
(4868101, 6, 6247.91, -4112.89, 721.482, 'Arcane Roller'),
(4868101, 7, 6261.13, -4110.28, 718.787, 'Arcane Roller'),
(4868101, 8, 6274.36, -4103.5, 715.604, 'Arcane Roller'),
(4868101, 9, 6285.37, -4086.53, 710.668, 'Arcane Roller'),
(4868101, 10, 6293.89, -4064.3, 703.658, 'Arcane Roller'),
(4868101, 11, 6303.82, -4052.11, 701.461, 'Arcane Roller'),
(4868101, 12, 6315.01, -4049.11, 698.762, 'Arcane Roller'),
(4868101, 13, 6331.43, -4046.85, 697.436, 'Arcane Roller'),
(4868101, 14, 6354.13, -4042.56, 694.022, 'Arcane Roller'),
(4868101, 15, 6360.85, -4041.71, 686.905, 'Arcane Roller'),
(4868101, 16, 6378.82, -4041.17, 664.861, 'Arcane Roller');

DELETE FROM `waypoints` WHERE `entry`=4868100;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(4868100, 1, 6196.32, -4105.12, 744.028, 'Arcane Roller'),
(4868100, 2, 6206.79, -4105.92, 739.795, 'Arcane Roller'),
(4868100, 3, 6217.26, -4106.65, 732.59, 'Arcane Roller'),
(4868100, 4, 6229.98, -4107.5, 723.776, 'Arcane Roller'),
(4868100, 5, 6241.76, -4107.84, 722.284, 'Arcane Roller'),
(4868100, 6, 6251.8, -4107.76, 721.797, 'Arcane Roller'),
(4868100, 7, 6265.56, -4100.52, 716.464, 'Arcane Roller'),
(4868100, 8, 6272.71, -4094.95, 713.936, 'Arcane Roller'),
(4868100, 9, 6277.71, -4084.69, 711.392, 'Arcane Roller'),
(4868100, 10, 6282.68, -4072.6, 708.661, 'Arcane Roller'),
(4868100, 11, 6290.79, -4054.63, 704.589, 'Arcane Roller'),
(4868100, 12, 6298.65, -4047.94, 703.035, 'Arcane Roller'),
(4868100, 13, 6310.25, -4044.52, 699.736, 'Arcane Roller'),
(4868100, 14, 6323.37, -4041.15, 697.811, 'Arcane Roller'),
(4868100, 15, 6345.7, -4034.72, 695.33, 'Arcane Roller'),
(4868100, 16, 6355.44, -4032.31, 690.346, 'Arcane Roller'),
(4868100, 17, 6377.85, -4026.08, 664.858, 'Arcane Roller');

DELETE FROM `waypoints` WHERE `entry`=4868102;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(4868102, 1, 6195.2, -4118.27, 743.577, 'Arcane Roller'),
(4868102, 2, 6202.19, -4118.49, 740.402, 'Arcane Roller'),
(4868102, 3, 6214.4, -4120.41, 732.631, 'Arcane Roller'),
(4868102, 4, 6229.96, -4123.08, 723.281, 'Arcane Roller'),
(4868102, 5, 6241.04, -4121.34, 721.329, 'Arcane Roller'),
(4868102, 6, 6254.89, -4119.31, 720.073, 'Arcane Roller'),
(4868102, 7, 6275.18, -4112.21, 716.654, 'Arcane Roller'),
(4868102, 8, 6286.03, -4103.33, 714.633, 'Arcane Roller'),
(4868102, 9, 6296.62, -4080.74, 706.334, 'Arcane Roller'),
(4868102, 10, 6313.36, -4054.73, 698.949, 'Arcane Roller'),
(4868102, 11, 6344.67, -4053.76, 695.791, 'Arcane Roller'),
(4868102, 12, 6353.26, -4052.85, 693.859, 'Arcane Roller'),
(4868102, 13, 6389.22, -4047.54, 658.284, 'Arcane Roller');

-- Quest Fix: Yetiphobia
-- Legacki SAI
SET @ENTRY := 10978;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,0,17166,0,0,0,80,1097800,2,0,0,0,0,1,0,0,0,0,0,0,0,"Legacki - On Spellhit 'Release Umi's Yeti' - Run Script"),
(@ENTRY,0,1,0,40,0,100,0,15,10978,0,0,80,@ENTRY*100+01,2,0,0,0,0,1,0,0,0,0,0,0,0,"Legacki - On Waypoint 15 Reached - Run Script");
-- Also add quest credit upon spell hit.
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES (10978, 0, 2, 0, 8, 0, 100, 0, 17166, 0, 0, 0, 33, 10978, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Legacki - On Spellhit \'Release Umi\'s Yeti\' - Quest Credit \'Are We There, Yeti?\'');

-- Actionlist SAI
SET @ENTRY := 1097800;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,1000,1000,0,0,53,1,10978,0,0,0,2,1,0,0,0,0,0,0,0,"On Script - Start Waypoint"),
(@ENTRY,9,2,0,0,0,100,0,1000,1000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1");

-- Actionlist SAI
SET @ENTRY := 1097801;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,2000,2000,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 2"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 3"),
(@ENTRY,9,2,0,0,0,100,0,2000,2000,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 4");

DELETE FROM `creature_text` WHERE `entry`=10978;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(10978, 0, 0, '%s jumps in fright!', 16, 0, 100, 0, 0, 0, 6301, 'Legacki'),
(10978, 1, 0, 'Ahhhhh!!!', 12, 0, 100, 0, 0, 0, 6302, 'Legacki'),
(10978, 2, 0, 'You big meanie! Who put you up to this?', 12, 0, 100, 0, 0, 0, 6303,'Legacki'),
(10978, 3, 0, 'It was Umi, wasn''t it?! She''s always playing jokes on me, and now she''s got you in on it too!', 12, 0, 100, 0, 0, 0, 6304, 'Legacki'),
(10978, 4, 0, '%s sighs.', 16, 0, 100, 0, 0, 0, 6305, 'Legacki');

-- Umi's Mechanical Yeti SAI
SET @ENTRY := 10980;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,54,0,100,0,0,0,0,0,29,5,10,0,0,0,0,19,10978,10,0,0,0,0,0,"Umi's Mechanical Yeti - On Just Summoned - Start Follow Closest Creature 'Legacki'"),
(@ENTRY,0,1,3,61,0,100,0,0,0,0,0,1,0,1000,0,0,0,0,1,0,0,0,0,0,0,0,"Umi's Mechanical Yeti - On Just Summoned - Say Line 0"),
(@ENTRY,0,2,0,52,0,100,0,0,10980,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Umi's Mechanical Yeti - On Text 0 Over - Say Line 1"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,41,30000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Umi's Mechanical Yeti - On Just Summoned - Despawn In 30000 ms");

DELETE FROM `creature_text` WHERE `entry`=10980;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(10980, 0, 0, '%s marches around, roaring and making a ruckus.', 16, 0, 100, 0, 0, 0, 6327, 'Umi\'s Mechanical Yeti'),
(10980, 1, 0, 'RAAAAAAAR!', 12, 0, 100, 0, 0, 0, 6329, 'Umi\'s Mechanical Yeti');

DELETE FROM `waypoints` WHERE `entry`=10978;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(10978, 1,6713.694,-4682.374,721.6163, 'Legacki'),
(10978, 2,6731.523,-4675.635,721.2076, 'Legacki'),
(10978, 3,6736.806,-4672.955,721.9132, 'Legacki'),
(10978, 4,6749.889,-4662.13,725.0011, 'Legacki'),
(10978, 5,6750.157,-4661.562,724.9875, 'Legacki'),
(10978, 6,6732.859,-4641.781,722.8784, 'Legacki'),
(10978, 7,6727.719,-4641.233,721.6862, 'Legacki'),
(10978, 8,6715.982,-4643.644,721.1971, 'Legacki'),
(10978, 9,6719.307,-4652.817,721.231, 'Legacki'),
(10978, 10,6715.004,-4685.518,721.466, 'Legacki'),
(10978, 11,6718.75,-4705.675,721.9097, 'Legacki'),
(10978, 12,6712.263,-4717.947,721.5077, 'Legacki'),
(10978, 13,6701.636,-4714.284,721.6529, 'Legacki'),
(10978, 14,6701.108,-4695.846,722.3613, 'Legacki'),
(10978, 15,6705.608,-4686.181,721.9736, 'Legacki');

DELETE FROM `creature_addon` WHERE `guid`=65250;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES 
(65250,0,0,3,1,0, '');

-- Silithus - Complete Questrelated Stuff
-- teleporter position
DELETE FROM `spell_target_position` WHERE `id`=93726;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(93726, 0, -10878, -2683.63, 9.61432, 0.691772);

UPDATE `creature_template` SET `gossip_menu_id`=6687 WHERE  `entry`=15612;
DELETE FROM `gossip_menu` WHERE `entry`=6687;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (6687, 8059);

-- Beetix Ficklespragg SAI
SET @ENTRY := 15189;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,8278,0,0,0,1,0,0,0,0,0,0,19,15190,20,0,0,0,0,0,"Beetix Ficklespragg - On Quest 'Noggle's Last Hope' Finished - Say Line 0"),
(@ENTRY,0,1,0,1,0,100,0,5000,5000,20000,20000,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Beetix Ficklespragg - Out of Combat - Say Line 0");

-- Noggle Ficklespragg SAI
SET @ENTRY := 15190;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,8282,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Noggle Ficklespragg - On Quest 'Noggle's Lost Satchel' Finished - Say Line 1");

DELETE FROM `creature_text` WHERE `entry` IN (15190, 15189);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(15190, 0, 0, 'Yes! I\'m cured!  I\'m cure... whoa, got up too fast!', 12, 0, 100, 0, 0, 0, 10675, ''),
(15190, 1, 0, 'Thanks for finding my reagents!  *yawn*  That poison\'s made me... a bit tired...', 12, 0, 100, 0, 0, 0, 10676, ''),
(15189, 0, 0, 'You were always the troublemaker in the family, Noggle.  Always!', 12, 0, 100, 0, 0, 0, 10751, '');

-- Commander Mar'alith SAI
SET @ENTRY := 15181;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,8304,0,0,0,1,0,0,0,0,0,0,21,20,0,0,0,0,0,0,"Commander Mar'alith - On Quest 'Dearest Natalia' Taken - Say Line 0");

DELETE FROM `creature_text` WHERE `entry` IN (15181);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(15181, 0, 0, 'Take this enchanted shard, $n. I will be able to hear the dwarves myself through its twin.', 12, 0, 100, 0, 0, 0, 50667, '');

-- Pathing for  Entry: 11804 'TDB FORMAT' 
SET @NPC := 64455;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-6942.874,`position_y`=1110.533,`position_z`=0.7047257 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-6942.874,1110.533,0.7047257,0,0,0,0,100,0),
(@PATH,2,-6950.666,1104.255,0.8397331,0,0,0,0,100,0),
(@PATH,3,-6950.607,1104.366,0.5897331,0,0,0,0,100,0),
(@PATH,4,-6950.251,1104.682,0.8397331,0,0,0,0,100,0),
(@PATH,5,-6942.69,1110.832,0.8038445,0,0,0,0,100,0),
(@PATH,6,-6937.58,1115.91,1.38085,0,0,0,0,100,0),
(@PATH,7,-6934.83,1118.41,1.88085,0,0,0,0,100,0),
(@PATH,8,-6932.83,1120.41,2.38085,0,0,0,0,100,0),
(@PATH,9,-6932.487,1120.662,2.760576,0,0,0,0,100,0),
(@PATH,10,-6931.737,1121.412,2.760576,0,0,0,0,100,0),
(@PATH,11,-6929.237,1124.162,3.510576,0,0,0,0,100,0),
(@PATH,12,-6929.332,1124.039,3.269823,0,0,0,0,100,0),
(@PATH,13,-6932.028,1121.268,2.675391,0,0,0,0,100,0),
(@PATH,14,-6933.528,1119.768,2.175391,0,0,0,0,100,0),
(@PATH,15,-6935.278,1118.018,1.675391,0,0,0,0,100,0),
(@PATH,16,-6937.735,1115.59,1.140587,0,0,0,0,100,0),
(@PATH,17,-6943.003,1110.408,0.6729961,0,0,0,0,100,0),
(@PATH,18,-6950.686,1104.583,0.8397331,0,0,0,0,100,0);

-- Pathing for  Entry: 11803 'TDB FORMAT' 
SET @NPC := 64865;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `spawndist`=0,`MovementType`=2,`position_x`=-7963.382,`position_y`=1977.28,`position_z`=6.844955 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES (@NPC,@PATH,0,0,1,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_flag`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,-7963.382,1977.28,6.844955,0,0,0,0,100,0),
(@PATH,2,-7964.632,1977.03,7.094955,0,0,0,0,100,0),
(@PATH,3,-7972.582,1976.436,7.295979,0,0,0,0,100,0),
(@PATH,4,-7977.582,1973.686,6.795979,0,0,0,0,100,0),
(@PATH,5,-7973.047,1976.35,7.348126,0,0,0,0,100,0),
(@PATH,6,-7972.883,1976.539,7.290794,0,0,0,0,100,0),
(@PATH,7,-7972.383,1976.539,7.290794,0,0,0,0,100,0),
(@PATH,8,-7964.361,1977.418,7.03627,0,0,0,0,100,0),
(@PATH,9,-7960.611,1977.668,6.53627,0,0,0,0,100,0),
(@PATH,10,-7960.552,1977.869,6.533808,0,0,0,0,100,0),
(@PATH,11,-7957.052,1978.119,6.283808,0,0,0,0,100,0),
(@PATH,12,-7957.295,1977.961,6.037067,0,0,0,0,100,0),
(@PATH,13,-7957.259,1978.09,6.310391,0,0,0,0,100,0),
(@PATH,14,-7963.009,1977.59,6.810391,0,0,0,0,100,0),
(@PATH,15,-7964.595,1977.169,6.948967,0,0,0,0,100,0);

-- Mistress Natalia Mar'alith SAI
SET @ENTRY := 15215;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,4,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mistress Natalia Mar'alith - On Aggro - Say Line 0"),
(@ENTRY,0,1,0,0,0,100,0,10000,12000,10000,12000,11,44415,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mistress Natalia Mar'alith - In Combat - Cast 'Blackout'"),
(@ENTRY,0,2,0,2,0,100,1,0,50,0,0,11,13704,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mistress Natalia Mar'alith - Between 0-50% Health - Cast 'Psychic Scream' (No Repeat)"),
(@ENTRY,0,3,0,0,0,100,0,12000,16000,16000,24000,11,11639,0,0,0,0,0,2,0,0,0,0,0,0,0,"Mistress Natalia Mar'alith - In Combat - Cast 'Shadow Word: Pain'");

DELETE FROM `creature_text` WHERE `entry` IN (15215);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(15215, 0, 0, 'Take this enchanted shard, $n. I will be able to hear the dwarves myself through its twin.', 12, 0, 100, 0, 0, 0, 10742, '');