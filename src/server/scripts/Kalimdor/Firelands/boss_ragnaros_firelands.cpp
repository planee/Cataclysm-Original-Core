/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PassiveAI.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "firelands.h"

enum Texts
{
    // Cenarius
    SAY_CENARIUS_1              = 0,
    SAY_CENARIUS_OUTRO_1        = 1,
    SAY_CENARIUS_OUTRO_2        = 2,
    SAY_CENARIUS_OUTRO_3        = 3,

    // Malfurion
    SAY_MALFURION_1             = 0,
    SAY_MALFURION_OUTRO_1       = 1,
    SAY_MALFURION_OUTRO_2       = 2,

    // Hamuul
    SAY_HAMUUL_OUTRO_1          = 0,

    // Ragnaros
    SAY_ARRIVE                  = 0,
    SAY_DEATH_NORMAL            = 1,
    SAY_DEATH_HEROIC            = 2,
    SAY_SLAY                    = 3,
    SAY_ANNOUNCE_SPLIT          = 4,
    SAY_SUBMERGE                = 5,
    SAY_EMERGE                  = 6,
    SAY_SONS_OF_FLAME_1         = 7,
    SAY_SONS_OF_FLAME_2         = 8,
    SAY_ANNOUNCE_EMERGE         = 9,
    SAY_MAGMA_TRAP              = 10,
    SAY_ANNOUNCE_SULFURAS_SMASH = 11,
    SAY_SUBMERGE_HEROIC         = 12,
    SAY_INTRO_HEROIC_1          = 13,
    SAY_INTRO_HEROIC_2          = 14,
    SAY_ENGULFING_FLAMES        = 15,
    SAY_WORLD_IN_FLAMES         = 16,
    SAY_EMPOWER_SULFURAS        = 17,
    SAY_DREADFLAME              = 18,
    SAY_HEART_OF_FLAME_QUEST    = 19,
};

enum Spells
{
    // Ragnaros Base Auras
    SPELL_BASE_VISUAL                       = 98860,
    SPELL_BURNING_WOUNDS_AURA               = 99401,
    SPELL_BURNING_WOUNDS_PROC               = 99399,
    SPELL_BURNING_BLAST                     = 99400,
    SPELL_HITTING_YA                        = 94132,
    SPELL_HITTING_YA_PLAYER                 = 94131,
    // Magma
    SPELL_MAGMA_PERIODIC                    = 99908,
    SPELL_BERSERK                           = 110055,

    // Spell Sulfuras Smash
    SPELL_SULFURAS_SMASH_DUMMY              = 98703,
    SPELL_SULFURAS_SMASH                    = 98710,
    SPELL_SULFURAS_SMASH_MISSILE            = 98708,
    SPELL_SULFURAS_SMASH_TARGET             = 98706,

    // Lava Wave Summons
    SPELL_LAVA_WAVE_DUMMY_NORTH             = 98874,
    SPELL_LAVA_WAVE_DUMMY_EAST              = 98875,
    SPELL_LAVA_WAVE_DUMMY_WEST              = 98876,
    SPELL_LAVA_POOL                         = 98712,
    SPELL_SCORCHED_GROUND                   = 98871,
    SPELL_LAVA_WAVE_DAMAGE                  = 98928,

    // Magma Strike
    SPELL_MAGMA_STRIKE                      = 98313,

    // Magma Trap
    SPELL_MAGMA_TRAP                        = 98164,
    SPELL_MAGMA_TRAP_UNK                    = 98159,
    SPELL_MAGMA_TRAP_VISUAL                 = 98179,
    SPELL_MAGMA_TRAP_PERIODIC               = 98172,
    SPELL_MAGMA_TRAP_PERIODIC_TICK          = 98171,
    SPELL_MAGMA_TRAP_ERUPTION               = 98175,

    // Hand of Ragnaros
    SPELL_HAND_OF_RAGNAROS                  = 98237,

    // Wrath of Ragnaros
    SPELL_WRATH_OF_RAGNAROS                 = 98263,

    // Lava Wave
    SPELL_LAVA_WAVE                         = 98873,
    SPELL_LAVA_WAVE_TRIGGERED               = 98928,

    // Splitting Blow
    SPELL_SUBMERGE                          = 100051,
    SPELL_SUBMERGE_AURA                     = 98982,
    SPELL_LAVA_BOLT                         = 98981,
    SPELL_SPLITTING_BLOW_WEST               = 98953,
    SPELL_SPLITTING_BLOW_NORTH              = 98952,
    SPELL_SPLITTING_BLOW_EAST               = 98951,
    SPELL_SUMMON_SULFURAS_AURA              = 99056,

    // Sulfuras, Hand of Ragnaros
    SPELL_INVOKE_SONS                       = 99054,
    SPELL_INVOKE_SONS_MISSILE               = 99050,
    SPELL_INVOKE_SONS_TRIGGERED_SPELL       = 99051,
    SPELL_SULFURAS_AURA                     = 100456,

    // Son of Flame
    SPELL_HIT_ME                            = 100446,
    SPELL_PRE_VISUAL                        = 98983,
    SPELL_BURNING_SPEED                     = 98473,
    SPELL_BURNING_SPEED_STACKS              = 99414,
    SPELL_SUPERNOVA                         = 99112,

    // Engulfing Flames
    SPELL_ENGULFING_FLAMES_BOTTOM           = 99236,
    SPELL_ENGULFING_FLAMES_CENTER           = 99235,
    SPELL_ENGULFING_FLAMES_MELEE            = 99172,
    SPELL_ENGULFING_FLAMES_VISUAL_MELEE     = 99216,
    SPELL_ENGULFING_FLAMES_VISUAL_CENTER    = 99217,
    SPELL_ENGULFING_FLAMES_VISUAL_BOTTOM    = 99218,
    SPELL_WOLRD_IN_FLAMES                   = 100171,

    // Molten Seed
    SPELL_MOLTEN_SEED                       = 98333,
    SPELL_MOLTEN_SEED_SUMMON                = 100141,

    SPELL_MOLTEN_SEED_MISSILE               = 98495,
    SPELL_MOLTEN_SEED_DAMAGE                = 98498,
    SPELL_UNK_1                             = 98497,

    // Molten Elemental
    SPELL_INVISIBLE_PRE_VISUAL              = 100153,
    SPELL_MOLTEN_SEED_VISUAL                = 98520,
    SPELL_MOLTEN_INFERNO                    = 98518,
    SPELL_MOLTEN_POWER_AURA                 = 100157,
    SPELL_MOLTEN_POWER_PERIODIC             = 100158,
    SPELL_REDUCE_CRITICAL_STRIKE_CHANCE     = 64481,
    SPELL_AGGRO_DEMON_CREATOR               = 100142,

    // Lava Scion
    SPELL_BLAZING_HEAT                      = 100460,
    SPELL_BLAZING_HEAT_SONAR                = 100459,

    // Blazing Heat
    SPELL_BLAZING_HEAT_AURA                 = 99126,
    SPELL_BLAZING_HEAT_SUMMON               = 99129,
    SPELL_BLAZING_HEAT_DUMMY                = 99125,
    SPELL_BLAZING_HEAT_DAMAGE_AURA          = 99128,

    // Living Meteor
    SPELL_LIVING_METEOR_MISSILE             = 99268,
    SPELL_LIVING_METEOR_DAMAGE_REDUCTION    = 100904,
    SPELL_LIVING_METEOR_COMBUSTIBLE         = 99296,
    SPELL_LIVING_METEOR_COMBUSTITION        = 99303, // Knockback Spell (only castable if the meteor has the aura)
    SPELL_LIVING_METEOR_EXPLOSION_TRIGGER   = 99269,
    SPELL_LIVING_METEOR_INCREASE_SPEED      = 100278,
    SPELL_LIVING_METEOR_SPEED_TRIGGERED     = 100277,
    SPELL_LIVING_METEOR_FIXATE              = 99849,
    SPELL_LIVING_METEOR_SUMMON_METEOR       = 99267,
    SPELL_LIVING_METEOR_EXPLOSION           = 99287,

    // Heroic Encounter

    // Ragnaros
    SPELL_LEGS_HEAL                         = 100346,
    SPELL_TRANSFORM                         = 100420,
    SPELL_SUPERHEATED                       = 100593,
    SPELL_SUPERHEATED_TRIGGERED             = 100594,

    SPELL_EMPOWER_SULFURAS                  = 100604,
    SPELL_EMPOWER_SULFURAS_TRIGGER          = 100605,
    SPELL_EMPOWER_SULFURAS_MISSILE          = 100606,

    // Dreadflame
    SPELL_DREADFLAME_SUMMON_2               = 100679, // summon 54127 NPC_DREADFLAME 23:58:47.797
    // X: 1041.25 Y: -57.4478 Z: 55.5
    SPELL_DREADFLAME_CONTROL_AURA           = 100695, // aura of NPC_DREADFLAME 23:58:47.797
    SPELL_DREADFLAME_CONTROL_AURA_1         = 100965, // trigger SPELL_DREADFLAME_AURA_1_TRIGERED
    SPELL_DREADFLAME_AURA_1_TRIGERED        = 100966, // ??
    SPELL_DREADFLAME_CONTROL_AURA_2         = 100696, // 23:58:47.797 trigger SPELL_DREADFLAME_AURA_2_TRIGERED
    SPELL_DREADFLAME_AURA_2_TRIGERED        = 100823,
    SPELL_DREADFLAME_CONTROL_AURA_3         = 100905, // 23:58:47.922 trigger SPELL_DREADFLAME_AURA_3_TRIGERED
    SPELL_DREADFLAME_AURA_3_TRIGERED        = 100906,


    SPELL_DREADFLAME_MISSILE                = 100675, // 23:59:08.625 by ragna on DestPos
    //  X: 1103 Y: -33 Z: 55.5
    //  X: 1083 Y: -88 Z: 55.5
    SPELL_DREADFLAME_SUMMON                 = 100876, // summon 54203 NPC_DREADFLAME_SPAWN 23:59:12.656
    SPELL_DREADFLAME_PERIODIC_DAMAGE        = 100692, // aura of NPC_DREADFLAME_SPAWN 23:59:13.172 make NPC_DREADFLAME_SPAWN despawn after cast at 23:59:12.875 by NPC_DREADFLAME 23:59:15.812 23:59:19.328 23:59:22.922

    SPELL_DREADFLAME_PERIODIC_DUMMY         = 100691,
    SPELL_DREADFLAME_DAMAGE                 = 100941,
    SPELL_DREADFLAME_DUMMY                  = 100673, // maybe used for beserk?

    // Protection Traps
    SPELL_BREADTH_OF_FROST                  = 100472,
    SPELL_BREADTH_OF_FROST_MISSILE          = 100479,
    SPELL_BREADTH_OF_FROST_STUN             = 100567, // condition to target living meteors in all 3 spell effects
    SPELL_BREADTH_OF_FROST_PERIODIC         = 100478,
    SPELL_BREADTH_OF_FROST_PROTECTION       = 100503, // spell effect script effect needs spellscript for immunity spell id (100594)

    SPELL_ENTRAPPING_ROOTS                  = 100645,
    SPELL_ENTRAPPING_ROOTS_AURA_MISSILE     = 100646,
    SPELL_ENTRAPPING_ROOTS_ROOT             = 100653, // condition to target ragnaros

    SPELL_CLOUDBURST                        = 100751, // condition to target platform stalker / triggers summon spell on hit
    SPELL_CLOUDBURST_VISUAL                 = 100758, // visual for npc
    SPELL_CLOUDBURST_DELUGE                 = 100713,
    SPELL_CLOUDBURST_DELUGE_FIRE_REMOVE     = 100757,

    // Hamuul Runetotem
    SPELL_TRANSFORM_HAMUUL                  = 100311,
    SPELL_HAMUL_DRAW_FIRELORD               = 100344,

    // Malfurion Stormrage
    SPELL_TRANSFORM_MALFURION               = 100310,
    SPELL_MALFURION_DRAW_FIRELORD           = 100342,

    // Cenarius
    SPELL_CENARIUS_DRAW_FIRELORD            = 100345,
    SPELL_METEOR_FREEZE                     = 100907,

    // Heart of Ragnaros
    SPELL_RAGE_OF_RAGNAROS                  = 101110,
    SPELL_HEART_OF_RAGNAROS_SUMMON          = 101254, // summons the heart npc
    SPELL_HEART_OF_RAGNAROS_DUMMY_AURA      = 101127,
    SPELL_HEART_OF_RAGNAROS_CREATE_HEART    = 101125,

    // Magma Geyser
    SPELL_MAGMA_GEYSER                      = 100777,
    SPELL_MAGMA_GEYSER_PERIODIC             = 100858,

    SPELL_RAGNAROS_CREDITS                  = 102237,
};

enum Phases
{
    PHASE_INTRO     = 1,
    PHASE_1         = 2,
    PHASE_2         = 3,
    PHASE_3         = 4,
    PHASE_SUBMERGED = 5,
    PHASE_HEROIC    = 6,
};

enum Events
{
    // Ragnaros
    EVENT_INTRO = 1,
    EVENT_ATTACK,
    EVENT_SULFURAS_SMASH_TRIGGER,
    EVENT_SULFURAS_SMASH,
    EVENT_MAGMA_TRAP,
    EVENT_WRATH_OF_RAGNAROS,
    EVENT_HAND_OF_RAGNAROS,
    EVENT_SPLITTING_BLOW,
    EVENT_CALL_SONS,
    EVENT_ANNOUNCE_EMERGE,
    EVENT_EMERGE,
    EVENT_ENGULFING_FLAMES,
    EVENT_MOLTEN_SEED,
    EVENT_LIVING_METEOR,
    EVENT_ATTACK_HEROIC,
    EVENT_NORMAL_MODE_DEATH,
    EVENT_BERSERK,

    // Sulfuras Smash
    EVENT_SCORCH,
    EVENT_MOVE_LAVA_WAVE,
    EVENT_SUMMON_WAVE_1,
    EVENT_SUMMON_WAVE_2,
    EVENT_SUMMON_WAVE_3,

    // Magma Trap
    EVENT_PREPARE_TRAP,

    // Son of Flame
    EVENT_MOVE_HAMMER,
    EVENT_CHECK_HEALTH,

    // Sulfuras, Hand of Ragnaros
    EVENT_CHECK_SONS,

    // Molten Elemental
    EVENT_MOLTEN_SEED_MISSILE,
    EVENT_PREPARE_ELEMENTAL,
    EVENT_ACTIVATE,

    // Lava Scion
    EVENT_BLAZING_HEAT,

    // Living Meteor
    EVENT_STALK_PLAYER,
    EVENT_KILL_PLAYER,
    EVENT_ENABLE_KNOCKBACK,
    EVENT_REAPPLY_DAMAGE_REDUCE,

    // Archdruids
    EVENT_SAY_PULL,
    EVENT_SAY_CAUGHT,
    EVENT_DRAW_RAGNAROS,
    EVENT_TRANSFORM,

    // Ragnaros Heroic Events
    EVENT_EMERGE_HEROIC,
    EVENT_TALK,
    EVENT_TAUNT_EMOTE,
    EVENT_STANDUP,
    EVENT_FREEZE_PLATFORM,
    EVENT_BREAK_PLATFORM,
    EVENT_IDLE,
    EVENT_TRANSFORM_RAGNAROS,
    EVENT_DREADFLAME,
    EVENT_DREADFLAME_2,
    EVENT_SUMMON_ROOTS,
    EVENT_EMPOWER_SULFURAS,

    // Cenarius
    EVENT_BREADTH_OF_FROST,
    EVENT_TALK_CENARIUS_1,
    EVENT_TALK_CENARIUS_2,
    EVENT_TALK_CENARIUS_3,

    // Hamuul
    EVENT_ENTRAPPING_ROOTS,
    EVENT_TALK_HAMUUL_1,

    // Malfurion
    EVENT_CLOUDBURST,
    EVENT_TALK_MALFURION_1,
    EVENT_TALK_MALFURION_2,

    // Dreadflame
    EVENT_CHECK_PLAYER,
    EVENT_SPREAD_FLAME,

    // Dreadflame
    EVENT_SUMMON_DREADFLAME,

    // breadth of frost
    EVENT_CHECK_PLAYER_CLUMB,
};

enum AnimKits
{
    // Ragnaros
    ANIM_KIT_RESURFACE  = 1465,
    ANIM_KIT_EMERGE     = 1467,
    ANIM_KIT_TAUNT      = 1468,
    ANIM_KIT_STAND_UP   = 1486,
    ANIM_KIT_SUBMERGE   = 1522,
    // Malfurion
    ANIM_KIT_TALK       = 1523,

    // Son of Flame
    ANIM_KIT_UNK_1      = 1370,
};

enum RagnarosQuest
{
    QUEST_HEART_OF_FIRE_A = 29307,
    QUEST_HEART_OF_FIRE_H = 29308,

    ITEM_PRE_LEGENDARY_STAFF = 71085,
};

Position const RagnarosSummonPosition = {1075.201f, -57.84896f, 55.42427f,  3.159046f   };
Position const SplittingTriggerNorth  = {1023.55f,  -57.158f,   55.4215f,   3.12414f    };
Position const SplittingTriggerEast   = {1035.45f,  -25.3646f,  55.4924f,   2.49582f    };
Position const SplittingTriggerWest   = {1036.27f,  -89.2396f,  55.5098f,   3.83972f    };

Position const CenariusSummonPosition = {795.504f,  -60.138f,   83.652f,    0.02050f    };
Position const HamuulSummonPosition   = {790.017f,  -50.393f,   97.115f,    6.22572f    };
Position const MalfurionSummonPosition = {781.208f, -69.534f,   98.061f,    3.89547f    };

Position const MalfurionPoint         = {984.996f,  -73.638f,   55.348f  };
Position const CenariusPoint          = {984.1371f, -57.65625f, 55.36652f};
Position const HamuulPoint            = {982.9132f, -43.22049f, 55.35419f};

Position const CenterPlatform         = {1041.25f, -57.4478f, 55.5f};

/*
    Positions for Sons of Flame
*/

const Position SonsOfFlameHeroicPos[] =
{
    { 999.533f,  -45.74826f, 55.56229f, 5.009095f },
    { 1012.120f, -26.89063f, 55.56400f, 4.398230f },
    { 1013.458f, -68.08507f, 55.42097f, 2.513274f },
    { 1014.134f, -43.94445f, 55.42412f, 3.979351f },
    { 1014.325f, -88.80209f, 55.52722f, 1.919862f },
    { 1024.845f, -97.67882f, 55.52884f, 2.234021f },
    { 1030.372f, -23.63715f, 55.44154f, 4.049164f },
    { 1034.168f, -15.93056f, 55.60827f, 4.049164f },
    { 1040.394f, -91.81944f, 55.42585f, 2.234021f },
    { 1042.033f, -114.9132f, 55.44709f, 1.919862f },
    { 1051.760f, 0.1284722f, 55.44715f, 4.031711f },
    { 1051.497f, -113.7292f, 55.44935f, 2.391101f },
    { 1055.556f, -8.875000f, 55.43348f, 3.874631f },
    { 1061.340f, -16.74132f, 55.56819f, 3.769911f },
    { 1065.257f, -6.946181f, 55.56818f, 3.839724f },
    { 1065.372f, -108.8698f, 55.56829f, 2.373648f },
    { 1068.151f, -101.3924f, 55.56828f, 2.565634f },
    { 1074.866f, -100.7882f, 55.42414f, 2.600541f },
};

const Position SonsOfFlameNormalEastPos[] =
{
    { 1074.64f, -12.63f,  55.35f, 3.37f },
    { 1057.27f, -3.22f,   55.34f, 2.63f },
    { 996.03f,  -40.50f,  55.48f, 2.31f },
    { 996.83f,  -75.31f,  55.46f, 4.96f },
    { 1012.88f, -89.04f,  55.44f, 2.63f },
    { 1033.71f, -98.31f,  55.49f, 2.86f },
    { 1058.12f, -109.16f, 55.34f, 1.91f },
    { 1076.07f, -101.70f, 55.35f, 0.45f },
};

const Position SonsOfFlameNormalWestPos[] =
{
    { 1058.34f, -109.44f, 55.34f, 6.00f },
    { 1075.41f, -101.80f, 55.34f, 0.43f },
    { 996.82f,  -74.99f,  55.45f, 6.23f },
    { 996.15f,  -40.54f,  55.48f, 1.10f },
    { 1011.53f, -26.56f,  55.42f, 0.44f },
    { 1031.21f, -16.94f,  55.50f, 0.30f },
    { 1055.82f, -5.44f,   55.34f, 0.14f },
    { 1073.62f, -12.86f,  55.34f, 5.95f },
};

const Position SonsOfFlameNormalNorthPos[] =
{
    { 1074.63f, -12.91f,  55.35f, 2.93f },
    { 1058.60f, -5.95f,   55.35f, 1.44f },
    { 1031.51f, -16.92f,  55.49f, 3.48f },
    { 1012.93f, -26.53f,  55.48f, 3.56f },
    { 1076.54f, -100.86f, 55.35f, 3.67f },
    { 1058.33f, -108.65f, 55.34f, 0.78f },
    { 1031.86f, -98.40f,  55.52f, 2.78f },
    { 1014.80f, -89.23f,  55.50f, 3.06f },
};

/*
    Positions for engulfing flames
*/

const Position EngulfingFlamesMelee[] =
{
    {1086.55f, -18.0885f, 55.4228f},
    {1091.83f, -21.9254f, 55.4241f},
    {1092.52f, -92.3924f, 55.4241f},
    {1079.15f, -15.5312f, 55.4230f},
    {1078.01f, -97.7760f, 55.4227f},
    {1065.44f, -17.7049f, 55.4250f},
    {1063.59f, -97.0573f, 55.4934f},
    {1051.80f, -24.0903f, 55.4258f},
    {1049.27f, -90.6892f, 55.4259f},
    {1042.34f, -32.1059f, 55.4254f},
    {1041.26f, -81.4340f, 55.4240f},
    {1036.82f, -44.3385f, 55.4425f},
    {1036.34f, -69.8281f, 55.4425f},
    {1034.76f, -63.9583f, 55.4397f},
    {1033.93f, -57.0920f, 55.4225f},
    {1086.42f, -96.7812f, 55.4226f},
};

const Position EngulfingFlamesRange[] =
{
    {1052.58f, -120.561f, 55.4561f},
    {1049.73f, -118.396f, 55.5661f},
    {1035.56f, -114.155f, 55.4471f},
    {1024.91f, -106.851f, 55.4471f},
    {1012.09f, -97.5121f, 55.4570f},
    {1005.48f, -86.4569f, 55.4275f},
    {1003.44f, -74.0243f, 55.4063f},
    {1003.07f, -66.4913f, 55.4067f},
    {1002.00f, -58.2396f, 55.4331f},
    {1002.21f, -49.7048f, 55.4075f},
    {1002.71f, -40.7430f, 55.4063f},
    {1006.09f, -27.3680f, 55.4277f},
    {1014.15f, -17.3281f, 55.4628f},
    {1024.44f, -8.1388f,  55.4469f},
    {1035.91f, 0.9097f,   55.4469f},
    {1049.32f, 5.0434f,   55.4632f},
    {1055.33f, 5.0677f,   55.4471f},
    {1032.47f, 13.2708f,  55.4468f},
    {1023.83f, 12.9774f,  55.4469f},
    {1019.59f, 7.7691f,   55.4469f},
    {1012.70f, -4.8333f,  55.6050f},
    {1005.79f, -8.8177f,  55.4672f},
    {1000.80f, -14.5069f, 55.4566f},
    {991.79f,  -25.0955f, 55.4440f},
    {986.60f,  -37.7655f, 55.4411f},
    {988.20f,  -50.3646f, 55.4291f},
    {980.92f,  -58.2655f, 55.4542f},
    {989.86f,  -66.0868f, 55.4331f},
    {985.17f,  -77.3785f, 55.4408f},
    {991.73f,  -87.1632f, 55.4445f},
    {999.75f,  -98.4792f, 55.4426f},
    {1009.55f, -108.161f, 55.4697f},
    {1018.28f, -117.833f, 55.4471f},
    {1023.04f, -128.257f, 55.4471f},
    {1035.17f, -125.646f, 55.4471f},
};

const Position EngulfingFlamesCenter[] =
{
    {1069.66f, -4.5399f,  55.4308f},
    {1062.93f, -4.3420f,  55.5681f},
    {1057.03f, -4.1041f,  55.4258f},
    {1049.97f, -7.2239f,  55.4537f},
    {1036.90f, -14.6181f, 55.5714f},
    {1025.33f, -25.8472f, 55.4068f},
    {1021.84f, -33.7482f, 55.4239f},
    {1018.46f, -43.7673f, 55.4217f},
    {1016.98f, -57.5642f, 55.4133f},
    {1018.28f, -70.1875f, 55.4231f},
    {1021.48f, -79.6075f, 55.4261f},
    {1025.29f, -86.2325f, 55.4071f},
    {1030.21f, -92.8403f, 55.4343f},
    {1038.53f, -100.253f, 55.6012f},
    {1049.66f, -104.905f, 55.4556f},
    {1062.13f, -109.004f, 55.4259f},
    {1069.91f, -109.651f, 55.4277f},
};

const Position LavaScionPos[] =
{
    {1027.306f, -121.7465f, 55.4471f, 1.361f},
    {1026.861f, 5.895833f, 55.44697f, 4.904f},
};

class at_sulfuron_keep : public AreaTriggerScript
{
    public:
        at_sulfuron_keep() : AreaTriggerScript("at_sulfuron_keep") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if ((!instance->IsDone(DATA_RAGNAROS) && !player->GetMap()->IsHeroic())
                    || (!instance->IsDone(DATA_RAGNAROS) && player->GetMap()->IsHeroic()
                    && instance->IsDoneInHeroic(DATA_SHANNOX) && instance->IsDoneInHeroic(DATA_BALOROC)
                    && instance->IsDoneInHeroic(DATA_LORD_RHYOLITH) && instance->IsDoneInHeroic(DATA_BETHTILAC)
                    && instance->IsDoneInHeroic(DATA_ALYSRAZOR) && instance->IsDoneInHeroic(DATA_MAJORDOMO_STAGHELM)))
                    if (Creature* ragnaros = Creature::GetCreature(*player, instance->GetData64(DATA_RAGNAROS)))
                        ragnaros->AI()->DoAction(ACTION_START_INTRO);
            return true;
        }
};

class boss_ragnaros_firelands : public CreatureScript
{
public:
    boss_ragnaros_firelands() : CreatureScript("boss_ragnaros_firelands") { }

    struct boss_ragnaros_firelandsAI : public BossAI
    {
        boss_ragnaros_firelandsAI(Creature* creature) : BossAI(creature, DATA_RAGNAROS)
        {
            _submergeCounter = 0;
            _sonCounter = 0;
            introDone = false;
            me->SetVisible(false);
            me->SetDisableGravity(true);
        }

        void Reset() override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = RAID_MODE(66995760, 200987985, 87300000, 290500000);
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            _Reset();
            events.SetPhase(PHASE_1);
            me->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetReactState(REACT_PASSIVE);
            me->AddAura(SPELL_BASE_VISUAL, me);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->RemoveAurasDueToSpell(SPELL_BURNING_WOUNDS_AURA);
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING); // Test
            if (GameObject* platform = me->FindNearestGameObject(GO_RAGNAROS_PLATFORM, 200.0f))
                platform->SetDestructibleState(GO_DESTRUCTIBLE_INTACT);

            _submergeCounter = 0;
            _sonCounter = 0;
            _meteorCount = 0;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            AddEncounterFrame();
            me->SetReactState(REACT_AGGRESSIVE);
            DoCast(me, SPELL_HITTING_YA, true);
            events.SetPhase(PHASE_1);
            events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 30000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_MAGMA_TRAP, 15500, 0, PHASE_1);
            events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, 25000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_BERSERK, 1080000);
            me->AddAura(SPELL_BURNING_WOUNDS_AURA, me);

            bool QuestText = false;
            heartQuest = false;

            Map::PlayerList const& player = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = player.begin(); itr != player.end(); ++itr)
            {
                if (Player* player = itr->getSource())
                {
                    if (player->HasItemOrGemWithIdEquipped(ITEM_PRE_LEGENDARY_STAFF, 1) && !QuestText)
                    {
                        int32 questId = player->GetTeam() == HORDE ? QUEST_HEART_OF_FIRE_H : QUEST_HEART_OF_FIRE_A;

                        QuestStatusMap::const_iterator status = player->getQuestStatusMap().find(questId);
                        if (status->second.Status != QUEST_STATUS_INCOMPLETE)
                            continue;

                        if (status->second.CreatureOrGOCount[0] == 250)
                        {
                            QuestText = true;
                            heartQuest = true;

                            if (!IsHeroic())
                                me->CastSpell(player, SPELL_RAGE_OF_RAGNAROS, true);

                            Talk(SAY_HEART_OF_FLAME_QUEST);
                        }
                    }
                }
            }

            //me->AI()->DoAction(ACTION_ACTIVATE_HEROIC);
        }

        void JustDied(Unit* /*killer*/) override
        {
            instance->CompleteGuildCriteriaForGuildGroup(18102);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SUPERHEATED_TRIGGERED);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CLOUDBURST_DELUGE);
            instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_RAGNAROS_CREDITS);
            me->DespawnCreaturesInArea(NPC_SULFURAS_HAND_OF_RAGNAROS, 200.f);
            me->DespawnCreaturesInArea(NPC_SPLITTING_BLOW_TRIGGER, 200.f);

            if (heartQuest)
                if (Player* player = me->FindNearestPlayer(50.f))
                    player->SummonCreature(NPC_HEART_OF_RAGNAROS, 1078.43f, -57.529f, 53.6618f, TEMPSUMMON_MANUAL_DESPAWN);

            if (!IsHeroic())
            {
                if (GameObject* chest = me->FindNearestGameObject(Is25ManRaid() ? GO_CACHE_OF_THE_FIRELORD_25 : GO_CACHE_OF_THE_FIRELORD_10, 500.0f))
                    chest->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
            }
            else
            {
                Talk(SAY_DEATH_HEROIC);
                if (Creature* malfurion = me->FindNearestCreature(NPC_MALFURION, 200.0f, true))
                    malfurion->AI()->DoAction(ACTION_SCHEDULE_OUTRO);

                if (Creature* cenarius = me->FindNearestCreature(NPC_CENARIUS, 200.0f, true))
                    cenarius->AI()->DoAction(ACTION_SCHEDULE_OUTRO);

                if (Creature* hamuul = me->FindNearestCreature(NPC_HAMUUL, 200.0f, true))
                    hamuul->AI()->DoAction(ACTION_SCHEDULE_OUTRO);
            }

            _JustDied();
        }

        void EnterEvadeMode() override
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->SetData(DATA_GROUND_STATE, BUILDING_STATE_INTACT);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SUPERHEATED_TRIGGERED);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CLOUDBURST_DELUGE);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RAGE_OF_RAGNAROS);
            me->DespawnCreaturesInArea(NPC_SULFURAS_HAND_OF_RAGNAROS, 200.0f);
            me->DespawnCreaturesInArea(NPC_SPLITTING_BLOW_TRIGGER, 200.0f);
            me->DespawnCreaturesInArea(NPC_MALFURION, 200.0f);
            me->DespawnCreaturesInArea(NPC_CENARIUS, 200.0f);
            me->DespawnCreaturesInArea(NPC_HAMUUL, 200.0f);
            BossAI::EnterEvadeMode();
        }

        void KilledUnit(Unit* killed) override
        {
            if (killed->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_SLAY);
        }

        void JustSummoned(Creature* summon) override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = summon->GetMaxHealth() / (IsHeroic() ? 0.85f : 0.75f);
                summon->SetMaxHealth(preNerfHealth);
                summon->SetHealth(preNerfHealth);
            }
            summons.Summon(summon);
            switch (summon->GetEntry())
            {
                case NPC_SULFURAS_SMASH_TARGET:
                    summon->AddAura(SPELL_LAVA_POOL, summon);
                    break;
                case NPC_SON_OF_FLAME:
                    summon->SetReactState(REACT_PASSIVE);
                    summon->AddAura(SPELL_HIT_ME, summon);
                    summon->AddAura(SPELL_PRE_VISUAL, summon);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                    summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    break;
                case NPC_LIVING_METEOR:
                case NPC_LAVA_SCION:
                    summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    break;
                case NPC_DREADFLAME:
                    summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    summon->CastSpell(summon, SPELL_DREADFLAME_CONTROL_AURA, true);
                    summon->CastSpell(summon, SPELL_DREADFLAME_CONTROL_AURA_1, true);
                    summon->CastSpell(summon, SPELL_DREADFLAME_CONTROL_AURA_2, true);
                    summon->CastSpell(summon, SPELL_DREADFLAME_CONTROL_AURA_3, true);
                    summon->DespawnOrUnsummon(10000);
                    break;
                default:
                    break;
            }
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_LAVA_SCION)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, summon);
        }

        void CleanSubmerge()
        {
            me->DespawnCreaturesInArea(NPC_SULFURAS_HAND_OF_RAGNAROS, 200.f);
            me->DespawnCreaturesInArea(NPC_SPLITTING_BLOW_TRIGGER, 200.f);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(70, damage) && _submergeCounter == 0 || me->HealthBelowPctDamaged(40, damage) && _submergeCounter == 1)
            {
                _submergeCounter++;

                me->DespawnCreaturesInArea(NPC_SULFURAS_SMASH_TARGET, 200.f);

                events.SetPhase(PHASE_SUBMERGED);
                events.ScheduleEvent(EVENT_SPLITTING_BLOW, 1, 0, PHASE_SUBMERGED);
            }
            else if (me->HealthBelowPctDamaged(10, damage) && _submergeCounter == 2)
            {
                _submergeCounter++;

                if (!IsHeroic())
                {
                    Talk(SAY_DEATH_NORMAL);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                    me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
                    events.Reset();
                    events.ScheduleEvent(EVENT_NORMAL_MODE_DEATH, 1500);
                }
                else
                    DoAction(ACTION_ACTIVATE_HEROIC);
            }
        }

        void DoAction(const int32 action) override
        {
            switch (action)
            {
                case ACTION_START_INTRO:
                {
                    if (!introDone)
                    {
                        introDone = true;
                        me->SetVisible(true);
                        events.SetPhase(PHASE_INTRO);
                        events.ScheduleEvent(EVENT_INTRO, 5500, 0, PHASE_INTRO);
                        me->setActive(true);
                        me->PlayOneShotAnimKit(ANIM_KIT_EMERGE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->NearTeleportTo(me->GetHomePosition());
                        if (Creature* magma = me->FindNearestCreature(NPC_MAGMA_POOL_TRIGGER, 20.0f, true))
                        {
                            magma->AddAura(SPELL_MAGMA_PERIODIC, magma);
                            magma->setFaction(me->getFaction());
                        }

                        std::list<Creature*> units;
                        GetCreatureListWithEntryInGrid(units, me, NPC_MOLTEN_SPEWER, 500.0f);
                        GetCreatureListWithEntryInGrid(units, me, NPC_MOLTEN_ERUPTER, 500.0f);
                        for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
                            (*itr)->AI()->DoAction(ACTION_EMERGE_WORM);
                    }
                    break;
                }
                case ACTION_SON_KILLED:
                {
                    _sonCounter++;
                    if (_sonCounter == 8)
                    {
                        DoAction(ACTION_INSTANT_EMERGE);
                        _sonCounter = 0;
                    }
                    break;
                }
                case ACTION_INSTANT_EMERGE:
                    events.CancelEvent(EVENT_ANNOUNCE_EMERGE);
                    events.CancelEvent(EVENT_EMERGE);
                    events.ScheduleEvent(EVENT_EMERGE, 1000);
                    break;
                case ACTION_ACTIVATE_HEROIC:
                    if (Creature* cenarius = me->SummonCreature(NPC_CENARIUS, CenariusSummonPosition, TEMPSUMMON_MANUAL_DESPAWN, 0))
                        me->SetFacingToObject(cenarius);
                    instance->SetData(DATA_GROUND_STATE, BUILDING_STATE_DESTROYED);
                    me->SummonCreature(NPC_MALFURION, MalfurionSummonPosition, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    me->SummonCreature(NPC_HAMUUL, HamuulSummonPosition, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    events.Reset();
                    break;
                case ACTION_SUBMERGE:
                    Talk(SAY_SUBMERGE_HEROIC);
                    me->CastStop();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    events.Reset();
                    me->RemoveAllAuras();
                    me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
                    events.ScheduleEvent(EVENT_TRANSFORM_RAGNAROS, 3000);
                    events.ScheduleEvent(EVENT_EMERGE_HEROIC, 14500);
                    break;
                case ACTION_HEART_OF_FLAME_FAILED:
                    if (!IsHeroic())
                        heartQuest = false;
                    break;
                default:
                    break;
            }
        }

        void DoMeleeAttackIfReady()
        {
            if (me->getVictim())
            {
                if (me->isAttackReady() && !me->IsNonMeleeSpellCasted(false))
                {
                    if (me->IsWithinMeleeRange(me->getVictim()))
                    {
                        me->AttackerStateUpdate(me->getVictim());
                        me->resetAttackTimer();
                    }
                    else if (!me->HasUnitState(UNIT_STATE_CASTING) && !me->HasAura(SPELL_SUBMERGE_AURA) && !events.IsInPhase(PHASE_HEROIC))
                    {
                        me->resetAttackTimer();
                        DoCastVictim(SPELL_MAGMA_STRIKE);
                    }
                }
            }
        }

        uint32 GetData(uint32 data) const override
        {
            if (data == DATA_METEOR_SUMMON_COUNT)
                return _meteorCount;

            return 0;
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!events.IsInPhase(PHASE_INTRO) && !UpdateVictim())
                return;

            if (events.IsInPhase(PHASE_HEROIC) && me->GetPositionZ() <= 53.0f)
                me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 56.00f, me->GetOrientation());

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INTRO:
                        Talk(SAY_ARRIVE);
                        me->AddAura(SPELL_BASE_VISUAL, me);
                        me->PlayOneShotAnimKit(ANIM_KIT_TAUNT);
                        break;
                    case EVENT_ATTACK:
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->Attack(me->getVictim(), false);
                        me->AddAura(SPELL_BASE_VISUAL, me);
                        if (!me->HasAura(SPELL_BURNING_WOUNDS_AURA))
                            me->AddAura(SPELL_BURNING_WOUNDS_AURA, me);
                        break;
                    case EVENT_SULFURAS_SMASH_TRIGGER:
                    {
                        uint32 worldInFlames = sSpellMgr->GetSpellIdForDifficulty(SPELL_WOLRD_IN_FLAMES, me);
                        if (me->HasAura(worldInFlames) || me->HasUnitState(UNIT_STATE_CASTING))
                            events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 1000);
                        else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, 0))
                        {
                            me->SetFacingToObject(target);
                            DoCast(SPELL_SULFURAS_SMASH_DUMMY);
                            events.ScheduleEvent(EVENT_SULFURAS_SMASH, 500);
                            if (events.IsInPhase(PHASE_1))
                                events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 29500, 0, PHASE_1);
                            else if (events.IsInPhase(PHASE_2))
                                events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 39500, 0, PHASE_2);
                            else if (events.IsInPhase(PHASE_3))
                                events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 29500, 0, PHASE_3);
                        }
                        break;
                    }
                    case EVENT_SULFURAS_SMASH:
                        if (Unit* trigger = me->FindNearestCreature(NPC_SULFURAS_SMASH_TRIGGER, 60.0f, true))
                        {
                            Talk(SAY_ANNOUNCE_SULFURAS_SMASH);
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            me->SetFacingToObject(trigger);
                            DoCast(SPELL_SULFURAS_SMASH);
                            DoCast(SPELL_SULFURAS_SMASH_TARGET);
                            events.ScheduleEvent(EVENT_ATTACK, 6000);
                            events.ScheduleEvent(EVENT_WRATH_OF_RAGNAROS, 12000, 0, PHASE_1);
                        }
                        break;
                    case EVENT_MAGMA_TRAP:
                        Talk(SAY_MAGMA_TRAP);
                        DoCastRandom(SPELL_MAGMA_TRAP, 100.0f);
                        events.ScheduleEvent(EVENT_MAGMA_TRAP, urand(25000, 30000), 0, PHASE_1);
                        return;
                    case EVENT_WRATH_OF_RAGNAROS:
                        DoCastRandom(SPELL_WRATH_OF_RAGNAROS, 300.0f);
                        return;
                    case EVENT_HAND_OF_RAGNAROS:
                        DoCastAOE(SPELL_HAND_OF_RAGNAROS);
                        events.ScheduleEvent(EVENT_HAND_OF_RAGNAROS, 25000, 0, PHASE_1);
                        break;
                    case EVENT_SPLITTING_BLOW:
                    {
                        uint32 worldInFlames = sSpellMgr->GetSpellIdForDifficulty(SPELL_WOLRD_IN_FLAMES, me);
                        if (me->HasAura(worldInFlames) || me->HasUnitState(UNIT_STATE_CASTING))
                            events.ScheduleEvent(EVENT_SPLITTING_BLOW, 1000);
                        else
                        {
                            events.CancelEvent(EVENT_ATTACK);
                            events.ScheduleEvent(EVENT_CALL_SONS, 10000, 0, PHASE_SUBMERGED);

                            me->DespawnCreaturesInArea(NPC_SULFURAS_SMASH_TARGET, 200.f);

                            me->CastStop();
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            Talk(SAY_ANNOUNCE_SPLIT);
                            Talk(SAY_SUBMERGE);
                            Position spawnPos;
                            uint8 spawnIndex = urand(0, 2);
                            std::list<Position> sonSpawnPos;
                            switch (spawnIndex)
                            {
                                case 0: // Splitting Blow East
                                {
                                    spawnPos = SplittingTriggerEast;
                                    DoCastAOE(SPELL_SPLITTING_BLOW_EAST);
                                    if (!IsHeroic())
                                    {
                                        for (uint8 i = 0; i < 8; i++)
                                            sonSpawnPos.push_back(SonsOfFlameNormalEastPos[i]);
                                    }
                                    break;
                                }
                                case 1: // Splitting Blow West
                                {
                                    spawnPos = SplittingTriggerWest;
                                    if (!IsHeroic())
                                    {
                                        for (uint8 i = 0; i < 8; i++)
                                            sonSpawnPos.push_back(SonsOfFlameNormalWestPos[i]);
                                    }
                                    DoCastAOE(SPELL_SPLITTING_BLOW_WEST);
                                    break;
                                }
                                case 2: // Splitting Blow North
                                {
                                    spawnPos = SplittingTriggerNorth;
                                    if (!IsHeroic())
                                    {
                                        for (uint8 i = 0; i < 8; i++)
                                            sonSpawnPos.push_back(SonsOfFlameNormalNorthPos[i]);
                                    }
                                    DoCastAOE(SPELL_SPLITTING_BLOW_NORTH);
                                    break;
                                }
                            }

                            if (Creature* trigger = me->SummonCreature(NPC_SPLITTING_BLOW_TRIGGER, spawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                me->SetFacingToObject(trigger);
                                if (IsHeroic())
                                {
                                    for (uint8 i = 0; i < 17; i++)
                                    {
                                        if (trigger->GetExactDist2d(&SonsOfFlameHeroicPos[i]) >= 10.0f)
                                            sonSpawnPos.push_back(SonsOfFlameHeroicPos[i]);
                                    }

                                    Trinity::Containers::RandomResizeList(sonSpawnPos, 8);
                                }

                                for (Position const& pos : sonSpawnPos)
                                    me->SummonCreature(NPC_SON_OF_FLAME, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            }
                        }
                        break;
                    }
                    case EVENT_CALL_SONS:
                        if (_submergeCounter == 0)
                            Talk(SAY_SONS_OF_FLAME_1);
                        else
                            Talk(SAY_SONS_OF_FLAME_2);

                        me->RemoveAurasDueToSpell(SPELL_BASE_VISUAL);
                        events.ScheduleEvent(EVENT_ANNOUNCE_EMERGE, 40000, 0, PHASE_SUBMERGED);
                        events.ScheduleEvent(EVENT_EMERGE, 45000, 0, PHASE_SUBMERGED);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (_submergeCounter == 2)
                            for (uint32 x = 0; x < 2; ++x)
                                me->SummonCreature(NPC_LAVA_SCION, LavaScionPos[x], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                        break;
                    case EVENT_ANNOUNCE_EMERGE:
                        Talk(SAY_ANNOUNCE_EMERGE);
                        break;
                    case EVENT_EMERGE:
                    {
                        if (!events.IsInPhase(PHASE_SUBMERGED))
                            break;

                        Talk(SAY_EMERGE);
                        CleanSubmerge();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->RemoveAllAuras();
                        me->AddAura(SPELL_BASE_VISUAL, me);
                        me->PlayOneShotAnimKit(ANIM_KIT_RESURFACE);

                        events.ScheduleEvent(EVENT_ATTACK, 4500);
                        events.CancelEvent(EVENT_SULFURAS_SMASH_TRIGGER);
                        events.CancelEvent(EVENT_ENGULFING_FLAMES);

                        std::list<Creature*> units;
                        GetCreatureListWithEntryInGrid(units, me, NPC_SON_OF_FLAME, 200.0f);
                        for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
                        {
                            if (!(*itr)->isDead())
                                (*itr)->Kill(*itr);
                        }

                        if (_submergeCounter == 1)
                        {
                            events.SetPhase(PHASE_2);
                            if (IsHeroic())
                            {
                                events.ScheduleEvent(EVENT_MOLTEN_SEED, 15000, 0, PHASE_2);
                                events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 6000, 0, PHASE_2);
                            }
                            else
                            {
                                events.ScheduleEvent(EVENT_MOLTEN_SEED, 21500, 0, PHASE_2);
                                events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 15000, 0, PHASE_2);
                            }
                            events.ScheduleEvent(EVENT_ENGULFING_FLAMES, 40000, 0, PHASE_2);
                        }
                        if (_submergeCounter == 2)
                        {
                            events.SetPhase(PHASE_3);
                            events.ScheduleEvent(EVENT_SULFURAS_SMASH_TRIGGER, 15500, 0, PHASE_3);
                            events.ScheduleEvent(EVENT_ENGULFING_FLAMES, 30000, 0, PHASE_3);
                            events.ScheduleEvent(EVENT_LIVING_METEOR, 45000, 0, PHASE_3);
                        }
                        break;
                    }
                    case EVENT_ENGULFING_FLAMES:
                        if (!IsHeroic())
                        {
                            Talk(SAY_ENGULFING_FLAMES);
                            switch (urand(0, 2))
                            {
                                case 0: // Melee
                                {
                                    for (uint32 x = 0; x < 16; ++x)
                                        me->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesMelee[x], TEMPSUMMON_TIMED_DESPAWN, 18000);

                                    DoCastAOE(SPELL_ENGULFING_FLAMES_VISUAL_MELEE);
                                    DoCastAOE(SPELL_ENGULFING_FLAMES_MELEE);
                                    break;
                                }
                                case 1: // Range
                                {
                                    for (uint32 x = 0; x < 35; ++x)
                                        me->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesRange[x], TEMPSUMMON_TIMED_DESPAWN, 18000);

                                    DoCastAOE(SPELL_ENGULFING_FLAMES_VISUAL_BOTTOM);
                                    DoCastAOE(SPELL_ENGULFING_FLAMES_BOTTOM);
                                    break;
                                }
                                case 2: // Center
                                {
                                    for (uint32 x = 0; x < 17; ++x)
                                        me->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesCenter[x], TEMPSUMMON_TIMED_DESPAWN, 18000);

                                    DoCastAOE(SPELL_ENGULFING_FLAMES_VISUAL_CENTER);
                                    DoCastAOE(SPELL_ENGULFING_FLAMES_CENTER);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            Talk(SAY_WORLD_IN_FLAMES);
                            me->AddAura(SPELL_WOLRD_IN_FLAMES, me);
                        }

                        if (_submergeCounter == 1)
                            events.ScheduleEvent(EVENT_ENGULFING_FLAMES, IsHeroic() ? 60000 : 40000, 0, PHASE_2);
                        else if (_submergeCounter == 2)
                            events.ScheduleEvent(EVENT_ENGULFING_FLAMES, 30000, 0, PHASE_3);
                        break;
                    case EVENT_MOLTEN_SEED:
                        DoCastAOE(SPELL_MOLTEN_SEED);
                        events.ScheduleEvent(EVENT_MOLTEN_SEED, 60000, 0, PHASE_2);
                        break;
                    case EVENT_LIVING_METEOR:
                        _meteorCount++;
                        DoCastAOE(SPELL_LIVING_METEOR_SUMMON_METEOR);
                        events.ScheduleEvent(EVENT_LIVING_METEOR, 45000, 0, PHASE_3);
                        break;
                    case EVENT_EMERGE_HEROIC:
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                        me->SendMovementHover();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->PlayOneShotAnimKit(ANIM_KIT_EMERGE);
                        me->GetMotionMaster()->MoveLand(1, { me->GetPositionX(), me->GetPositionY(), 57.f });
                        events.ScheduleEvent(EVENT_TALK, 3250);
                        events.ScheduleEvent(EVENT_FREEZE_PLATFORM, 200);
                        events.ScheduleEvent(EVENT_TAUNT_EMOTE, 5500);
                        break;
                    case EVENT_TALK:
                        Talk(SAY_INTRO_HEROIC_1);
                        if (Creature* cenarius = me->FindNearestCreature(NPC_CENARIUS, 200.0f))
                            cenarius->CastStop();
                        if (Creature* hamuul = me->FindNearestCreature(NPC_HAMUUL, 200.0f))
                            hamuul->CastStop();
                        if (Creature* malfurion = me->FindNearestCreature(NPC_MALFURION, 200.0f))
                            malfurion->CastStop();
                        events.ScheduleEvent(EVENT_STANDUP, 9400);
                        events.ScheduleEvent(EVENT_BREAK_PLATFORM, 9400);
                        break;
                    case EVENT_TAUNT_EMOTE:
                        me->PlayOneShotAnimKit(ANIM_KIT_TAUNT);
                        break;
                    case EVENT_FREEZE_PLATFORM:
                        if (GameObject* platform = me->FindNearestGameObject(GO_RAGNAROS_PLATFORM, 200.0f))
                            platform->SetDestructibleState(GO_DESTRUCTIBLE_DAMAGED);
                        if (Creature* magma = me->FindNearestCreature(NPC_MAGMA_POOL_TRIGGER, 20.0f, true))
                            magma->RemoveAllAuras();
                        break;
                    case EVENT_STANDUP:
                        events.SetPhase(PHASE_HEROIC);
                        Talk(SAY_INTRO_HEROIC_2);
                        me->RemoveAurasDueToSpell(SPELL_BASE_VISUAL);
                        me->PlayOneShotAnimKit(ANIM_KIT_STAND_UP);
                        me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                        me->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND);
                        me->SendMovementHover();
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->SetHover(false);
                        events.ScheduleEvent(EVENT_ATTACK_HEROIC, 7500);
                        events.ScheduleEvent(EVENT_DREADFLAME, 24000);
                        events.ScheduleEvent(EVENT_SUMMON_ROOTS, urand(44000, 51000));
                        events.ScheduleEvent(EVENT_EMPOWER_SULFURAS, 54000);
                        if (Creature* malfurion = me->FindNearestCreature(NPC_MALFURION, 200.0f, true))
                            malfurion->AI()->DoAction(ACTION_SCHEDULE_CLOUDBURST);
                        if (Creature* cenarius = me->FindNearestCreature(NPC_CENARIUS, 200.0f, true))
                            cenarius->AI()->DoAction(ACTION_SCHEDULE_BREADTH);
                        break;
                    case EVENT_BREAK_PLATFORM:
                        if (GameObject* platform = me->FindNearestGameObject(GO_RAGNAROS_PLATFORM, 200.0f))
                            platform->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    case EVENT_ATTACK_HEROIC:
                        //me->SetBaseAttackTime(BASE_ATTACK, 1500);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        DoCast(SPELL_SUPERHEATED);
                        break;
                    case EVENT_TRANSFORM_RAGNAROS:
                        DoCast(me, SPELL_LEGS_HEAL);
                        DoCast(me, SPELL_TRANSFORM);
                        break;
                    case EVENT_SUMMON_ROOTS:
                        if (Creature* hamuul = me->FindNearestCreature(NPC_HAMUUL, 200.0f, true))
                            hamuul->AI()->DoAction(ACTION_SCHEDULE_ROOTS);
                        break;
                    case EVENT_EMPOWER_SULFURAS:
                    {
                        Talk(SAY_EMPOWER_SULFURAS);
                        std::list<Creature*> units; // Temphack
                        GetCreatureListWithEntryInGrid(units, me, NPC_MOLTEN_SEED_CASTER, 500.0f);
                        for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
                            (*itr)->AddAura(SPELL_EMPOWER_SULFURAS_TRIGGER, (*itr));
                        me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        DoCast(SPELL_EMPOWER_SULFURAS);
                        uint32 nextCooldown = urand(56000, 64000); // 56-64sec variations
                        events.ScheduleEvent(EVENT_EMPOWER_SULFURAS, nextCooldown);
                        nextCooldown -= urand(3000, 11000); // 56-60sec variations. Always cast before empowered sulf, varies between 3 sec before and like 11 sec before.
                        events.ScheduleEvent(EVENT_SUMMON_ROOTS, nextCooldown);
                        break;
                    }
                    case EVENT_DREADFLAME:
                        //Talk(SAY_DREADFLAME);
                        me->CastSpell(CenterPlatform.m_positionX, CenterPlatform.m_positionY, CenterPlatform.m_positionZ, SPELL_DREADFLAME_SUMMON_2, true);
                        events.ScheduleEvent(EVENT_DREADFLAME_2, 20000);
                        events.ScheduleEvent(EVENT_DREADFLAME, 40000);
                        break;
                    case EVENT_DREADFLAME_2:
                    {
                        Position pos;
                        me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                        me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                        me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                        me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                        if (Is25ManRaid())
                        {
                            me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                            me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                            me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                            me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                            me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                            me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                            me->GetRandomPoint(CenterPlatform, 40.0f, pos);
                            me->CastSpell(pos.m_positionX, pos.m_positionY, 55.5f, SPELL_DREADFLAME_MISSILE, true);
                        }
                        break;
                    }
                    case EVENT_NORMAL_MODE_DEATH:
                        me->SetVisible(false);
                        me->Kill(me);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        private:
            uint8 _submergeCounter;
            uint8 _sonCounter;
            uint8 _meteorCount;
            bool introDone;
            bool heartQuest;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<boss_ragnaros_firelandsAI>(creature);
    }
};

class npc_fl_magma_trap : public CreatureScript
{
    public:
        npc_fl_magma_trap() :  CreatureScript("npc_fl_magma_trap") { }

        struct npc_fl_magma_trapAI : public ScriptedAI
        {
            npc_fl_magma_trapAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
            {
                _exploded = false;
            }


            void IsSummonedBy(Unit* /*summoner*/) override
            {
                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                    ragnaros->AI()->JustSummoned(me);

                if (me->GetPositionZ() > 55.3459f)
                    me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 55.3459f, me->GetOrientation());
                events.ScheduleEvent(EVENT_PREPARE_TRAP, 5000);
                me->AddAura(SPELL_MAGMA_TRAP_VISUAL, me);
                me->SetReactState(REACT_PASSIVE);
                SetCombatMovement(false);
            }

            void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override
            {
                if (spell->Id == SPELL_MAGMA_TRAP_PERIODIC_TICK && !_exploded)
                {
                    DoCastAOE(SPELL_MAGMA_TRAP_ERUPTION);
                    me->RemoveAurasDueToSpell(SPELL_MAGMA_TRAP_VISUAL);
                    _exploded = true;
                    me->DespawnOrUnsummon(3000);
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PREPARE_TRAP:
                            DoCast(SPELL_MAGMA_TRAP_PERIODIC);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            bool _exploded;
            InstanceScript *instance;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_magma_trapAI>(creature);
        }
};

class npc_fl_sulfuras_smash : public CreatureScript
{
    public:
        npc_fl_sulfuras_smash() :  CreatureScript("npc_fl_sulfuras_smash") { }

        struct npc_fl_sulfuras_smashAI : public ScriptedAI
        {
            npc_fl_sulfuras_smashAI(Creature* creature) : ScriptedAI(creature)
            {
                _summonCounter = 0;
            }

            uint8 _summonCounter;

            void IsSummonedBy(Unit* summoner) override
            {
                events.ScheduleEvent(EVENT_SCORCH, 5400);
                events.ScheduleEvent(EVENT_SUMMON_WAVE_1, 5400);
                me->SetOrientation(summoner->GetOrientation());
                me->setFaction(summoner->getFaction());
            }

            void JustSummoned(Creature* summon) override
            {
                summon->setFaction(me->getFaction());
                _summonCounter++;
                if (_summonCounter == 2)
                    summon->SetOrientation(me->GetOrientation() + M_PI/2);
                else if (_summonCounter == 3)
                {
                    summon->SetOrientation(me->GetOrientation() - M_PI/2);
                    _summonCounter = 0;
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SCORCH:
                            me->RemoveAurasDueToSpell(SPELL_LAVA_POOL);
                            me->AddAura(SPELL_SCORCHED_GROUND, me);
                            break;
                        case EVENT_SUMMON_WAVE_1:
                            DoCastAOE(SPELL_LAVA_WAVE_DUMMY_NORTH);
                            events.ScheduleEvent(EVENT_SUMMON_WAVE_2, 50);
                            break;
                        case EVENT_SUMMON_WAVE_2:
                            DoCastAOE(SPELL_LAVA_WAVE_DUMMY_WEST);
                            events.ScheduleEvent(EVENT_SUMMON_WAVE_3, 50);
                            break;
                        case EVENT_SUMMON_WAVE_3:
                            DoCastAOE(SPELL_LAVA_WAVE_DUMMY_EAST);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_sulfuras_smashAI>(creature);
        }
};

class npc_fl_lava_wave : public CreatureScript
{
    public:
        npc_fl_lava_wave() :  CreatureScript("npc_fl_lava_wave") { }

        struct npc_fl_lava_waveAI : public ScriptedAI
        {
            npc_fl_lava_waveAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void EnterCombat(Unit* /*target*/) override {}
            void AttackStart(Unit* /*target*/) override {}
            void EnterEvadeMode() override {}

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetInCombatWithZone();
                events.ScheduleEvent(EVENT_MOVE_LAVA_WAVE, 100);
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_LAVA_WAVE_TRIGGERED, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_LEAP_BACK, true);
                me->SetSpeed(MOVE_RUN, 2.0f);
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_LAVA_WAVE:
                            DoCast(me, SPELL_LAVA_WAVE, false);
                            me->GetMotionMaster()->MovePoint(0, me->GetPositionX()+cos(me->GetOrientation())*100, me->GetPositionY()+sin(me->GetOrientation())*100, me->GetPositionZ(), false);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_lava_waveAI>(creature);
        }
};

class npc_fl_sulfuras_hammer : public CreatureScript // Temphack until dest area effects can affect npc's
{
    public:
        npc_fl_sulfuras_hammer() :  CreatureScript("npc_fl_sulfuras_hammer") { }

        struct npc_fl_sulfuras_hammerAI : public ScriptedAI
        {
            npc_fl_sulfuras_hammerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                me->AddAura(SPELL_SULFURAS_AURA, me);
                DoCastAOE(SPELL_INVOKE_SONS);
                events.ScheduleEvent(EVENT_CHECK_SONS, 500);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell) override
            {
                if (spell->Id == SPELL_INVOKE_SONS_TRIGGERED_SPELL && target->GetEntry() == NPC_SON_OF_FLAME)
                {
                    if (Creature* son = target->ToCreature())
                        son->AI()->DoAction(ACTION_ACTIVATE_SON);
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_SONS:
                        {
                            std::list<Creature*> units;
                            GetCreatureListWithEntryInGrid(units, me, NPC_SON_OF_FLAME, 5.0f);
                            for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
                            {
                                if ((*itr)->HasAura(SPELL_HIT_ME))
                                {
                                    (*itr)->RemoveAurasDueToSpell(SPELL_HIT_ME);
                                    (*itr)->StopMoving();
                                    (*itr)->CastSpell((*itr), SPELL_SUPERNOVA);
                                    me->Kill((*itr), false);
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_SONS, 500);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_sulfuras_hammerAI>(creature);
        }
};

class npc_fl_son_of_flame : public CreatureScript
{
    public:
        npc_fl_son_of_flame() :  CreatureScript("npc_fl_son_of_flame") { }

        struct npc_fl_son_of_flameAI : public ScriptedAI
        {
            npc_fl_son_of_flameAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) {}

            void JustSummoned(Creature* /*summoner*/) override
            {
                me->SetReactState(REACT_PASSIVE);
                me->ApplySpellImmune(0, IMMUNITY_ID, 82691, true); // ring of frost
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                    ragnaros->AI()->DoAction(ACTION_SON_KILLED);
            }

            void DoAction(const int32 action) override
            {
                switch (action)
                {
                    case ACTION_ACTIVATE_SON:
                        me->SetSpeed(MOVE_RUN, 1.0f, true);
                        me->SetSpeed(MOVE_WALK, 1.0f, true);
                        me->RemoveAurasDueToSpell(SPELL_PRE_VISUAL);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                        me->ApplySpellImmune(0, IMMUNITY_ID, 20549, true); // War Stomp
                        me->AddAura(SPELL_BURNING_SPEED, me);
                        me->AddAura(SPELL_BURNING_SPEED_STACKS, me);
                        me->SetAuraStack(SPELL_BURNING_SPEED_STACKS, me, 10);
                        me->PlayOneShotAnimKit(ANIM_KIT_UNK_1);
                        events.ScheduleEvent(EVENT_MOVE_HAMMER, 1500);
                        events.ScheduleEvent(EVENT_CHECK_HEALTH, 100);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                if (me->GetPositionZ() <= 54.70f)
                    me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 55.40f, me->GetOrientation());

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_HAMMER:
                            if (Creature* sulfuras = me->FindNearestCreature(NPC_SULFURAS_HAND_OF_RAGNAROS, 200.0f))
                                if (!me->HasAuraWithMechanic(1 << MECHANIC_STUN))
                                    me->GetMotionMaster()->MovePoint(0, sulfuras->GetPositionX(), sulfuras->GetPositionY(), sulfuras->GetPositionZ());
                            events.ScheduleEvent(EVENT_MOVE_HAMMER, 500);
                            break;
                        case EVENT_CHECK_HEALTH:
                            if (Aura* aur = me->GetAura(SPELL_BURNING_SPEED_STACKS))
                            {
                                float healPct = me->GetHealthPct();

                                if (healPct >= 50.0f)
                                    aur->SetStackAmount(uint32(floor(healPct / 10.0f)));
                                else // Remove all auras at < 50 of health
                                {
                                    me->RemoveAurasDueToSpell(SPELL_BURNING_SPEED_STACKS);
                                    me->RemoveAurasDueToSpell(SPELL_BURNING_SPEED);
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_HEALTH, 500);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_son_of_flameAI>(creature);
        }
};

class npc_fl_engulfing_flame : public CreatureScript
{
    public:
        npc_fl_engulfing_flame() :  CreatureScript("npc_fl_engulfing_flame") { }

        struct npc_fl_engulfing_flameAI : public ScriptedAI
        {
            npc_fl_engulfing_flameAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void UpdateAI(const uint32 /*diff*/)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetFirelandsAI<npc_fl_engulfing_flameAI>(creature);
        }
};

class npc_fl_molten_elemental : public CreatureScript
{
    public:
        npc_fl_molten_elemental() :  CreatureScript("npc_fl_molten_elemental") { }

        struct npc_fl_molten_elementalAI : public ScriptedAI
        {
            npc_fl_molten_elementalAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) {}

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                    ragnaros->AI()->JustSummoned(me);

                DoCast(me, SPELL_INVISIBLE_PRE_VISUAL, true);
                events.ScheduleEvent(EVENT_MOLTEN_SEED_MISSILE, 500);
                events.ScheduleEvent(EVENT_PREPARE_ELEMENTAL, 2500);
                me->SetControlled(true, UNIT_STATE_ROOT);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_PASSIVE);
            }

            void JustDied(Unit* /*killer*/) override
            {
                me->RemoveOwnedAura(SPELL_AGGRO_DEMON_CREATOR, me->GetGUID());
                me->DespawnOrUnsummon(5000);
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOLTEN_SEED_MISSILE:
                            if (Creature* seedCaster = me->FindNearestCreature(NPC_MOLTEN_SEED_CASTER, 100.0f, true))
                                seedCaster->CastSpell(me, SPELL_MOLTEN_SEED_MISSILE);
                            break;
                        case EVENT_PREPARE_ELEMENTAL:
                            me->RemoveAurasDueToSpell(SPELL_INVISIBLE_PRE_VISUAL);
                            me->AddAura(SPELL_MOLTEN_SEED_VISUAL, me);
                            me->SetDisplayId(38520);
                            events.ScheduleEvent(EVENT_ACTIVATE, 10000);
                            break;
                        case EVENT_ACTIVATE:
                        {
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED);
                            me->SetControlled(false, UNIT_STATE_ROOT);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoCastAOE(SPELL_MOLTEN_INFERNO);
                            DoCast(me, SPELL_REDUCE_CRITICAL_STRIKE_CHANCE, true);
                            if (IsHeroic())
                                DoCast(me, SPELL_MOLTEN_POWER_AURA, true);
                            me->SetInCombatWithZone();
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true, -SPELL_AGGRO_DEMON_CREATOR);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true);

                            if (target)
                            {
                                me->AddAura(SPELL_AGGRO_DEMON_CREATOR, target);
                                me->getThreatManager().tauntApply(target);
                                me->GetMotionMaster()->MoveChase(target);
                                me->AI()->AttackStart(target);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
            InstanceScript *instance;

        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_molten_elementalAI>(creature);
        }
};

class lavaTargetSelector : public std::unary_function<Unit*, bool>
{
public:
    lavaTargetSelector(Unit* creature) : _creature(creature) {}

    bool operator()(Unit* unit) const
    {
        uint32 burningHeat = sSpellMgr->GetSpellIdForDifficulty(SPELL_BLAZING_HEAT, _creature);
        if (unit->GetTypeId() != TYPEID_PLAYER || unit->HasAura(SPELL_HITTING_YA_PLAYER) || unit->HasAura(burningHeat) || (unit->getVictim() && unit->getVictim()->GetEntry() == NPC_LAVA_SCION))
            return false;
        return true;
    }
private:
    Unit* _creature;
};

class npc_fl_lava_scion : public CreatureScript
{
    public:
        npc_fl_lava_scion() :  CreatureScript("npc_fl_lava_scion") { }

        struct npc_fl_lava_scionAI : public ScriptedAI
        {
            npc_fl_lava_scionAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap events;

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                if (Unit* player = me->FindNearestPlayer(200.0f))
                    me->AI()->AttackStart(player);
            }

            void EnterCombat(Unit* /*victim*/) override
            {
                AddEncounterFrame();
                events.ScheduleEvent(EVENT_BLAZING_HEAT, 12000);
            }

            void JustDied(Unit* /*killer*/) override
            {
                me->DespawnOrUnsummon(5000);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLAZING_HEAT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, lavaTargetSelector(me)))
                                DoCast(target, SPELL_BLAZING_HEAT);
                            events.ScheduleEvent(EVENT_BLAZING_HEAT, 21000);
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_lava_scionAI>(creature);
        }
};

class npc_fl_blazing_heat : public CreatureScript
{
    public:
        npc_fl_blazing_heat() :  CreatureScript("npc_fl_blazing_heat") { }

        struct npc_fl_blazing_heatAI : public ScriptedAI
        {
            npc_fl_blazing_heatAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                DoCastAOE(SPELL_BLAZING_HEAT_DAMAGE_AURA);
            }

            void UpdateAI(const uint32 /*diff*/) override
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_blazing_heatAI>(creature);
        }
};

class livingMeteorTargetSelector : public std::unary_function<Unit*, bool>
{
public:
    livingMeteorTargetSelector(uint64 tarGUID) : _prevTargetGUID(tarGUID) { }

    bool operator()(Unit const* unit) const
    {
        if (!unit)
            return false;

        if (unit->GetTypeId() != TYPEID_PLAYER)
            return false;

        if (unit->GetGUID() == _prevTargetGUID || unit->HasAura(SPELL_HITTING_YA_PLAYER))
            return false;

        return true;
    }
private:
    uint64 _prevTargetGUID;
};

class npc_fl_living_meteor : public CreatureScript
{
    public:
        npc_fl_living_meteor() :  CreatureScript("npc_fl_living_meteor") { }

        struct npc_fl_living_meteorAI : public ScriptedAI
        {
            npc_fl_living_meteorAI(Creature* creature) : ScriptedAI(creature)
            {
                targetGUID = 0;
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetInCombatWithZone();
                DoCast(me, SPELL_LIVING_METEOR_DAMAGE_REDUCTION, true);

                if (IsHeroic())
                    DoCast(me, SPELL_LIVING_METEOR_INCREASE_SPEED, true);

                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, livingMeteorTargetSelector(targetGUID)))
                {
                    DoCast(target, SPELL_LIVING_METEOR_FIXATE);
                    targetGUID = target->GetGUID();
                }
                else if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, 0))
                {
                    DoCast(target, SPELL_LIVING_METEOR_FIXATE);
                    targetGUID = target->GetGUID();
                }
                events.ScheduleEvent(EVENT_STALK_PLAYER, 3000);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
            }

            void DamageTaken(Unit* attacker, uint32& /*damage*/) override
            {
                if (me->HasAura(SPELL_LIVING_METEOR_COMBUSTIBLE) && !me->HasAura(SPELL_BREADTH_OF_FROST_STUN))
                {
                    events.Reset();
                    me->GetMotionMaster()->Clear();
                    me->RemoveAurasDueToSpell(SPELL_LIVING_METEOR_COMBUSTIBLE);
                    uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIVING_METEOR_SPEED_TRIGGERED, me);
                    me->RemoveAurasDueToSpell(spellId);
                    DoCast(attacker, SPELL_LIVING_METEOR_COMBUSTITION, true);
                    if (Unit *prevTarget = Unit::GetUnit(*me, targetGUID))
                        prevTarget->RemoveAurasDueToSpell(SPELL_LIVING_METEOR_FIXATE, me->GetGUID());
                    if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, livingMeteorTargetSelector(targetGUID)))
                    {
                        events.ScheduleEvent(EVENT_STALK_PLAYER, 3000);
                        me->CastSpell(target, SPELL_LIVING_METEOR_FIXATE, false);
                        targetGUID = target->GetGUID();
                    }
                    else if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, 0))
                    {
                        events.ScheduleEvent(EVENT_STALK_PLAYER, 3000);
                        me->CastSpell(target, SPELL_LIVING_METEOR_FIXATE, false);
                        targetGUID = target->GetGUID();
                    }
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                if (me->HasAura(SPELL_METEOR_FREEZE) || me->HasAura(SPELL_BREADTH_OF_FROST_STUN))
                    return;

                if (me->GetPositionZ() <= 53.0f)
                    me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 56.00f, me->GetOrientation());

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STALK_PLAYER:
                            me->ClearUnitState(UNIT_STATE_CASTING);
                            if (Unit *target = Unit::GetUnit(*me, targetGUID))
                                me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                            events.ScheduleEvent(EVENT_KILL_PLAYER, 500);
                            events.ScheduleEvent(EVENT_ENABLE_KNOCKBACK, 5000);
                            break;
                        case EVENT_KILL_PLAYER:
                        {
                            Unit *target = Unit::GetUnit(*me, targetGUID);
                            if (!target || (target && (!target->isAlive() || !target->HasAura(SPELL_LIVING_METEOR_FIXATE))) || me->FindNearestPlayer(5.0f))
                            {
                                events.Reset();
                                me->GetMotionMaster()->Clear();
                                me->RemoveAurasDueToSpell(SPELL_LIVING_METEOR_COMBUSTIBLE);
                                uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIVING_METEOR_SPEED_TRIGGERED, me);
                                me->RemoveAurasDueToSpell(spellId);
                                DoCastAOE(SPELL_LIVING_METEOR_EXPLOSION);
                                if (target)
                                    target->RemoveAurasDueToSpell(SPELL_LIVING_METEOR_FIXATE, me->GetGUID());
                                if (Unit *ntarget = SelectTarget(SELECT_TARGET_RANDOM, 0, livingMeteorTargetSelector(targetGUID)))
                                {
                                    events.ScheduleEvent(EVENT_STALK_PLAYER, 3000);
                                    me->CastSpell(ntarget, SPELL_LIVING_METEOR_FIXATE, false);
                                    targetGUID = ntarget->GetGUID();
                                }
                                else if (Unit *ntarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, 0))
                                {
                                    events.ScheduleEvent(EVENT_STALK_PLAYER, 3000);
                                    me->CastSpell(ntarget, SPELL_LIVING_METEOR_FIXATE, false);
                                    targetGUID = ntarget->GetGUID();
                                }
                            }
                            else
                                events.ScheduleEvent(EVENT_KILL_PLAYER, 500);
                            break;
                        }
                        case EVENT_ENABLE_KNOCKBACK:
                            me->AddAura(SPELL_LIVING_METEOR_COMBUSTIBLE, me);
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            uint64 targetGUID;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_living_meteorAI>(creature);
        }
};

class npc_fl_archdruids : public CreatureScript
{
    public:
        npc_fl_archdruids() :  CreatureScript("npc_fl_archdruids") { }

        struct npc_fl_archdruidsAI : public ScriptedAI
        {
            npc_fl_archdruidsAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            void DoAction(const int32 action) override
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_CLOUDBURST:
                        events.ScheduleEvent(EVENT_CLOUDBURST, 24000);
                        break;
                    case ACTION_SCHEDULE_ROOTS:
                        events.ScheduleEvent(EVENT_ENTRAPPING_ROOTS, 1);
                        break;
                    case ACTION_SCHEDULE_BREADTH:
                        events.ScheduleEvent(EVENT_BREADTH_OF_FROST, 9000);
                        break;
                    case ACTION_SCHEDULE_OUTRO:
                        me->CastStop();
                        me->DespawnOrUnsummon(60000);
                        events.Reset();
                        switch (me->GetEntry())
                        {
                            case NPC_MALFURION:
                                events.ScheduleEvent(EVENT_TALK_MALFURION_1, 7000);
                                break;
                            case NPC_CENARIUS:
                                events.ScheduleEvent(EVENT_TALK_CENARIUS_1, 10000);
                                break;
                             case NPC_HAMUUL:
                                events.ScheduleEvent(EVENT_TALK_HAMUUL_1, 26000);
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                switch (me->GetEntry())
                {
                    case NPC_CENARIUS:
                        me->SetWalk(false);
                        me->SetSpeed(MOVE_RUN, 3.0f, true);
                        me->GetMotionMaster()->MovePoint(1, CenariusPoint.GetPositionX(), CenariusPoint.GetPositionY(), CenariusPoint.GetPositionZ(), true);
                        break;
                    case NPC_MALFURION:
                        me->SetCanFly(true);
                        me->SetSpeed(MOVE_FLIGHT, 3.0f, true);
                        me->SetDisableGravity(true);
                        me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        me->GetMotionMaster()->MovePoint(1, MalfurionPoint.GetPositionX(), MalfurionPoint.GetPositionY(), MalfurionPoint.GetPositionZ(), true);
                        break;
                    case NPC_HAMUUL:
                        me->SetCanFly(true);
                        me->SetSpeed(MOVE_FLIGHT, 3.0f, true);
                        me->SetDisableGravity(true);
                        me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        me->GetMotionMaster()->MovePoint(1, HamuulPoint.GetPositionX(), HamuulPoint.GetPositionY(), HamuulPoint.GetPositionZ(), true);
                        break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (pointId)
                {
                    case 1:
                        switch (me->GetEntry())
                        {
                            case NPC_CENARIUS:
                                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                                    ragnaros->AI()->DoAction(ACTION_SUBMERGE);
                                events.ScheduleEvent(EVENT_SAY_PULL, 3700);
                                events.ScheduleEvent(EVENT_DRAW_RAGNAROS, 9600);
                                break;
                            case NPC_MALFURION:
                            case NPC_HAMUUL:
                                DoCastAOE(me->GetEntry() == NPC_MALFURION ? SPELL_TRANSFORM_MALFURION : SPELL_TRANSFORM_HAMUUL);
                                events.ScheduleEvent(EVENT_TRANSFORM, 1000);
                                events.ScheduleEvent(EVENT_DRAW_RAGNAROS, me->GetEntry() == NPC_MALFURION ? 8300 : 7100);
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SAY_PULL:
                            Talk(SAY_CENARIUS_1);
                            break;
                        case EVENT_DRAW_RAGNAROS:
                            switch (me->GetEntry())
                            {
                                case NPC_CENARIUS:
                                    DoCastAOE(SPELL_CENARIUS_DRAW_FIRELORD);
                                    break;
                                case NPC_HAMUUL:
                                    DoCastAOE(SPELL_HAMUL_DRAW_FIRELORD);
                                    break;
                                case NPC_MALFURION:
                                    DoCastAOE(SPELL_MALFURION_DRAW_FIRELORD);
                                    events.ScheduleEvent(EVENT_SAY_CAUGHT, 14500);
                                    break;
                            }
                            break;
                        case EVENT_TRANSFORM:
                            me->SetDisableGravity(false);
                            me->SetCanFly(false);
                            me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                            DoCastAOE(me->GetEntry() == NPC_MALFURION ? SPELL_TRANSFORM_MALFURION : SPELL_TRANSFORM_HAMUUL);
                            break;
                        case EVENT_SAY_CAUGHT:
                            Talk(SAY_MALFURION_1);
                            break;
                        case EVENT_CLOUDBURST:
                            DoCastAOE(SPELL_CLOUDBURST);
                            break;
                        case EVENT_ENTRAPPING_ROOTS:
                            DoCastAOE(SPELL_ENTRAPPING_ROOTS);
                            break;
                        case EVENT_BREADTH_OF_FROST:
                            DoCastAOE(SPELL_BREADTH_OF_FROST);
                            events.ScheduleEvent(EVENT_BREADTH_OF_FROST, 45000);
                            break;
                        case EVENT_TALK_MALFURION_1:
                            Talk(SAY_MALFURION_OUTRO_1);
                            events.ScheduleEvent(EVENT_TALK_MALFURION_2, 5000);
                            break;
                        case EVENT_TALK_MALFURION_2:
                            Talk(SAY_MALFURION_OUTRO_2);
                            break;
                        case EVENT_TALK_CENARIUS_1:
                            Talk(SAY_CENARIUS_OUTRO_1);
                            events.ScheduleEvent(EVENT_TALK_CENARIUS_2, 6000);
                            events.ScheduleEvent(EVENT_TALK_CENARIUS_3, 28000);
                            break;
                        case EVENT_TALK_CENARIUS_2:
                            Talk(SAY_CENARIUS_OUTRO_2);
                            break;
                        case EVENT_TALK_CENARIUS_3:
                            Talk(SAY_CENARIUS_OUTRO_3);
                            break;
                        case EVENT_TALK_HAMUUL_1:
                            Talk(SAY_HAMUUL_OUTRO_1);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetFirelandsAI<npc_fl_archdruidsAI>(creature);
        }
};

class npc_fl_molten_erupter : public CreatureScript
{
    public:
        npc_fl_molten_erupter() : CreatureScript("npc_fl_molten_erupter") { }

        struct npc_fl_molten_erupterAI : public ScriptedAI
        {
            npc_fl_molten_erupterAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset() override
            {
                me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
            }

            void EnterCombat(Unit* /*who*/) override
            {
            }

            void DoAction(const int32 action) override
            {
                switch (action)
                {
                    case ACTION_EMERGE_WORM:
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->PlayOneShotAnimKit(ANIM_KIT_EMERGE);
                        break;
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case 0:
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_molten_erupterAI>(creature);
    }
};

class npc_fl_molten_spewer : public CreatureScript
{
    public:
        npc_fl_molten_spewer() : CreatureScript("npc_fl_molten_spewer") { }

        struct npc_fl_molten_spewerAI : public ScriptedAI
        {
            npc_fl_molten_spewerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset() override
            {
                me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
            }

            void EnterCombat(Unit* /*who*/) override
            {
            }

            void DoAction(const int32 action) override
            {
                switch (action)
                {
                    case ACTION_EMERGE_WORM:
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        me->PlayOneShotAnimKit(ANIM_KIT_EMERGE);
                        break;
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case 0:
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_molten_spewerAI>(creature);
    }
};

class npc_fl_cloudburst : public CreatureScript
{
public:
    npc_fl_cloudburst() : CreatureScript("npc_fl_cloudburst") { }

    struct npc_fl_cloudburstAI : public ScriptedAI
    {
        npc_fl_cloudburstAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) {}

        void IsSummonedBy(Unit* summoner)
        {
            if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                ragnaros->AI()->JustSummoned(me);

            DoCast(me, SPELL_CLOUDBURST_VISUAL, true);
            clickCount = 0;
        }

        void OnSpellClick(Unit* clicker, bool& result)
        {
            if (++clickCount >= uint8(Is25ManRaid() ? 3 : 1))
                me->DespawnOrUnsummon();
        }
    private:
        uint8 clickCount;
        InstanceScript *instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_cloudburstAI>(creature);
    }
};

class npc_fl_dreadflame : public CreatureScript
{
public:
    npc_fl_dreadflame() : CreatureScript("npc_fl_dreadflame") { }

    struct npc_fl_dreadflameAI : public ScriptedAI
    {
        npc_fl_dreadflameAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                ragnaros->AI()->JustSummoned(me);
            DoCast(me, SPELL_DREADFLAME_PERIODIC_DAMAGE, true);
            DoCast(me, SPELL_DREADFLAME_CONTROL_AURA_2, true); // Deluge distance check
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING); // ???
            events.ScheduleEvent(EVENT_SUMMON_DREADFLAME, urand(3000, 3500));
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spellInfo)
        {
            if (spellInfo->Id == SPELL_DREADFLAME_AURA_2_TRIGERED)
            {
                target->CastSpell(me, SPELL_CLOUDBURST_DELUGE_FIRE_REMOVE, true);
                me->DespawnOrUnsummon(100);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_DREADFLAME:
                    {
                        float angle = frand(0.0f, 2.0f * M_PI);
                        float step = float(M_PI) / 8.0f;
                        float spawnDist = 8.0f;
                        for (uint8 i = 0; i < 16; ++i)
                        {
                            float tempAngle = me->NormalizeOrientation(angle);
                            std::list<Creature*> list;
                            Position pos(*me);
                            me->MovePositionToFirstCollision(pos, spawnDist, tempAngle);
                            me->GetCreatureListWithEntryInGrid(list, NPC_DREADFLAME_SPAWN, pos.GetPositionX(), pos.GetPositionY(), 10.0f);
                            bool validPos = true;
                            for (std::list<Creature *>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                                if ((*itr)->GetExactDist2d(pos.GetPositionX(), pos.GetPositionY()) < spawnDist / 2.0f)
                                {
                                    validPos = false;
                                    break;
                                }
                            if (validPos)
                            {
                                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                                    ragnaros->CastSpell(pos.GetPositionX(), pos.GetPositionY(), CenterPlatform.m_positionZ, SPELL_DREADFLAME_SUMMON, true);
                                break;
                            }
                            angle += step;
                        }
                        events.ScheduleEvent(EVENT_SUMMON_DREADFLAME, 3000);
                        break;
                    }
                }
            }
        }

    private:
        EventMap events;
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_dreadflameAI>(creature);
    }
};

class npc_fl_entrapping_roots : public CreatureScript
{
public:
    npc_fl_entrapping_roots() : CreatureScript("npc_fl_entrapping_roots") { }

    struct npc_fl_entrapping_rootsAI : public ScriptedAI
    {
        npc_fl_entrapping_rootsAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void SpellHitTarget(Unit* target, SpellInfo const* /*spell*/)
        {
            if (target->GetEntry() == BOSS_RAGNAROS && target->isAlive())
                me->DespawnOrUnsummon();

        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                ragnaros->AI()->JustSummoned(me);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetDisplayId(11686);

            if (Creature* hamuul = me->FindNearestCreature(NPC_HAMUUL, 200.0f))
                hamuul->CastSpell(me, SPELL_ENTRAPPING_ROOTS_AURA_MISSILE);
        }

        void UpdateAI(const uint32 /*diff*/) override {}

    private:
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_entrapping_rootsAI>(creature);
    }
};

class npc_fl_breadth_of_frost : public CreatureScript
{
public:
    npc_fl_breadth_of_frost() : CreatureScript("npc_fl_breadth_of_frost") { }

    struct npc_fl_breadth_of_frostAI : public ScriptedAI
    {
        npc_fl_breadth_of_frostAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spellInfo)
        {
            if (spellInfo->Id == SPELL_BREADTH_OF_FROST_STUN && target->GetEntry() == NPC_LIVING_METEOR && target->isAlive())
                me->DespawnOrUnsummon();
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                ragnaros->AI()->JustSummoned(me);
            me->DespawnOrUnsummon(90000);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetDisplayId(11686);
            if (Creature* cenarius = me->FindNearestCreature(NPC_CENARIUS, 200.0f))
                cenarius->CastSpell(me, SPELL_BREADTH_OF_FROST_MISSILE);

            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
                events.ScheduleEvent(EVENT_CHECK_PLAYER_CLUMB, 2000);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_PLAYER_CLUMB:
                    {
                        uint8 maxPlayersinRange = Is25ManRaid() ? 10 : 4;
                        std::list<Player*> playerList = me->GetPlayersInRange(6.6f, true);
                        for (auto itr : playerList)
                        {
                            std::list<Player*> playersInRange = itr->GetPlayersInRange(5.0f, true);
                            if (playersInRange.size() >= maxPlayersinRange)
                            {
                                if (Creature* ragnaros = Creature::GetCreature(*me, instance->GetData64(DATA_RAGNAROS)))
                                    ragnaros->CastSpell(itr, SPELL_MAGMA_GEYSER, true);
                                me->DespawnOrUnsummon();
                                break;
                            }
                        }
                        events.ScheduleEvent(EVENT_CHECK_PLAYER_CLUMB, 250);
                        break;
                    }
                }
            }
        }

    private:
        EventMap events;
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<npc_fl_breadth_of_frostAI>(creature);
    }
};

class spell_fl_splitting_blow : public SpellScriptLoader
{
public:
    spell_fl_splitting_blow() : SpellScriptLoader("spell_fl_splitting_blow") { }

    class spell_fl_splitting_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_splitting_blow_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (!finish)
            {
                if (GetHitUnit()->GetEntry() == NPC_SPLITTING_BLOW)
                {
                    GetHitUnit()->CastSpell(GetHitUnit(), SPELL_SUMMON_SULFURAS_AURA);
                    GetCaster()->CastSpell(GetCaster(), SPELL_SUBMERGE_AURA, true);
                    finish = true;
                }
            }
        }

        bool finish = false;

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fl_splitting_blow_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_splitting_blow_SpellScript();
    }
};

class spell_fl_invoke_sons : public SpellScriptLoader
{
public:
    spell_fl_invoke_sons() : SpellScriptLoader("spell_fl_invoke_sons") { }

    class spell_fl_invoke_sons_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_invoke_sons_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_INVOKE_SONS_MISSILE))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_INVOKE_SONS_MISSILE, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fl_invoke_sons_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_invoke_sons_SpellScript();
    }
};

class spell_fl_blazing_heat : public SpellScriptLoader
{
    public:
        spell_fl_blazing_heat() :  SpellScriptLoader("spell_fl_blazing_heat") { }

        class spell_fl_blazing_heat_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fl_blazing_heat_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BLAZING_HEAT_SUMMON))
                    return false;
                return true;
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_BLAZING_HEAT_SUMMON, false);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_fl_blazing_heat_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_fl_blazing_heat_AuraScript();
        }
};

class spell_fl_world_in_flames_periodic : public SpellScriptLoader
{
public:
    spell_fl_world_in_flames_periodic() : SpellScriptLoader("spell_fl_world_in_flames_periodic") { }

    class spell_fl_world_in_flames_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fl_world_in_flames_periodic_AuraScript);

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            // aura delay for SetPeriodicTimer hack
            aurEff->GetBase()->SetMaxDuration(9200);
            aurEff->GetBase()->SetDuration(9200);
        }

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            if (Unit* caster = GetCaster())
            {
                if (AuraEffect* aurEff = GetEffect(EFFECT_0))
                {
                    int32 oldPosition = aurEff->GetAmount();
                    int32 newPosition = 0;
                    aurEff->SetPeriodicTimer(3050); // hackfix - without this everytime the first ground effect is broken
                    do
                        newPosition = urand(0, 2);
                    while (oldPosition == newPosition);

                    caster->CastCustomSpell(caster, GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, &newPosition, NULL, NULL, true);
                    aurEff->SetAmount(newPosition);
                }
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_fl_world_in_flames_periodic_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_fl_world_in_flames_periodic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_fl_world_in_flames_periodic_AuraScript();
    }
};

class spell_fl_world_in_flames : public SpellScriptLoader
{
public:
    spell_fl_world_in_flames() : SpellScriptLoader("spell_fl_world_in_flames") { }

    class spell_fl_world_in_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_world_in_flames_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
            {
                switch (GetEffectValue())
                {
                    case 0: // Melee
                    {
                        for (uint32 x = 0; x < 16; ++x)
                            caster->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesMelee[x], TEMPSUMMON_TIMED_DESPAWN, 4500);

                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_VISUAL_MELEE, true);
                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_MELEE);
                        break;
                    }
                    case 1: // Range
                    {
                        for (uint32 x = 0; x < 35; ++x)
                            caster->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesRange[x], TEMPSUMMON_TIMED_DESPAWN, 4500);

                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_VISUAL_BOTTOM, true);
                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_BOTTOM);
                        break;
                    }
                    case 2: // Center
                    {
                        for (uint32 x = 0; x < 17; ++x)
                            caster->SummonCreature(NPC_ENGULFING_FLAMES_TRIGGER, EngulfingFlamesCenter[x], TEMPSUMMON_TIMED_DESPAWN, 4500);

                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_VISUAL_CENTER, true);
                        caster->CastSpell(caster, SPELL_ENGULFING_FLAMES_CENTER);
                        break;
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fl_world_in_flames_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_world_in_flames_SpellScript();
    }
};

class spell_fl_empower_sulfuras : public SpellScriptLoader
{
    public:
        spell_fl_empower_sulfuras() : SpellScriptLoader("spell_fl_empower_sulfuras") { }

        class spell_fl_empower_sulfuras_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fl_empower_sulfuras_AuraScript);

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                // 33 %
                if (!urand(0, 2))
                    GetCaster()->CastSpell(GetCaster(), SPELL_EMPOWER_SULFURAS_MISSILE);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_fl_empower_sulfuras_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_fl_empower_sulfuras_AuraScript();
        }
};

class spell_fl_molten_inferno : public SpellScriptLoader
{
public:
    spell_fl_molten_inferno() : SpellScriptLoader("spell_fl_molten_inferno") { }

    class spell_fl_molten_inferno_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_molten_inferno_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            float distance = GetCaster()->GetExactDist2d(GetHitUnit());
            bool is25ManRaid = GetCaster()->GetMap()->Is25ManRaid();
            int32 minDamage = is25ManRaid ? irand(3750, 4500) : irand(9375, 11250);
            SetHitDamage(int32(GetHitDamage() - (3000 * distance)));
            if (GetCaster()->GetMap()->IsHeroic())
            {
                if (GetHitDamage() < minDamage)
                    SetHitDamage(minDamage);
            }
            else if (GetHitDamage() < int32(is25ManRaid ? 1125 : 3125))
                SetHitDamage(int32(is25ManRaid ? 1125 : 3125));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fl_molten_inferno_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_molten_inferno_SpellScript();
    }
};

class spell_fl_summon_living_meteor : public SpellScriptLoader
{
public:
    spell_fl_summon_living_meteor() : SpellScriptLoader("spell_fl_summon_living_meteor") { }

    class spell_fl_summon_living_meteor_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_summon_living_meteor_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Creature* caster = GetCaster()->ToCreature())
            {
                uint8 summonCount = caster->AI()->GetData(DATA_METEOR_SUMMON_COUNT);

                switch (summonCount)
                {
                    case 1:
                    case 2:
                        summonCount = 1;
                        break;
                    case 3:
                    case 4:
                        summonCount = 2;
                        break;
                    default:
                        summonCount = 4;
                        break;
                }

                targets.remove_if([](WorldObject* target)
                {
                    return target->GetTypeId() != TYPEID_PLAYER;
                });

                if (!targets.empty())
                    Trinity::Containers::RandomResizeList(targets, summonCount);
            }
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_summon_living_meteor_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_fl_summon_living_meteor_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_summon_living_meteor_SpellScript();
    }
};

class spell_tank_burning_wounds : public SpellScriptLoader
{
public:
    spell_tank_burning_wounds() : SpellScriptLoader("spell_tank_burning_wounds") { }

    class spell_tank_burning_wounds_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tank_burning_wounds_AuraScript);

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (Unit *target = eventInfo.GetProcTarget())
                if (target->GetTypeId() == TYPEID_PLAYER)
                {
                    if (target->HasSpellCooldown(SPELL_BURNING_WOUNDS_PROC))
                        return;
                    if (Unit *caster = GetCaster())
                        caster->CastSpell(target, SPELL_BURNING_WOUNDS_PROC, true);
                    target->ToPlayer()->AddSpellCooldown(SPELL_BURNING_WOUNDS_PROC, 0, time(NULL) + 5);
                }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_tank_burning_wounds_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_tank_burning_wounds_AuraScript();
    }
};

class spell_breadth_of_frost : public SpellScriptLoader
{
public:
    spell_breadth_of_frost() : SpellScriptLoader("spell_breadth_of_frost") { }

    class spell_breadth_of_frost_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_breadth_of_frost_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(GetSpellInfo()->Effects[effIndex].BasePoints, GetHitUnit());
            GetHitUnit()->RemoveAurasDueToSpell(spellId);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_breadth_of_frost_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_breadth_of_frost_SpellScript();
    }
};

class spell_fl_ragnaros_stalker_cast : public SpellScriptLoader
{
public:
    spell_fl_ragnaros_stalker_cast() : SpellScriptLoader("spell_fl_ragnaros_stalker_cast") { }

    class spell_fl_ragnaros_stalker_cast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_ragnaros_stalker_cast_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                return target->GetEntry() != NPC_PLATFORM_TRIGGER;
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            GetHitUnit()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_ragnaros_stalker_cast_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_fl_ragnaros_stalker_cast_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fl_ragnaros_stalker_cast_SpellScript();
    }
};

class spell_fl_cloudburst_summon : public SpellScriptLoader
{
public:
    spell_fl_cloudburst_summon() : SpellScriptLoader("spell_fl_cloudburst_summon") { }

    class spell_fl_cloudburst_summon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_cloudburst_summon_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                return target->GetEntry() != NPC_PLATFORM_TRIGGER;
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_cloudburst_summon_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_fl_cloudburst_summon_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_cloudburst_summon_SpellScript();
    }
};

class spell_fl_dreadflame_periodic : public SpellScriptLoader
{
public:
    spell_fl_dreadflame_periodic() : SpellScriptLoader("spell_fl_dreadflame_periodic") { }

    class spell_fl_dreadflame_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fl_dreadflame_periodic_AuraScript);

        void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32& amplitude)
        {
            isPeriodic = true;
            amplitude = 1000;
        }

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
            {
                std::list<Player*> player = caster->GetPlayersInRange(3.0f, true);
                for (auto itr : player)
                    caster->CastSpell(itr, SPELL_DREADFLAME_DAMAGE, true);
            }
        }

        void Register()
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_fl_dreadflame_periodic_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_fl_dreadflame_periodic_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fl_dreadflame_periodic_AuraScript();
    }
};

class spell_fl_superheated : public SpellScriptLoader
{
public:
    spell_fl_superheated() : SpellScriptLoader("spell_fl_superheated") { }

    class spell_fl_superheated_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_superheated_SpellScript);

        std::list<WorldObject*> sharedTargets;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 deluge = sSpellMgr->GetSpellIdForDifficulty(SPELL_CLOUDBURST_DELUGE, GetCaster());
            targets.remove_if(Trinity::UnitAuraCheck(true, deluge));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_BREADTH_OF_FROST_PROTECTION));
            sharedTargets = targets;
        }

        void FilterTargetsSubsequent(std::list<WorldObject*>& targets)
        {
            targets = sharedTargets;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_superheated_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_superheated_SpellScript::FilterTargetsSubsequent, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_superheated_SpellScript();
    }
};

class spell_fl_lava_bolt : public SpellScriptLoader
{
public:
    spell_fl_lava_bolt() : SpellScriptLoader("spell_fl_lava_bolt") { }

    class spell_fl_lava_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_lava_bolt_SpellScript);

        std::list<WorldObject*> sharedTargets;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, GetSpellInfo()->Id));
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, GetCaster()->GetMap()->Is25ManRaid() ? 8 : 3);

            sharedTargets = targets;
        }

        void ShareTargets(std::list<WorldObject*>& targets)
        {
            targets = sharedTargets;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_lava_bolt_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_lava_bolt_SpellScript::ShareTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_lava_bolt_SpellScript();
    }
};

class spell_ragnaros_rage_AuraScript : public AuraScript
{
    PrepareAuraScript(spell_ragnaros_rage_AuraScript);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* ragnaros = GetTarget()->FindNearestCreature(BOSS_RAGNAROS, 300.f))
            ragnaros->AI()->DoAction(ACTION_HEART_OF_FLAME_FAILED);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_ragnaros_rage_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_fl_breadth_of_frost_stun : public SpellScriptLoader
{
public:
    spell_fl_breadth_of_frost_stun() : SpellScriptLoader("spell_fl_breadth_of_frost_stun") { }

    class spell_fl_breadth_of_frost_stun_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fl_breadth_of_frost_stun_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->GetTypeId() != TYPEID_UNIT)
                return;

            GetTarget()->RemoveAurasDueToSpell(SPELL_LIVING_METEOR_DAMAGE_REDUCTION);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->GetTypeId() != TYPEID_UNIT || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                return;

            GetTarget()->AddAura(SPELL_LIVING_METEOR_DAMAGE_REDUCTION, GetTarget());
            uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIVING_METEOR_SPEED_TRIGGERED, GetTarget());
            GetTarget()->RemoveAurasDueToSpell(spellId);

        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_fl_breadth_of_frost_stun_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_fl_breadth_of_frost_stun_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fl_breadth_of_frost_stun_AuraScript();
    }
};

class spell_fl_meteor_freeze : public SpellScriptLoader
{
public:
    spell_fl_meteor_freeze() : SpellScriptLoader("spell_fl_meteor_freeze") { }

    class spell_fl_meteor_freeze_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fl_meteor_freeze_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->GetTypeId() != TYPEID_UNIT || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                return;

            uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIVING_METEOR_SPEED_TRIGGERED, GetTarget());
            GetTarget()->RemoveAurasDueToSpell(spellId);
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_fl_meteor_freeze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fl_meteor_freeze_AuraScript();
    }
};

class spell_fl_engulfing_flames_damage_filter : public SpellScriptLoader
{
public:
    spell_fl_engulfing_flames_damage_filter() : SpellScriptLoader("spell_fl_engulfing_flames_damage_filter") { }

    class spell_fl_engulfing_flames_damage_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_engulfing_flames_damage_filter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                targets.remove_if([caster](WorldObject* target)
                {
                    Position pos(*target);
                    return caster->GetExactDist(&pos) > 10.0f;
                });
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_engulfing_flames_damage_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_engulfing_flames_damage_filter_SpellScript();
    }
};

class npc_fl_heart_of_flame : public CreatureScript
{
public:
    npc_fl_heart_of_flame() : CreatureScript("npc_fl_heart_of_flame") {}

    struct npc_fl_heart_of_flameAI : public ScriptedAI
    {
        npc_fl_heart_of_flameAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) {}

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetHover(true);
            me->SendMovementHover();
            DoCast(me, SPELL_HEART_OF_RAGNAROS_DUMMY_AURA, true);
            clickCount = 0;
        }

        void OnSpellClick(Unit* clicker, bool& result)
        {
            if (clickCount++ == 1)
            {
                clicker->CastSpell(clicker, SPELL_HEART_OF_RAGNAROS_CREATE_HEART, true);
                me->DespawnOrUnsummon();
            }
        }
    private:
        uint8 clickCount;
        InstanceScript *instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fl_heart_of_flameAI(creature);
    }
};


// 99399 Burning Wound
class spell_fl_burning_wound_player : public SpellScriptLoader
{
public:
    spell_fl_burning_wound_player() : SpellScriptLoader("spell_fl_burning_wound_player") { }

    class spell_fl_burning_wound_player_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fl_burning_wound_player_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (Unit* target = eventInfo.GetProcTarget())
                return !GetTarget()->IsFriendlyTo(target);
            return false;
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (Unit* target = eventInfo.GetProcTarget())
                GetTarget()->CastSpell(target, SPELL_BURNING_BLAST, true);
        }

        void Register()
        {
            DoCheckProc += AuraCheckProcFn(spell_fl_burning_wound_player_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_fl_burning_wound_player_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fl_burning_wound_player_AuraScript();
    }
};

// 99400 Burning Blast
class spell_fl_burning_blast : public SpellScriptLoader
{
public:
    spell_fl_burning_blast() : SpellScriptLoader("spell_fl_burning_blast") { }

    class spell_fl_burning_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_burning_blast_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Aura* aura = GetCaster()->GetAura(SPELL_BURNING_WOUNDS_PROC))
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), 10 * aura->GetStackAmount()));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fl_burning_blast_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_fl_burning_blast_SpellScript();
    }
};

class spell_fl_lava_wave_triggered : public SpellScriptLoader
{
public:
    spell_fl_lava_wave_triggered() : SpellScriptLoader("spell_fl_lava_wave_triggered") { }

    class spell_fl_lava_wave_triggered_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fl_lava_wave_triggered_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove(GetCaster());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fl_lava_wave_triggered_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fl_lava_wave_triggered_SpellScript();
    }
};

void AddSC_boss_ragnaros_firelands()
{
    new at_sulfuron_keep();
    new boss_ragnaros_firelands();
    new npc_fl_magma_trap();
    new npc_fl_sulfuras_smash();
    new npc_fl_lava_wave();
    new npc_fl_sulfuras_hammer();
    new npc_fl_son_of_flame();
    new npc_fl_engulfing_flame();
    new npc_fl_molten_elemental();
    new npc_fl_lava_scion();
    new npc_fl_blazing_heat();
    new npc_fl_living_meteor();
    new npc_fl_archdruids();
    new npc_fl_molten_erupter();
    new npc_fl_molten_spewer();
    new npc_fl_cloudburst();
    new npc_fl_dreadflame();
    new npc_fl_entrapping_roots();
    new npc_fl_breadth_of_frost();
    new spell_fl_splitting_blow();
    new spell_fl_invoke_sons();
    new spell_fl_blazing_heat();
    new spell_fl_world_in_flames_periodic();
    new spell_fl_world_in_flames();
    new spell_fl_empower_sulfuras();
    new spell_fl_molten_inferno();
    new spell_fl_summon_living_meteor();
    new spell_tank_burning_wounds();
    new spell_breadth_of_frost();
    new spell_fl_ragnaros_stalker_cast();
    new spell_fl_cloudburst_summon();
    new spell_fl_dreadflame_periodic();
    new spell_fl_superheated();
    new spell_fl_lava_bolt();
    new AuraScriptLoaderEx<spell_ragnaros_rage_AuraScript>("spell_ragnaros_rage");
    new spell_fl_breadth_of_frost_stun();
    new spell_fl_meteor_freeze();
    new spell_fl_engulfing_flames_damage_filter();
    new npc_fl_heart_of_flame();
    new spell_fl_burning_wound_player();
    new spell_fl_burning_blast();
    new spell_fl_lava_wave_triggered();
}
