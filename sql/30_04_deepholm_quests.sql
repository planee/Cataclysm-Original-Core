/*Quests*/
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'npc_deepholme_therazane' WHERE `entry`= 42465;
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'generic_creature' WHERE `entry`= 43809;
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'npc_aeosera' WHERE `entry`= 43641;
UPDATE `creature_template` SET `AIName`= '', `ScriptName`= 'npc_boulder_platform' WHERE `entry`= 45191;

DELETE FROM `creature` WHERE `id`= 45191;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (43809, 42465, 43641, 45191);

DELETE FROM `creature_text` WHERE `entry` IN (42465, 43809);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `type`, `language`, `probability`, `emote`, `sound`, `comment`, `BroadcastTextId`, `text`) VALUES
(42465, 0, 0, 12, 0, 100, 1, 21093, '', 43869, 'And so the "champion" arrives.'),
(42465, 1, 0, 12, 0, 100, 1, 21094, '', 43870, 'So small.  So soft.  It''s a wonder you''ve even lasted this long down here, fleshling.'),
(42465, 2, 0, 12, 0, 100, 1, 21095, '', 43871, 'Or perhaps a testament to your abilities?'),
(42465, 3, 0, 12, 0, 100, 1, 21096, '', 43872, 'All of this for a piece of the World Pillar?  What exactly, shaman, do you intend to do with it?'),
(43809, 0, 0, 12, 0, 100, 1, 21117, '', 43879, 'We came here to seal da rift Deathwing tore into Azeroth. We need the World Pillar to repair da hole on dis side.'),
(42465, 4, 0, 12, 0, 100, 1, 21097, '', 43873, 'And what then? Will you leave Deepholm to its denizens?'),
(43809, 1, 0, 12, 0, 100, 1, 21118, '', 43880, 'We''ll leave. We''ve no desire to stay where we''re not welcome.'),
(42465, 5, 0, 12, 0, 100, 1, 21098, '', 43874, 'I''m glad you''re at least that perceptive.'),
(42465, 6, 0, 12, 0, 100, 1, 21099, '', 43875, 'I propose this: We will march on the Twilight cult and extinguish any utterings of Deathwing''s name from this realm.  Your Earthen Ring will march with us.'),
(42465, 7, 0, 12, 0, 100, 1, 21100, '', 43876, 'You do this, and I will send Gorsik to the temple with our piece of the World Pillar.'),
(42465, 8, 0, 12, 0, 100, 1, 21101, '', 43877, 'Seal the rift and leave this place.'),
(43809, 2, 0, 12, 0, 100, 1, 21119, '', 43881, 'We''ll be doin'' exactly that then. Ya have our tanks, Stonemother.'),
(42465, 9, 0, 12, 0, 100, 1, 21102, '', 43878, 'Save your thanks.  We march upon the Twilight Precipice as soon as our forces are gathered. I expect to see you on the battlefield.');

DELETE FROM `gossip_menu_option` WHERE `menu_id`= 12009;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_id`, `npc_option_npcflag`, `OptionBroadcastTextID`, `option_text`) VALUES
(12009, 0, 1, 1, 45522, 'I''ve come at your call, Stonemother.');