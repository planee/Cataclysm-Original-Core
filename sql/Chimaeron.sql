-- Chimaeron 

-- Finkle Einhorn Texts
DELETE FROM `npc_text` WHERE `ID` IN (16565,16593,16594,16595,16596,16597);
INSERT INTO `npc_text` VALUES
(16565, 'Leaping Leper Gnomes! You\'re a sight for sore eyes. I was worried the giant magma worm out front would deter visitors. Now that you\'re here you can give me a hand.', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(16593, 'What? Of course not! I could easily escape this simple contraption, but I\'m much too busy. See, a while back I spent a few months inside the belly of a beast...', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(16594, 'It wasn\'t strictly voluntary, but I discovered something truly incredible. The beast seemed to be able to consume almost anything with no ill effects.', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(16595, 'Exactly. I ran some tests on the residual digestive fluid coating my suit and...\n<Finkle trails off>\nSay, you haven\'t seen my skinning knife anywhere, have you? I seem to have misplaced it.', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(16596, 'Oh yes, the bile; it\'s amazing stuff. With the proper catalysts it renders anything to which it\'s applied practically immortal*. I\'ve been harvesting it with my reprogrammed Bile-O-Tron 800 and I\'m certain I\'ve got the mixture right this time. I just need a few volunteers to test it out against a suitable creature. What do you say, friend?\n*Immortality subject to certain restrictions.', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(16597, 'Oops! That lever activates the Bile-O-Tron. Oh, and if you happen to come across a key...', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

-- Bile o tron pathing
DELETE FROM `waypoint_data` WHERE id = 4441800;
INSERT INTO `waypoint_data` VALUES 
(4441800, 1, -135.795, 15.569, 73.165, 0, 0, 0, 0, 100, 0, 0),
(4441800, 2, -129.176, -10.488, 73.079, 0, 0, 0, 0, 100, 0, 0),
(4441800, 3, -106.186, -18.533, 72.798, 0, 0, 0, 0, 100, 0, 0),
(4441800, 4, -77.951, 0.702, 73.093, 0, 0, 0, 0, 100, 0, 0),
(4441800, 5, -77.466, 31.038, 73.177, 0, 0, 0, 0, 100, 0, 0),
(4441800, 6, -120.426, 34.491, 72.057, 0, 0, 0, 0, 100, 0, 0);

-- Remove bile o tron trash spawn
DELETE FROM `creature` WHERE `id` = 44418;