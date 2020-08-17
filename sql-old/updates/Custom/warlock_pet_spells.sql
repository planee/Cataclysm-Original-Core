DELETE FROM spell_script_names WHERE spell_id IN (54049, 3110);
INSERT INTO `spell_script_names` VALUES ('3110', 'spell_warl_pet_damage_spells');
INSERT INTO `spell_script_names` VALUES ('54049', 'spell_warl_pet_damage_spells');
INSERT INTO `spell_script_names` VALUES ('6360', 'spell_warl_pet_damage_spells');
INSERT INTO `spell_script_names` VALUES ('7814', 'spell_warl_pet_damage_spells');
UPDATE spell_bonus_data SET direct_bonus=0 WHERE entry IN(3110, 54049);
INSERT INTO `spell_bonus_data` VALUES ('6360', '0', '0', '0', '0', 'Whiplash');
INSERT INTO `spell_bonus_data` VALUES ('7814', '0', '0', '0', '0', 'Lash of pain');