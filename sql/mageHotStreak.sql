-- Hot streak - Proc on cast
DELETE FROM `spell_proc_event` WHERE `entry` IN (48108,44445,44446,44448);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(48108, 0, 3, 4194304, 0, 8, 0, 0, 0, 0, 0),
(44445, 0, 3, 4194323, 4096, 4096, 0, 2, 0, 0, 0),
(44446, 0, 3, 4194323, 4096, 4096, 0, 0, 0, 0, 0),
(44448, 0, 3, 4194323, 4096, 4096, 0, 0, 0, 0, 0);
