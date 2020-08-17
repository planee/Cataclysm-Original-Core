--  [The Last Wave of Survivors] quest=13518 by gexir
-- id Report=402
-- http://www.wowhead.com/quest=13518/the-last-wave-of-survivors

-- Cerellean Whiteclaw SAI
SET @ENTRY := 33093;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
UPDATE `creature_template` SET ScriptName='' , npcflag=1  , unit_flags2=2049 , dynamicflags=0 WHERE entry =@entry;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,64,0,100,0,0,0,0,0,33,@ENTRY,0,0,0,0,0,7,0,0,0,0,0,0,0,"Cerellean Whiteclaw - On Gossip Hello - Quest Credit 'The Last Wave of Survivors'"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Cerellean Whiteclaw - On Gossip Hello - Close Gossip");


-- Gershala Nightwhisper SAI
SET @ENTRY := 32911;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
UPDATE `creature_template` SET ScriptName='' , npcflag=1  , unit_flags2=2049 , dynamicflags=0 WHERE entry =@entry;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,64,0,100,0,0,0,0,0,33,@ENTRY,0,0,0,0,0,7,0,0,0,0,0,0,0,"Gershala Nightwhisper - On Gossip Hello - Quest Credit 'The Last Wave of Survivors'"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Gershala Nightwhisper - On Gossip Hello - Close Gossip");


-- Shaldyn SAI
SET @ENTRY := 33095;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
UPDATE `creature_template` SET ScriptName='' , npcflag=1  ,  unit_flags2=2049 , dynamicflags=0 WHERE entry =@entry;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,64,0,100,0,0,0,0,0,33,@ENTRY,0,0,0,0,0,7,0,0,0,0,0,0,0,"Shaldyn - On Gossip Hello - Quest Credit 'The Last Wave of Survivors'"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Shaldyn - On Gossip Hello - Close Gossip");


-- Volcor SAI
SET @ENTRY := 33094;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
UPDATE `creature_template` SET ScriptName='' , npcflag=1  , unit_flags2=2049 , dynamicflags=0 WHERE entry =@entry;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,64,0,100,0,0,0,0,0,33,@ENTRY,0,0,0,0,0,7,0,0,0,0,0,0,0,"Shaldyn - On Gossip Hello - Quest Credit 'The Last Wave of Survivors'"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,"Shaldyn - On Gossip Hello - Close Gossip");



--  [Stranglethorn Fever]quest =26597 by gexir
-- http://www.wowhead.com/quest=26597/stranglethorn-fever
-- Report id=399

UPDATE `quest_template` SET SpecialFlags =1 WHERE id =26598;

DELETE FROM `creature_questrelation` WHERE `id`=1449 AND `quest`=26598;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (1449, 26598);

-- Witch Doctor Unbagwa SAI
SET @ENTRY := 1449;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,26598,0,0,0,80,@ENTRY*100+00,0,0,0,0,0,1,0,0,0,0,0,0,0,"Witch Doctor Unbagwa - On Quest 'The Heart of Mokk' Finished - Run Script");

-- Actionlist SAI
SET @ENTRY := 144900;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Witch Doctor Unbagwa - On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,81,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Witch Doctor Unbagwa - On Script - Set Npc Flag "),
(@ENTRY,9,2,0,0,0,100,0,9000,15000,0,0,12,1511,1,50000,0,0,0,8,0,0,0,-13814.7,4.09956,25.472,1.2342,"Witch Doctor Unbagwa - On Script - Summon Creature 'Enraged Silverback Gorilla'"),
(@ENTRY,9,3,0,0,0,100,0,13000,16000,0,0,12,1516,1,50000,0,0,0,8,0,0,0,-13814.7,4.09956,25.472,1.2342,"Witch Doctor Unbagwa - On Script - Summon Creature 'Konda'"),
(@ENTRY,9,4,0,0,0,100,0,13000,16000,0,0,12,1514,1,50000,0,0,0,8,0,0,0,-13814.7,4.09956,25.472,1.2342,"Witch Doctor Unbagwa - On Script - Summon Creature 'Mokk the Savage'"),
(@ENTRY,9,5,0,0,0,100,0,2000,3000,0,0,81,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Witch Doctor Unbagwa - On Script - Set Npc Flags Gossip & Questgiver");

-- Enraged Silverback Gorilla SAI
SET @ENTRY := 1511;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,1,0,1,0,100,0,0,0,0,0,49,0,0,0,0,0,0,21,5,0,0,0,0,0,0,"Enraged Silverback Gorilla - Out of Combat - Start Attacking"),
(@ENTRY,0,2,0,11,0,100,0,0,0,0,0,53,1,1514,0,0,0,0,1,0,0,0,0,0,0,0,"Enraged Silverback Gorilla - On Respawn - Start Waypoint");

-- Konda SAI
SET @ENTRY := 1516;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,1,0,1,0,100,0,0,0,0,0,49,0,0,0,0,0,0,21,5,0,0,0,0,0,0,"Konda - Out of Combat - Start Attacking"),
(@ENTRY,0,2,0,11,0,100,0,0,0,0,0,53,1,1514,0,0,0,0,1,0,0,0,0,0,0,0,"Konda - On Respawn - Start Waypoint");

-- Mokk the Savage SAI
SET @ENTRY := 1514;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,1,0,1,0,100,0,0,0,0,0,49,0,0,0,0,0,0,21,5,0,0,0,0,0,0,"Mokk the Savage - Out of Combat - Start Attacking"),
(@ENTRY,0,2,0,11,0,100,0,0,0,0,0,53,1,1514,0,0,0,0,1,0,0,0,0,0,0,0,"Mokk the Savage - On Respawn - Start Waypoint");

DELETE FROM `creature_text` WHERE `entry`=1449 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (1449, 0, 0, 'Get ready everyone! Here come de apes!', 14, 0, 100, 0, 2000, 0, '');



DELETE FROM `waypoints` WHERE `entry`=1514 AND `pointid`=1;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (1514, 1, -13805.9, 6.49262, 29.4409, '');
DELETE FROM `waypoints` WHERE `entry`=1514 AND `pointid`=2;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (1514, 2, -13790, 5.46866, 33.8343, '');
DELETE FROM `waypoints` WHERE `entry`=1514 AND `pointid`=3;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (1514, 3, -13776.1, -1.97773, 40.417, '');
DELETE FROM `waypoints` WHERE `entry`=1514 AND `pointid`=4;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (1514, 4, -13766.8, -9.60726, 43.9905, '');
DELETE FROM `waypoints` WHERE `entry`=1514 AND `pointid`=5;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES (1514, 5, -13751.8, -19.0758, 44.0893, '');


--  [The Yorgen Worgen] quest=26717 by gexir
-- http://www.wowhead.com/quest=26717/the-yorgen-worgen
-- id report=320

-- Mound of Loose Dirt SAI
SET @ENTRY := 204777;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,1,64,0,100,1,0,0,0,0,33,3322,0,0,0,0,0,7,0,0,0,0,0,0,0,"Mound of Loose Dirt - On Gossip Hello - Quest Credit 'The Yorgen Worgen'"),
(@ENTRY,1,1,0,61,0,100,0,0,0,0,0,78,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Mound of Loose Dirt - On Gossip Hello - Reset all Script'");



-- Lurking Worgen SAI
SET @ENTRY := 43799;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
UPDATE `creature_template_addon` SET auras='' WHERE entry=@entry;
UPDATE `creature_template` SET ScriptName='' WHERE entry =@entry;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,54,0,100,0,0,0,0,0,11,81957,0,0,0,0,0,21,30,0,0,0,0,0,0,"Lurking Worgen - On Just Summoned - Cast '<Spell not found!>'"),
(@ENTRY,0,1,0,60,0,100,1,500,500,0,0,53,1,43799,0,0,1000,0,1,0,0,0,0,0,0,0,"Lurking Worgen - On Update - Start Waypoint (No Repeat)");


-- Waypoints
SET @ENTRY :=43799;
DELETE FROM `waypoints` WHERE `entry`=@entry;
INSERT INTO `waypoints` (`entry`,`pointid`,`position_x`,`position_y`,`position_z`,`point_comment`) VALUES
(@entry,1,-11121.03,-511.02,33.54,'Lurking Worgen'),
(@entry,2,-11119.43,-507.90,33.48,'Lurking Worgen'),
(@entry,3,-11123.89,-482.61,34.90,'Lurking Worgen');

