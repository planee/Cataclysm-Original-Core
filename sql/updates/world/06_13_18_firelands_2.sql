update creature set movementtype = 0 WHERE id in (53494); -- remove baleroc moving like an idiot 
update creature set movementtype = 1 WHERE id in (53631); -- spiderlings should move 

-- scripts are missing in mop sql.. wtf 
UPDATE creature_template set scriptname = 'npc_firelands_molten_lord' where entry = 53115;
UPDATE creature_template set scriptname = 'npc_firelands_inferno_hawk' where entry = 53648;
UPDATE creature_template set scriptname = 'npc_firelands_cinderweb_spinner' where entry = 53642;
UPDATE creature_template set scriptname = 'npc_firelands_flamewaker_trapper' where entry = 53209;
UPDATE creature_template set scriptname = 'npc_firelands_hell_hound' where entry = 53206;
UPDATE creature_template set scriptname = 'npc_firelands_flamewaker_animator' where entry = 53187;


DELETE FROM `creature` WHERE `id` IN (53694, 53695, 53691);

-- Shannox pathing
DELETE FROM waypoint_data WHERE id = 536910;
INSERT INTO waypoint_data (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(536910, 1, -59.8127, -62.7088, 57.634, 0, 0, 0, 0, 100, 0),
(536910, 2, -57.865, -44.2347, 57.494, 0, 0, 0, 0, 100, 0),
(536910, 3, -49.3376, -13.7828, 56.9726, 0, 0, 0, 0, 100, 0),
(536910, 4, -43.7032, 18.4531, 56.9657, 0, 0, 0, 0, 100, 0),
(536910, 5, -37.1545, 57.666, 56.4158, 0, 0, 0, 0, 100, 0),
(536910, 6, -15.4109, 92.2006, 56.7529, 0, 0, 0, 0, 100, 0),
(536910, 7, 6.17113, 128.312, 51.444, 0, 0, 0, 0, 100, 0),
(536910, 8, 13.2458, 173.183, 46.1776, 0, 0, 0, 0, 100, 0),
(536910, 9, 5.56866, 188.9, 46.8936, 0, 0, 0, 0, 100, 0),
(536910, 10, -11.7439, 200.786, 47.7547, 0, 0, 0, 0, 100, 0),
(536910, 11, -35.0406, 193.434, 47.157, 0, 0, 0, 0, 100, 0),
(536910, 12, -63.8269, 175.575, 45.2257, 0, 0, 0, 0, 100, 0),
(536910, 13, -79.7898, 172.996, 44.6366, 0, 0, 0, 0, 100, 0),
(536910, 14, -99.1355, 180.973, 45.1656, 0, 0, 0, 0, 100, 0),
(536910, 15, -125.018, 191.654, 46.1573, 0, 0, 0, 0, 100, 0),
(536910, 16, -144.934, 192.572, 46.1573, 0, 0, 0, 0, 100, 0),
(536910, 17, -163.712, 180.881, 46.1769, 0, 0, 0, 0, 100, 0),
(536910, 18, -187.103, 172.588, 46.6414, 0, 0, 0, 0, 100, 0),
(536910, 19, -201.015, 163.942, 46.9231, 0, 0, 0, 0, 100, 0),
(536910, 20, -211.965, 153.641, 47.5703, 0, 0, 0, 0, 100, 0),
(536910, 21, -222.144, 150.537, 48.281, 0, 0, 0, 0, 100, 0),
(536910, 22, -241.106, 131.485, 49.1467, 0, 0, 0, 0, 100, 0),
(536910, 23, -241.469, 102.368, 47.757, 0, 0, 0, 0, 100, 0),
(536910, 24, -258.243, 87.8479, 46.4122, 0, 0, 0, 0, 100, 0),
(536910, 25, -277.66, 77.1509, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 26, -295.067, 70.5455, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 27, -318.14, 67.2302, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 28, -318.945, 50.58, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 29, -318.49, 35.3322, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 30, -306.529, 22.5945, 46.1587, 0, 0, 0, 0, 100, 0),
(536910, 31, -304.976, 0.39024, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 32, -316.883, -18.1339, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 33, -320.798, -42.3185, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 34, -317.779, -58.2317, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 35, -301.298, -74.7997, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 36, -298.867, -100.43, 46.1577, 0, 0, 0, 0, 100, 0),
(536910, 37, -287.777, -113.865, 45.4976, 0, 0, 0, 0, 100, 0),
(536910, 38, -280.219, -131.932, 45.9268, 0, 0, 0, 0, 100, 0),
(536910, 39, -265.147, -153.295, 51.5844, 0, 0, 0, 0, 100, 0),
(536910, 40, -235.903, -181.735, 53.8719, 0, 0, 0, 0, 100, 0),
(536910, 41, -221.968, -193.93, 54.4089, 0, 0, 0, 0, 100, 0),
(536910, 42, -209.11, -212.015, 54.4089, 0, 0, 0, 0, 100, 0),
(536910, 43, -191.505, -232.906, 55.6273, 0, 0, 0, 0, 100, 0),
(536910, 44, -171.458, -237.961, 55.7297, 0, 0, 0, 0, 100, 0),
(536910, 45, -138.705, -219.182, 57.12, 0, 0, 0, 0, 100, 0),
(536910, 46, -117.074, -201.403, 57.1181, 0, 0, 0, 0, 100, 0),
(536910, 47, -115.557, -165.245, 57.1178, 0, 0, 0, 0, 100, 0),
(536910, 48, -107.908, -152.124, 57.1219, 0, 0, 0, 0, 100, 0),
(536910, 49, -86.1841, -127.773, 56.85, 0, 0, 0, 0, 100, 0),
(536910, 50, -75.1308, -111.249, 56.8839, 0, 0, 0, 0, 100, 0),
(536910, 51, -67.791, -82.0142, 57.0661, 0, 0, 0, 0, 100, 0);
-- Shannox Controller
DELETE FROM creature_text WHERE entry = 53910;
INSERT INTO creature_text (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `TextRange`, `comment`) VALUES
(53910, 0, 0, "|TInterface\\ICONS\\INV_Misc_Horn_03.blp:16|tAs the creatures of the Firelands fall, a huntsman\'s horn sounds in the distance.", 41, 0, 100, 0, 0, 7034, 3, 'Shannox Controller - EMOTE_HORN_1'),
(53910, 1, 0, '|TInterface\\ICONS\\INV_Misc_Horn_03.blp:16|tThe hunting horn sounds again, nearer and more urgently.', 41, 0, 100, 0, 0, 13363, 3, 'Shannox Controller - EMOTE_HORN_2');


UPDATE `creature_template` SET `ScriptName`='npc_shannox_controller' WHERE (`entry`='53910');

-- Shannox
DELETE FROM creature_text WHERE entry = 53691;
INSERT INTO creature_text (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `TextRange`, `comment`) VALUES
(53691, 0, 0, "Yes... I smell them too, Riplimb. Outsiders encroach on the Firelord's private grounds. Find their trail, find them for me, that I may dispense punishment!", 14, 0, 100, 0, 0, 24584, 3, 'Shannox YELL_SPAWN'),
(53691, 1, 0, 'A-hah! The interlopers! Kill them. EAT THEM!', 14, 0, 100, 0, 0, 24565, 0, 'Shannox YELL_AGGRO'),
(53691, 2, 0, 'Ohh... the pain. Lord of Fire, it hurts....', 14, 0, 100, 0, 0, 24568, 0, 'Shannox YELL_DEATH'),
(53691, 3, 0, 'Fetch your supper!', 14, 0, 100, 0, 0, 24569, 0, 'Shannox YELL_HURL_SPEAR'),
(53691, 4, 0, 'Riplimb! No... no! Oh, you terrible little beasts! How could you?!', 14, 0, 100, 0, 0, 24574, 0, 'Shannox YELL_RIPLIMB_DEATH'),
(53691, 5, 0, 'Oh, you murderers! Why? Why would you kill such a noble animal?', 14, 0, 100, 0, 0, 24575, 0, 'Shannox YELL_RAGEFACE_DEATH'),
(53691, 6, 0, 'Now you burn!', 14, 0, 100, 0, 0, 24576, 0, 'Shannox YELL_MAGMA_RUPTURE'),
(53691, 6, 1, 'Twist in flames, interlopers!', 14, 0, 100, 0, 0, 24577, 0, 'Shannox YELL_MAGMA_RUPTURE'),
(53691, 7, 0, 'Dog food!', 14, 0, 100, 0, 0, 24578, 0, 'Shannox YELL_KILL'),
(53691, 7, 1, 'Now you stay dead!', 14, 0, 100, 0, 0, 24579, 0, 'Shannox YELL_KILL'),
(53691, 7, 2, 'The Firelord will be most pleased.', 14, 0, 100, 0, 0, 24580, 0, 'Shannox YELL_KILL'),
(53691, 7, 3, 'Yes... oh yes!', 14, 0, 100, 0, 0, 24581, 0, 'Shannox YELL_KILL'),
(53691, 8, 0, '%s becomes enraged at seeing one of his companions fall!', 41, 0, 100, 0, 0, 0, 0, 'Shannox EMOTE_DOG_DIED'),
(53691, 9, 0, 'Riplimb collapses into a smouldering heap....', 41, 0, 100, 0, 0, 0, 0, 'Shannox EMOTE_RIPLIMB_DIED'),
(53691, 10, 0, 'Riplimb reanimates and surges into battle to protect his master!', 41, 0, 100, 0, 0, 0, 0, 'Shannox EMOTE_RIPLIMB_REVIVED');

DELETE from instance_template WHERE map = 720;
INSERT into instance_template (map, parent, script, allowmount) VALUES (720, 0, 'instance_firelands', 1);