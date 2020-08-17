DELETE FROM `creature` WHERE `guid` = 820025; -- Non retail spawn
UPDATE `creature_template` SET `unit_flags` = 33555266 WHERE `entry` = 46965;

UPDATE `creature_template` SET `scale` = 1 WHERE `entry` IN (45676,50193); -- Void adds on chogall entrance

-- Chogall Outro
DELETE FROM `creature_text` WHERE `groupId` IN (8,10,11,12) AND `entry` = 46965;
INSERT INTO `creature_text`(entry,groupid,Id,`text`,`type`,`language`,probability,emote,duration,Sound,BroadCastTextId,`Comment`) VALUES
(46965,8,0,'Valiona, Theralion, put them in their place.',14,0,100,0,0,22063,0,'Cho\'gall'),
(46965,10,0,'Welcome, mortals. (Frail little children). Few have laid eyes on this inner sanctum. (They reach out to touch the fire!)',14,0,100,0,0,22065,0, 'Cho\gall'),
(46965,11,0,'It is here the chosen realize their destiny. (One with eternity). And it is here that you will realize yours.(End of the flesh. End of the flesh)!',14,0,100,0,0,22066,0,'Cho\gall'),
(46965,12,0,'Brothers of Twilight! The Hammer calls to you! (Fire water earth air). Leave your mortal shell behind. (Fire water earth air). Embrace your new forms for here and everafter! (Burn and drown and crush and suffocate). And use your gifts to destroy the unbelievers. (Burn and drown and crush and suffocate)!!',14,0,100,0,0,22067,0,'Cho\gall');

-- Chogall Boss
DELETE FROM `creature_text` WHERE `entry` = 43324 AND `groupid` = 22;
INSERT INTO `creature_text`(entry,groupid,Id,`text`,`type`,`language`,probability,emote,duration,Sound,BroadCastTextId,`Comment`) VALUES
(43324,22,0,'Foolish mortals-(Usurper\'s children!) nothing you have done- (Spawn of a lesser god!) I am TRYING to speak here. (Words, words, words. The Master wants murder.) ALL falls to chaos. ALL will be destroyed. (Chaos, chaos!) Your work here today changes nothing. (Chaos, chaos, all things end) No mortal may see what you have and live. Your end has come.',14,0,100,0,0,22068,0,'Cho\gall');
