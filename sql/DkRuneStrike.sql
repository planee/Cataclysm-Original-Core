-- Rune Strike parry/dodge aura: learn together with rune strike.
DELETE FROM `spell_learn_spell` WHERE `entry`=56815 AND `SpellID`=56816;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES (56815, 56816, 0);

-- Rune strIke activation on dodge/parry
DELETE FROM `spell_script_names` WHERE `spell_id`=56817 AND `ScriptName`='spell_dk_rune_strike_proc';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (56817, 'spell_dk_rune_strike_proc');

-- Rune Strike Activation Aura: Gets consumed on Rune Strike
DELETE FROM `spell_proc_event` WHERE `entry`=56817;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(56817, 0, 15, 0, 536870912, 0, 0, 0, 0, 0, 0);
