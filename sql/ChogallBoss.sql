DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=15673 AND `type`=11;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES (15673, 11, 0, 0, 'achievement_abyss_will_gaze_back_into_you');

DELETE FROM `disables` WHERE `sourceType` = 9 AND `entry` IN (43324);
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES 
(9, 43324, 0, '', '', 'Disable Pathfinding for Chogall Boss');