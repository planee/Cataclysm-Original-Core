DROP TABLE IF EXISTS `character_current_artifacts`;
CREATE TABLE `character_current_artifacts` (
	`guid` INT(10) UNSIGNED NOT NULL,
	`branchId` INT(10) NOT NULL DEFAULT '0',
	`entry` INT(10) NOT NULL DEFAULT '0',
	PRIMARY KEY (`guid`, `branchId`, `entry`)
)COLLATE='utf8_general_ci' ENGINE=MyISAM;