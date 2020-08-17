/*Grim Batol*/
DELETE FROM `spell_script_names` WHERE `spell_id`= 90972;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(90972, 'spell_drahga_supernova');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`= 16001;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(16001, 11, 0, 0, 'achievement_break_the_eggs');
