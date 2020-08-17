-- Atramedes

-- Atramedes is spawned by instance script
DELETE FROM `creature` WHERE `id` = 41442;

-- Remove bad shield spawn
DELETE FROM `creature` WHERE `guid` BETWEEN 500080 AND 500087;

-- Ancient bell flags
UPDATE `gameobject_template` SET `flags` = 1 WHERE `entry` = 204276;

-- Proper Maloriak event npc id
UPDATE `creature` SET `id` = '43404' WHERE `id` = 49938;

-- Proper script for Maloriak
UPDATE `creature_template` SET `ScriptName` = 'npc_maloriak_atramedes_event' WHERE `entry` = 43404;
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 49938;

-- Respawn time for dwarven spirits
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `id` IN (43119,43126,43127,43130,43128,43125,43129,43122);

-- Disable pathfinding for atramedes
DELETE FROM `disables` WHERE `sourceType`=9 AND `entry`=41442;
INSERT INTO `disables` VALUES (9, 41442, 0, '', '', 'Disable Pathfinding for Atramedes');

-- Fix intro path for atramedes
DELETE FROM `waypoint_data` WHERE `id` = 4144200;
INSERT INTO `waypoint_data` VALUES 
(4144200, 1, 386.094, -314.484, 137.624, 0, 0, 1, 0, 100, 0, 0),
(4144200, 2, 387.687, -251.759, 129.786, 0, 0, 1, 0, 100, 0, 0),
(4144200, 3, 337.967, -225.78, 122.211, 0, 0, 1, 0, 100, 0, 0),
(4144200, 4, 261.698, -224.221, 106.788, 0, 0, 1, 0, 100, 0, 0);

-- Tracking flames + sonar pulse dummy
UPDATE `creature_template` SET `flags_extra` = 128, `minlevel` = 85, `maxlevel` = 85 WHERE `entry` IN (41879,41546,49623);

-- Throne faction 
UPDATE `gameobject_template` SET `faction` = 16 WHERE `entry` IN (202832);

-- Ancient Dwarven shield spellclick
UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` IN (42949);

-- Vertigo aura application
DELETE FROM `conditions` WHERE `SourceEntry`=77611;
INSERT INTO `conditions` VALUES 
(13, 3, 77611, 0, 0, 31, 0, 3, 41442, 0, 0, 0, 0, '', 'Resonating Clash - target Atramedes');

UPDATE `conditions` SET `SourceGroup`='3' WHERE  `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=77611 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=41442 AND `ConditionValue3`=0;

-- Resonating Clash target swap
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=78168 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=41442 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 1, 78168, 0, 0, 31, 0, 3, 41442, 0, 0, 0, 0, '', 'Resonating clash swap target - Hits atramedes');

-- Resonating Clash Sound Clear
UPDATE `conditions` SET `SourceGroup`='6' WHERE  `SourceTypeOrReferenceId`=13 AND `SourceGroup`=2 AND `SourceEntry`=77709 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=4 AND `ConditionValue2`=0 AND `ConditionValue3`=0;

-- Resonating clash clear noisy aura spellscript
DELETE FROM `spell_script_names` WHERE `spell_id` = 77709;
INSERT INTO `spell_script_names` VALUES (77709, 'spell_resonating_clash_aur');

-- Resonating clash vertigo application
DELETE FROM `spell_script_names` WHERE `spell_id`=77611;
INSERT INTO `spell_script_names` VALUES (77611, 'spell_resonating_clash');