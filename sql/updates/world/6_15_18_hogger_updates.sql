-- Update Hoggers movement handlded by script 
UPDATE creature set SpawnDist = 0, MovementType = 0 WHERE id = 448;
UPDATE creature_template set MovementType = 0 WHERE entry = 448;
DELETE from creature_text WHERE entry = 448 AND groupid = 6;
INSERT INTO `creature_text` (`entry`, `groupid`, `text`, `type`, `probability`, `comment`) VALUES ('448', '6', 'Grrr...', '12', '100', 'TALK_HOGGER_NOOOO');
