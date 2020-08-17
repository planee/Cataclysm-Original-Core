-- Charge is dropped by spellmod, no need for this
DELETE FROM `spell_proc_event` WHERE `entry` = 90174;

-- Proc event fix
UPDATE `spell_proc_event` SET `CustomChance`='7' WHERE  `entry`  = 85117;
UPDATE `spell_proc_event` SET `CustomChance`='15' WHERE  `entry` = 86172;

-- Useless hacky scripts
DELETE FROM `spell_script_names` WHERE `spell_id` IN (879,2812,24275,53385,54158,84963,85256);