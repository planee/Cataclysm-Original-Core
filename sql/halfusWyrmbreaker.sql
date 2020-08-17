-- Drakes gossip menus
UPDATE `creature_template` SET `gossip_menu_id` = 12076 WHERE `entry` IN (44645,44650,44652,44797);
DELETE FROM `gossip_menu_option` WHERE `menu_id`=12076 AND `option_id`=1;
INSERT INTO `gossip_menu_option` VALUES (12076, 1, 0, 'Release the drake.', 1, 1, 0, 0, 0, 0, '', 0, 0);

-- Conditions
DELETE FROM `conditions` WHERE `SourceEntry` IN (83609,83603,83611,84092,83601);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ElseGroup`, `Comment`) VALUES 
('13', '1', '83609', '31', '3', '44687', '0', 'Atrophic Poison - Hits Proto Behemoth'),
('13', '1', '83603', '31', '3', '44600', '0', 'Stone Grip - Hits Halfus Wyrmbreaker'),
('13', '7', '83611', '31', '3', '44600', '0', 'Nether Blindness - Hits Halfus Wyrmbreaker'),
('13', '1', '84092', '31', '3', '44600', '0', 'Cyclone Winds - Hits Halfus Wyrmbreaker'),
('13', '1', '83601', '31', '3', '44687', '0', 'Time Dilation - Hits Proto Behemoth');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=15094 AND `type`=11;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(15094, 11, 0, 0, 'achievement_the_only_escape');