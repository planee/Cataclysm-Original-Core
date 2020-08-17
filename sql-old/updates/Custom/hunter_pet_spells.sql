INSERT INTO `spell_script_names` VALUES ('16827', 'spell_hun_pet_damage_spells');

INSERT INTO `spell_script_names` VALUES ('49966', 'spell_hun_pet_damage_spells');

INSERT INTO `spell_script_names` VALUES ('17253', 'spell_hun_pet_damage_spells');

INSERT INTO `spell_script_names` VALUES ('83381', 'spell_hun_pet_damage_spells');

UPDATE spell_bonus_data SET ap_bonus=0 WHERE entry = 83381;
INSERT INTO `spell_bonus_data` VALUES ('16827', '0', '0', '0', '0', 'Claw');

INSERT INTO `spell_bonus_data` VALUES ('49966', '0', '0', '0', '0', 'Bite');
INSERT INTO `spell_bonus_data` VALUES ('17253', '0', '0', '0', '0', 'Smack');
