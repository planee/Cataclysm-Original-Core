DELETE from command WHERE `name` = 'npc get';
INSERT into command (`name`, `security`, `help`) VALUES
('npc get', 1, 'Usage: Select a creature and type .npc get\nReturns Entry, Name, and LootId of creature');