-- https://www.wowhead.com/quest=24774/proving-pit

DELETE FROM `npc_text` WHERE `ID`=15251;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `BroadcastTextID0`, `BroadcastTextID1`, `BroadcastTextID2`, `BroadcastTextID3`, `BroadcastTextID4`, `BroadcastTextID5`, `BroadcastTextID6`, `BroadcastTextID7`, `BroadcastTextID8`, `WDBVerified`) VALUES (15251, 'This ain\'t much of a big challenge, mon, but we gotta know ya can handle the pit before we can take ya in.', 'This ain\'t much of a big challenge, mon, but we gotta know ya can handle the pit before we can take ya in.', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15595);

DELETE FROM `creature_text` WHERE `entry`=39062;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(39062, 1, 0, 'Get in the pit and show us your stuff, $G boy:girl;.', 12, 0, 100, 1, 0, 0, 'Jailor'),
(39062, 2, 0, 'Well done, $n!', 12, 0, 100, 0, 0, 0, 'Jailor');

DELETE FROM `creature_text` WHERE `entry`=38142;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(38142, 0, 0, 'They sssend you to your death youngling.', 12, 0, 100, 0, 0, 0, 0, 'Captive Spitescale Scout Quote 1'),
(38142, 0, 1, 'I sshal ssslaughter you, Darksspear runt!', 12, 0, 100, 0, 0, 0, 0, 'Captive Spitescale Scout Quotes 2'),
(38142, 0, 2, 'I sshal tasste your blood, landling.', 12, 0, 100, 0, 0, 0, 39092, 'Captive Spitescale Scout Quote 3'),
(38142, 0, 3, 'The Sssea Witch will kill you all.', 12, 0, 100, 0, 0, 0, 39090, 'Captive Spitescale Scout Quote 4');

UPDATE `gameobject_template` SET `faction`='4' WHERE  `entry`=201968;

