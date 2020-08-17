-- paladin t13 4p proc - Proc only with zealotry.
DELETE FROM `spell_proc_event` WHERE `entry`=105820;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(105820, 2, 10, 0, 0, 2097152, 0, 0, 0, 0, 0);
