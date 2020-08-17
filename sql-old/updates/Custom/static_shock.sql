-- Static Shock
DELETE FROM `spell_proc_event` WHERE `entry` IN (51525, 51526, 51527);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(51525, 0, 11, 0, 294912, 135172, 16, 0, 0, 0, 0),
(51526, 0, 11, 0, 294912, 135172, 16, 0, 0, 0, 0),
(51527, 0, 11, 0, 294912, 135172, 16, 0, 0, 0, 0);