-- Thassarian Bloody breakout escort points.
DELETE FROM `script_waypoint` WHERE `entry`=28912;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES
(28912,0,1653.52,-6038.37,127.585,1000,'Jump off'),
(28912,1,1653.98,-6034.61,127.585,5000,'To Box'),
(28912,2,1653.85,-6034.73,127.585,0,'Equip'),
(28912,3,1652.3,-6035.67,127.585,1000,'Recover'),
(28912,4,1639.76,-6046.34,127.948,0,'Escape'),
(28912,5,1640.96,-6028.12,134.74,0,''),
(28912,6,1625.81,-6029.2,134.74,0,''),
(28912,7,1626.84,-6015.08,134.74,0,''),
(28912,8,1649.15,-6016.98,133.24,0,''),
(28912,9,1653.06,-5974.84,132.652,5000,'Mount'),
(28912,10,1654.75,-5926.42,121.191,0,'Disappear');

-- Where Kings Walk: teleport to stormwind
DELETE FROM `spell_target_position` WHERE `id`=17334;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(17334, 0, -8998.14, 861.254, 29.6206, 2.25);


