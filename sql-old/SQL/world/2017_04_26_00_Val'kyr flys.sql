-- Val'kyr flying
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (28487, 0, 0, 33554432, 0, 0, NULL);

UPDATE creature_template SET InhabitType = 4 WHERE entry = 28487;