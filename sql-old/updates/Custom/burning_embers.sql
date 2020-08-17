-- Burning Embers
DELETE FROM `spell_proc_event` WHERE `entry` IN (85112, 91986);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(85112, 4, 5, 4096, 128, 0, 0, 0, 0, 0, 0),
(91986, 4, 5, 4096, 128, 0, 0, 0, 0, 0, 0);