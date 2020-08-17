ALTER TABLE `mail_loot_template`
	CHANGE COLUMN `item` `item` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `entry`;
ALTER TABLE `disenchant_loot_template`
	CHANGE COLUMN `item` `item` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `entry`;
ALTER TABLE `milling_loot_template`
	CHANGE COLUMN `item` `item` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `entry`;
ALTER TABLE `prospecting_loot_template`
	CHANGE COLUMN `item` `item` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `entry`;
ALTER TABLE `spell_loot_template`
	CHANGE COLUMN `item` `item` MEDIUMINT(8) NOT NULL DEFAULT '0' AFTER `entry`;

DELETE FROM `item_loot_template` WHERE `entry` = 44113;
INSERT INTO `item_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (44113, -81, 100, 1, 0, 100, 100);