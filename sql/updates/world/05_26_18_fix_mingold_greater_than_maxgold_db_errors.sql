
SELECT entry, name, mingold, maxgold from creature_template where entry in(36805, 36829, 36855, 37028, 37029, 37031, 37662);


UPDATE `creature_template` SET `mingold`='7617', `maxgold`='7947' WHERE  `entry`=36805;
UPDATE `creature_template` SET `mingold`='22960', `maxgold`='23866' WHERE  `entry`=36829;
UPDATE `creature_template` SET `mingold`='1093073', `maxgold`='1176587' WHERE  `entry`=36855;
UPDATE `creature_template` SET `mingold`='7536', `maxgold`='8152' WHERE  `entry`=37028;
UPDATE `creature_template` SET `mingold`='7665', `maxgold`='8391' WHERE  `entry`=37029;
UPDATE `creature_template` SET `mingold`='7510', `maxgold`='8578' WHERE  `entry`=37031;
UPDATE `creature_template` SET `mingold`='7599', `maxgold`='7777' WHERE  `entry`=37662;
