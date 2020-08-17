-- Protection Guardian
UPDATE `creature_template` SET `ScriptName` = 'npc_defense_guardian', `Unit_Flags` = 33554690, `AIName` = '' WHERE  `entry` = 46490;

-- Ancient fury coeff damage is fully calculated in script.
DELETE FROM `spell_bonus_data` WHERE `entry` = 86704;
-- INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
-- (86704, 0, 0, 0.061, 0, 'Ancient guardian - damage');