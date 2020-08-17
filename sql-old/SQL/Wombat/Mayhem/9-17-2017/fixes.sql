*https://wowzealot.com/bugtracker/issue/1944/Ashyen-and-Keleth - 

DELETE FROM `creature_text` WHERE `entry` = 17901;
DELETE FROM `creature_text` WHERE `entry` = 17900;

*https://wowzealot.com/bugtracker/issue/1920/Report-for-Tryouts

UPDATE `quest_template` SET `PrevQuestId` = 28349, `NextQuestId` = 24567, `ExclusiveGroup` = 0 WHERE `Id` = 14071;

*https://wowzealot.com/bugtracker/issue/1953/Sharth-Voldoun-NPC---Quest-item-not-dropping

REPLACE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (18554, 29912, 85, 1, 0, 1, 1);

*https://wowzealot.com/bugtracker/issue/1947/Ango%27rosh-Souleater

REPLACE INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES (18121, 0, 3, 0, 11, 0, 100, 0, 0, 0, 0, 0, 11, 35194, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ango\'rosh Souleater - Respawn cast Shadowform');
