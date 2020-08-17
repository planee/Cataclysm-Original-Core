-- Hammer of Wrath
UPDATE `spell_bonus_data` SET `direct_bonus`='0.15', `ap_bonus`='0.5' WHERE  `entry` = 24275;

DELETE FROM  `spell_script_names` WHERE `spell_id` = 24275;
INSERT INTO  `spell_script_names` VALUES
(24275, 'spell_pal_hammer_of_wrath');