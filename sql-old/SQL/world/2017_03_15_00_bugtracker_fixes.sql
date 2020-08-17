

-- Bugtracker id : 450
-- link : http://wowzealot.com/bugtracker/issue/450/%28Q%29-Clam-Bait-%5BDesolace%5D
-- quest : [Clam Bait] 6142


DELETE FROM `gameobject` WHERE  `id`=177784 and `map`=1 and `zone`=0 and `area`=0 ;

INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -464.644, 2797.81, -2.32749, -2.61799, 0, 0, -0.965925, 0.258821, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -611.243, 2969.67, -5.75299, 2.40855, 0, 0, 0.93358, 0.358368, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -588.835, 2648.15, -5.43225, 1.67551, 0, 0, 0.743144, 0.669132, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -645.7, 2896.28, -23.6495, -1.69297, 0, 0, -0.748956, 0.66262, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -241.083, 2690.35, -37.2636, -2.75762, 0, 0, -0.981627, 0.190812, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -545.825, 2710.22, -12.0421, -2.1293, 0, 0, -0.874619, 0.48481, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -370.996, 2697.79, -15.2298, 2.16421, 0, 0, 0.882947, 0.469473, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -305.28, 2763.53, -10.2838, -3.05432, 0, 0, -0.999048, 0.0436191, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -277.379, 2846.93, -12.2052, 2.82743, 0, 0, 0.987688, 0.156436, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -573.625, 2860.44, 2.42516, -0.523599, 0, 0, -0.258819, 0.965926, 300, 100, 1);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zone`, `area`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (NULL, 177784, 1, 0, 0, 1, 1, -606.141, 2758.65, -21.3477, -1.16937, 0, 0, -0.551936, 0.833886, 300, 100, 1);

UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=33.7 WHERE  `entry`=12347 AND `item`=15874;


-- Bugtracker id : 0452
-- link : http://wowzealot.com/bugtracker/issue/452
-- quest : [Falcon Watch] 9499


UPDATE `quest_template` SET `RequiredRaces`=512 WHERE  `Id`=9499;




-- Bugtracker id : 0449
-- link : http://wowzealot.com/bugtracker/issue/449/Those-That-Couldn%27t-Let-Go
-- quest : [Those That Couldn't Let Go] 27168

-- The Withdrawn Souls never turn against you, and they are missing texts, and they also disappear when the quest item is used.

-- Creature Text
DELETE FROM `creature_text` WHERE  `entry`=45166;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(45166, 0, 0, 'I... I think I\'m ready to leave now.', 12, 0, 100, 0, 0, 0, 'On Friendly'),
(45166, 0, 1, 'I must be going now...', 12, 0, 100, 0, 0, 0, 'On Friendly'), 
(45166, 0, 2, 'Well, I\'ve said my goodbyes. Time to move on.', 12, 0, 100, 0, 0, 0, 'On Friendly'), 
(45166, 1, 0, 'You Thoughtless $r! You\'ll pay for your Interference!', 14, 0, 100, 0, 0, 0, 'On Enemy'),
(45166, 1, 1, 'Who are you? What have you done to me?', 14, 0, 100, 0, 0, 0, 'On Enemy'), 
(45166, 1, 2, 'You thoughtless $r! You\'ll pay for your', 14, 0, 100, 0, 0, 0, 'On Enemy');

-- Creature MovmentType
UPDATE `creature` SET `MovementType`=1 WHERE  `id`=45166;
UPDATE `creature` SET `spawndist`=5 WHERE  `id`=45166;

-- Withdrawn Soul SAI
SET @ENTRY := 45166;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,8,0,100,1,84375,0,0,0,30,1,1,1,2,2,2,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Set Random Phase(1, 1, 1, 2, 2, 2) (No Repeat)"),
(@ENTRY,0,1,0,8,1,100,0,84375,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Say Line 0"),
(@ENTRY,0,2,0,8,1,100,0,84375,0,0,0,33,45166,0,0,0,0,0,7,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Quest Credit 'Those That Couldn't Let Go'"),
(@ENTRY,0,3,0,8,1,100,0,84375,0,0,0,46,10,0,0,0,0,0,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Move Forward 10 Yards"),
(@ENTRY,0,4,0,8,1,100,0,84375,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Despawn In 2000 ms"),
(@ENTRY,0,5,0,8,2,100,0,84375,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Say Line 1"),
(@ENTRY,0,6,0,8,2,100,0,84375,0,0,0,2,14,0,0,0,0,0,1,0,0,0,0,0,0,0,"Withdrawn Soul - On Spellhit '<Spell not found!>' - Set Faction 14");


