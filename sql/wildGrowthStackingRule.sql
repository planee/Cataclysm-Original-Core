DELETE FROM `spell_group` WHERE `spell_id` IN (16886, 24907, 2895, 49868);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES 
(1034, 16886),
(1034, 24907),
(1034, 2895),
(1034, 49868);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1034;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES 
(1034, 3);