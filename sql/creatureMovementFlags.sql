-- Enable creatures that only fly to also be able to land.
UPDATE `creature_template` SET `InhabitType` = 5 WHERE `InhabitType` = 4;