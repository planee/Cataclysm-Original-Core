-- Dummy Triggers
UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` IN (43691,41264);

UPDATE `creature_template` SET `modelid2` = 0 WHERE `entry` = 41264;