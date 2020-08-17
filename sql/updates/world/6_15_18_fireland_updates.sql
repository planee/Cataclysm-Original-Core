UPDATE creature_template SET AIName = '' WHERE entry in (53187, 53901, 53185, 53185, 54161, 54143);

DELETE from `creature_sparring_template` WHERE AttackerEntry in (53222, 53119);
INSERT INTO `creature_sparring_template` (`AttackerEntry`, `victimEntry`, `HealthLimitPct`) VALUES 
('53222', '53119', '50'),
('53119', '53222', '50');