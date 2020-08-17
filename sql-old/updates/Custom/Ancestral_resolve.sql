delete from spell_script_names where spell_id = '-77829';
INSERT INTO `spell_script_names` VALUES ('-77829', 'spell_sha_ancestral_resolve');
delete from spell_ranks where first_spell_id = 77829 AND (rank = 1 || rank = 2);
INSERT INTO `spell_ranks` VALUES ('77829', '77829', '1');
INSERT INTO `spell_ranks` VALUES ('77829', '77830', '2');
