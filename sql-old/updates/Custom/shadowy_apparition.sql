INSERT INTO `spell_proc_event` VALUES ('78202', '0', '6', '32768', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `spell_proc_event` VALUES ('78203', '0', '6', '32768', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `spell_proc_event` VALUES ('78204', '0', '6', '32768', '0', '0', '0', '0', '0', '0', '0');
UPDATE creature_template SET speed_walk = 0.5, speed_run = 0.5, ScriptName="npc_shadowy_apparition" WHERE entry=46954;
INSERT INTO `spell_bonus_data` VALUES ('87532', '0.515', '0', '0', '0', 'Shadowy apparition-damage');
