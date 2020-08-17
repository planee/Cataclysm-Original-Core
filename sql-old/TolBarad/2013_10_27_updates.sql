-- tolbarad towers
delete from gameobject where id in (204588, 204590, 204589);

-- tolbarad guards
delete from creature where id in (51166, 51165);

-- tolbarad keep trashes
delete from creature where id in(48069, 48066, 48070, 48061);

delete from creature where id in(47595, 47598, 47600, 47599);

delete from creature where id in(47607, 47608, 47609, 47610);

-- tolbarad banners
delete from gameobject where id in (205096, 205139, 205103, 205068, 205101, 205138, 207391, 207400);

-- tolbarad vehicle
delete from creature where id = 45344;

-- tolbarad spirit guides.
delete from creature where id in (45066, 45079);
update creature_template set npcflag = 32769 where entry in (45066, 45079);


-- tolbarad quest npcs.
delete from creature where id in (47550, 47590, 47552, 48036);
delete from creature where id in (47534, 47531, 47537);
delete from creature where id in (47544, 47542, 47540, 47549);
delete from creature where id in(47591, 47593);

-- re enable tolbarad daily quests
UPDATE `quest_template` SET `Method` = 2, `MinLevel` = 85, `MaxLevel` = 85 WHERE `id` IN (28117, 28660, 28118, 28661, 28120, 28662, 28165, 28663, 28185, 28664, 28186, 28665, 28122, 28657, 28658, 28162, 28163, 28659, 28188, 28668, 28223, 28669, 28232, 28670);

-- re enable tolbarad dynamic quests
UPDATE `quest_template` SET `MinLevel`='85', `MaxLevel`='85' WHERE id in (28663, 28665, 28664, 28165, 28186, 28185, 28661, 28660, 28662, 28118, 28117, 28120, 28670, 28669, 28668, 28232, 28188, 28223);

-- dynamic quests giver
delete from creature where id in(48062, 48074,  48071, 48039, 48070, 48061);
