SET @GROUP_ID = 0;
SET @ID = 0;

-- Fix instance entrance initial spawn
UPDATE creature SET id = 48964 WHERE id = 49226;

DELETE FROM `creature_text` WHERE `Entry` IN (41379,43396,48964,49226,49580,49799);
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `comment`) VALUES
(41379, @GROUP_ID+0, @ID+0, 'Ha ha ha ha ha! The heroes have made it to the glorious finale. I take it you are in good spirits? Prepared for the final battle? Then gaze now upon my ultimate creation! RISE, SISTER!', 14, 0, 100, 1, 0, 20066, 0, 'Lord Victor Nefarius to Player'),
(41379, @GROUP_ID+1, @ID+0, 'Behold the brood mother, Onyxia, REBORN! Perhaps my finest work.', 14, 0, 100, 22, 0, 20070, 0, 'Lord Victor Nefarius to Player'),
(41379, @GROUP_ID+2, @ID+0, 'My dearest sibling, do you hunger? THEN FEAST UPON OUR ENEMIES!', 14, 0, 100, 22, 0, 20071, 0, 'Lord Victor Nefarius to Player'),

-- Intro
(48964, @GROUP_ID+0, @ID+0, 'Mortals that fancy themselves HEROES have entered the Broken Hall. Oh, I do hope this \"raid\" will amuse me more than the last.', 14, 0, 100, 0, 0, 23379, 0, 'Lord Victor Nefarius to Player'),
(48964, @GROUP_ID+1, @ID+0, 'Welcome to my personal sanctum, heroes! I hope you find the accommodations to your liking. Herein you will find many of my failed experiments and ultimately my greatest triumph!',14,0,100,0,0,20090,49415,'VO_BD_Nefarian_Event22'),

-- Chimareon
(48964, @GROUP_ID+2, @ID+0, 'Ah, Chimaeron, truly a living testament to my scientific prowess. I reworked and twisted his form countless times over the years, and the final result is truly something to behold.', 14, 0, 100, 0, 0, 23362, 0, 'Lord Victor Nefarius to Player'),
(48964, @GROUP_ID+3, @ID+0, 'This fight has gotten a bit stale; allow me to spice things up!', 14, 0, 100, 0, 0, 23365, 0, 'Lord Victor Nefarius to Chimaeron'),
(48964, @GROUP_ID+4, @ID+0, 'Hmm... a shame to lose that experiment.', 14, 0, 100, 0, 0, 23361, 0, 'Lord Victor Nefarius to Player'),
(48964, @GROUP_ID+5, @ID+0, 'Stop fighting yourself THIS INSTANT Chimaeron!',14,0,100,0,0,23364,0, 'Nefarius Chimaeron'),
(48964, @GROUP_ID+6, @ID+0, 'Impressive! You managed to destroy one of my most horrific creations - a task I\'d thought impossible until now.',14,0,100,0,0,20088,0,'Nefarius Chimaeron - SAY_NEFARIUS_OUTRO_NORMAL'),

-- Omnontrom defense system
(49226, @GROUP_ID+0, @ID+0, 'Hmm... the Omnotron Defense System. Centuries ago these constructs were considered the Dwarves\' greatest technical achievements. With so many counters to each construct\'s attacks, I\'ll have to rectify these designs for them to provide me any entertainment.', 14, 0, 100, 0, 0, 23374, 0, 'Lord Victor Nefarius to Player'),
(49226, @GROUP_ID+1, @ID+0, 'Stupid Dwarves and your fascination with runes! Why would you create something that would help your enemy?', 14, 0, 100, 0, 0, 23376, 0, 'Lord Victor Nefarius to Arcanotron'),
(49226, @GROUP_ID+2, @ID+0, 'I should\'ve known better than to rely on something so stupidly named... to entertain me for long.',14,0,100,0,0,23373,49059,'VO_BD_Nefarian_OmnitronDead01'),
(49226, @GROUP_ID+3, @ID+0, 'You would like to move away from that, wouldn\'t you? Too bad.',14,0,100,0,0,23375,49055,'VO_BD_Nefarian_OmnitronSpell01'),
(49226, @GROUP_ID+4, @ID+0, 'Lightning Conductor works much better in a crowd, yes?',14,0,100,0,0,23377,49057,'VO_BD_Nefarian_OmnitronSpell03'),
(49226, @GROUP_ID+5, @ID+0, 'Were you planing on using Toxitron\'s chemicals to damage the other constructs? Clever plan. Let me ruin that for you.',14,0,100,0,0,23378,49058,'VO_BD_Nefarian_OmnitronSpell04'),
(49226, @GROUP_ID+6, @ID+0, 'That malfunctioning piece of junk was murder on the repair bills.',14,0,100,0,0,20086,49411,'VO_BD_Nefarian_Event18'),

-- Magmaw
-- Weird, nefarian (49427) is the actually speech actor here, but sniffs get victor nefarious id for it?
-- (49226, @GROUP_ID+7, @ID+0,'I found this fascinating speciman in the lava underneath this very room. Magmaw should provide an adequate challenge for your pathetic little band!',14,0,100,0,0,23367,49060,'VO_BD_Nefarian_MagmawIntro01'),
-- (49226, @GROUP_ID+8, @ID+0,'Finally someone to deal with that unsightly pest! You have my thanks, "heroes."',14,0,100,0,0,20085,49410,'VO_BD_Nefarian_Event17'),
-- (49226, @GROUP_ID+9, @ID+0,'You won! How... disappointing. I could\'ve easily watched you all continue to stand in the fire.',14,0,100,0,0,23366,49062,'VO_BD_Nefarian_MagmawDead01'),
-- (49226, @GROUP_ID+11, @ID+0,'INCONCEIVEABLE! You may actually defeat my lava worm! Perhaps I can help... TIP the scales.',14,0,100,0,0,23368,49782,'VO_BD_Nefarian_MagmawLowHealth'),
-- (49226, @GROUP_ID+12, @ID+0,'On second thought, what fun is an adequate challenge?',14,0,100,0,0,23369,49061,'VO_BD_Nefarian_MagmawSummon01'),
-- 
-- Atramedes intro
(43396, @GROUP_ID+0, @ID+0, 'Ah, Maloriak. How gracious of you to finally appear from that quarantine you call a laboratory. What have you got this time?', 12, 0, 100, 1, 0, 20091, 0, 'Lord Victor Nefarius'),
(43396, @GROUP_ID+1, @ID+0, 'By all means, enlighten me.',12,0,100,1,0,20092,43489,'Atramedes intro - Nefarian'),
(43396, @GROUP_ID+2, @ID+0, 'It appears as though your experiment has failed. That whelp has gone completely blind! Look at him.',12,0,100,1,0,20093,43490,'Atramedes intro - Nefarian'),
(43396, @GROUP_ID+3, @ID+0, 'LOOK AT HIM!',12,0,100,25,0,20156,43491,'Atramedes intro - Nefarian'),
(43396, @GROUP_ID+4, @ID+0, 'No, not yet.',12,0,100,1,0,20094,43492,'Atramedes intro - Nefarian'),
(43396, @GROUP_ID+5, @ID+0, 'This object of your chagrin may still prove... valuable... to me.',12,0,100,1,0,20095,43493,'Atramedes intro - Nefarian'),
(43396, @GROUP_ID+6, @ID+0, 'Leave now, pathetic alchemist. I anxiously await your next failure.',12,0,100,1,0,20096,43494,'Atramedes intro - Nefarian'),

-- Atramedes fight
(49580, @GROUP_ID+0, @ID+0, 'Atramedes, are you going deaf as well as blind? Hurry up and kill them all.', 14, 0, 100, 0, 0, 23360, 0, 'Lord Victor Nefarius to Player'),
(49580, @GROUP_ID+1, @ID+0, 'Atramedes, the heroes are right THERE!', 14, 0, 100, 0, 0, 23359, 0, 'Lord Victor Nefarius to Atramedes'),
(49580, @GROUP_ID+2, @ID+0, 'You should\'ve been disposed of a long time ago.',14,0,100,0,0,23357,49077,'VO_BD_Nefarian_AtramedesDead'),
(49580, @GROUP_ID+3, @ID+0, 'Now that was really unnecessary.',14,0,100,0,0,23387,49412,'VO_BD_Nefarian_Event19'),
(49580, @GROUP_ID+4, @ID+0, 'These shields are proving quite the nuisance!',14,0,100,0,0,23358,49076,'VO_BD_Nefarian_AtramedesGong'),

-- Maloryak fight
(49799, @GROUP_ID+0, @ID+0, 'Maloriak, try not to lose to these mortals. Semicompetent help is SO hard to create.', 14, 0, 100, 0, 0, 23372, 0, 'Lord Victor Nefarius to Player'),
(49799, @GROUP_ID+1, @ID+0, 'Your mixtures are weak, Maloriak! They need a bit more... kick!', 14, 0, 100, 0, 0, 23370, 0, 'Lord Victor Nefarius to Maloriak'),
(49799, @GROUP_ID+2, @ID+0, '|TInterface\\Icons\\INV_ELEMENTAL_PRIMAL_SHADOW.BLP:20|t%s throws |cFF660099dark|r magic into the cauldron!', 41, 0, 100, 0, 0, 0, 0, 'Lord Victor Nefarius to Maloriak'),
(49799, @GROUP_ID+3, @ID+0, 'Congratulations! Allow me to grant you a title befitting the amazing achievement you just performed. Henceforth you shall be known as the \"Slayer of Stupid, Incompetent, and Disappointing Minions.\"', 14, 0, 100, 21, 0, 23371, 0, 'Lord Victor Nefarius to Player');