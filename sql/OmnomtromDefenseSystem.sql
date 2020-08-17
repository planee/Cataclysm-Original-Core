-- Omnontrom defense system

-- Trash conditons
DELETE FROM `conditions` WHERE `Comment` LIKE '%Recharge - target%' AND `ConditionTypeOrReference` = 18;

-- Arcanotron power generator
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=3 AND `SourceEntry`=79629 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=42166 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 3, 79629, 0, 0, 31, 0, 3, 42166, 0, 0, 0, 0, '', 'Power Generator - target Arcanotron');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=3 AND `SourceEntry`=79629 AND `SourceId`=0 AND `ElseGroup`=1 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=42179 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 3, 79629, 0, 1, 31, 0, 3, 42179, 0, 0, 0, 0, '', 'Power Generator - target Electron');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=3 AND `SourceEntry`=79629 AND `SourceId`=0 AND `ElseGroup`=2 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=42178 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 3, 79629, 0, 2, 31, 0, 3, 42178, 0, 0, 0, 0, '', 'Power Generator - target Magmatron');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=3 AND `SourceEntry`=79629 AND `SourceId`=0 AND `ElseGroup`=3 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=4 AND `ConditionValue2`=0 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 3, 79629, 0, 3, 31, 0, 4, 0, 0, 0, 0, 0, '', 'Power Generator - target players');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=3 AND `SourceEntry`=79629 AND `SourceId`=0 AND `ElseGroup`=4 AND `ConditionTypeOrReference`=31 AND `ConditionTarget`=0 AND `ConditionValue1`=3 AND `ConditionValue2`=42180 AND `ConditionValue3`=0;
INSERT INTO `conditions` VALUES (13, 3, 79629, 0, 4, 31, 0, 3, 42180, 0, 0, 0, 0, '', 'Power Generator - target Toxitron');
