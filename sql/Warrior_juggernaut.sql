-- Juggernaut, only proc on slam or mortal strike
DELETE FROM `spell_proc_event` WHERE `entry`=65156;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(65156, 0, 4, 35651584, 0, 0, 69648, 0, 0, 0, 0);
