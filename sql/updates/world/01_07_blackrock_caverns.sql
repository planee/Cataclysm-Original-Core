UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'npc_karsh_bound_flames' WHERE `entry`= 50417;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (93453, 75851, 76002, 75846, 93567);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(93453, 'spell_skullcracker'),
(75851, 'spell_karsh_heat_wave'),
(76002, 'spell_karsh_heat_wave'),
(75846, 'spell_superheated_quicksilver_armor'),
(93567, 'spell_superheated_quicksilver_armor');