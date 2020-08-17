DELETE FROM `creature` WHERE `id` in (38987, 38989);

-- It's a hard knock life.. for us
SET @GUID = 9760000;

DELETE FROM `creature` WHERE `guid` between @GUID and @GUID+31;
DELETE FROM `creature_addon` WHERE `guid` between @GUID and @GUID+31;

INSERT INTO creature_addon (`guid`, `mount`, `bytes1`, `bytes2`) VALUES
(@GUID+0,  0, 256, 0),
(@GUID+1,  0, 256, 0),
(@GUID+2,  0, 256, 0),
(@GUID+3,  0, 256, 0),
(@GUID+4,  0, 256, 0),
(@GUID+5,  0, 256, 0),
(@GUID+6,  0, 256, 0),
(@GUID+7,  0, 256, 0),
(@GUID+8,  0, 256, 0),
(@GUID+9,  0, 256, 0),
(@GUID+10, 0, 65536, 1),
(@GUID+11, 0, 65536, 1),
(@GUID+12, 0, 0, 1),
(@GUID+13, 0, 0, 1),
(@GUID+14, 0, 0, 1),
(@GUID+15, 0, 0, 1),
(@GUID+16, 0, 65536, 1),
(@GUID+17, 0, 65536, 257),

(@GUID+18, 14341, 65536, 1),
(@GUID+19, 14341, 65536, 1),
(@GUID+20, 14341, 65536, 1),
(@GUID+21, 14341, 65536, 1),
(@GUID+22, 14341, 65536, 1),
(@GUID+23, 14341, 65536, 1),
(@GUID+24, 14341, 65536, 1),
(@GUID+25, 14341, 65536, 1),
(@GUID+26, 14341, 65536, 1),
(@GUID+27, 14341, 65536, 1),
(@GUID+28, 14341, 65536, 1),
(@GUID+29, 14341, 65536, 1),
(@GUID+30, 14341, 65536, 1);

INSERT INTO creature (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `equipment_id`, `npcflag`, `unit_flags`) VALUES

(@GUID+0 , 38217, 1, -1266.214, -5565.162, 21.09024, 2.513274,  1,  0, 0, 33280),
(@GUID+1 , 38217, 1, -1239.809, -5553.768, 15.30029, 0.3490658, 1,  0, 0, 33280),
(@GUID+2 , 38217, 1, -1243.125, -5535.399, 16.17907, 0.2617994, 1,  1, 0, 33280),
(@GUID+3 , 38217, 1, -1299.207, -5576.774, 24.49938, 0.7330383, 1,  0, 0, 33280),
(@GUID+4 , 38217, 1, -1294.714, -5580.677, 24.49858, 1.099557,  1,  0, 0, 33280),
(@GUID+5 , 38217, 1, -1293.903, -5544.651, 21.25047, 5.340707,  1,  0, 0, 33280),
(@GUID+6 , 38217, 1, -1303.161, -5506.752, 14.84873, 2.408554,  1,  0, 0, 33280),
(@GUID+7 , 38217, 1, -1319.191, -5509.28,  14.92528, 1.605703,  1,  0, 0, 33280),
(@GUID+8 , 38217, 1, -1279.5,   -5530.799, 20.92569, 4.572762,  1,  0, 0, 33280),
(@GUID+9 , 38440, 1, -1329.891, -5556.582, 21.55126, 4.276057,  17, 1, 2, 33088), 
(@GUID+10, 38931, 1, -1327.415, -5557.939, 21.5484,  1.761504,  17, 1, 1, 33536),
(@GUID+11, 38987, 1, -1337.813, -5564.8,   20.98915, 0.6108652, 1,  0, 0, 32768),
(@GUID+12, 38987, 1, -1328.398, -5561.901, 21.38232, 0.122173,  1,  0, 0, 32768),
(@GUID+13, 38987, 1, -1342.337, -5562.295, 20.66571, 0.3141593, 1,  0, 0, 32768),
(@GUID+14, 38987, 1, -1342.42,  -5557.662, 20.90166, 5.899213,  1,  0, 0, 32768),
(@GUID+15, 38989, 1, -1330.837, -5558.896, 21.48784, 0.8475284, 17, 0, 1, 33024),
(@GUID+16, 39027, 1, -1337.078, -5553.458, 21.70512, 4.045179,  17, 1, 0, 32832),
(@GUID+17, 39028, 1, -1318.168, -5527.401, 20.77204, 5.410521,  17, 0, 0, 134250752),
(@GUID+18, 39028, 1, -1321.035, -5530.469, 20.8207,  5.427974,  17, 0, 0, 134250752),
(@GUID+19, 39028, 1, -1271.615, -5545.944, 21.07296, 0.296706,  17, 0, 0, 134250752),
(@GUID+20, 39028, 1, -1342.302, -5538.684, 20.81846, 5.358161,  17, 1, 0, 134250752),
(@GUID+21, 39028, 1, -1273.116, -5553.075, 21.0282,  0.1570796, 17, 0, 0, 134250752),
(@GUID+22, 39028, 1, -1324.198, -5533.587, 20.85115, 5.427974,  17, 0, 0, 134250752),
(@GUID+23, 39028, 1, -1337.073, -5538.205, 20.82421, 5.410521,  17, 0, 0, 134250752),
(@GUID+24, 39028, 1, -1268.313, -5547.897, 20.94538, 0.2792527, 17, 0, 0, 134250752),
(@GUID+25, 39028, 1, -1273.22,  -5549.733, 21.00912, 0.2268928, 17, 0, 0, 134250752),
(@GUID+26, 39028, 1, -1327.352, -5536.042, 21.06844, 5.51524,   17, 0, 0, 134250752),
(@GUID+27, 39028, 1, -1267.925, -5552.052, 21.01184, 0.296706,  17, 0, 0, 134250752),
(@GUID+28, 39028, 1, -1270.184, -5556.087, 21.14443, 0.2268928, 17, 0, 0, 134250752),
(@GUID+29, 39028, 1, -1331.458, -5537.667, 21.02953, 5.427974,  17, 0, 0, 134250752),
(@GUID+30, 39028, 1, -1294.651, -5565.743, 21.22984, 0.7330383, 17, 0, 0, 134250752);

UPDATE `creature` SET `MovementType` = 1, `spawndist` = 5 WHERE `guid` = @GUID+16;

UPDATE `creature_template` SET `VehicleId`='664', `AIName`='', `ScriptName`='npc_bloodtalon_thrasher', `speed_run` = 2.01 WHERE  `entry`=38991;

UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 38440;

DELETE FROM smart_scripts WHERE entryorguid=38440 AND source_type=0;

INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES 
--  EnOrGUID |  SrcType |       ID |     LINK |  EvtType | EvtPhase | EvtChanc | EvtFlags | EvtPara1 | EvtPara2 | EvtPara3 | EvtPara4 | EvtPara5 |  ActType | ActPara1 | ActPara2 | ActPara3 | ActPara4 | ActPara5 | ActPara6 |  TrgType | TrgPara1 | TrgPara2 | TrgPara3 | TrgPara4 |  TrgPosX |  TrgPosY |  TrgPosZ |  TrgPosO |  Comment |
  (     38440,         0,         1,         0,        19,         0,       100,         0,     25035,         0,         0,         0,         0,         1,         0,         0,         0,         0,         0,         0,        19,         38989,         10,         0,         0,         0,         0,         0,         0, ""),
  (     38440,         0,         0,         0,         1,         0,       100,         0,      2000,      4000,      4000,      4000,         0,        10,         1,         6,       273,         0,         0,         0,         1,         0,         0,         0,         0,         0,         0,         0,         0, "");

DELETE FROM smart_scripts WHERE entryorguid=38989 AND source_type=0;

INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES 
--  EnOrGUID |  SrcType |       ID |     LINK |  EvtType | EvtPhase | EvtChanc | EvtFlags | EvtPara1 | EvtPara2 | EvtPara3 | EvtPara4 | EvtPara5 |  ActType | ActPara1 | ActPara2 | ActPara3 | ActPara4 | ActPara5 | ActPara6 |  TrgType | TrgPara1 | TrgPara2 | TrgPara3 | TrgPara4 |  TrgPosX |  TrgPosY |  TrgPosZ |  TrgPosO |  Comment |
  (     38989,         0,         1,         2,        62,         0,       100,         0,     11131,         0,         0,         0,         0,        72,         0,         0,         0,         0,         0,         0,         7,         0,         0,         0,         0,         0,         0,         0,         0, "On Gossip Menu Select - Close Gossip - Invoker"),
  (     38989,         0,         2,         0,        61,         0,       100,         0,         0,         0,         0,         0,         0,        85,     73208,         0,         0,         0,         0,         0,         0,         0,         0,         0,         0,         0,         0,         0,         0, "On Link - Invoker Cast Spell Summon Possessed Bloodtalon Thrasher"),
  (     38989,         0,         0,         0,         1,         0,       100,         0,      3000,      5000,      4000,      4000,         0,        10,         1,         6,       273,         0,         0,         0,         1,         0,         0,         0,         0,         0,         0,         0,         0, "");



UPDATE `creature_template` SET `gossip_menu_id`='15500', `minlevel`='2', `maxlevel`='2' WHERE  `entry`=38931;

DELETE FROM `gossip_menu` WHERE `entry`=15500 AND `text_id`=15500;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES (15500, 15500);

DELETE FROM `creature_equip_template` WHERE `entry` in (38440, 38931, 39027, 39028, 38966, 38217);
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES 
(38440, 1, 36791, 0, 0),
(39027, 1, 19903, 13319, 0),
(39028, 1, 28914, 0, 0),
(38966, 1, 14085, 0, 14118),
(38217, 1, 28914, 0, 2507),
(38931, 1, 52875, 0, 0);

UPDATE `creature_template` SET `equipment_id`='38217' WHERE  `entry`=38217;
UPDATE `creature_template` SET `equipment_id`='38931' WHERE  `entry`=38931;
UPDATE `creature_template` SET `equipment_id`='39028' WHERE  `entry`=39028;

DELETE FROM `creature_sparring_template` WHERE `AttackerEntry` IN (38324, 38326, 38301, 38300);
INSERT INTO `creature_sparring_template` (`AttackerEntry`, `victimEntry`, `HealthLimitPct`) VALUES
(38324,38301,50),
(38324,38300,50),
(38326,38301,50),
(38326,38300,50),
(38301,38324,50),
(38301,38326,50),
(38300,38324,50),
(38300,38326,50);
