DELETE FROM `spell_proc_event` WHERE `entry` IN (74976, 90737);
INSERT INTO `spell_proc_event` (`entry`, `CustomChance`) VALUES 
('74976', '100'),
('90737', '100');
UPDATE `spell_proc_event` SET `procFlags`='87312' WHERE `entry` IN (74976,90737);