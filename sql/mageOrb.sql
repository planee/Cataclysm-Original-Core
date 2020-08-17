DELETE FROM `spell_script_names` WHERE `spell_id` = (84765,84714);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(84765, 'spell_mage_orb_summon'),
(84714, 'spell_mage_orb_summon');

UPDATE `creature_template` SET `InhabitType` = 5, `Hoverheight` = 8 WHERE `entry` IN (44214,45322);

DELETE FROM `creature_template_addon` WHERE `entry` IN (44214,45322);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`, `VisibilityDistanceType`) VALUES 
(44214, 0, 0, 0, 0, 0, '50331648', -1),
(45322, 0, 0, 0, 0, 0, '50331648', -1);