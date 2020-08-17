/*Bastion of Twilight*/
UPDATE `creature_template` SET `faction_A`= 16, `faction_H`= 16 WHERE `entry` IN (44600, 46209, 46210, 46211);
UPDATE `creature_template` SET `flags_extra`= 128 WHERE `entry`= 41264;

DELETE FROM `spell_script_names` WHERE `spell_id`= 82630;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(82630, 'spell_chogall_consume_blood_of_old_god');