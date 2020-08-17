-- Slitherblade Sorceress
-- id report=448

-- Slitherblade Sorceress SAI
SET @ENTRY := 4712;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,4,0,100,1,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Set Phase 1 on Aggro"),
(@ENTRY,0,1,0,4,1,100,1,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Stop Moving on Aggro"),
(@ENTRY,0,2,0,4,1,100,1,0,0,0,0,11,34425,0,0,0,0,0,2,0,0,0,0,0,0,0,"Cast bolt on Aggro"),
(@ENTRY,0,3,0,9,1,100,0,0,40,3400,4700,11,34425,0,0,0,0,0,2,0,0,0,0,0,0,0,"Cast bolt"),
(@ENTRY,0,4,0,9,1,100,0,40,100,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Start Moving when not in bolt Range"),
(@ENTRY,0,5,0,9,1,100,0,10,15,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Stop Moving at 15 Yards"),
(@ENTRY,0,6,0,9,1,100,0,0,40,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Stop Moving when in bolt Range"),
(@ENTRY,0,7,0,3,1,100,0,0,15,0,0,22,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"Set Phase 2 at 15% Mana"),
(@ENTRY,0,8,0,3,2,100,0,0,15,0,0,21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Start Moving at 15% Mana"),
(@ENTRY,0,9,0,3,2,100,0,30,100,100,100,22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Set Phase 1 When Mana is above 30%"),
(@ENTRY,0,10,0,2,2,100,1,0,15,0,0,22,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"Set Phase 3 at 15% HP"),
(@ENTRY,0,11,0,2,3,100,1,0,15,0,0,25,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Flee at 15% HP"),
(@ENTRY,0,12,0,7,3,100,1,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Reset on Evade"),
(@ENTRY,0,13,0,2,3,100,1,0,15,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Say Text at 15% HP"),
(@ENTRY,0,14,0,1,0,100,0,500,1000,600000,600000,11,12544,0,0,0,0,0,1,0,0,0,0,0,0,0,"Cast Frost Armor on Spawn"),
(@ENTRY,0,15,0,0,1,100,0,5000,8000,17000,22000,11,77921,0,0,0,0,0,2,0,0,0,0,0,0,0,"Cast Slitherstrike"),
(@ENTRY,0,16,0,1,0,100,1,1000,1000,1000,1000,11,45846,2,0,0,0,0,1,0,0,0,0,0,0,0,"Cast Frost Aura on Spawn");


--  [Spared from Madness] quest =26736 by gexir
-- https://www.wow-freakz.com/quest_helper.php?quest=26736&name=spared-from-madness
-- id report =351

-- Osborn Obnoticus SAI
SET @ENTRY := 43884;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,1,1,0,0,11,82214,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Cast '<Spell not found!>'"),
(@ENTRY,0,1,0,61,0,100,0,1,1,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Despawn Instant");

-- Kurzen Cage SAI
SET @ENTRY := 204828;
UPDATE `gameobject_template` SET `AIName`="SmartGameObjectAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,1,0,0,70,0,100,0,2,0,0,0,33,43919,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kurzen Cage - On Gameobject State Changed - Quest Credit 'Spared from Madness'"),
(@ENTRY,1,1,0,70,0,100,0,2,0,0,0,33,43920,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kurzen Cage - On Gameobject State Changed - Quest Credit 'Spared from Madness'"),
(@ENTRY,1,2,3,70,0,100,0,2,0,0,0,33,43921,0,0,0,0,0,7,0,0,0,0,0,0,0,"Kurzen Cage - On Gameobject State Changed - Quest Credit 'Spared from Madness'"),
(@ENTRY,1,3,0,61,0,100,0,2,0,0,0,45,1,1,0,0,0,0,11,0,10,0,0,0,0,0,"Kurzen Cage - On Gameobject State Changed - Set Data 1 1");


-- Emerine Junis SAI
SET @ENTRY := 43885;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,1,1,0,0,11,82213,0,0,0,0,0,1,0,0,0,0,0,0,0,"Emerine Junis - On Data Set 1 1 - Cast '<Spell not found!>'"),
(@ENTRY,0,1,0,61,0,100,0,1,1,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Emerine Junis - On Data Set 1 1 - Despawn Instant");

-- Berrin Burnquill SAI
SET @ENTRY := 43886;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,38,0,100,0,1,1,0,0,11,82212,0,0,0,0,0,1,0,0,0,0,0,0,0,"Berrin Burnquill - On Data Set 1 1 - Cast '<Spell not found!>'"),
(@ENTRY,0,1,0,61,0,100,0,1,1,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Berrin Burnquill - On Data Set 1 1 - Despawn Instant");

-- Berrin Burnquill SAI
SET @ENTRY := 43921;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-11572.86,-573.80,33.68,0,"Berrin Burnquill - On Just Summoned - Move To Position"),
(@ENTRY,0,1,0,34,0,100,0,8,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Berrin Burnquill - On Reached Point 0 - Say Line 0"),
(@ENTRY,0,3,4,38,0,100,0,2,2,0,0,11,41232,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Cast 'Teleport Visual Only'"),
(@ENTRY,0,4,0,61,0,100,0,1,1,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Despawn Instant");

-- Osborn Obnoticus SAI
SET @ENTRY := 43919;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-11570.92,-573.60,33.68,0,"Osborn Obnoticus - On Just Summoned - Move To Position"),
(@ENTRY,0,1,2,34,0,100,0,8,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Reached Point 0 - Say Line 0"),
(@ENTRY,0,3,4,38,0,100,0,2,2,0,0,11,41232,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Cast 'Teleport Visual Only'"),
(@ENTRY,0,4,0,61,0,100,0,1,1,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Osborn Obnoticus - On Data Set 1 1 - Despawn Instant");

-- Emerine Junis SAI
SET @ENTRY := 43920;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,54,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,-11568.06,-573.73,33.68,0,"Emerine Junis - On Just Summoned - Move To Position"),
(@ENTRY,0,1,0,34,0,100,0,8,0,0,0,80,4392000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Emerine Junis - On Reached Point 0 - Run Script");


-- Actionlist SAI
SET @ENTRY := 4392000;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,1000,1000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 0"),
(@ENTRY,9,1,0,0,0,100,0,2000,2000,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 1"),
(@ENTRY,9,2,0,0,0,100,0,3000,3000,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Say Line 3"),
(@ENTRY,9,3,0,0,0,100,0,2000,2000,0,0,45,2,2,0,0,0,0,11,0,10,0,0,0,0,0,"On Script - Set Data 1 1"),
(@ENTRY,9,4,0,0,0,100,0,0,0,0,0,11,41232,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Cast 'Teleport Visual Only'"),
(@ENTRY,9,5,0,0,0,100,0,1000,1000,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Script - Despawn Instant");



DELETE FROM `creature_text` WHERE `entry`=43919 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (43919, 0, 0, 'Woo hoo.', 12, 0, 100, 0, 0, 0, '');
DELETE FROM `creature_text` WHERE `entry`=43920 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (43920, 0, 0, 'Thank you! I don\'t know how much longer we would have survived.', 12, 0, 100, 0, 0, 0, '');
DELETE FROM `creature_text` WHERE `entry`=43920 AND `groupid`=1 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (43920, 1, 0, 'That cage was stifling my magic.', 12, 0, 100, 0, 0, 0, '');
DELETE FROM `creature_text` WHERE `entry`=43920 AND `groupid`=2 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (43920, 2, 0, 'If you don\'t mind, I\'m going to teleport back us to the Rebel Camp. We\'ll thank you in person later.', 12, 0, 100, 0, 0, 0, '');
DELETE FROM `creature_text` WHERE `entry`=43921 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (43921, 0, 0, 'Freedom! Sweet, sweet freedom!', 12, 0, 100, 0, 0, 0, '');


--  [What I Do Best] quest =27371 by gexir
-- http://www.wowhead.com/quest=27371/what-i-do-best
-- id report =483

-- Tarenar Sunstrike SAI
SET @ENTRY := 45429;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,27371,0,0,0,85,84902,2,0,0,0,0,7,0,0,0,0,0,0,0,"Tarenar Sunstrike - On Quest 'What I Do Best' Taken - Invoker Cast '<Spell not found!>'");



UPDATE `quest_template` SET Flags=0 WHERE id=27371;


-- Tarenar Sunstrike SAI
SET @ENTRY := 45445;
UPDATE `creature_template` SET `AIName`="SmartAI" , unit_class=2 WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,1,0,100,1,1000,1000,0,0,11,79977,0,0,0,0,0,23,0,0,0,0,0,0,0,"Tarenar Sunstrike - Out of Combat - Cast 'Blessing of might' (No Repeat)"),
(@ENTRY,0,1,0,0,0,100,0,3000,5000,9000,9000,11,79970,0,0,0,0,0,2,0,0,0,0,0,0,0,"Tarenar Sunstrike - In Combat - Cast 'Divine Storm'"),
(@ENTRY,0,2,0,12,0,100,0,0,20,5000,5000,11,79971,0,0,0,0,0,2,0,0,0,0,0,0,0,"Tarenar Sunstrike - Target Between 0-20% Health - Cast 'Hammer of wrath'"),
(@ENTRY,0,3,0,0,0,100,0,7000,7000,11000,11000,11,79972,0,0,0,0,0,2,0,0,0,0,0,0,0,"Tarenar Sunstrike - In Combat - Cast 'Seal of truth'"),
(@ENTRY,0,4,0,14,0,100,0,99,50,5000,5000,20,1,0,0,0,0,0,11,0,0,0,0,0,0,0,"Tarenar Sunstrike - Friendly At 99 Health - Start Attacking"),
(@ENTRY,0,5,0,54,0,100,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Tarenar Sunstrike - On Just Summoned - Stop Attacking"),
(@ENTRY,0,6,0,1,0,100,1,2000,2000,0,0,11,79977,0,0,0,0,0,1,0,0,0,0,0,0,0,"Tarenar Sunstrike - Out of Combat - Cast 'Blessing of might' (No Repeat)");
