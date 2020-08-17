-- Parasitic Infection debuff (periodic damage)
DELETE FROM `spell_script_names` WHERE `spell_id` IN (78941,91913,94678,94679,78020) OR `ScriptName` LIKE '%spell_magmaw_parasitic_infection_aura%';
INSERT INTO `spell_script_names` VALUES 
(78941, 'spell_magmaw_parasitic_infection_aura'),
(91913, 'spell_magmaw_parasitic_infection_aura'),
(94678, 'spell_magmaw_parasitic_infection_aura'),
(94679, 'spell_magmaw_parasitic_infection_aura');

-- Lava parasite (ride vehicle)
DELETE FROM `spell_script_names` WHERE `spell_id `= 78020;
INSERT INTO `spell_script_names` VALUES (78020, 'spell_magmaw_lava_parasite_ride_vehicle');

-- Light Show trigger
UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` IN (47196);

-- Vengeful rage targeting
DELETE FROM `conditions` WHERE `SourceEntry` = 80035;
INSERT INTO `conditions` VALUES 
(13, 7, 80035, 0, 1, 31, 0, 3, 42362, 0, 0, 0, 0, '', 'Vengeful Rage - target dredge'),
(13, 7, 80035, 0, 2, 31, 0, 3, 42649, 0, 0, 0, 0, '', 'Vengeful Rage - Target Drakonid');