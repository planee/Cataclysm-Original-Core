DELETE FROM spell_linked_spell WHERE spell_effect IN(72752, 72757);
UPDATE spell_script_names SET spell_id = 42292 WHERE spell_id=72752;
UPDATE spell_script_names SET spell_id = 7744 WHERE spell_id=72757;