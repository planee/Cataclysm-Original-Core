DROP TABLE IF EXISTS `character_digsites`;
CREATE TABLE `character_digsites` (
	`guid` INT(10) UNSIGNED NOT NULL,
	`entry` INT(10) NOT NULL DEFAULT '0',
	PRIMARY KEY (`guid`, `entry`)
)COLLATE='utf8_general_ci' ENGINE=MyISAM;