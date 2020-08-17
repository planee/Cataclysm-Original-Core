-- Races:
SET @HUMAN := 1;
SET @ORC := 2;
SET @DWARF := 3;
SET @NIGHT_ELF := 4;
SET @UNDEAD := 5;
SET @TAUREN := 6;
SET @GNOME := 7;
SET @TROLL := 8;
SET @GOBLIN := 9;
SET @BLOOD_ELF := 10;
SET @DRAENEI := 11;
SET @WORGEN := 22;
-- Classes:
SET @HUNTER := 3;

INSERT INTO `playercreateinfo_spell` (`race`,`class`,`Spell`,`Note`) VALUES 
-- Hunter Human
(@HUMAN,@HUNTER,60192,'Freezing Trap TL'),
(@HUMAN,@HUNTER,82939,'Explosive Trap TL'),
(@HUMAN,@HUNTER,82941,'Ice Trap TL'),
(@HUMAN,@HUNTER,82945,'Immolation Trap TL'),
(@HUMAN,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Orc
(@ORC,@HUNTER,60192,'Freezing Trap TL'),
(@ORC,@HUNTER,82939,'Explosive Trap TL'),
(@ORC,@HUNTER,82941,'Ice Trap TL'),
(@ORC,@HUNTER,82945,'Immolation Trap TL'),
(@ORC,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Dwarf
(@DWARF,@HUNTER,60192,'Freezing Trap TL'),
(@DWARF,@HUNTER,82939,'Explosive Trap TL'),
(@DWARF,@HUNTER,82941,'Ice Trap TL'),
(@DWARF,@HUNTER,82945,'Immolation Trap TL'),
(@DWARF,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Night Elf
(@NIGHT_ELF,@HUNTER,60192,'Freezing Trap TL'),
(@NIGHT_ELF,@HUNTER,82939,'Explosive Trap TL'),
(@NIGHT_ELF,@HUNTER,82941,'Ice Trap TL'),
(@NIGHT_ELF,@HUNTER,82945,'Immolation Trap TL'),
(@NIGHT_ELF,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Undead
(@UNDEAD,@HUNTER,60192,'Freezing Trap TL'),
(@UNDEAD,@HUNTER,82939,'Explosive Trap TL'),
(@UNDEAD,@HUNTER,82941,'Ice Trap TL'),
(@UNDEAD,@HUNTER,82945,'Immolation Trap TL'),
(@UNDEAD,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Tauren
(@TAUREN,@HUNTER,60192,'Freezing Trap TL'),
(@TAUREN,@HUNTER,82939,'Explosive Trap TL'),
(@TAUREN,@HUNTER,82941,'Ice Trap TL'),
(@TAUREN,@HUNTER,82945,'Immolation Trap TL'),
(@TAUREN,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Troll
(@TROLL,@HUNTER,60192,'Freezing Trap TL'),
(@TROLL,@HUNTER,82939,'Explosive Trap TL'),
(@TROLL,@HUNTER,82941,'Ice Trap TL'),
(@TROLL,@HUNTER,82945,'Immolation Trap TL'),
(@TROLL,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Goblin
(@GOBLIN,@HUNTER,60192,'Freezing Trap TL'),
(@GOBLIN,@HUNTER,82939,'Explosive Trap TL'),
(@GOBLIN,@HUNTER,82941,'Ice Trap TL'),
(@GOBLIN,@HUNTER,82945,'Immolation Trap TL'),
(@GOBLIN,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Blood Elf
(@BLOOD_ELF,@HUNTER,60192,'Freezing Trap TL'),
(@BLOOD_ELF,@HUNTER,82939,'Explosive Trap TL'),
(@BLOOD_ELF,@HUNTER,82941,'Ice Trap TL'),
(@BLOOD_ELF,@HUNTER,82945,'Immolation Trap TL'),
(@BLOOD_ELF,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Draenei
(@DRAENEI,@HUNTER,60192,'Freezing Trap TL'),
(@DRAENEI,@HUNTER,82939,'Explosive Trap TL'),
(@DRAENEI,@HUNTER,82941,'Ice Trap TL'),
(@DRAENEI,@HUNTER,82945,'Immolation Trap TL'),
(@DRAENEI,@HUNTER,82948,'Snake Trap TL'),
-- Hunter Worgen
(@WORGEN,@HUNTER,60192,'Freezing Trap TL'),
(@WORGEN,@HUNTER,82939,'Explosive Trap TL'),
(@WORGEN,@HUNTER,82941,'Ice Trap TL'),
(@WORGEN,@HUNTER,82945,'Immolation Trap TL'),
(@WORGEN,@HUNTER,82948,'Snake Trap TL')
