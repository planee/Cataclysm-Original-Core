DELETE FROM `gameobject_template` WHERE `entry` IN (203071, 204282, 206836, 202655, 203078, 207189, 207188, 207187, 207190);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(203071, 3, 10493, 'Night Elf Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 203071, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(204282, 3, 10487, 'Dwarf Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 204282, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(206836, 3, 10490, 'Fossil Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 206836, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(202655, 3, 10494, 'Troll Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 202655, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(203078, 3, 10495, 'Nerubian Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 203078, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(207189, 3, 10488, 'Vrykul Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 207189, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(207188, 3, 10491, 'Draenei Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 207188, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(207187, 3, 10489, 'Orc Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 207187, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329),
(207190, 3, 10492, 'Tol\'vir Archaeology Find', '', '', '', 0, 0, 0.5, 0, 0, 0, 0, 0, 0, 1859, 207190, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13329);

DELETE FROM `gameobject_loot_template` WHERE `entry` IN (203071, 204282, 202655, 203078, 207189, 207188, 207187, 207190);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES 
(204282, 52843, 30, 1, 0, 1, 1),
(203071, 63127, 30, 1, 0, 1, 1),
(202655, 63128, 30, 1, 0, 1, 1),
(203078, 64396, 30, 1, 0, 1, 1),
(207189, 64395, 30, 1, 0, 1, 1),
(207188, 64394, 30, 1, 0, 1, 1),
(207187, 64392, 30, 1, 0, 1, 1),
(207190, 64397, 30, 1, 0, 1, 1);