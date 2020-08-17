/*Grim Batol*/
UPDATE `creature_template` SET `flags_extra`= 128 WHERE `entry`= 40355;
UPDATE `creature_template` SET `flags_extra`= 128, `faction_A`= 14, `faction_H`= 14 WHERE `entry`= 40365;
UPDATE `creature_template` SET `AIName`= '' WHERE `entry`= 40357;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (74984, 75245, 90915, 75809, 75520, 91049, 75763, 91040);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(74984, 'spell_throngus_mightly_stomp'),
(75245, 'spell_drahga_burning_shadowbolt'),
(90915, 'spell_drahga_burning_shadowbolt'),
(75809, 'spell_erudax_shield_of_nightmares'),
(75520, 'spell_erudax_twilight_corruption'),
(91049, 'spell_erudax_twilight_corruption'),
(75520, 'spell_erudax_twilight_corruption_aura'),
(91049, 'spell_erudax_twilight_corruption_aura'),
(75763, 'spell_erudax_umbral_mending'),
(91040, 'spell_erudax_umbral_mending');

DELETE FROM `spell_target_position` WHERE `id`= 75704;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(75704, 670, -641.515, -827.8, 235.5, 3.069);