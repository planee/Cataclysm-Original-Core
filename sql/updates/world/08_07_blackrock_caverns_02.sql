/*Blackrock Caverns*/
UPDATE `creature_template` SET `unit_flags`= `unit_flags` | 512, `ScriptName`= 'npc_karsh_quicksilver' WHERE `entry` IN (40004, 40005);
UPDATE `creature` SET `spawnMask`= 2 WHERE `id` IN (40008, 40013, 40011);
UPDATE `creature_template` SET `ScriptName`= 'npc_runty' WHERE `entry`= 40015;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (82301);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(82301, 'spell_karsh_burning_heat'),
(82301, 'spell_karsh_burning_heat_aura');