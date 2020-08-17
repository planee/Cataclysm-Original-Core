     /*
 * Copyright (C) 2010-2017 OMFG.GG <http://www.omfg.gg/>
 * Copyright (C) 2017-2017 WoWZealot <http://www.wowzealot.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef DEF_SHADOWFANG_H
#define DEF_SHADOWFANG_H

#define DataHeader "SK"

enum ShadowfangKeepBoss
{
   BOSS_BARON_ASHBURY             = 46962,
   BOSS_BARON_SILVERLAINE         =  3887,
   BOSS_COMMANDER_SPRINGVALE      =  4278,
   BOSS_LORD_GODFREY              = 46964,
   BOSS_LORD_WALDEN               = 46963,
};

enum Npcs
{
   NPC_TORMENTED_OFFICER                = 50615,
   NPC_WAILING_GUARDSMAN                = 50613,
   NPC_DREAD_SCRYER                     = 47141,
   NPC_DESECRATION_TR                   = 50503,
   NPC_TOXIN_TRIGGER                    = 50439,
   NPC_HAUNTED_TELEPORTER               = 51400,
   // Apothecary Trio - Love is in the Air Festival
   NPC_HUMMEL                           = 36296,
   NPC_BAXTER                           = 36565,
   NPC_FRYE                             = 36272,
   NPC_BLOODFANG_BERSERKER              = 47027, // ally version of 47030
   NPC_PACKLEADER_IVAR_BLOODFANG        = 47006, // ally versopm of 47293
   NPC_VETERAN_FORSAKEN_TROOPER         = 47030,
   NPC_DEATHSTALKER_COMMANDER_BELMOT    = 47293,
   NPC_POISON_STALKER                   = 23837,
   NPC_HIGH_WARLORD_CROMUSH             = 47294,
   NPC_SPITEBONE_FLAYER                 = 47146,
   NPC_DREADSCRYER                      = 47141
};

enum Achievements
{
   ACHIEVEMENT_TO_THE_GROUND      = 5504,
};

enum GameObjectIds
{
   GO_BARON_ASHBURY_DOOR          = 18895,
   GO_LORD_GODFREY_DOOR           = 18971,
   GO_SORCERS_GATE                = 18972,
};

enum Data
{
    DATA_BARON_ASHBURY_EVENT,
    DATA_BARON_SILVERLAINE_EVENT,
    DATA_COMMANDER_SPRINGVALE_EVENT,
    DATA_LORD_GODFREY_EVENT,
    DATA_LORD_WALDEN_EVENT,
    MAX_ENCOUNTER,

    TEAM_IN_INSTANCE,
    DATA_GODFREY_PRE_EVENT
};

enum TeleporterDatas
{
    DATA_TELEPORT_HIGH_HALL = 1,
    DATA_TELEPORT_CHAPELLE,
    DATA_TELEPORT_REMPARTS,
    DATA_TELEPORT_LABO,
};

enum Data64
{
    DATA_BARON_ASHBURY,
    DATA_BARON_SILVERLAINE,
    DATA_COMMANDER_SPRINGVALE,
    DATA_LORD_GODFREY,
    DATA_LORD_WALDEN
};

enum DungeonSpells
{
    SPELL_DISEASE_CLOUD = 88198,
};

#endif
