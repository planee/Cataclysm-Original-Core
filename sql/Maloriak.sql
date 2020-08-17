-- Maloriak Texts
DELETE FROM `creature_text` WHERE `entry` = 41378;
INSERT INTO `creature_text` VALUES
(41378, 1, 0, 'There can be no disruptions! Mustn\'t keep the master waiting! Mustn\'t fail again!', 14, 0, 100, 0, 0, 19847, 47028, 'VO_BD_Maloriak_Event01'),
(41378, 2, 0, 'Strip the flesh, harvest the organs!', 14, 0, 100, 0, 0, 19849, 47030, 'VO_BD_Maloriak_Event03'),
(41378, 2, 1, 'Nothing goes to waste...', 14, 0, 100, 0, 0, 19848, 47029, 'VO_BD_Maloriak_Event02'),
(41378, 3, 0, 'There will never be another like me...', 14, 0, 100, 0, 0, 19850, 47031, 'VO_BD_Maloriak_Event04'),
(41378, 4, 0, 'Mix and stir, apply heat...', 14, 0, 100, 0, 0, 19851, 41594, 'VO_BD_Maloriak_Event05'),
(41378, 5, 0, 'How well does the mortal shell handle extreme temperature change? Must find out! For science!', 14, 0, 100, 0, 0, 19853, 41595, 'VO_BD_Maloriak_Event07'),
(41378, 6, 0, 'This one\'s a little unstable, but what\'s progress without failure?', 14, 0, 100, 0, 0, 19852, 41596, 'VO_BD_Maloriak_Event06'),
(41378, 7, 0, 'Meet the brawn to my brains! Prime subjects, devour them!', 14, 0, 100, 0, 0, 19859, 47035, 'VO_BD_Maloriak_event12'),
(41378, 7, 1, 'Too early, but no choice... They must be tested! Face now my prime subjects!', 14, 0, 100, 0, 0, 19858, 47034, 'VO_BD_Maloriak_Event11'),
(41378, 7, 2, 'If one goes to the limits, the results become unpredictable.', 14, 0, 100, 0, 0, 19854, 41625, 'VO_BD_Maloriak_Event08'),
(41378, 8, 0, 'Maloriak throws a blue vial into the cauldron!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak blue vial'),
(41378, 9, 0, 'Maloriak throws a red vial into the cauldron!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak red vial'),
(41378, 10, 0, 'Maloriak throws a green vial into the cauldron!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak green vial'),
(41378, 11, 0, 'Maloriak releases Prime Subjects from the cages nearby!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak prime subjects'),
(41378, 12, 0, 'Maloriak releases Aberations from the cages nearby!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak aberations'),
(41378, 13, 0, 'Maloriak summons exploding Ice Spheres!', 41, 0, 100, 0, 0, 0, 0, 'Maloriak absolute zero');

-- Aberrations and Prime Subject release conditions
DELETE FROM `conditions` WHERE `SourceEntry` IN (77569,77991);
INSERT INTO `conditions` VALUES 
(13, 1, 77569, 0, 0, 31, 0, 3, 41440, 0, 0, 0, 0, '', 'Release Aberrations - Target Aberrations'),
(13, 1, 77991, 0, 0, 31, 0, 3, 41440, 0, 0, 0, 0, '', 'Release All - Target Aberrations'),
(13, 2, 77991, 0, 0, 31, 0, 3, 41841, 0, 0, 0, 0, '', 'Release All - Target Prime Subjects');

-- Aberrations and Prime subjects 
UPDATE `creature_template` SET `unit_flags` = 33554818 WHERE `entry` IN (41440,41841);

-- Vial chambers
UPDATE `gameobject_template` SET `faction` = 16, `flags` = 4 WHERE `entry` IN (402205,402206);

-- Cauldron
UPDATE `gameobject_template` SET `faction` = 16 WHERE `entry` IN (401904);
