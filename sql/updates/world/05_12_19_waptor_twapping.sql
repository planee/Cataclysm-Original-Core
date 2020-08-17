
DELETE FROM `smart_scripts` WHERE `entryorguid` in (3256, 3483100, 34831);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
 (3256, 0, 0, 1, 8, 0, 100, 0, 66282, 0, 5000, 5000, 69, 2, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - On Spellhit- Move To Invoker'),
 (3256, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - Link - Set React State'),
 (3256, 0, 2, 0, 34, 0, 100, 0, 0, 2, 0, 0, 80, 3483100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Sunscale Scytheclaw - On movement inform - Start Script'),
 (3483100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Update - Set Root'),
 (3483100, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 86, 66284, 0, 19, 34831, 6, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Update - Cast Shrink'),
 (3483100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Update - Set Faction'),
 (3483100, 9, 3, 0, 0, 0, 100, 0, 1500, 1500, 0, 0, 86, 66286, 0, 19, 34831, 6, 0, 19, 34831, 6, 0, 0, 0, 0, 0, 0, 'Update - Cast Fill the Waptor Twap'),
 (3483100, 9, 4, 0, 0, 0, 100, 0, 500, 500, 0, 0, 41, 0, 0, 0, 0, 0, 0, 19, 34831, 6, 0, 0, 0, 0, 0, 0, 'Update - Despawn Trap'),
 (3483100, 9, 5, 0, 0, 0, 100, 0, 1900, 1900, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Update - Despawn Self'),
 (34831, 0, 0, 0, 60, 0, 100, 1, 1000, 1000, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Waptor Twap - Update - Say Line 0'),
 (34831, 0, 1, 0, 60, 0, 100, 1, 1000, 1000, 0, 0, 11, 66282, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Waptor Twap - Update - Cast Waptor Twap Scweech');
