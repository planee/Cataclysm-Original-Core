-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.2.13-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             9.5.0.5196
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table 0000cataclysmwowauth.account
CREATE TABLE IF NOT EXISTS `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `username` varchar(32) NOT NULL DEFAULT '',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '',
  `sessionkey` varchar(80) NOT NULL DEFAULT '',
  `v` varchar(64) NOT NULL DEFAULT '',
  `s` varchar(64) NOT NULL DEFAULT '',
  `email` varchar(254) NOT NULL DEFAULT '',
  `joindate` timestamp NOT NULL DEFAULT current_timestamp(),
  `last_ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `failed_logins` int(10) unsigned NOT NULL DEFAULT 0,
  `locked` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `last_login` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `online` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `expansion` tinyint(3) unsigned NOT NULL DEFAULT 3,
  `mutetime` bigint(20) NOT NULL DEFAULT 0,
  `mutereason` varchar(255) NOT NULL DEFAULT '',
  `muteby` varchar(50) NOT NULL DEFAULT '',
  `locale` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `os` varchar(3) NOT NULL DEFAULT '',
  `recruiter` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Account System';

-- Dumping data for table 0000cataclysmwowauth.account: ~0 rows (approximately)
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
/*!40000 ALTER TABLE `account` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.accounts_to_delete
CREATE TABLE IF NOT EXISTS `accounts_to_delete` (
  `id` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Account System';

-- Dumping data for table 0000cataclysmwowauth.accounts_to_delete: 0 rows
/*!40000 ALTER TABLE `accounts_to_delete` DISABLE KEYS */;
/*!40000 ALTER TABLE `accounts_to_delete` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_access
CREATE TABLE IF NOT EXISTS `account_access` (
  `id` int(10) unsigned NOT NULL,
  `gmlevel` tinyint(3) unsigned NOT NULL,
  `RealmID` int(11) NOT NULL DEFAULT -1,
  `Comment` text DEFAULT NULL,
  PRIMARY KEY (`id`,`RealmID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.account_access: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_access` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_access` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_banned
CREATE TABLE IF NOT EXISTS `account_banned` (
  `id` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Account id',
  `bandate` int(10) unsigned NOT NULL DEFAULT 0,
  `unbandate` int(10) unsigned NOT NULL DEFAULT 0,
  `bannedby` varchar(50) DEFAULT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Ban List';

-- Dumping data for table 0000cataclysmwowauth.account_banned: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_banned` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_banned` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_ip_history
CREATE TABLE IF NOT EXISTS `account_ip_history` (
  `id` int(10) DEFAULT NULL,
  `RealmID` int(10) DEFAULT NULL,
  `IP` varchar(15) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 0000cataclysmwowauth.account_ip_history: ~0 rows (approximately)
/*!40000 ALTER TABLE `account_ip_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_ip_history` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_premium
CREATE TABLE IF NOT EXISTS `account_premium` (
  `id` int(11) NOT NULL DEFAULT 0 COMMENT 'Account id',
  `setdate` bigint(40) NOT NULL DEFAULT 0,
  `unsetdate` bigint(40) NOT NULL DEFAULT 0,
  `premium_type` tinyint(4) unsigned NOT NULL DEFAULT 1,
  `active` tinyint(4) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`,`setdate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Premium Accounts';

-- Dumping data for table 0000cataclysmwowauth.account_premium: 0 rows
/*!40000 ALTER TABLE `account_premium` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_premium` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_punishment
CREATE TABLE IF NOT EXISTS `account_punishment` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `account` int(11) unsigned NOT NULL,
  `by` int(11) NOT NULL,
  `penalty_points` int(11) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL,
  `time` bigint(20) unsigned NOT NULL,
  `active` tinyint(3) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.account_punishment: 0 rows
/*!40000 ALTER TABLE `account_punishment` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_punishment` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_tempban
CREATE TABLE IF NOT EXISTS `account_tempban` (
  `accountId` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  PRIMARY KEY (`accountId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.account_tempban: 0 rows
/*!40000 ALTER TABLE `account_tempban` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_tempban` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.account_warning
CREATE TABLE IF NOT EXISTS `account_warning` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `accountId` int(11) unsigned NOT NULL,
  `by_account` int(11) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL,
  `time` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.account_warning: 0 rows
/*!40000 ALTER TABLE `account_warning` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_warning` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.autobroadcast
CREATE TABLE IF NOT EXISTS `autobroadcast` (
  `id` int(10) DEFAULT NULL,
  `weight` int(10) DEFAULT NULL,
  `text` longtext DEFAULT NULL,
  `text_1` longtext DEFAULT NULL,
  `text_2` longtext DEFAULT NULL,
  `text_3` longtext DEFAULT NULL,
  `text_4` longtext DEFAULT NULL,
  `text_5` longtext DEFAULT NULL,
  `text_6` longtext DEFAULT NULL,
  `text_7` longtext DEFAULT NULL,
  `RealmID` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table 0000cataclysmwowauth.autobroadcast: ~0 rows (approximately)
/*!40000 ALTER TABLE `autobroadcast` DISABLE KEYS */;
/*!40000 ALTER TABLE `autobroadcast` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.bug_tickets
CREATE TABLE IF NOT EXISTS `bug_tickets` (
  `ticketId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `realm` int(10) unsigned NOT NULL DEFAULT 0,
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier of ticket creator',
  `message` text NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT 0,
  `pool` varchar(32) NOT NULL,
  `mapId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `closedBy` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`ticketId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Support System';

-- Dumping data for table 0000cataclysmwowauth.bug_tickets: 0 rows
/*!40000 ALTER TABLE `bug_tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `bug_tickets` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.ip2nation
CREATE TABLE IF NOT EXISTS `ip2nation` (
  `ip` int(11) unsigned NOT NULL DEFAULT 0,
  `country` varchar(2) NOT NULL DEFAULT '',
  KEY `ip` (`ip`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.ip2nation: 0 rows
/*!40000 ALTER TABLE `ip2nation` DISABLE KEYS */;
/*!40000 ALTER TABLE `ip2nation` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.ip2nationcountries
CREATE TABLE IF NOT EXISTS `ip2nationcountries` (
  `code` varchar(2) NOT NULL DEFAULT '',
  `iso_code_2` varchar(2) NOT NULL DEFAULT '',
  `iso_code_3` varchar(3) DEFAULT '',
  `iso_country` varchar(255) NOT NULL DEFAULT '',
  `country` varchar(255) NOT NULL DEFAULT '',
  `lat` float NOT NULL DEFAULT 0,
  `lon` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`code`),
  KEY `code` (`code`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.ip2nationcountries: 0 rows
/*!40000 ALTER TABLE `ip2nationcountries` DISABLE KEYS */;
/*!40000 ALTER TABLE `ip2nationcountries` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.ip_banned
CREATE TABLE IF NOT EXISTS `ip_banned` (
  `ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `bandate` int(10) unsigned NOT NULL,
  `unbandate` int(10) unsigned NOT NULL,
  `bannedby` varchar(50) NOT NULL DEFAULT '[Console]',
  `banreason` varchar(255) NOT NULL DEFAULT 'no reason',
  PRIMARY KEY (`ip`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Banned IPs';

-- Dumping data for table 0000cataclysmwowauth.ip_banned: ~0 rows (approximately)
/*!40000 ALTER TABLE `ip_banned` DISABLE KEYS */;
/*!40000 ALTER TABLE `ip_banned` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.logs
CREATE TABLE IF NOT EXISTS `logs` (
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  `realm` int(10) unsigned NOT NULL DEFAULT 0,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `string` text CHARACTER SET latin1 DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table 0000cataclysmwowauth.logs: ~0 rows (approximately)
/*!40000 ALTER TABLE `logs` DISABLE KEYS */;
/*!40000 ALTER TABLE `logs` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.realmcharacters
CREATE TABLE IF NOT EXISTS `realmcharacters` (
  `realmid` int(10) unsigned NOT NULL DEFAULT 0,
  `acctid` int(10) unsigned NOT NULL,
  `numchars` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`realmid`,`acctid`),
  KEY `acctid` (`acctid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Realm Character Tracker';

-- Dumping data for table 0000cataclysmwowauth.realmcharacters: ~0 rows (approximately)
/*!40000 ALTER TABLE `realmcharacters` DISABLE KEYS */;
/*!40000 ALTER TABLE `realmcharacters` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.realmlist
CREATE TABLE IF NOT EXISTS `realmlist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL DEFAULT '',
  `address` varchar(255) NOT NULL DEFAULT '127.0.0.1',
  `localAddress` varchar(255) NOT NULL DEFAULT '127.0.0.1',
  `localSubnetMask` varchar(255) NOT NULL DEFAULT '255.255.255.0',
  `port` smallint(5) unsigned NOT NULL DEFAULT 8085,
  `icon` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `flag` tinyint(3) unsigned NOT NULL DEFAULT 2,
  `timezone` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `allowedSecurityLevel` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `population` float unsigned NOT NULL DEFAULT 0,
  `gamebuild` int(10) unsigned NOT NULL DEFAULT 15595,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Realm System';

-- Dumping data for table 0000cataclysmwowauth.realmlist: ~1 rows (approximately)
/*!40000 ALTER TABLE `realmlist` DISABLE KEYS */;
INSERT INTO `realmlist` (`id`, `name`, `address`, `localAddress`, `localSubnetMask`, `port`, `icon`, `flag`, `timezone`, `allowedSecurityLevel`, `population`, `gamebuild`) VALUES
	(1, 'Damieninabox', '127.0.0.1', '127.0.0.1', '255.255.255.0', 8085, 0, 0, 1, 0, 0, 15595);
/*!40000 ALTER TABLE `realmlist` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.uptime
CREATE TABLE IF NOT EXISTS `uptime` (
  `realmid` int(10) unsigned NOT NULL,
  `starttime` int(10) unsigned NOT NULL DEFAULT 0,
  `uptime` int(10) unsigned NOT NULL DEFAULT 0,
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT 0,
  `revision` varchar(255) NOT NULL DEFAULT 'Trinitycore',
  PRIMARY KEY (`realmid`,`starttime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Uptime system';

-- Dumping data for table 0000cataclysmwowauth.uptime: ~0 rows (approximately)
/*!40000 ALTER TABLE `uptime` DISABLE KEYS */;
/*!40000 ALTER TABLE `uptime` ENABLE KEYS */;

-- Dumping structure for table 0000cataclysmwowauth.vip_accounts
CREATE TABLE IF NOT EXISTS `vip_accounts` (
  `id` int(11) NOT NULL DEFAULT 0 COMMENT 'Account id',
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `setdate` bigint(40) NOT NULL DEFAULT 0,
  `unsetdate` bigint(40) NOT NULL DEFAULT 0,
  `active` tinyint(4) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`,`setdate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='VIP Accounts';

-- Dumping data for table 0000cataclysmwowauth.vip_accounts: 0 rows
/*!40000 ALTER TABLE `vip_accounts` DISABLE KEYS */;
/*!40000 ALTER TABLE `vip_accounts` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
