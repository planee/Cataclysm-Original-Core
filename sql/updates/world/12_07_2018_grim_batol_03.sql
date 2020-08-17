/*grim batol*/
UPDATE `creature_template` SET `unit_flags`= `unit_flags` &~ 512 WHERE `entry`= 39294;
UPDATE `creature_template` SET `flags_extra`= 128 WHERE `entry` IN (40040, 40030, 48711);
UPDATE `creature_template` SET `faction_A`= 14, `faction_H`= 14, `unit_flags`= `unit_flags` | 33554432, `InhabitType`= 4, `ScriptName`= 'npc_throngus_twilight_archer' WHERE `entry`= 40197;
UPDATE `creature_template` SET `ScriptName`= 'npc_throngus_cave_in_stalker' WHERE `entry`= 40228;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (91937, 74846, 74636, 74976, 90737, 74944, 90810);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(91937, 'spell_umbriss_bleeding_wound'),
(74846, 'spell_umbriss_bleeding_wound'),
(74636, 'spell_ground_siege_selector'),
(74976, 'spell_throngus_disorienting_roar'),
(90737, 'spell_throngus_disorienting_roar'),
(74944, 'spell_throngus_flaming_arrow'),
(90810, 'spell_throngus_flaming_arrow');

DELETE FROM `conditions` WHERE `SourceEntry` IN (74670, 90250, 74634, 90249) AND `SourceTypeOrReferenceId`= 13;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 74670, 0, 1, 31, 0, 3, 40040, 0, 0, 0, 0, '', 'General Umbriss - Blitz'),
(13, 1, 90250, 0, 1, 31, 0, 3, 40040, 0, 0, 0, 0, '', 'General Umbriss - Blitz'),
(13, 1, 74634, 0, 1, 31, 0, 3, 40030, 0, 0, 0, 0, '', 'General Umbriss - Ground Siege'),
(13, 1, 90249, 0, 1, 31, 0, 3, 40030, 0, 0, 0, 0, '', 'General Umbriss - Ground Siege');