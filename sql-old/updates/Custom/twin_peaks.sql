DELETE FROM `battleground_template` WHERE `id` IN (108);
INSERT INTO `battleground_template` (`id`, `MinPlayersPerTeam`, `MaxPlayersPerTeam`, `MinLvl`, `MaxLvl`, `AllianceStartLoc`, `AllianceStartO`, `HordeStartLoc`, `HordeStartO`, `StartMaxDist`, `Weight`, `ScriptName`, `Comment`) VALUES
(108, 5, 10, 85, 85, 1726, 2.53684, 1727, 6.27533, 55, 1, '', 'Twin Peaks');

DELETE FROM `trinity_string` WHERE `entry` IN (10074, 10075, 10076, 10077, 10078, 10079, 10080, 10081, 10082, 10083, 10084, 10085, 10086, 10087, 10088);
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(10088, 'The Horde flag is now placed at its base.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10087, 'The Alliance flag is now placed at its base.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10086, 'The flags are now placed at their bases.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10085, 'The Alliance Flag was picked up by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10084, 'The Horde flag was picked up by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10083, 'The Horde flag was returned to its base by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10082, 'The Alliance Flag was returned to its base by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10081, 'The Alliance Flag was dropped by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10080, 'The Horde flag was dropped by $n!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10079, '$n captured the Alliance flag!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10078, '$n captured the Horde flag!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10077, 'Let the battle for Twin Peaks begin!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10076, 'The battle for Twin Peaks begins in 30 seconds. Prepare yourselves!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10075, 'The battle for Twin Peaks begins in 1 minute.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(10074, 'The battle for Twin Peaks begins in 2 minutes.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

DELETE FROM `gameobject` WHERE `map` = 726;

DELETE FROM `creature` WHERE `id` IN (13116, 13117) AND `map` = 726;