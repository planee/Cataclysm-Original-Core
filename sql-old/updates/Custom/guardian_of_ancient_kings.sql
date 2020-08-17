INSERT INTO `spell_script_names` VALUES ('86150', 'spell_pal_guardian');
INSERT INTO `spell_script_names` VALUES ('86698', 'spell_pal_guardian_triggered');
UPDATE `creature_template` SET `unit_flags` = 0x2|0x2000000, ScriptName="npc_melee_guardian" WHERE `entry` = 46506;
INSERT INTO `spell_bonus_data` VALUES ('86704', '0', '0', '0.061', '0', 'Ancient guardian - damage');

UPDATE `creature_template` SET `unit_flags` = 0x2|0x100|0x2000|0x2000000, AIName="PassiveAI" WHERE `entry` = 46499;
UPDATE `creature_template` SET `unit_flags` = 0x2|0x100|0x2000|0x2000000, AIName="PassiveAI" WHERE `entry` = 46490;