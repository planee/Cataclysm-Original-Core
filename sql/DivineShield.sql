-- Divine Shield THreat removal
DELETE FROM `spell_script_names` WHERE `spell_id` IN (642);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
('642', 'spell_pal_divine_shield');