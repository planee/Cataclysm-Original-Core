/*Vortex Pinnacle issue*/
UPDATE `creature_template` SET `ScriptName`= 'npc_altairus_twister' WHERE `entry`= 47342;
UPDATE `creature_template` SET `InhabitType`= 4 WHERE `entry`= 47342;
UPDATE `creature` SET `position_z`= 746.96 WHERE `guid`= 170572;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (86340, 93994);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(86340, 'spell_summon_lurker_tempest'),
(93994, 'spell_asaad_supremacy_of_the_storm');