ALTER TABLE `creature_template` ADD COLUMN `speed_swim` FLOAT NOT NULL DEFAULT 1 AFTER `speed_run`;
ALTER TABLE `creature_template` ADD COLUMN `speed_fly` FLOAT NOT NULL DEFAULT 1 AFTER `speed_swim`;

/*not 100% blizzlike but more coherent than actual 1.0 rate... Need manual adjustment to complete this*/

UPDATE `creature_template` SET `speed_swim` = `speed_walk`;
UPDATE `creature_template` SET `speed_fly` = `speed_run`;
