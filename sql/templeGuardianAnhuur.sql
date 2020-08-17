-- Temple Guardian Anhuur - Allow Dks to handle the levers.
DELETE FROM `playercreateinfo_spell` WHERE `spell` = 68398 AND class = 6;
INSERT INTO `playercreateinfo_spell` (`race`,`class`,`spell`,`Note`) VALUES
(1,6,68398,'Opening'),
(2,6,68398,'Opening'),
(3,6,68398,'Opening'),
(4,6,68398,'Opening'),
(5,6,68398,'Opening'),
(6,6,68398,'Opening'),
(7,6,68398,'Opening'),
(8,6,68398,'Opening'),
(9,6,68398,'Opening'),
(10,6,68398,'Opening'),
(11,6,68398,'Opening'),
(22,6,68398,'Opening');