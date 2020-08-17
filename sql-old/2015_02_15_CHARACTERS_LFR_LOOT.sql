DROP TABLE IF EXISTS `character_lfr_looted`;

CREATE TABLE `character_lfr_looted` (
  `objectEntry` int(32) unsigned NOT NULL,
  `playerGUID` bigint(20) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
