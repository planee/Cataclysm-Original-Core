-- DEVOURING FLAMES
UPDATE `creature_template` SET `flags_extra` = 128, `faction_a` = 14, `faction_h` = 14, `unit_flags` = 33554562, `minlevel` = 85, `maxlevel` = 85 WHERE `entry` = 48798;

-- Valiona & theralion
UPDATE `creature_template` SET `InhabitType` = 7 WHERE `entry` IN (45992,45993);

UPDATE `creature_template` SET `npcflag`='16777216' WHERE  `entry`=46301;

-- Collapsing portal spellclick
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 46301;
INSERT INTO `npc_spellclick_spells` VALUES
(46301,95013,3,0);

-- Conditions
DELETE FROM `conditions` WHERE `SourceEntry` = 95013 AND `ConditionValue2` = 46301;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES ('13', '1', '95013', '0', '31', '3', '46301', '0', '0', '', 'Twilight Portal Exit - Hits nearest portal npc');

DELETE FROM `conditions` WHERE `SourceEntry` = 86408 AND `ConditionValue2` = 46374;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES ('13', '1', '86408', '0', '31', '3', '46374', '0', '0', '', 'Dazzling DEstruction - Hits dazzlign destruction npc');

DELETE FROM `conditions` WHERE `SourceEntry` = 86406 AND `ConditionValue2` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES ('13', '2', '86406', '0', '31', '4', '0', '0', '0', '', 'Dazzling DEstruction Damage - Hits players');

-- SpellScripts
DELETE FROM `spell_script_names` WHERE `spell_id` IN (95013,-86788, 86788, -86825);
INSERT INTO `spell_script_names` VALUES
(95013,'spell_twilight_portal_teleport'),
(-86788, 'spell_valiona_blackout'),
(-86825, 'spell_valiona_blackout_dmg');

-- Thrash/Mismatch data
DELETE FROM `spell_script_names` WHERE  `spell_id`=86788 AND `ScriptName`='spell_blackout_bot';
DELETE FROM `spell_script_names` WHERE  `spell_id`=92876 AND `ScriptName`='spell_blackout_bot';
DELETE FROM `spell_script_names` WHERE  `spell_id`=92877 AND `ScriptName`='spell_blackout_bot';
DELETE FROM `spell_script_names` WHERE  `spell_id`=92878 AND `ScriptName`='spell_blackout_bot';
DELETE FROM `spell_linked_spell` WHERE  `spell_trigger`=-86788 AND `spell_effect`=86825 AND `type`=0;
DELETE FROM `spell_linked_spell` WHERE  `spell_trigger`=-92876 AND `spell_effect`=86825 AND `type`=0;
DELETE FROM `spell_linked_spell` WHERE  `spell_trigger`=-92877 AND `spell_effect`=86825 AND `type`=0;
DELETE FROM `spell_linked_spell` WHERE  `spell_trigger`=-92878 AND `spell_effect`=86825 AND `type`=0;