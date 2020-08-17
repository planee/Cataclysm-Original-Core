DROP TABLE IF EXISTS `phase_definitions`;
CREATE TABLE `phase_definitions` (
       `zoneId` MEDIUMINT (7),
       `entry` SMALLINT (5),
       `phasemask` BIGINT (20),
       `phaseId` TINYINT (3),
       `originmap` SMALLINT (5),
       `terrainswapmap` SMALLINT (5),
       `flags` TINYINT (3),
       `comment` TEXT
);
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1519','1','129','0','0','0','0','Stormwind: [A] Heros Call: Vashj\'ir');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1519','2','257','0','0','0','0','Stormwind: [A] Heros Call: Hyjal');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1519','3','513','0','0','0','0','Stormwind: [A] Heros Call: Deepholm');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1519','4','1025','0','0','0','0','Stormwind: [A] Heros Call: Uldum');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1519','5','2049','0','0','0','0','Stormwind: [A] Heros Call: Twilight Highlands');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1637','1','129','0','0','0','0','Orgrimmar: [H] Warchiefs Command: Vashj\'ir');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1637','2','257','0','0','0','0','Orgrimmar: [H] Warchiefs Command: Hyjal');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1637','3','513','0','0','0','0','Orgrimmar: [H] Warchiefs Command: Deepholm');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1637','4','1025','0','0','0','0','Orgrimmar: [H] Warchiefs Command: Uldum');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('1637','5','2049','0','0','0','0','Orgrimmar: [H] Warchiefs Command: Twilight Highlands');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('616','1','0','165','0','719','0','Mount Hyjal: Default Terrainswap');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('876','1','2','0','0','0','0','[Example] Gm Island');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','7','32768','183','0','659','0','Lost Island Phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','6','16384','183','0','659','1','Lost Island Phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','5','32768','183','0','659','1','Lost Island Phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','4','32768','183','0','659','1','Lost Island Phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','3','8192','180','0','661','1','Lost Island Phase 3');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','2','4096','180','0','661','1','Lost Island');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','1','2048','180','0','661','0','Lost Island');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4720','8','1024','183','0','659','1','Lost Island Phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4714','1','4','182','654','655','1','Gilneas Havre du soir phase 4');
INSERT INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) VALUES('4714','2','8','186','654','656','1','Gilneas Havre du soir phase 8');
