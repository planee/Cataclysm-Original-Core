-- Through fire and flames

DELETE FROM `creature_involvedrelation` WHERE `id`=34284 AND `quest`=13878;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (34284, 13878);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (195001, 195003, 195004, -81313, -81314, -81315, 34285001, 34285002, 34285003, -180710, -180709, -81312);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(195001, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 81315, 34287, 0, 0, 0, 0, 0, 0, 'Wolf Chains - State Change - Set Data'),
(195001, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 33, 34285, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Quest Credit'),
(195001, 1, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Set Visibility Off'),
(195003, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 81313, 34287, 0, 0, 0, 0, 0, 0, 'Wolf Chains - On State Change - Set Data'),
(195003, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 33, 34285, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Quest Credit'),
(195003, 1, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Set Visibility Off'),
(195004, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 10, 81314, 34287, 0, 0, 0, 0, 0, 0, 'Wolf Chains - State Change - Set Data'),
(195004, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 33, 34285, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Quest Credit'),
(195004, 1, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 47, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Wolf Chains - Link - Set Visibility Off'),

(-81313, 0, 0, 0, 38, 0, 100, 0, 1, 1, 3000, 3000, 97, 10, 3, 1, 0, 0, 0, 1, 0, 0, 0, 0, -4.61, -3637.3, 28.21, 0, 'Chain Origin - On Data Set - Jump To Pos'),
(-81313, 0, 1, 2, 34, 0, 100, 0, 16, 1, 0, 0, 11, 65086, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - On Movement Inform - Cast Breaking The Chains'),
(-81313, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 70, 15, 0, 0, 0, 0, 0, 15, 195003, 9, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Respawn Wolf Chains'),
(-81313, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 80, 34285001, 0, 0, 0, 0, 0, 10, 180710, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Call TAL'),
(-81313, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Despawn Self'),
(-81313, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 10, 180710, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Remove Aura'),
(-81314, 0, 0, 0, 38, 0, 100, 0, 1, 1, 3000, 3000, 97, 10, 3, 1, 0, 0, 0, 1, 0, 0, 0, 0, -9.30419, -3625.09, 28.1245, 0, 'Chain Origin - On Data Set - Jump To Pos'),
(-81314, 0, 1, 2, 34, 0, 100, 0, 16, 1, 0, 0, 11, 65086, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - On Movement Inform - Cast Breaking The Chains'),
(-81314, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 70, 15, 0, 0, 0, 0, 0, 15, 195004, 9, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Respawn Wolf Chains'),
(-81314, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 80, 34285003, 0, 0, 0, 0, 0, 10, 81312, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Call TAL'),
(-81314, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Despawn Self'),
(-81314, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 10, 81312, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Remove Aura'),
(-81315, 0, 0, 0, 38, 0, 100, 0, 1, 1, 3000, 3000, 97, 10, 3, 1, 0, 0, 0, 1, 0, 0, 0, 0, -0.814201, -3632.55, 28.2692, 0, 'Chain Origin - On Data Set - Jump To Pos'),
(-81315, 0, 1, 2, 34, 0, 100, 0, 16, 1, 0, 0, 11, 65086, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - On Movement Inform - Cast Breaking The Chains'),
(-81315, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 70, 15, 0, 0, 0, 0, 0, 15, 195001, 9, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Respawn Wolf Chains'),
(-81315, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 80, 34285002, 0, 0, 0, 0, 0, 10, 180709, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Call TAL'),
(-81315, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Despawn Self'),
(-81315, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 10, 180709, 34285, 0, 0, 0, 0, 0, 0, 'Chain Origin - Link - Remove Aura'),

(-180710, 0, 0, 2, 1, 0, 100, 1, 1000, 1000, 0, 0, 86, 54549, 0, 10, 147523, 34287, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - OOC 1s - Cross Cast Cosmetic Chains'),
(-180710, 0, 1, 0, 34, 0, 100, 0, 8, 1, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - On Movement Inform - Despawn Self'),
(-180709, 0, 1, 0, 34, 0, 100, 0, 8, 1, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - On Movement Inform - Despawn Self'),
(-180709, 0, 0, 2, 1, 0, 100, 1, 1000, 1000, 0, 0, 86, 54549, 0, 10, 147521, 34287, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - OOC 1s - Cross Cast Cosmetic Chains'),
(-81312, 0, 1, 0, 34, 0, 100, 0, 8, 1, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - On Movement Inform - Despawn Self'),
(-81312, 0, 0, 2, 1, 0, 100, 1, 1000, 1000, 0, 0, 86, 54549, 0, 10, 147522, 34287, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - OCC 1s - Cross Cast Cosmetic Chains'),

(34285001, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - Update 0s - Set Speed'),
(34285001, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 97, 9, 9, 0, 0, 0, 0, 1, 0, 0, 0, 0, -5.737, -3630.89, 27.368, 0, 'Trapped Wolf - Update 0s - Jump To Pos'),
(34285001, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 41.9181, -3576.42, 27.987, 0, 'Trapped Wolf - Update 1s - Move To Pos'),

(34285002, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - Update 0s - Set Speed'),
(34285002, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 97, 9, 9, 0, 0, 0, 0, 1, 0, 0, 0, 0, -3.78, -3627.33, 27.311, 0, 'Trapped Wolf - Update 0s - Jump To Pos'),
(34285002, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 1.93808, -3602.74, 28.0277, 0, 'Trapped Wolf - Update 1s - Move To Pos'),

(34285003, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Trapped Wolf - Update 0s - Set Speed'),
(34285003, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 97, 9, 9, 0, 0, 0, 0, 1, 0, 0, 0, 0, -6.38756, -3629.25, 27.3484, 0, 'Trapped Wolf - Update 0s - Jump To Pos'),
(34285003, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 37.6664, -3606.33, 27.5026, 0, 'Trapped Wolf - Update 1s - Move To Pos');

