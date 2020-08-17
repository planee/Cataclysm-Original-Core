-- Remove wrong control pet spell from hunter trainer
DELETE FROM `npc_trainer` WHERE `spell` = 79682;