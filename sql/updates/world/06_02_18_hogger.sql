UPDATE creature_template set scriptname = 'npc_hogger' WHERE entry = 448;
UPDATE creature_template set scriptname = 'npc_general_marcus_jonathan' WHERE entry = 46942;
UPDATE creature_template set scriptname = 'npc_high_sorcerer_andromath' WHERE entry = 46941;
UPDATE creature_template set scriptname = 'npc_maginor_dumas' WHERE entry = 46940;
UPDATE creature_template set scriptname = 'npc_hogger_ragamuffin' WHERE entry = 46943;

DELETE from creature_text where entry in (448, 46942, 46941, 46940, 46943);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- hogger 
(448, 0, 0, 'More bones to gnaw on...', 14, 0, 100, 0, 0, 0, 1871, 'Hogger Agro Say 1'),
(448, 0, 1, 'Grrrr... fresh meat!', 14, 0, 100, 0, 0, 0, 1870, 'Hogger Agro Say 2'),
(448, 1, 0, 'Yipe! Help Hogger', 14, 0, 100, 0, 0, 0, 0, 'TALK_HELP_HOGGER'),
(448, 2, 0, 'Hogger is eating! Stop him!', 41, 0, 100, 0, 0, 0, 0, 'TALK_HURT_HOGGER'),
(448, 3, 0, 'No hurt Hogger!', 14, 0, 100, 0, 0, 0, 0, 'TALK_HURT_HOGGER'),
(448, 4, 0, 'Nooooo...', 12, 0, 100, 0, 0, 0, 0, 'TALK_HOGGER_NOOOO'),
(448, 5, 0, 'Hogger is stunned!', 41, 0, 100, 0, 0, 0, 0, 'TALK_HOGGER_STUNNED'),
-- general marcus jonathan 
(46942, 0, 0, 'Hold your blade, adventurer!', 14, 0, 100, 0, 0, 0, 0, 'TALK_HOLD_YOUR_BLADE'),
(46942, 1, 0, 'This beast leads the Riverpaw gnoll gang and may be the key to ending gnoll aggression in Elwynn.', 12, 0, 100, 0, 0, 0, 0, 'TALK_GENERAL_MARCUS_2'),
(46942, 2, 0, 'We\'re taking him into custody in the name of King Varian Wrynn.', 12, 0, 100, 0, 0, 0, 0, 'TALK_GENERAL_MARCUS_3'),
(46942, 3, 0, 'Take us to the Stockades, Andromath.', 12, 0, 100, 0, 0, 0, 0, 'TALK_GENERAL_MARCUS_3'),
-- ragamuffin
(46943, 0, 0, 'General Marcus Jonathan!', 12, 0, 100, 0, 0, 0, 0, 'TALK_RAGAMUFFIN_1'),
(46943, 1, 0, 'Wow!', 12, 0, 100, 0, 0, 0, 0, 'TALK_RAGAMUFFIN_2');