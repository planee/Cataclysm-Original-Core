-- Glyph of Starsurge
DELETE FROM `spell_proc_event` WHERE `entry` = 62971;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(62971, 0, 7, 0, 0, 0x02000000, 0, 0, 0, 0, 0);