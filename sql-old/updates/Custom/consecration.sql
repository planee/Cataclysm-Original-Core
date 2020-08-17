UPDATE `creature_template` SET `unit_flags` = 0x2|0x4|0x100|0x200|0x2000000, ScriptName="npc_consecration" WHERE `entry` = 43499;
INSERT INTO `spell_script_names` VALUES ('26573', 'spell_pal_consecration');
UPDATE spell_bonus_data SET entry=82366 WHERE entry=26573;