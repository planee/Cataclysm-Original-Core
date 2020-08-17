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

-- Dumping structure for table 0000cataclysmwowcharacters.account_data
CREATE TABLE IF NOT EXISTS `account_data` (
  `accountId` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  `data` blob NOT NULL,
  PRIMARY KEY (`accountId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.account_instance_times
CREATE TABLE IF NOT EXISTS `account_instance_times` (
  `accountId` int(10) unsigned NOT NULL,
  `instanceId` int(10) unsigned NOT NULL DEFAULT 0,
  `releaseTime` bigint(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`accountId`,`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.account_tutorial
CREATE TABLE IF NOT EXISTS `account_tutorial` (
  `accountId` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `tut0` int(10) unsigned NOT NULL DEFAULT 0,
  `tut1` int(10) unsigned NOT NULL DEFAULT 0,
  `tut2` int(10) unsigned NOT NULL DEFAULT 0,
  `tut3` int(10) unsigned NOT NULL DEFAULT 0,
  `tut4` int(10) unsigned NOT NULL DEFAULT 0,
  `tut5` int(10) unsigned NOT NULL DEFAULT 0,
  `tut6` int(10) unsigned NOT NULL DEFAULT 0,
  `tut7` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`accountId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.addons
CREATE TABLE IF NOT EXISTS `addons` (
  `name` varchar(120) NOT NULL DEFAULT '',
  `crc` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Addons';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.arena_team
CREATE TABLE IF NOT EXISTS `arena_team` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT 0,
  `name` varchar(24) NOT NULL,
  `captainGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `rating` smallint(5) unsigned NOT NULL DEFAULT 0,
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT 0,
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT 0,
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT 0,
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT 0,
  `rank` int(10) unsigned NOT NULL DEFAULT 0,
  `backgroundColor` int(10) unsigned NOT NULL DEFAULT 0,
  `emblemColor` int(10) unsigned NOT NULL DEFAULT 0,
  `borderColor` int(10) unsigned NOT NULL DEFAULT 0,
  `emblemStyle` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `borderStyle` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `isSoloQueueTeam` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`arenaTeamId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.arena_team_member
CREATE TABLE IF NOT EXISTS `arena_team_member` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT 0,
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT 0,
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT 0,
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT 0,
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT 0,
  `personalRating` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`arenaTeamId`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.auctionhouse
CREATE TABLE IF NOT EXISTS `auctionhouse` (
  `id` int(10) unsigned NOT NULL DEFAULT 0,
  `auctioneerguid` int(10) unsigned NOT NULL DEFAULT 0,
  `itemguid` int(10) unsigned NOT NULL DEFAULT 0,
  `itemowner` int(10) unsigned NOT NULL DEFAULT 0,
  `buyoutprice` int(10) unsigned NOT NULL DEFAULT 0,
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  `buyguid` int(10) unsigned NOT NULL DEFAULT 0,
  `lastbid` int(10) unsigned NOT NULL DEFAULT 0,
  `startbid` int(10) unsigned NOT NULL DEFAULT 0,
  `deposit` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `item_guid` (`itemguid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.auctionhousebot
CREATE TABLE IF NOT EXISTS `auctionhousebot` (
  `auctionhouse` int(11) NOT NULL DEFAULT 0 COMMENT 'mapID of the auctionhouse.',
  `name` char(25) DEFAULT NULL COMMENT 'Text name of the auctionhouse.',
  `minitems` int(11) DEFAULT 0 COMMENT 'This is the minimum number of items you want to keep in the auction house. a 0 here will make it the same as the maximum.',
  `maxitems` int(11) DEFAULT 0 COMMENT 'This is the number of items you want to keep in the auction house.',
  `percentgreytradegoods` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the Grey Trade Goods auction items',
  `percentwhitetradegoods` int(11) DEFAULT 27 COMMENT 'Sets the percentage of the White Trade Goods auction items',
  `percentgreentradegoods` int(11) DEFAULT 12 COMMENT 'Sets the percentage of the Green Trade Goods auction items',
  `percentbluetradegoods` int(11) DEFAULT 10 COMMENT 'Sets the percentage of the Blue Trade Goods auction items',
  `percentpurpletradegoods` int(11) DEFAULT 1 COMMENT 'Sets the percentage of the Purple Trade Goods auction items',
  `percentorangetradegoods` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the Orange Trade Goods auction items',
  `percentyellowtradegoods` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the Yellow Trade Goods auction items',
  `percentgreyitems` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the non trade Grey auction items',
  `percentwhiteitems` int(11) DEFAULT 10 COMMENT 'Sets the percentage of the non trade White auction items',
  `percentgreenitems` int(11) DEFAULT 30 COMMENT 'Sets the percentage of the non trade Green auction items',
  `percentblueitems` int(11) DEFAULT 8 COMMENT 'Sets the percentage of the non trade Blue auction items',
  `percentpurpleitems` int(11) DEFAULT 2 COMMENT 'Sets the percentage of the non trade Purple auction items',
  `percentorangeitems` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the non trade Orange auction items',
  `percentyellowitems` int(11) DEFAULT 0 COMMENT 'Sets the percentage of the non trade Yellow auction items',
  `minpricegrey` int(11) DEFAULT 100 COMMENT 'Minimum price of Grey items (percentage).',
  `maxpricegrey` int(11) DEFAULT 150 COMMENT 'Maximum price of Grey items (percentage).',
  `minpricewhite` int(11) DEFAULT 150 COMMENT 'Minimum price of White items (percentage).',
  `maxpricewhite` int(11) DEFAULT 250 COMMENT 'Maximum price of White items (percentage).',
  `minpricegreen` int(11) DEFAULT 800 COMMENT 'Minimum price of Green items (percentage).',
  `maxpricegreen` int(11) DEFAULT 1400 COMMENT 'Maximum price of Green items (percentage).',
  `minpriceblue` int(11) DEFAULT 1250 COMMENT 'Minimum price of Blue items (percentage).',
  `maxpriceblue` int(11) DEFAULT 1750 COMMENT 'Maximum price of Blue items (percentage).',
  `minpricepurple` int(11) DEFAULT 2250 COMMENT 'Minimum price of Purple items (percentage).',
  `maxpricepurple` int(11) DEFAULT 4550 COMMENT 'Maximum price of Purple items (percentage).',
  `minpriceorange` int(11) DEFAULT 3250 COMMENT 'Minimum price of Orange items (percentage).',
  `maxpriceorange` int(11) DEFAULT 5550 COMMENT 'Maximum price of Orange items (percentage).',
  `minpriceyellow` int(11) DEFAULT 5250 COMMENT 'Minimum price of Yellow items (percentage).',
  `maxpriceyellow` int(11) DEFAULT 6550 COMMENT 'Maximum price of Yellow items (percentage).',
  `minbidpricegrey` int(11) DEFAULT 70 COMMENT 'Starting bid price of Grey items as a percentage of the randomly chosen buyout price. Default: 70',
  `maxbidpricegrey` int(11) DEFAULT 100 COMMENT 'Starting bid price of Grey items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpricewhite` int(11) DEFAULT 70 COMMENT 'Starting bid price of White items as a percentage of the randomly chosen buyout price. Default: 70',
  `maxbidpricewhite` int(11) DEFAULT 100 COMMENT 'Starting bid price of White items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpricegreen` int(11) DEFAULT 80 COMMENT 'Starting bid price of Green items as a percentage of the randomly chosen buyout price. Default: 80',
  `maxbidpricegreen` int(11) DEFAULT 100 COMMENT 'Starting bid price of Green items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpriceblue` int(11) DEFAULT 75 COMMENT 'Starting bid price of Blue items as a percentage of the randomly chosen buyout price. Default: 75',
  `maxbidpriceblue` int(11) DEFAULT 100 COMMENT 'Starting bid price of Blue items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpricepurple` int(11) DEFAULT 80 COMMENT 'Starting bid price of Purple items as a percentage of the randomly chosen buyout price. Default: 80',
  `maxbidpricepurple` int(11) DEFAULT 100 COMMENT 'Starting bid price of Purple items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpriceorange` int(11) DEFAULT 80 COMMENT 'Starting bid price of Orange items as a percentage of the randomly chosen buyout price. Default: 80',
  `maxbidpriceorange` int(11) DEFAULT 100 COMMENT 'Starting bid price of Orange items as a percentage of the randomly chosen buyout price. Default: 100',
  `minbidpriceyellow` int(11) DEFAULT 80 COMMENT 'Starting bid price of Yellow items as a percentage of the randomly chosen buyout price. Default: 80',
  `maxbidpriceyellow` int(11) DEFAULT 100 COMMENT 'Starting bid price of Yellow items as a percentage of the randomly chosen buyout price. Default: 100',
  `maxstackgrey` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackwhite` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackgreen` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackblue` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackpurple` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackorange` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `maxstackyellow` int(11) DEFAULT 0 COMMENT 'Stack size limits for item qualities - a value of 0 will disable a maximum stack size for that quality, which will allow the bot to create items in stack as large as the item allows.',
  `buyerpricegrey` int(11) DEFAULT 1 COMMENT 'Multiplier to vendorprice when buying grey items from auctionhouse',
  `buyerpricewhite` int(11) DEFAULT 3 COMMENT 'Multiplier to vendorprice when buying white items from auctionhouse',
  `buyerpricegreen` int(11) DEFAULT 5 COMMENT 'Multiplier to vendorprice when buying green items from auctionhouse',
  `buyerpriceblue` int(11) DEFAULT 12 COMMENT 'Multiplier to vendorprice when buying blue items from auctionhouse',
  `buyerpricepurple` int(11) DEFAULT 15 COMMENT 'Multiplier to vendorprice when buying purple items from auctionhouse',
  `buyerpriceorange` int(11) DEFAULT 20 COMMENT 'Multiplier to vendorprice when buying orange items from auctionhouse',
  `buyerpriceyellow` int(11) DEFAULT 22 COMMENT 'Multiplier to vendorprice when buying yellow items from auctionhouse',
  `buyerbiddinginterval` int(11) DEFAULT 1 COMMENT 'Interval how frequently AHB bids on each AH. Time in minutes',
  `buyerbidsperinterval` int(11) DEFAULT 1 COMMENT 'number of bids to put in per bidding interval',
  PRIMARY KEY (`auctionhouse`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.bugreport
CREATE TABLE IF NOT EXISTS `bugreport` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `type` longtext NOT NULL,
  `content` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Debug System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.calendar_events
CREATE TABLE IF NOT EXISTS `calendar_events` (
  `id` bigint(20) unsigned NOT NULL DEFAULT 0,
  `creator` int(10) unsigned NOT NULL DEFAULT 0,
  `title` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(255) NOT NULL DEFAULT '',
  `type` tinyint(1) unsigned NOT NULL DEFAULT 4,
  `dungeon` int(10) NOT NULL DEFAULT -1,
  `eventtime` int(10) unsigned NOT NULL DEFAULT 0,
  `flags` int(10) unsigned NOT NULL DEFAULT 0,
  `time2` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.calendar_invites
CREATE TABLE IF NOT EXISTS `calendar_invites` (
  `id` bigint(20) unsigned NOT NULL DEFAULT 0,
  `event` bigint(20) unsigned NOT NULL DEFAULT 0,
  `invitee` int(10) unsigned NOT NULL DEFAULT 0,
  `sender` int(10) unsigned NOT NULL DEFAULT 0,
  `status` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `statustime` int(10) unsigned NOT NULL DEFAULT 0,
  `rank` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `text` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.channels
CREATE TABLE IF NOT EXISTS `channels` (
  `name` varchar(128) NOT NULL,
  `team` int(10) unsigned NOT NULL,
  `announce` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `ownership` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `password` varchar(32) DEFAULT NULL,
  `bannedList` text DEFAULT NULL,
  `lastUsed` int(10) unsigned NOT NULL,
  PRIMARY KEY (`name`,`team`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Channel System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.characters
CREATE TABLE IF NOT EXISTS `characters` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `account` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `name` varchar(12) NOT NULL DEFAULT '0',
  `race` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `class` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `gender` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `xp` int(10) unsigned NOT NULL DEFAULT 0,
  `money` bigint(20) unsigned NOT NULL DEFAULT 0,
  `playerBytes` int(10) unsigned NOT NULL DEFAULT 0,
  `playerBytes2` int(10) unsigned NOT NULL DEFAULT 0,
  `playerFlags` int(10) unsigned NOT NULL DEFAULT 0,
  `position_x` float NOT NULL DEFAULT 0,
  `position_y` float NOT NULL DEFAULT 0,
  `position_z` float NOT NULL DEFAULT 0,
  `map` smallint(5) unsigned NOT NULL DEFAULT 0,
  `instance_id` int(10) unsigned NOT NULL DEFAULT 0,
  `instance_mode_mask` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `orientation` float NOT NULL DEFAULT 0,
  `taximask` text NOT NULL DEFAULT '',
  `online` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `cinematic` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `totaltime` int(10) unsigned NOT NULL DEFAULT 0,
  `leveltime` int(10) unsigned NOT NULL DEFAULT 0,
  `logout_time` int(10) unsigned NOT NULL DEFAULT 0,
  `is_logout_resting` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `rest_bonus` float NOT NULL DEFAULT 0,
  `resettalents_cost` int(10) unsigned NOT NULL DEFAULT 0,
  `resettalents_time` int(10) unsigned NOT NULL DEFAULT 0,
  `trans_x` float NOT NULL DEFAULT 0,
  `trans_y` float NOT NULL DEFAULT 0,
  `trans_z` float NOT NULL DEFAULT 0,
  `trans_o` float NOT NULL DEFAULT 0,
  `transguid` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `extra_flags` smallint(5) unsigned NOT NULL DEFAULT 0,
  `stable_slots` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `at_login` smallint(5) unsigned NOT NULL DEFAULT 0,
  `zone` smallint(5) unsigned NOT NULL DEFAULT 0,
  `death_expire_time` int(10) unsigned NOT NULL DEFAULT 0,
  `taxi_path` text DEFAULT '',
  `totalKills` int(10) unsigned NOT NULL DEFAULT 0,
  `todayKills` smallint(5) unsigned NOT NULL DEFAULT 0,
  `yesterdayKills` smallint(5) unsigned NOT NULL DEFAULT 0,
  `chosenTitle` int(10) unsigned NOT NULL DEFAULT 0,
  `watchedFaction` int(10) unsigned NOT NULL DEFAULT 0,
  `drunk` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `health` int(10) unsigned NOT NULL DEFAULT 0,
  `power1` int(10) unsigned NOT NULL DEFAULT 0,
  `power2` int(10) unsigned NOT NULL DEFAULT 0,
  `power3` int(10) unsigned NOT NULL DEFAULT 0,
  `power4` int(10) unsigned NOT NULL DEFAULT 0,
  `power5` int(10) unsigned NOT NULL DEFAULT 0,
  `latency` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `speccount` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `activespec` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `exploredZones` longtext DEFAULT NULL,
  `equipmentCache` longtext DEFAULT NULL,
  `knownTitles` longtext DEFAULT NULL,
  `actionBars` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `deleteInfos_Account` int(10) unsigned DEFAULT NULL,
  `deleteInfos_Name` varchar(12) DEFAULT NULL,
  `deleteDate` int(10) unsigned DEFAULT NULL,
  `achievementPoint` int(10) NOT NULL DEFAULT 0,
  `slot` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `grantableLevels` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `currentPetSlot` smallint(2) NOT NULL DEFAULT 0,
  `talentTree` varchar(10) NOT NULL DEFAULT '0 0',
  `guildId` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`) USING BTREE,
  KEY `idx_online` (`online`) USING BTREE,
  KEY `idx_name` (`name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_account_data
CREATE TABLE IF NOT EXISTS `character_account_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  `data` blob NOT NULL,
  PRIMARY KEY (`guid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_achievement
CREATE TABLE IF NOT EXISTS `character_achievement` (
  `guid` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_achievement_progress
CREATE TABLE IF NOT EXISTS `character_achievement_progress` (
  `guid` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` bigint(20) unsigned NOT NULL DEFAULT 0,
  `date` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_action
CREATE TABLE IF NOT EXISTS `character_action` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `button` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `action` int(10) unsigned NOT NULL DEFAULT 0,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spec`,`button`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_arena_stats
CREATE TABLE IF NOT EXISTS `character_arena_stats` (
  `guid` int(10) NOT NULL,
  `slot` tinyint(3) NOT NULL,
  `matchMakerRating` smallint(5) NOT NULL,
  PRIMARY KEY (`guid`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_aura
CREATE TABLE IF NOT EXISTS `character_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT 0 COMMENT 'Full Global Unique Identifier',
  `item_guid` bigint(20) unsigned NOT NULL DEFAULT 0,
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `effect_mask` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `recalculate_mask` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `amount0` int(11) NOT NULL DEFAULT 0,
  `amount1` int(11) NOT NULL DEFAULT 0,
  `amount2` int(11) NOT NULL DEFAULT 0,
  `base_amount0` int(11) NOT NULL DEFAULT 0,
  `base_amount1` int(11) NOT NULL DEFAULT 0,
  `base_amount2` int(11) NOT NULL DEFAULT 0,
  `maxduration` int(11) NOT NULL DEFAULT 0,
  `remaintime` int(11) NOT NULL DEFAULT 0,
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_banned
CREATE TABLE IF NOT EXISTS `character_banned` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `bandate` int(10) unsigned NOT NULL DEFAULT 0,
  `unbandate` int(10) unsigned NOT NULL DEFAULT 0,
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`guid`,`bandate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Ban List';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_battleground_data
CREATE TABLE IF NOT EXISTS `character_battleground_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `instanceId` int(10) unsigned NOT NULL COMMENT 'Instance Identifier',
  `team` smallint(5) unsigned NOT NULL,
  `joinX` float NOT NULL DEFAULT 0,
  `joinY` float NOT NULL DEFAULT 0,
  `joinZ` float NOT NULL DEFAULT 0,
  `joinO` float NOT NULL DEFAULT 0,
  `joinMapId` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `taxiStart` int(10) unsigned NOT NULL DEFAULT 0,
  `taxiEnd` int(10) unsigned NOT NULL DEFAULT 0,
  `mountSpell` mediumint(8) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_battleground_random
CREATE TABLE IF NOT EXISTS `character_battleground_random` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_battleground_stats
CREATE TABLE IF NOT EXISTS `character_battleground_stats` (
  `guid` int(10) NOT NULL,
  `win` int(10) NOT NULL,
  `played` int(10) NOT NULL,
  `rating` int(10) NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_bug_submission
CREATE TABLE IF NOT EXISTS `character_bug_submission` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `content` varchar(500) DEFAULT NULL,
  `playerId` bigint(20) DEFAULT NULL,
  `position_x` float DEFAULT NULL,
  `position_y` float DEFAULT NULL,
  `position_z` float DEFAULT NULL,
  `orientation` float DEFAULT NULL,
  `mapId` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_cuf_profiles
CREATE TABLE IF NOT EXISTS `character_cuf_profiles` (
  `guid` int(10) unsigned NOT NULL COMMENT 'Character Guid',
  `id` tinyint(3) unsigned NOT NULL COMMENT 'Profile Id (0-4)',
  `name` varchar(12) NOT NULL COMMENT 'Profile Name',
  `frameHeight` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Frame Height',
  `frameWidth` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Frame Width',
  `sortBy` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Frame Sort By',
  `healthText` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Frame Health Text',
  `boolOptions` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Many Configurable Bool Options',
  `unk146` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int8',
  `unk147` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int8',
  `unk148` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int8',
  `unk150` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int16',
  `unk152` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int16',
  `unk154` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Profile Unk Int16',
  PRIMARY KEY (`guid`,`id`),
  KEY `index` (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_currency
CREATE TABLE IF NOT EXISTS `character_currency` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `currency` smallint(5) unsigned NOT NULL DEFAULT 0,
  `total_count` int(10) unsigned NOT NULL DEFAULT 0,
  `season_count` int(11) unsigned NOT NULL DEFAULT 0,
  `week_count` int(10) unsigned NOT NULL DEFAULT 0,
  `week_cap` int(10) unsigned NOT NULL DEFAULT 0,
  `new_cap` int(10) unsigned NOT NULL DEFAULT 0,
  `season_cap` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`currency`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_currency_weekcap
CREATE TABLE IF NOT EXISTS `character_currency_weekcap` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `source` smallint(3) unsigned NOT NULL DEFAULT 0,
  `max_week_rating` smallint(5) unsigned NOT NULL DEFAULT 0,
  `week_cap` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`source`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_current_artifacts
CREATE TABLE IF NOT EXISTS `character_current_artifacts` (
  `guid` int(10) unsigned NOT NULL,
  `branchId` int(10) NOT NULL DEFAULT 0,
  `entry` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`branchId`,`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_declinedname
CREATE TABLE IF NOT EXISTS `character_declinedname` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `genitive` varchar(15) NOT NULL DEFAULT '',
  `dative` varchar(15) NOT NULL DEFAULT '',
  `accusative` varchar(15) NOT NULL DEFAULT '',
  `instrumental` varchar(15) NOT NULL DEFAULT '',
  `prepositional` varchar(15) NOT NULL DEFAULT '',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_digsites
CREATE TABLE IF NOT EXISTS `character_digsites` (
  `guid` int(10) unsigned NOT NULL,
  `entry` int(10) NOT NULL DEFAULT 0,
  `count` int(11) DEFAULT NULL,
  PRIMARY KEY (`guid`,`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_equipmentsets
CREATE TABLE IF NOT EXISTS `character_equipmentsets` (
  `guid` int(10) NOT NULL DEFAULT 0,
  `setguid` bigint(20) NOT NULL AUTO_INCREMENT,
  `setindex` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `name` varchar(31) NOT NULL,
  `iconname` varchar(100) NOT NULL,
  `ignore_mask` int(11) unsigned NOT NULL DEFAULT 0,
  `item0` int(11) unsigned NOT NULL DEFAULT 0,
  `item1` int(11) unsigned NOT NULL DEFAULT 0,
  `item2` int(11) unsigned NOT NULL DEFAULT 0,
  `item3` int(11) unsigned NOT NULL DEFAULT 0,
  `item4` int(11) unsigned NOT NULL DEFAULT 0,
  `item5` int(11) unsigned NOT NULL DEFAULT 0,
  `item6` int(11) unsigned NOT NULL DEFAULT 0,
  `item7` int(11) unsigned NOT NULL DEFAULT 0,
  `item8` int(11) unsigned NOT NULL DEFAULT 0,
  `item9` int(11) unsigned NOT NULL DEFAULT 0,
  `item10` int(11) unsigned NOT NULL DEFAULT 0,
  `item11` int(11) unsigned NOT NULL DEFAULT 0,
  `item12` int(11) unsigned NOT NULL DEFAULT 0,
  `item13` int(11) unsigned NOT NULL DEFAULT 0,
  `item14` int(11) unsigned NOT NULL DEFAULT 0,
  `item15` int(11) unsigned NOT NULL DEFAULT 0,
  `item16` int(11) unsigned NOT NULL DEFAULT 0,
  `item17` int(11) unsigned NOT NULL DEFAULT 0,
  `item18` int(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`setguid`),
  UNIQUE KEY `idx_set` (`guid`,`setguid`,`setindex`) USING BTREE,
  KEY `Idx_setindex` (`setindex`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_gifts
CREATE TABLE IF NOT EXISTS `character_gifts` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `item_guid` int(10) unsigned NOT NULL DEFAULT 0,
  `entry` int(10) unsigned NOT NULL DEFAULT 0,
  `flags` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_guid`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_glyphs
CREATE TABLE IF NOT EXISTS `character_glyphs` (
  `guid` int(10) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `glyph1` smallint(5) unsigned DEFAULT 0,
  `glyph2` smallint(5) unsigned DEFAULT 0,
  `glyph3` smallint(5) unsigned DEFAULT 0,
  `glyph4` smallint(5) unsigned DEFAULT 0,
  `glyph5` smallint(5) unsigned DEFAULT 0,
  `glyph6` smallint(5) unsigned DEFAULT 0,
  `glyph7` smallint(5) unsigned DEFAULT 0,
  `glyph8` smallint(5) unsigned DEFAULT 0,
  `glyph9` smallint(5) unsigned DEFAULT 0,
  PRIMARY KEY (`guid`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_guild_reputation
CREATE TABLE IF NOT EXISTS `character_guild_reputation` (
  `guid` int(10) unsigned NOT NULL,
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `disband_time` int(10) unsigned NOT NULL DEFAULT 0,
  `weekly_rep` bigint(20) NOT NULL DEFAULT 0,
  `total_rep` bigint(20) NOT NULL DEFAULT 0,
  UNIQUE KEY `guid_key` (`guid`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Characters Guild Reputation System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_homebind
CREATE TABLE IF NOT EXISTS `character_homebind` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `zoneId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_instance
CREATE TABLE IF NOT EXISTS `character_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `instance` int(10) unsigned NOT NULL DEFAULT 0,
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `resttime` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_inventory
CREATE TABLE IF NOT EXISTS `character_inventory` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `bag` int(10) unsigned NOT NULL DEFAULT 0,
  `slot` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `item` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Item Global Unique Identifier',
  PRIMARY KEY (`item`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_lfr_looted
CREATE TABLE IF NOT EXISTS `character_lfr_looted` (
  `playerGUID` int(10) DEFAULT NULL,
  `objectEntry` int(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_npcbot
CREATE TABLE IF NOT EXISTS `character_npcbot` (
  `owner` int(10) NOT NULL DEFAULT 0,
  `entry` int(10) NOT NULL DEFAULT 0,
  `race` tinyint(3) DEFAULT NULL,
  `class` tinyint(3) DEFAULT NULL,
  `istank` tinyint(3) DEFAULT NULL,
  `equipMhEx` int(10) DEFAULT NULL,
  `equipOhEx` int(10) DEFAULT NULL,
  `equipRhEx` int(10) DEFAULT NULL,
  `equipHead` int(10) DEFAULT NULL,
  `equipShoulders` int(10) DEFAULT NULL,
  `equipChest` int(10) DEFAULT NULL,
  `equipWaist` int(10) DEFAULT NULL,
  `equipLegs` int(10) DEFAULT NULL,
  `equipFeet` int(10) DEFAULT NULL,
  `equipWrist` int(10) DEFAULT NULL,
  `equipHands` int(10) DEFAULT NULL,
  `equipBack` int(10) DEFAULT NULL,
  `equipBody` int(10) DEFAULT NULL,
  `equipFinger1` int(10) DEFAULT NULL,
  `equipFinger2` int(10) DEFAULT NULL,
  `equipTrinket1` int(10) DEFAULT NULL,
  `equipTrinket2` int(10) DEFAULT NULL,
  `equipNeck` int(10) DEFAULT NULL,
  `active` tinyint(3) DEFAULT NULL,
  PRIMARY KEY (`owner`,`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_pet
CREATE TABLE IF NOT EXISTS `character_pet` (
  `id` int(10) unsigned NOT NULL DEFAULT 0,
  `entry` int(10) unsigned NOT NULL DEFAULT 0,
  `owner` int(10) unsigned NOT NULL DEFAULT 0,
  `modelid` int(10) unsigned DEFAULT 0,
  `CreatedBySpell` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `PetType` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `level` smallint(5) unsigned NOT NULL DEFAULT 1,
  `exp` int(10) unsigned NOT NULL DEFAULT 0,
  `Reactstate` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `name` varchar(21) NOT NULL DEFAULT 'Pet',
  `renamed` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `slot` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `curhealth` int(10) unsigned NOT NULL DEFAULT 1,
  `curmana` int(10) unsigned NOT NULL DEFAULT 0,
  `savetime` int(10) unsigned NOT NULL DEFAULT 0,
  `abdata` text DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`) USING BTREE,
  KEY `idx_slot` (`slot`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_pet_declinedname
CREATE TABLE IF NOT EXISTS `character_pet_declinedname` (
  `id` int(10) unsigned NOT NULL DEFAULT 0,
  `owner` int(10) unsigned NOT NULL DEFAULT 0,
  `genitive` varchar(12) NOT NULL DEFAULT '',
  `dative` varchar(12) NOT NULL DEFAULT '',
  `accusative` varchar(12) NOT NULL DEFAULT '',
  `instrumental` varchar(12) NOT NULL DEFAULT '',
  `prepositional` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `owner_key` (`owner`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_pvp_stats
CREATE TABLE IF NOT EXISTS `character_pvp_stats` (
  `data` int(11) DEFAULT NULL,
  `completedEncounters` int(11) DEFAULT NULL,
  `hardMode` int(11) DEFAULT NULL,
  `matchMakerRating` int(11) DEFAULT NULL,
  `map` int(11) DEFAULT NULL,
  `guid` int(11) DEFAULT NULL,
  `slot` int(11) DEFAULT NULL,
  `id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus
CREATE TABLE IF NOT EXISTS `character_queststatus` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `status` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `explored` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `timer` int(10) unsigned NOT NULL DEFAULT 0,
  `mobcount1` smallint(5) unsigned NOT NULL DEFAULT 0,
  `mobcount2` smallint(5) unsigned NOT NULL DEFAULT 0,
  `mobcount3` smallint(5) unsigned NOT NULL DEFAULT 0,
  `mobcount4` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount1` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount2` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount3` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount4` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount5` smallint(5) unsigned NOT NULL DEFAULT 0,
  `itemcount6` smallint(5) unsigned NOT NULL DEFAULT 0,
  `playercount` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus_daily
CREATE TABLE IF NOT EXISTS `character_queststatus_daily` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus_monthly
CREATE TABLE IF NOT EXISTS `character_queststatus_monthly` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus_rewarded
CREATE TABLE IF NOT EXISTS `character_queststatus_rewarded` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `active` tinyint(10) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus_seasonal
CREATE TABLE IF NOT EXISTS `character_queststatus_seasonal` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `event` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Event Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_queststatus_weekly
CREATE TABLE IF NOT EXISTS `character_queststatus_weekly` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_reputation
CREATE TABLE IF NOT EXISTS `character_reputation` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `faction` smallint(5) unsigned NOT NULL DEFAULT 0,
  `standing` int(11) NOT NULL DEFAULT 0,
  `flags` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_research_history
CREATE TABLE IF NOT EXISTS `character_research_history` (
  `guid` int(11) DEFAULT NULL,
  `projectid` int(11) DEFAULT NULL,
  `count` int(11) DEFAULT NULL,
  `time` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_skills
CREATE TABLE IF NOT EXISTS `character_skills` (
  `guid` int(10) unsigned NOT NULL COMMENT 'Global Unique Identifier',
  `skill` smallint(5) unsigned NOT NULL,
  `value` smallint(5) unsigned NOT NULL,
  `max` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_social
CREATE TABLE IF NOT EXISTS `character_social` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `friend` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Friend Global Unique Identifier',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Friend Flags',
  `note` varchar(48) NOT NULL DEFAULT '' COMMENT 'Friend Note',
  PRIMARY KEY (`guid`,`friend`,`flags`),
  KEY `friend` (`friend`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_spell
CREATE TABLE IF NOT EXISTS `character_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `active` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_spell_cooldown
CREATE TABLE IF NOT EXISTS `character_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `item` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Item Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_stats
CREATE TABLE IF NOT EXISTS `character_stats` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT 0,
  `maxpower1` int(10) unsigned NOT NULL DEFAULT 0,
  `maxpower2` int(10) unsigned NOT NULL DEFAULT 0,
  `maxpower3` int(10) unsigned NOT NULL DEFAULT 0,
  `maxpower4` int(10) unsigned NOT NULL DEFAULT 0,
  `maxpower5` int(10) unsigned NOT NULL DEFAULT 0,
  `strength` int(10) unsigned NOT NULL DEFAULT 0,
  `agility` int(10) unsigned NOT NULL DEFAULT 0,
  `stamina` int(10) unsigned NOT NULL DEFAULT 0,
  `intellect` int(10) unsigned NOT NULL DEFAULT 0,
  `spirit` int(10) unsigned NOT NULL DEFAULT 0,
  `armor` int(10) unsigned NOT NULL DEFAULT 0,
  `resHoly` int(10) unsigned NOT NULL DEFAULT 0,
  `resFire` int(10) unsigned NOT NULL DEFAULT 0,
  `resNature` int(10) unsigned NOT NULL DEFAULT 0,
  `resFrost` int(10) unsigned NOT NULL DEFAULT 0,
  `resShadow` int(10) unsigned NOT NULL DEFAULT 0,
  `resArcane` int(10) unsigned NOT NULL DEFAULT 0,
  `blockPct` float unsigned NOT NULL DEFAULT 0,
  `dodgePct` float unsigned NOT NULL DEFAULT 0,
  `parryPct` float unsigned NOT NULL DEFAULT 0,
  `critPct` float unsigned NOT NULL DEFAULT 0,
  `rangedCritPct` float unsigned NOT NULL DEFAULT 0,
  `spellCritPct` float unsigned NOT NULL DEFAULT 0,
  `attackPower` int(10) unsigned NOT NULL DEFAULT 0,
  `rangedAttackPower` int(10) unsigned NOT NULL DEFAULT 0,
  `spellPower` int(10) unsigned NOT NULL DEFAULT 0,
  `resilience` int(10) unsigned NOT NULL DEFAULT 0,
  `mastery` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_talent
CREATE TABLE IF NOT EXISTS `character_talent` (
  `guid` int(10) unsigned NOT NULL,
  `spell` mediumint(8) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.character_void_storage
CREATE TABLE IF NOT EXISTS `character_void_storage` (
  `itemId` bigint(20) unsigned NOT NULL,
  `playerGuid` int(10) unsigned NOT NULL,
  `itemEntry` mediumint(8) unsigned NOT NULL,
  `slot` tinyint(3) unsigned NOT NULL,
  `creatorGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `randomProperty` int(10) unsigned NOT NULL DEFAULT 0,
  `suffixFactor` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`itemId`),
  UNIQUE KEY `idx_player_slot` (`playerGuid`,`slot`) USING BTREE,
  KEY `idx_player` (`playerGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.corpse
CREATE TABLE IF NOT EXISTS `corpse` (
  `corpseGuid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `orientation` float NOT NULL DEFAULT 0,
  `mapId` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `phaseMask` smallint(5) unsigned NOT NULL DEFAULT 1,
  `displayId` int(10) unsigned NOT NULL DEFAULT 0,
  `itemCache` text NOT NULL,
  `bytes1` int(10) unsigned NOT NULL DEFAULT 0,
  `bytes2` int(10) unsigned NOT NULL DEFAULT 0,
  `flags` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `dynFlags` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  `corpseType` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `instanceId` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Instance Identifier',
  PRIMARY KEY (`corpseGuid`),
  KEY `idx_type` (`corpseType`) USING BTREE,
  KEY `idx_time` (`time`) USING BTREE,
  KEY `idx_player` (`guid`) USING BTREE,
  KEY `idx_instance` (`instanceId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Death System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.creature_respawn
CREATE TABLE IF NOT EXISTS `creature_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT 0,
  `mapId` smallint(10) unsigned NOT NULL DEFAULT 0,
  `instanceId` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.cronjobs
CREATE TABLE IF NOT EXISTS `cronjobs` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `param1` varchar(255) NOT NULL,
  `param2` varchar(255) NOT NULL,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.daily_players_reports
CREATE TABLE IF NOT EXISTS `daily_players_reports` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `creation_time` int(10) unsigned NOT NULL DEFAULT 0,
  `average` float NOT NULL DEFAULT 0,
  `total_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `speed_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `fly_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `jump_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `waterwalk_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `teleportplane_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `climb_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.gameobject_respawn
CREATE TABLE IF NOT EXISTS `gameobject_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT 0,
  `instanceId` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Instance Identifier',
  `mapId` smallint(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.game_event_condition_save
CREATE TABLE IF NOT EXISTS `game_event_condition_save` (
  `condition_id` int(10) unsigned NOT NULL DEFAULT 0,
  `done` float DEFAULT 0,
  `eventEntry` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`eventEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.game_event_save
CREATE TABLE IF NOT EXISTS `game_event_save` (
  `eventEntry` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `next_start` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`eventEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.gm_subsurveys
CREATE TABLE IF NOT EXISTS `gm_subsurveys` (
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `subsurveyId` int(10) unsigned NOT NULL DEFAULT 0,
  `rank` int(10) unsigned NOT NULL DEFAULT 0,
  `comment` text NOT NULL,
  PRIMARY KEY (`surveyId`,`subsurveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.gm_surveys
CREATE TABLE IF NOT EXISTS `gm_surveys` (
  `guid` int(10) NOT NULL,
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mainSurvey` int(10) unsigned NOT NULL DEFAULT 0,
  `overallComment` longtext NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`surveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.gm_tickets
CREATE TABLE IF NOT EXISTS `gm_tickets` (
  `ticketId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) NOT NULL COMMENT 'Name of ticket creator',
  `message` text NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT 0,
  `mapId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `lastModifiedTime` int(10) unsigned NOT NULL DEFAULT 0,
  `closedBy` int(10) NOT NULL DEFAULT 0,
  `assignedTo` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text NOT NULL,
  `response` text NOT NULL,
  `completed` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `escalated` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `viewed` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `haveTicket` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.gold_stats
CREATE TABLE IF NOT EXISTS `gold_stats` (
  `AverageGold` int(11) DEFAULT NULL,
  `OverallGold` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.groups
CREATE TABLE IF NOT EXISTS `groups` (
  `guid` int(10) unsigned NOT NULL,
  `leaderGuid` int(10) unsigned NOT NULL,
  `lootMethod` tinyint(3) unsigned NOT NULL,
  `looterGuid` int(10) unsigned NOT NULL,
  `lootThreshold` tinyint(3) unsigned NOT NULL,
  `icon1` int(10) unsigned NOT NULL,
  `icon2` int(10) unsigned NOT NULL,
  `icon3` int(10) unsigned NOT NULL,
  `icon4` int(10) unsigned NOT NULL,
  `icon5` int(10) unsigned NOT NULL,
  `icon6` int(10) unsigned NOT NULL,
  `icon7` int(10) unsigned NOT NULL,
  `icon8` int(10) unsigned NOT NULL,
  `groupType` tinyint(3) unsigned NOT NULL,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `raiddifficulty` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`),
  KEY `leaderGuid` (`leaderGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.group_instance
CREATE TABLE IF NOT EXISTS `group_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `instance` int(10) unsigned NOT NULL DEFAULT 0,
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.group_member
CREATE TABLE IF NOT EXISTS `group_member` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `memberGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `memberFlags` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `subgroup` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `roles` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`memberGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild
CREATE TABLE IF NOT EXISTS `guild` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `name` varchar(24) NOT NULL DEFAULT '',
  `leaderguid` int(10) unsigned NOT NULL DEFAULT 0,
  `EmblemStyle` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `EmblemColor` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `BorderStyle` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `BorderColor` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `BackgroundColor` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `info` text NOT NULL,
  `motd` varchar(128) NOT NULL DEFAULT '',
  `createdate` int(10) unsigned NOT NULL DEFAULT 0,
  `BankMoney` bigint(20) unsigned NOT NULL DEFAULT 0,
  `level` int(10) unsigned DEFAULT 1,
  `todayExperience` bigint(20) unsigned DEFAULT 0,
  `RaidChallenge` int(10) NOT NULL DEFAULT 0,
  `ChallengeCount` int(32) unsigned NOT NULL DEFAULT 0,
  `RatedBGChallenge` int(10) NOT NULL DEFAULT 0,
  `DungeonChallenge` int(10) NOT NULL DEFAULT 0,
  `experience` bigint(20) unsigned DEFAULT 0,
  PRIMARY KEY (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_achievement
CREATE TABLE IF NOT EXISTS `guild_achievement` (
  `guildId` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT 0,
  `guids` text NOT NULL,
  PRIMARY KEY (`guildId`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_achievement_progress
CREATE TABLE IF NOT EXISTS `guild_achievement_progress` (
  `guildId` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT 0,
  `completedGuid` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildId`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_bank_eventlog
CREATE TABLE IF NOT EXISTS `guild_bank_eventlog` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Log record identificator - auxiliary column',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Guild bank TabId',
  `EventType` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Event type',
  `PlayerGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `ItemOrMoney` int(10) unsigned NOT NULL DEFAULT 0,
  `ItemStackCount` smallint(5) unsigned NOT NULL DEFAULT 0,
  `DestTabId` tinyint(3) unsigned NOT NULL DEFAULT 0 COMMENT 'Destination Tab Id',
  `TimeStamp` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`,`TabId`),
  KEY `guildid_key` (`guildid`) USING BTREE,
  KEY `Idx_PlayerGuid` (`PlayerGuid`) USING BTREE,
  KEY `Idx_LogGuid` (`LogGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_bank_item
CREATE TABLE IF NOT EXISTS `guild_bank_item` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `SlotId` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `item_guid` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`,`TabId`,`SlotId`),
  KEY `guildid_key` (`guildid`) USING BTREE,
  KEY `Idx_item_guid` (`item_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_bank_right
CREATE TABLE IF NOT EXISTS `guild_bank_right` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `rid` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `gbright` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `SlotPerDay` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`,`TabId`,`rid`),
  KEY `guildid_key` (`guildid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_bank_tab
CREATE TABLE IF NOT EXISTS `guild_bank_tab` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `TabName` varchar(16) NOT NULL DEFAULT '',
  `TabIcon` varchar(100) NOT NULL DEFAULT '',
  `TabText` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`guildid`,`TabId`),
  KEY `guildid_key` (`guildid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_challenges_completed
CREATE TABLE IF NOT EXISTS `guild_challenges_completed` (
  `guildId` int(10) unsigned NOT NULL DEFAULT 0,
  `challengeId` int(10) unsigned DEFAULT NULL,
  `dateCompleted` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`guildId`),
  KEY `challengeId` (`challengeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_eventlog
CREATE TABLE IF NOT EXISTS `guild_eventlog` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint(3) unsigned NOT NULL COMMENT 'Event type',
  `PlayerGuid1` int(10) unsigned NOT NULL COMMENT 'Player 1',
  `PlayerGuid2` int(10) unsigned NOT NULL COMMENT 'Player 2',
  `NewRank` tinyint(3) unsigned NOT NULL COMMENT 'New rank(in case promotion/demotion)',
  `TimeStamp` int(10) unsigned NOT NULL COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`) USING BTREE,
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`) USING BTREE,
  KEY `Idx_LogGuid` (`LogGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Eventlog';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_finder_applicant
CREATE TABLE IF NOT EXISTS `guild_finder_applicant` (
  `guildId` int(10) unsigned DEFAULT NULL,
  `playerGuid` int(10) unsigned DEFAULT NULL,
  `availability` tinyint(3) unsigned DEFAULT 0,
  `classRole` tinyint(3) unsigned DEFAULT 0,
  `interests` tinyint(3) unsigned DEFAULT 0,
  `comment` varchar(255) DEFAULT NULL,
  `submitTime` int(10) unsigned DEFAULT NULL,
  UNIQUE KEY `guildId` (`guildId`,`playerGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_finder_guild_settings
CREATE TABLE IF NOT EXISTS `guild_finder_guild_settings` (
  `guildId` int(10) unsigned NOT NULL,
  `availability` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `classRoles` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `interests` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `level` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `listed` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`guildId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_member
CREATE TABLE IF NOT EXISTS `guild_member` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `guid` int(10) unsigned NOT NULL,
  `rank` tinyint(3) unsigned NOT NULL,
  `pnote` varchar(31) NOT NULL DEFAULT '',
  `offnote` varchar(31) NOT NULL DEFAULT '',
  `activity` bigint(20) NOT NULL,
  `profession1_skillID` int(10) NOT NULL DEFAULT 0,
  `weekReputation` int(10) NOT NULL DEFAULT 0,
  `profession2_rank` int(10) NOT NULL DEFAULT 0,
  `profession2_skillID` int(10) NOT NULL DEFAULT 0,
  `profession1_rank` int(10) NOT NULL DEFAULT 0,
  `weekActivity` int(10) NOT NULL DEFAULT 0,
  `profession1_level` int(10) NOT NULL DEFAULT 0,
  `profession2_level` int(10) NOT NULL DEFAULT 0,
  UNIQUE KEY `guid_key` (`guid`) USING BTREE,
  KEY `guildid_key` (`guildid`) USING BTREE,
  KEY `guildid_rank_key` (`guildid`,`rank`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_member_withdraw
CREATE TABLE IF NOT EXISTS `guild_member_withdraw` (
  `guid` int(10) unsigned NOT NULL,
  `tab0` int(10) unsigned NOT NULL DEFAULT 0,
  `tab1` int(10) unsigned NOT NULL DEFAULT 0,
  `tab2` int(10) unsigned NOT NULL DEFAULT 0,
  `tab3` int(10) unsigned NOT NULL DEFAULT 0,
  `tab4` int(10) unsigned NOT NULL DEFAULT 0,
  `tab5` int(10) unsigned NOT NULL DEFAULT 0,
  `tab6` int(10) unsigned NOT NULL DEFAULT 0,
  `tab7` int(10) unsigned NOT NULL DEFAULT 0,
  `money` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Member Dayly Withdraws';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_newslog
CREATE TABLE IF NOT EXISTS `guild_newslog` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `TimeStamp` int(10) unsigned NOT NULL DEFAULT 0,
  `PlayerGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `LogGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `Flags` int(10) unsigned NOT NULL DEFAULT 0,
  `EventType` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `Value` int(10) unsigned NOT NULL DEFAULT 0,
  KEY `Idx_PlayerGuid` (`PlayerGuid`) USING BTREE,
  KEY `Idx_LogGuid` (`LogGuid`) USING BTREE,
  KEY `guildid_key` (`guildid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Newslog';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_old_member
CREATE TABLE IF NOT EXISTS `guild_old_member` (
  `guid` int(10) NOT NULL,
  `guildId` int(10) NOT NULL,
  `weekReputation` int(10) NOT NULL,
  `leaveDate` int(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.guild_rank
CREATE TABLE IF NOT EXISTS `guild_rank` (
  `guildid` int(10) unsigned NOT NULL DEFAULT 0,
  `rid` tinyint(3) unsigned NOT NULL,
  `rname` varchar(20) NOT NULL DEFAULT '',
  `rights` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `BankMoneyPerDay` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.instance
CREATE TABLE IF NOT EXISTS `instance` (
  `id` int(10) unsigned NOT NULL DEFAULT 0,
  `map` smallint(5) unsigned NOT NULL DEFAULT 0,
  `resettime` int(10) unsigned NOT NULL DEFAULT 0,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `completedEncounters` int(10) unsigned NOT NULL DEFAULT 0,
  `data` tinytext NOT NULL,
  `isLFG` smallint(5) unsigned NOT NULL DEFAULT 0,
  `hardMode` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `map` (`map`) USING BTREE,
  KEY `resettime` (`resettime`) USING BTREE,
  KEY `difficulty` (`difficulty`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.instance_reset
CREATE TABLE IF NOT EXISTS `instance_reset` (
  `mapid` smallint(5) unsigned NOT NULL DEFAULT 0,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `resettime` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`mapid`,`difficulty`),
  KEY `difficulty` (`difficulty`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.item_instance
CREATE TABLE IF NOT EXISTS `item_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `itemEntry` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `owner_guid` int(10) unsigned NOT NULL DEFAULT 0,
  `creatorGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `giftCreatorGuid` int(10) unsigned NOT NULL DEFAULT 0,
  `count` int(10) unsigned NOT NULL DEFAULT 1,
  `duration` int(10) NOT NULL DEFAULT 0,
  `charges` tinytext DEFAULT NULL,
  `flags` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `enchantments` text NOT NULL,
  `randomPropertyId` smallint(5) NOT NULL DEFAULT 0,
  `durability` smallint(5) unsigned NOT NULL DEFAULT 0,
  `playedTime` int(10) unsigned NOT NULL DEFAULT 0,
  `text` text DEFAULT NULL,
  PRIMARY KEY (`guid`),
  KEY `idx_owner_guid` (`owner_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.item_loot_items
CREATE TABLE IF NOT EXISTS `item_loot_items` (
  `container_id` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'guid of container (item_instance.guid)',
  `item_id` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'loot item entry (item_instance.itemEntry)',
  `item_count` int(10) NOT NULL DEFAULT 0 COMMENT 'stack size',
  `follow_rules` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'follow loot rules',
  `ffa` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'free-for-all',
  `blocked` tinyint(1) NOT NULL DEFAULT 0,
  `counted` tinyint(1) NOT NULL DEFAULT 0,
  `under_threshold` tinyint(1) NOT NULL DEFAULT 0,
  `needs_quest` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'quest drop',
  `rnd_prop` int(10) NOT NULL DEFAULT 0 COMMENT 'random enchantment added when originally rolled',
  `rnd_suffix` int(10) NOT NULL DEFAULT 0 COMMENT 'random suffix added when originally rolled'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.item_loot_money
CREATE TABLE IF NOT EXISTS `item_loot_money` (
  `container_id` int(10) NOT NULL DEFAULT 0 COMMENT 'guid of container (item_instance.guid)',
  `money` int(10) NOT NULL DEFAULT 0 COMMENT 'money loot (in copper)'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.item_refund_instance
CREATE TABLE IF NOT EXISTS `item_refund_instance` (
  `item_guid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `player_guid` int(10) unsigned NOT NULL COMMENT 'Player GUID',
  `paidMoney` int(10) unsigned NOT NULL DEFAULT 0,
  `paidExtendedCost` smallint(5) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_guid`,`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.item_soulbound_trade_data
CREATE TABLE IF NOT EXISTS `item_soulbound_trade_data` (
  `itemGuid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `allowedPlayers` text NOT NULL COMMENT 'Space separated GUID list of players who can receive this item in trade',
  PRIMARY KEY (`itemGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.lag_reports
CREATE TABLE IF NOT EXISTS `lag_reports` (
  `reportId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `lagType` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `mapId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `latency` int(10) unsigned NOT NULL DEFAULT 0,
  `createTime` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`reportId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.lfg_data
CREATE TABLE IF NOT EXISTS `lfg_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `dungeon` int(10) unsigned NOT NULL DEFAULT 0,
  `state` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LFG Data';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.mail
CREATE TABLE IF NOT EXISTS `mail` (
  `id` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Identifier',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `stationery` tinyint(3) NOT NULL DEFAULT 41,
  `mailTemplateId` smallint(5) unsigned NOT NULL DEFAULT 0,
  `sender` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `receiver` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `subject` longtext DEFAULT NULL,
  `body` longtext DEFAULT NULL,
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `expire_time` int(10) unsigned NOT NULL DEFAULT 0,
  `deliver_time` int(10) unsigned NOT NULL DEFAULT 0,
  `money` bigint(20) unsigned NOT NULL DEFAULT 0,
  `cod` bigint(20) unsigned NOT NULL DEFAULT 0,
  `checked` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mail System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.mail_items
CREATE TABLE IF NOT EXISTS `mail_items` (
  `mail_id` int(10) unsigned NOT NULL DEFAULT 0,
  `item_guid` int(10) unsigned NOT NULL DEFAULT 0,
  `receiver` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  PRIMARY KEY (`item_guid`),
  KEY `idx_receiver` (`receiver`) USING BTREE,
  KEY `idx_mail_id` (`mail_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.petition
CREATE TABLE IF NOT EXISTS `petition` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned DEFAULT 0,
  `name` varchar(24) NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`ownerguid`,`type`),
  UNIQUE KEY `index_ownerguid_petitionguid` (`ownerguid`,`petitionguid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.petition_sign
CREATE TABLE IF NOT EXISTS `petition_sign` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned NOT NULL DEFAULT 0,
  `playerguid` int(10) unsigned NOT NULL DEFAULT 0,
  `player_account` int(10) unsigned NOT NULL DEFAULT 0,
  `type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`petitionguid`,`playerguid`),
  KEY `Idx_playerguid` (`playerguid`) USING BTREE,
  KEY `Idx_ownerguid` (`ownerguid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pet_aura
CREATE TABLE IF NOT EXISTS `pet_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT 0 COMMENT 'Full Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `effect_mask` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `recalculate_mask` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `amount0` mediumint(8) NOT NULL,
  `amount1` mediumint(8) NOT NULL,
  `amount2` mediumint(8) NOT NULL,
  `base_amount0` mediumint(8) NOT NULL,
  `base_amount1` mediumint(8) NOT NULL,
  `base_amount2` mediumint(8) NOT NULL,
  `maxduration` int(11) NOT NULL DEFAULT 0,
  `remaintime` int(11) NOT NULL DEFAULT 0,
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pet_spell
CREATE TABLE IF NOT EXISTS `pet_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `active` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pet_spell_cooldown
CREATE TABLE IF NOT EXISTS `pet_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.players_reports_status
CREATE TABLE IF NOT EXISTS `players_reports_status` (
  `guid` int(10) unsigned NOT NULL DEFAULT 0,
  `creation_time` int(10) unsigned NOT NULL DEFAULT 0,
  `average` float NOT NULL DEFAULT 0,
  `total_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `speed_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `fly_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `jump_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `waterwalk_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `teleportplane_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  `climb_reports` bigint(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pool_quest_save
CREATE TABLE IF NOT EXISTS `pool_quest_save` (
  `pool_id` int(10) unsigned NOT NULL DEFAULT 0,
  `quest_id` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`pool_id`,`quest_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pvpstats_battlegrounds
CREATE TABLE IF NOT EXISTS `pvpstats_battlegrounds` (
  `matherMakerRating` int(11) DEFAULT NULL,
  `guid` int(11) DEFAULT NULL,
  `slot` int(11) DEFAULT NULL,
  `id` int(11) DEFAULT NULL,
  `winner_faction` int(11) DEFAULT NULL,
  `bracket_id` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `date` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.pvpstats_players
CREATE TABLE IF NOT EXISTS `pvpstats_players` (
  `battleground_id` int(11) DEFAULT NULL,
  `character_guid` int(11) DEFAULT NULL,
  `winner` int(11) DEFAULT NULL,
  `score_killing_blows` int(11) DEFAULT NULL,
  `score_deaths` int(11) DEFAULT NULL,
  `score_honorable_kills` int(11) DEFAULT NULL,
  `score_bonus_honor` int(11) DEFAULT NULL,
  `score_damage_done` int(11) DEFAULT NULL,
  `score_healing_done` int(11) DEFAULT NULL,
  `attr_1` int(11) DEFAULT NULL,
  `attr_2` int(11) DEFAULT NULL,
  `attr_3` int(11) DEFAULT NULL,
  `attr_4` int(11) DEFAULT NULL,
  `attr_5` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.reserved_name
CREATE TABLE IF NOT EXISTS `reserved_name` (
  `name` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player Reserved Names';

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.warden_action
CREATE TABLE IF NOT EXISTS `warden_action` (
  `wardenId` smallint(5) unsigned NOT NULL,
  `action` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`wardenId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table 0000cataclysmwowcharacters.worldstates
CREATE TABLE IF NOT EXISTS `worldstates` (
  `entry` int(10) unsigned NOT NULL DEFAULT 0,
  `value` int(10) unsigned NOT NULL DEFAULT 0,
  `comment` tinytext DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Variable Saves';

-- Data exporting was unselected.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
