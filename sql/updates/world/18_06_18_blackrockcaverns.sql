/*Blackrock Caverns*/
UPDATE `creature_template` SET `ScriptName`= 'npc_raz_the_crazed' WHERE `entry`= 39670;
UPDATE `creature_template` SET `ScriptName`= 'blackrock_caverns_teleport' WHERE `entry`= 51340;
UPDATE `creature_template` SET `ScriptName`= 'npc_twilight_zealot' WHERE `entry`= 50284;
UPDATE `creature_template` SET `ScriptName`= 'npc_corla_netheressence_trigger' WHERE `entry`= 49526;
UPDATE `creature_template` SET `ScriptName`= 'npc_shadow_of_obsidius' WHERE `entry`= 40817;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (80196, 75539, 82189, 75543, 75441);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80196, 'spell_aggro_nearby_target'),
(75539, 'spell_chains_of_woe'),
(82189, 'spell_chains_of_woe_eff'),
(75464, 'spell_chains_of_woe_teleport'),
(75543, 'spell_skullcracker'),
(75441, 'spell_chains_of_woe_caster_aura');

DELETE FROM `areatrigger_scripts` WHERE `entry`= 6014;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6014, 'AreaTrigger_at_second_bridge_blackrock_caverns');