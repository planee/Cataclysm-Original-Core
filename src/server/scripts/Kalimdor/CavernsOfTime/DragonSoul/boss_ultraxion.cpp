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
#include "dragonsoul.h"

enum Talks
{
    TALK_INTRO_1                            = -55320,
    TALK_INTRO_2                            = -55323,
    TALK_AGGRO                              = -55329,
    TALK_HOUR_OF_TWILIGHT                   = -55334,
    TALK_MORE_UNSTABLE                      = -55336,
    TALK_KILLING_A_PLAYER                   = 6,
    TALK_TWILIGHT_ERUPTION                  = -55337,
    TALK_DEATH                              = -55330,
    EMOTE_TWILIGHT_REALM                    = -55410,
    EMOTE_SOURCE_OF_MAGIC                   = -55340,
    EMOTE_TIMELOOP                          = -55341,
    EMOTE_LAST_DEFENDER                     = -55342,
    EMOTE_MORE_UNSTABLE                     = -55343,
    EMOTE_TWILIGHT_ERRUPTION                = -55344,
    EMOTE_MAX_UNSTABLE                      = -55345,
    EMOTE_HOUR_OF_TWILIGHT                  = -56545,


    // Ysera
    TALK_ESSENCE_OF_DREAMS                  = -55326,
    TALK_DEATH_YSERA                        = -56100,

    // Thrall
    TALK_LAST_DEFENDER                      = -55324,
    TALK_OUTRO_TRAHLL                       = -56531,

    // Alexstrasza
    TALK_GIFT_OF_LIFE                       = -55325,
    TALK_DEATH_ALEXSTRASZA                  = -56099,
    TALK_OUTRO_ALEXSTRASZA                  = -56530,

    // Kalecgos
    TALK_SOURCE_OF_MAGIC                    = -55327,

    // Nozdormu
    TALK_TIMELOOP                           = -55328,

    // Gauntlet
    TALK_GAUNTLET_INTRO_DEATHWING           = -56080,
    TALK_GAUNTLET_INTRO_DEATHWING_2         = -56646,
    TALK_GAUNTLET_INTRO_ALEXSTRASZA         = -56081,
    TALK_GAUNTLET_OUTRO_DEATHWING           = -55322,
    TALK_GAUNTLET_OUTRO_DEATHWING_2         = -56647,
    TALK_GAUNTLET_OUTRO_DEATHWING_3         = -56648,
    TALK_GAUNTLET_ULTRAXION_SUMMON_YSERA    = -55321
};

enum Spells
{
    SPELL_TWILIGHT_SHIFT                = 106369,
    SPELL_TWILIGHT_SHIFT_PHASING        = 106368,
    SPELL_INTRO_COSMETIC                = 109397,
    SPELL_INTRO_COSMETIC_2              = 109404, // hit vehicle -> 56259
    SPELL_INTRO_COSMETIC_3              = 109403,
    SPELL_INTRO_COSMETIC_4              = 109395,
    SPELL_INTRO_COSMETIC_5              = 109412,
    SPELL_HEROIC_WILL                   = 105554,
    SPELL_HEROIC_WILL_PHASING           = 106108,
    SPELL_HEROIC_WILL_ROOT              = 106175,
    SPELL_UNSTABLE_MONSTROSITY_6_S      = 106372,
    SPELL_UNSTABLE_MONSTROSITY_5_S      = 106376,
    SPELL_UNSTABLE_MONSTROSITY_4_S      = 106377,
    SPELL_UNSTABLE_MONSTROSITY_3_S      = 106378,
    SPELL_UNSTABLE_MONSTROSITY_2_S      = 106379,
    SPELL_UNSTABLE_MONSTROSITY_1_S      = 106380,
    SPELL_TWILIGHT_INSTABILITY_DAMAGE   = 106374,
    SPELL_HOUR_OF_TWILIGHT              = 106371,
    SPELL_LOOMING_DARKNESS_HC           = 106498,
    SPELL_TWILIGHT_BURST                = 106415,
    SPELL_TWILIGHT_ERUPTION             = 106388,
    SPELL_FADING_LIGHT_TANK             = 105925,
    SPELL_FADING_LIGHT_RAID             = 109075,
    SPELL_FADING_LIGHT_KILL             = 105926,
    SPELL_FADED_INTO_TWILIGHT           = 105927,
    SPELL_REDUCE_PARRY_AND_BLOCK        = 110470,
    SPELL_ACHIEVEMENT_AURA              = 109188,

    // Ultraxion Vehicle
    SPELL_LIGHTNING_COSMETIC            = 105592,
    SPELL_LIGHTNING_COSMETIC_2          = 109405,
    SPELL_LIGHTNING_COSMETIC_3          = 109406,
    SPELL_RIDE_VEHICLE                  = 46598,

    // Alexstrasza
    SPELL_WARD_OF_LIFE                  = 108163,
    SPELL_GIFT_OF_LIFE_SUMMON           = 106042,
    SPELL_GIFT_OF_LIFE_BONUS            = 105896,
    SPELL_CHARGE_DRAGON_SOUL_ALEX       = 108837,

    // Ysera
    SPELL_WARD_OF_DREAM                 = 108164,
    SPELL_ESSENCE_OF_DREAMS_SUMMON      = 106049,
    SPELL_ESSENCE_OF_DREAMS_BONUS       = 105900,
    SPELL_ESSENCE_OF_DREAMS_HEAL        = 105996,
    SPELL_CHARGE_DRAGON_SOUL_YSERA      = 108833,

    // Kalecgos
    SPELL_WARD_OF_MAGIC                 = 108162,
    SPELL_SOURCE_OF_MAGIC_SUMMON        = 106050,
    SPELL_SOURCE_OF_MAGIC_BONUS         = 105903,
    SPELL_DIALOGUE_YSERA                = 109644,
    SPELL_EVENTS_UNFOLD_YSERA           = 109661,
    SPELL_DIALOGUE_NOZDORMU             = 109645,
    SPELL_EVENTS_UNFOLD_NOZDORMU        = 109660,
    SPELL_DIALOGUE_KALECGOS             = 109646,
    SPELL_CHARGING_UP_KALECGOS          = 108491,
    SPELL_DIALOGUE_ALEXSTRAZA           = 109647,
    SPELL_EVENTS_UNFOLD_ALEXSTAZA       = 109663,
    SPELL_DIALOGUE_THRALL               = 109648,
    SPELL_DIALOGUE_2_THRALL             = 109652,
    SPELL_DIALOGUE_3_THRALL             = 109653,
    SPELL_EVENTS_UNFOLD_THRALL          = 109662,
    SPELL_PLAY_MOVIE_DEATHWING_1        = 106087,
    SPELL_SUMMON_ASPECTS_GUNSHIP        = 109438,
    SPELL_CHARGE_DRAGON_SOUL_KALECGOS   = 108836,

    // Nozdormu
    SPELL_WARD_OF_TIME                  = 108160,
    SPELL_TIMELOOP_PROTECTION           = 105984,
    SPELL_TIMELOOP_HEAL                 = 105992,
    SPELL_CHARGE_DRAGON_SOUL_NOZDORMU   = 108838,

    // Thrall
    SPELL_WARD_OF_EARTH                 = 108161,
    SPELL_LAST_DEFENDER_OF_AZEROTH      = 106182,
    SPELL_LAST_DEFENDER_WARRIOR         = 106080,
    SPELL_LAST_DEFENDER_DRUID           = 106224,
    SPELL_LAST_DEFENDER_PALADIN         = 106226,
    SPELL_LAST_DEFENDER_DEATH_KNIGHT    = 106227,
    SPELL_CHARGED_DRAGON_SOUL           = 108531,

    // gauntlet trigger
    SPELL_GAUNTLET_INITIALIZE           = 105795,
    SPELL_CALL_DRAKE                    = 105748, // 17sec
    SPELL_CALL_DRAKE_2                  = 105749, // 16-18
    SPELL_CALL_DRAKE_3                  = 105916,
    SPELL_PING                          = 110166,

    // all assaulter
    SPELL_TWILIGHT_BREATH               = 105858,
    SPELL_TWILIGHT_VISUAL               = 109684,
    SPELL_TEMPERAMENT                   = 98958,

    // twilight assaulter 1
    SPELL_TWILIGHT_FLAMES               = 105610, // 13 trigger hits 57281

    // twilight assaulter 2
    SPELL_TWILIGHT_FLAMES_2             = 105654, // hit 56250

    // twilight assaulter 3
    SPELL_TWILIGHT_ESCAPE               = 109904,

    // Life Warden
    SPELL_SUMMON_DRAKE                  = 108584,
    SPELL_RIDE_DRAKE                    = 108582,
    SPELL_EJECT_ALL_PASSENGERS          = 50630,

    // Hagara Portal pre event
    SPELL_PORTAL_DIALOGUE_THRALL        = 109520,
    SPELL_PORTAL_DIALOGUE_ALEX          = 109524,
    SPELL_PORTAL_DIALOGUE_KALEC         = 109518,
    SPELL_PORTAL_DIALOGUE_KALEC_2       = 109675,
    SPELL_PORTAL_DIALOGUE_THRALL_2      = 109521,
    SPELL_PORTAL_DIALOGUE_THRALL_3      = 109522,
    SPELL_PORTAL_DIALOGUE_YSERA         = 109523,
    SPELL_PORTAL_DIALOGUE_KALEC_3       = 109519,
    SPELL_OPEN_EYE_OF_ETERNITY          = 109527,
    SPELL_EYE_OF_ETERNITY_PORTAL_VIS    = 108227,
    SPELL_EYE_OF_ETERNITY_ACTIVE        = 108203,
};

enum Events
{
    // pre event
    EVENT_YSERA_DIALOGUE                        = 1,
    EVENT_NOZDORMU_DIALOGUE                     = 2,
    EVENT_KALECGOS_DIALOGUE                     = 3,
    EVENT_ALEXSTRAZA_DIALOGUE                   = 4,
    EVENT_THRALL_DIALOGUE                       = 5,
    EVENT_THRALL_DIALOGUE_2                     = 6,
    EVENT_THRALL_DIALOGUE_3                     = 7,
    EVENT_THRALL_EVENT_UNFOLD                   = 8,
    EVENT_DEATHWING_INTRO                       = 9,
    EVENT_DEATHWING_INTRO_2                     = 10,
    EVENT_DEATHWING_INTRO_3                     = 11,
    EVENT_CALL_DRAKE                            = 12,
    EVENT_CALL_DRAKE_2                          = 13,
    EVENT_CALL_DRAKE_3                          = 14,
    EVENT_PING_PLAYER                           = 15,
    EVENT_SUMMON_TWILIGHT_FLAME                 = 16,
    EVENT_TWILIGHT_BREATH                       = 17,
    EVENT_START_NEXT_WAYPOINT                   = 18,
    EVENT_PORTAL_DIALOGUE_THRALL                = 19,
    EVENT_PORTAL_DIALOGUE_ALEX                  = 20,
    EVENT_PORTAL_DIALOGUE_KALEC                 = 21,
    EVENT_PORTAL_DIALOGUE_KALEC_2               = 22,
    EVENT_PORTAL_DIALOGUE_THRALL_2              = 23,
    EVENT_PORTAL_DIALOGUE_THRALL_3              = 24,
    EVENT_PORTAL_DIALOGUE_YSERA                 = 25,
    EVENT_PORTAL_DIALOGUE_KALEC_3               = 26,
    EVENT_PORTAL_OPEN_EYE_OF_ETERNITY           = 27,
    EVENT_PORTAL_CORRECT_KALECGOS_ORIENTATION   = 28,
    EVENT_DEATHWING_OUTRO                       = 29,
    EVENT_DEATHWING_OUTRO_2                     = 30,
    EVENT_DEATHWING_OUTRO_3                     = 31,
    EVENT_YSERA_OUTRO                           = 32,
    EVENT_SUMMON_ULTRAXION                      = 33,
    EVENT_OUTRO_ASPECTS_MOVE_TO_GUNSHIP         = 34,
    EVENT_OUTRO_TAKE_DRAGON_SOUL                = 35,
    EVENT_OUTRO_THRALL_MOVE_TO_GUNSHIP          = 36,
    EVENT_OUTRO_START_GUNSHIP_VISUAL            = 37,

    // Ultraxion
    EVENT_INTRO_TWILIGHT_REALM                  = 1,
    EVENT_INTRO_YELL                            = 2,
    EVENT_NEW_UNSTABLE_MONSTROSITY              = 3,
    EVENT_FADING_LIGHT                          = 4,
    EVENT_HOUR_OF_TWILIGHT                      = 5,
    EVENT_GIFT_OF_LIFE                          = 6,
    EVENT_ESSENCE_OF_DREAMS                     = 7,
    EVENT_SOURCE_OF_MAGIC                       = 8,
    EVENT_LAST_DEFENDER                         = 9,
    EVENT_TIME_LOOP                             = 10,
    EVENT_OUTRO_ALEXSTRAZA                      = 11,
    EVENT_OUTRO_THRALL                          = 12,
    EVENT_OUTRO_START_CINEMATIC                 = 13,
    EVENT_DESPAWN_ALL_INTRO_PASSENGERS          = 14,
    EVENT_START_COMBAT                          = 15,
    EVENT_TWILIGHT_ERRUPTION                    = 16
};

enum Misc
{
    ACTION_START_ULTRAXION_GAUNTLET     = 1,
    ACTION_ULTRAXION_GAUNTLET_FINISHED  = 2,
    POINT_START_TWILIGHT_EVENT          = 3,
    ACTION_GAUNTLET_FAILED              = 4,
    POINT_WYRMREST_TEMPLE               = 5,
    ACTION_START_ULTRAXION_OUTRO        = 6,
    ACTION_SKIP_GAUNTLET                = 7,
    ACTION_TRIGGER_ERRUPTION            = 8
};

enum Phases
{
    PHASE_NONE  = 0,
    PHASE_INTRO = 1
};

static const DragonSoulEventSpawns twilightAssaulterPos[18] =
{
    { NPC_TWILIGHT_ASSAULTER, -1704.002f, -2342.484f, 362.6108f, 0.180f },
    { NPC_TWILIGHT_ASSAULTER, -1784.568f, -2298.917f, 349.1924f, 0.925f },
    { NPC_TWILIGHT_ASSAULTER, -1876.403f, -2382.273f, 384.1147f, 5.445f },
    { NPC_TWILIGHT_ASSAULTER, -1769.181f, -2298.728f, 361.3657f, 1.605f },
    { NPC_TWILIGHT_ASSAULTER, -1753.882f, -2494.729f, 363.5942f, 4.747f },
    { NPC_TWILIGHT_ASSAULTER, -1663.405f, -2380.936f, 356.7402f, 3.089f },
    { NPC_TWILIGHT_ASSAULTER_2, -1657.483f, -2391.934f, 353.1453f, 0.180f },
    { NPC_TWILIGHT_ASSAULTER_2, -1781.604f, -2475.783f, 345.5302f, 0.925f },
    { NPC_TWILIGHT_ASSAULTER_2, -1887.085f, -2384.481f, 344.0667f, 5.445f },
    { NPC_TWILIGHT_ASSAULTER_2, -1771.738f, -2483.953f, 352.8337f, 1.605f },
    { NPC_TWILIGHT_ASSAULTER_2, -1890.451f, -2377.163f, 343.4819f, 4.747f },
    { NPC_TWILIGHT_ASSAULTER_2, -1777.361f, -2291.009f, 348.2425f, 3.089f },
    { NPC_TWILIGHT_ASSAULTER_3, -1682.115f, -2383.554f, 349.1379f, 0.180f },
    { NPC_TWILIGHT_ASSAULTER_3, -1868.929f, -2395.351f, 377.3523f, 0.925f },
    { NPC_TWILIGHT_ASSAULTER_3, -1807.910f, -2470.686f, 371.6643f, 5.445f },
    { NPC_TWILIGHT_ASSAULTER_3, -1811.601f, -2307.161f, 385.3918f, 1.605f },
    { NPC_TWILIGHT_ASSAULTER_3, -1734.523f, -2302.342f, 358.5193f, 4.747f },
    { NPC_TWILIGHT_ASSAULTER_3, -1836.106f, -2308.285f, 358.7600f, 3.089f }
};

Position const centerPosition = { -1786.995f, -2393.187f, 341.354f, 3.28121f };

uint32 const wardenPathOnTopSize = 22;
const G3D::Vector3 wardenPathOnTheTop[wardenPathOnTopSize] =
{
    { -1763.820f, -2390.750f, 52.08333f },
    { -1762.820f, -2390.750f, 52.08333f },
    { -1740.578f, -2389.561f, 54.87398f },
    { -1673.682f, -2382.570f, 63.57836f },
    { -1677.319f, -2319.083f, 92.56178f },
    { -1736.845f, -2294.443f, 118.5724f },
    { -1774.819f, -2288.722f, 136.8562f },
    { -1836.321f, -2300.380f, 161.9949f },
    { -1879.974f, -2336.273f, 186.3531f },
    { -1894.601f, -2376.986f, 196.7108f },
    { -1898.238f, -2462.675f, 213.7834f },
    { -1862.262f, -2505.594f, 230.8618f },
    { -1799.793f, -2527.880f, 249.2513f },
    { -1733.674f, -2491.040f, 257.5978f },
    { -1710.720f, -2416.802f, 264.3541f },
    { -1715.627f, -2303.434f, 283.3491f },
    { -1757.130f, -2278.021f, 306.2845f },
    { -1803.323f, -2298.861f, 337.1920f },
    { -1815.491f, -2315.554f, 347.1057f },
    { -1810.962f, -2341.512f, 347.1057f },
    { -1801.165f, -2364.521f, 347.1057f },
    { -1801.165f, -2364.521f, 347.1057f }
};

Position const ultraxionPositions [2] =
{
    { -1564.000f, -2369.000f, 250.083f, 3.259168f }, // spawn position
    { -1707.890f, -2384.900f, 353.840f, 3.240329f }  // temple position
};

Position const lightningTargetPositions[37] =
{
    { -1743.150f, -2403.660f, 340.846f, 4.94472f },
    { -1744.770f, -2396.850f, 340.839f, 4.94472f },
    { -1747.070f, -2386.070f, 340.836f, 5.11358f },
    { -1748.770f, -2371.950f, 340.854f, 4.59915f },
    { -1748.430f, -2367.750f, 340.859f, 4.66198f },
    { -1745.620f, -2368.650f, 340.856f, 0.60932f },
    { -1747.370f, -2377.530f, 340.844f, 1.57144f },
    { -1744.660f, -2382.100f, 340.842f, 0.07132f },
    { -1744.380f, -2388.020f, 340.837f, 1.75208f },
    { -1743.210f, -2393.780f, 340.840f, 1.80706f },
    { -1743.000f, -2409.730f, 340.852f, 1.19444f },
    { -1738.590f, -2408.930f, 340.859f, 1.89738f },
    { -1734.520f, -2405.760f, 340.859f, 0.20877f },
    { -1737.740f, -2405.540f, 340.855f, 5.60838f },
    { -1739.490f, -2399.740f, 340.848f, 4.82299f },
    { -1739.980f, -2384.700f, 340.844f, 0.02419f },
    { -1739.540f, -2378.580f, 340.850f, 6.09533f },
    { -1742.860f, -2375.930f, 340.850f, 5.52592f },
    { -1743.350f, -2370.290f, 340.857f, 3.98654f },
    { -1739.970f, -2371.030f, 340.857f, 2.38826f },
    { -1738.700f, -2373.960f, 340.855f, 1.80314f },
    { -1735.170f, -2379.090f, 340.855f, 5.21961f },
    { -1740.030f, -2391.600f, 340.843f, 6.24062f },
    { -1737.150f, -2397.420f, 340.851f, 1.52824f },
    { -1731.580f, -2403.310f, 340.861f, 5.67514f },
    { -1729.370f, -2399.200f, 340.861f, 0.95882f },
    { -1728.260f, -2395.740f, 340.861f, 5.35314f },
    { -1727.550f, -2391.420f, 340.861f, 0.18128f },
    { -1727.630f, -2387.140f, 340.861f, 6.05607f },
    { -1728.460f, -2383.030f, 340.861f, 0.30695f },
    { -1730.410f, -2378.680f, 340.861f, 5.95789f },
    { -1733.900f, -2374.000f, 340.860f, 0.12130f },
    { -1735.850f, -2386.490f, 340.849f, 1.74423f },
    { -1730.490f, -2389.950f, 340.857f, 4.72482f },
    { -1733.850f, -2382.930f, 340.855f, 4.88974f },
    { -1733.300f, -2398.770f, 340.857f, 1.54002f },
    { -1733.860f, -2392.650f, 340.852f, 0.20091f }
};

static const DragonSoulEventSpawns AspectTargets[3] =
{
    { NPC_YSERA_TARGET,         -1758.713f, -2400.361f, 340.800f, 5.983f },
    { NPC_ALEXSTRASZA_TARGET,   -1760.401f, -2380.817f, 340.801f, 0.501f },
    { NPC_KALECGOS_TARGET,      -1755.512f, -2390.326f, 340.800f, 0.100f }
};

class UltraxionVehicleVisualEvent : public BasicEvent
{
public:
    UltraxionVehicleVisualEvent(Unit* owner) : _summon(owner) {}

    bool Execute(uint64 /*execTime*/, uint32 /*diff*/)
    {
        _summon->CastSpell(_summon, RAND(SPELL_LIGHTNING_COSMETIC, SPELL_LIGHTNING_COSMETIC_2, SPELL_LIGHTNING_COSMETIC_3), true);
        _summon->m_Events.AddEvent(this, _summon->m_Events.CalculateTime(urand(500, 3000)));
        return false;
    }

private:
    Unit* _summon;
};

class boss_ds_ultraxion : public CreatureScript
{
public:
    boss_ds_ultraxion() : CreatureScript("boss_ds_ultraxion") {}

    struct boss_ds_ultraxionAI : public BossAI
    {
        boss_ds_ultraxionAI(Creature* creature) : BossAI(creature, DATA_ULTRAXION) {}

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            mostrosityCounter = 0;
            fadingLightCount = 0;
            _Reset();
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
            {
                if (!me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
                    me->CastSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, true);
            }
            else if (instance->GetData(DATA_RAID_NERF) == RAID_NERF_ACTIVE && !me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
            {
                int32 currentNormalAndHcNerfAmount = -5;
                me->CastCustomSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, &currentNormalAndHcNerfAmount, &currentNormalAndHcNerfAmount, NULL, true);
                DoCast(me, SPELL_POWER_OF_THE_ASPECTS, true);
            }
        }

        void CleanupEncounter()
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT_PHASING);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_HEROIC_WILL);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LAST_DEFENDER_DEATH_KNIGHT);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LAST_DEFENDER_DRUID);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LAST_DEFENDER_WARRIOR);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LAST_DEFENDER_PALADIN);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ESSENCE_OF_DREAMS_BONUS);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TIMELOOP_PROTECTION);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GIFT_OF_LIFE_BONUS);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SOURCE_OF_MAGIC_BONUS);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FADING_LIGHT_TANK, me->GetGUID(), AURA_REMOVE_BY_CANCEL);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FADING_LIGHT_RAID, me->GetGUID(), AURA_REMOVE_BY_CANCEL);

            std::list<WorldObject*> targetList;
            Trinity::AllWorldObjectsInRange objects(me, 100.0f);
            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
            me->VisitNearbyObject(100.0f, searcher);
            for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                if (GameObject* go = (*i)->ToGameObject())
                    if (go->GetEntry() == GO_ESSENCE_OF_DREAMS || go->GetEntry() == GO_SOURCE_OF_MAGIC || go->GetEntry() == GO_GIFT_OF_LIFE)
                        go->Delete();
        }

        void IsSummonedBy(Unit* summoner)
        {
            /*Talk(TALK_INTRO_1);
            me->SetHomePosition(ultraxionPositions[1]);
            me->GetMotionMaster()->MovePoint(POINT_WYRMREST_TEMPLE, ultraxionPositions[1]);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetUInt32Value(UNIT_FIELD_BYTES_0, 0x100);*/

			// Fix pathing, Ultraxion didn't move to the platform:
			Talk(TALK_INTRO_1);
            DoZoneInCombat(me);
            me->SetCanFly(true);
            me->SetDisableGravity(true);	
            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            // Makes Ultraxion never being targetable again..
//          me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE);
			me->GetMotionMaster()->MovePoint(0, ultraxionPositions[1]);
            me->SetSpeed(MOVE_FLIGHT, 0.5f);
			me->SetUInt32Value(UNIT_FIELD_BYTES_0, 0x100);

            for (uint8 i = 0; i < 37; i++)
                me->SummonCreature(NPC_LIGHTNING_COSMETIC_TARGET, lightningTargetPositions[i], TEMPSUMMON_TIMED_DESPAWN, 30000);

            Position pos(*me);
            for (int32 i = 0; i < 6; i++)
            {
                if (Creature* passenger = me->SummonCreature(NPC_ULTRAXION_VEHICLE, pos))
                {
                    passenger->CastCustomSpell(me, SPELL_RIDE_VEHICLE, &i, NULL, NULL, TRIGGERED_FULL_MASK);
                    passenger->m_Events.AddEvent(new UltraxionVehicleVisualEvent(passenger), passenger->m_Events.CalculateTime(urand(300, 1000)));
                }
            }

            events.SetPhase(PHASE_INTRO);
            events.ScheduleEvent(EVENT_INTRO_TWILIGHT_REALM, 8000);
            events.ScheduleEvent(EVENT_DESPAWN_ALL_INTRO_PASSENGERS, 30000);
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);

            for (uint8 i = 0; i < 3; i++)
                me->SummonCreature(AspectTargets[i].npcId, AspectTargets[i].x, AspectTargets[i].y, AspectTargets[i].z, AspectTargets[i].orientation, TEMPSUMMON_MANUAL_DESPAWN);

            Talk(TALK_AGGRO);
            AddEncounterFrame();
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ACHIEVEMENT_AURA);
            DoCastAOE(SPELL_TWILIGHT_SHIFT, true);
            DoCastAOE(SPELL_HEROIC_WILL, true);
            DoCast(me, SPELL_REDUCE_PARRY_AND_BLOCK, true);
            DoCast(me, SPELL_UNSTABLE_MONSTROSITY_6_S, true);
            me->SetReactState(REACT_AGGRESSIVE);
            events.ScheduleEvent(EVENT_LAST_DEFENDER, 10000);
            events.ScheduleEvent(EVENT_HOUR_OF_TWILIGHT, 45500);
            events.ScheduleEvent(EVENT_NEW_UNSTABLE_MONSTROSITY, 60000);
            events.ScheduleEvent(EVENT_GIFT_OF_LIFE, 80000);
            events.ScheduleEvent(EVENT_ESSENCE_OF_DREAMS, 155000);
            events.ScheduleEvent(EVENT_SOURCE_OF_MAGIC, 215000);
            events.ScheduleEvent(EVENT_TIME_LOOP, 295000);
            _EnterCombat();
        }

        bool CanAIAttack(Unit const* target) const
        {
            return target->HasAura(SPELL_TWILIGHT_SHIFT_PHASING);
        }

        void JustDied(Unit* killer) override
        {
            Talk(TALK_DEATH);
            RemoveEncounterFrame();
            CleanupEncounter();
            _JustDied();
            if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
            {
                thrall->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                thrall->InterruptNonMeleeSpells(false);
                thrall->RemoveAllAuras();
                thrall->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                thrall->NearTeleportTo(thrall->GetHomePosition());
            }

            if (Creature* alex = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
            {
                alex->InterruptNonMeleeSpells(false);
                alex->RemoveAllAuras();
                alex->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                alex->NearTeleportTo(alex->GetHomePosition());
            }

            if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
            {
                kalecgos->InterruptNonMeleeSpells(false);
                kalecgos->RemoveAllAuras();
                kalecgos->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                kalecgos->NearTeleportTo(kalecgos->GetHomePosition());
                kalecgos->AI()->DoAction(ACTION_START_ULTRAXION_OUTRO);
            }

            if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
            {
                nozdormu->InterruptNonMeleeSpells(false);
                nozdormu->RemoveAllAuras();
                nozdormu->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                nozdormu->NearTeleportTo(nozdormu->GetHomePosition());
            }

            if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
            {
                ysera->InterruptNonMeleeSpells(false);
                ysera->RemoveAllAuras();
                ysera->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                ysera->NearTeleportTo(ysera->GetHomePosition());
            }

            if (Creature* dragonSoul = instance->GetCreature(DATA_THE_DRAGON_SOUL_ULTRAXION))
                dragonSoul->NearTeleportTo(dragonSoul->GetHomePosition());

            if (GameObject* chest = instance->GetGameObject((instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR) ? DATA_ULTRAXION_CHEST_LFR : DATA_ULTRAXION_CHEST))
            {
                chest->SetRespawnTime(chest->GetRespawnDelay());
                chest->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                chest->SetLootRecipient(me->GetLootRecipient());
            }
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(TALK_KILLING_A_PLAYER);
        }

        void EnterEvadeMode() override
        {
            RemoveEncounterFrame();
            CleanupEncounter();
            if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                thrall->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->DespawnOrUnsummon(1000);
            BossAI::EnterEvadeMode();
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_TRIGGER_ERRUPTION)
                events.ScheduleEvent(EVENT_TWILIGHT_ERRUPTION, 2000);
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
                    else if (!me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        me->resetAttackTimer();
                        DoCastVictim(SPELL_TWILIGHT_BURST);
                    }
                }
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!events.IsInPhase(PHASE_INTRO) && !UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_INTRO_TWILIGHT_REALM:
                        Talk(EMOTE_TWILIGHT_REALM, NULL, CHAT_MSG_RAID_BOSS_EMOTE, TEXT_RANGE_MAP);
                        DoCastAOE(SPELL_TWILIGHT_SHIFT, true);
                        events.ScheduleEvent(EVENT_INTRO_YELL, 4000);
                        break;
                    case EVENT_INTRO_YELL:
                        Talk(TALK_INTRO_2);
                        break;
                    case EVENT_NEW_UNSTABLE_MONSTROSITY:
                        switch (++mostrosityCounter)
                        {
                            case 1:
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_6_S);
                                DoCast(me, SPELL_UNSTABLE_MONSTROSITY_5_S, true);
                                break;
                            case 2:
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_5_S);
                                DoCast(me, SPELL_UNSTABLE_MONSTROSITY_4_S, true);
                                break;
                            case 3:
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_4_S);
                                DoCast(me, SPELL_UNSTABLE_MONSTROSITY_3_S, true);
                                break;
                            case 4:
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_3_S);
                                DoCast(me, SPELL_UNSTABLE_MONSTROSITY_2_S, true);
                                break;
                            case 5:
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_2_S);
                                DoCast(me, SPELL_UNSTABLE_MONSTROSITY_1_S, true);
                                break;
                            case 6:
                                Talk(EMOTE_TWILIGHT_ERRUPTION, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_MONSTROSITY_1_S);
                                DoCast(me, SPELL_TWILIGHT_ERUPTION);
                                Talk(TALK_TWILIGHT_ERUPTION);
                                return;
                            default:
                                break;
                        }
                        Talk(TALK_MORE_UNSTABLE);
                        if (mostrosityCounter < 5)
                            Talk(EMOTE_MORE_UNSTABLE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        else
                            Talk(EMOTE_MAX_UNSTABLE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        events.ScheduleEvent(EVENT_NEW_UNSTABLE_MONSTROSITY, 60000);
                        break;
                    case EVENT_FADING_LIGHT:
                        DoCastVictim(SPELL_FADING_LIGHT_TANK, true);
                        if (++fadingLightCount < (IsHeroic() ? 3 : 2))
                            events.ScheduleEvent(EVENT_FADING_LIGHT, IsHeroic() ? 10000 : 15000);
                        else
                            fadingLightCount = 0;
                        break;
                    case EVENT_HOUR_OF_TWILIGHT:
                        Talk(TALK_HOUR_OF_TWILIGHT);
                        Talk(EMOTE_HOUR_OF_TWILIGHT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        DoCast(me, SPELL_HOUR_OF_TWILIGHT);
                        events.ScheduleEvent(EVENT_HOUR_OF_TWILIGHT, 45500);
                        events.ScheduleEvent(EVENT_FADING_LIGHT, IsHeroic() ? 13000 : 20000);
                        break;
                    case EVENT_LAST_DEFENDER:
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                        {
                            Talk(EMOTE_LAST_DEFENDER, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                            thrall->AI()->Talk(TALK_LAST_DEFENDER);
                            thrall->CastSpell(thrall, SPELL_LAST_DEFENDER_OF_AZEROTH, true);
                        }
                        break;
                    case EVENT_GIFT_OF_LIFE:
                        if (Creature* alexstrasza = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                        {
                            alexstrasza->AI()->Talk(TALK_GIFT_OF_LIFE);
                            alexstrasza->CastSpell(alexstrasza, SPELL_GIFT_OF_LIFE_SUMMON, true);
                        }
                        break;
                    case EVENT_ESSENCE_OF_DREAMS:
                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                        {
                            ysera->AI()->Talk(TALK_ESSENCE_OF_DREAMS);
                            ysera->CastSpell(ysera, SPELL_ESSENCE_OF_DREAMS_SUMMON, true);
                        }
                        break;
                    case EVENT_SOURCE_OF_MAGIC:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                        {
                            Talk(EMOTE_SOURCE_OF_MAGIC, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                            kalecgos->AI()->Talk(TALK_SOURCE_OF_MAGIC);
                            kalecgos->CastSpell(kalecgos, SPELL_SOURCE_OF_MAGIC_SUMMON, true);
                        }
                        break;
                    case EVENT_TIME_LOOP:
                        if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                        {
                            Talk(EMOTE_TIMELOOP, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                            nozdormu->AI()->Talk(TALK_TIMELOOP);
                            nozdormu->CastSpell(nozdormu, SPELL_TIMELOOP_PROTECTION, true);
                        }
                        break;
                    case EVENT_DESPAWN_ALL_INTRO_PASSENGERS:
                        for (int8 i = 0; i < 7; i++)
                            if (Unit* passenger = me->GetVehicleKit()->GetPassenger(i))
                                passenger->ToCreature()->DespawnOrUnsummon();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        events.SetPhase(PHASE_NONE);
                        break;
                    case EVENT_TWILIGHT_ERRUPTION:
                        Talk(EMOTE_TWILIGHT_ERRUPTION, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        Talk(TALK_TWILIGHT_ERUPTION);
                        DoCast(me, SPELL_TWILIGHT_ERUPTION);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        uint8 mostrosityCounter;
        uint8 fadingLightCount;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ds_ultraxionAI(creature);
    }
};

class npc_ds_kalecgos : public CreatureScript
{
public:
    npc_ds_kalecgos() : CreatureScript("npc_ds_kalecgos") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                {
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->DoAction(ACTION_START_ULTRAXION_GAUNTLET);
                    player->CLOSE_GOSSIP_MENU();
                }
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
            if ((instance->IsDone(DATA_HAGARA) || instance->GetData(DATA_LFR_PART) == LFR_PART_TWO) && !instance->IsDone(DATA_ULTRAXION))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We were successful, Kalecgos.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(13373, creature->GetGUID());
        return true;
    }

    struct npc_ds_kalecgosAI : public Scripted_NoMovementAI
    {
        npc_ds_kalecgosAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_START_ULTRAXION_GAUNTLET:
                    events.ScheduleEvent(EVENT_YSERA_DIALOGUE, 1000);
                    break;
                case ACTION_ULTRAXION_GAUNTLET_FINISHED:
                    events.ScheduleEvent(EVENT_DEATHWING_OUTRO, 3000);
                    break;
                case ACTION_START_ULTRAXION_OUTRO:
                    events.ScheduleEvent(EVENT_OUTRO_ALEXSTRAZA, 16000);
                    break;
                case ACTION_SKIP_GAUNTLET:
                    events.ScheduleEvent(EVENT_YSERA_OUTRO, 500);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_YSERA_DIALOGUE:
                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                            DoCast(ysera, SPELL_DIALOGUE_YSERA, true);
                        events.ScheduleEvent(EVENT_NOZDORMU_DIALOGUE, 6000);
                        break;
                    case EVENT_NOZDORMU_DIALOGUE:
                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                            DoCast(ysera, SPELL_EVENTS_UNFOLD_YSERA, true);
                        if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                            DoCast(nozdormu, SPELL_DIALOGUE_NOZDORMU, true);
                        events.ScheduleEvent(EVENT_KALECGOS_DIALOGUE, 3000);
                        break;
                    case EVENT_KALECGOS_DIALOGUE:
                        if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                            DoCast(nozdormu, SPELL_EVENTS_UNFOLD_NOZDORMU, true);
                        DoCast(me, SPELL_DIALOGUE_KALECGOS, true);
                        events.ScheduleEvent(EVENT_ALEXSTRAZA_DIALOGUE, 6000);
                        break;
                    case EVENT_ALEXSTRAZA_DIALOGUE:
                        if (Creature* alexstraza = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                            DoCast(alexstraza, SPELL_DIALOGUE_ALEXSTRAZA, true);
                        DoCast(me, SPELL_CHARGING_UP_KALECGOS, true);
                        events.ScheduleEvent(EVENT_THRALL_DIALOGUE, 8000);
                        break;
                    case EVENT_THRALL_DIALOGUE:
                        if (Creature* alexstraza = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                            DoCast(alexstraza, SPELL_EVENTS_UNFOLD_ALEXSTAZA, true);
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                            DoCast(thrall, SPELL_DIALOGUE_THRALL, true);
                        events.ScheduleEvent(EVENT_THRALL_DIALOGUE_2, 6000);
                        break;
                    case EVENT_THRALL_DIALOGUE_2:
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                            DoCast(thrall, SPELL_DIALOGUE_2_THRALL, true);
                        events.ScheduleEvent(EVENT_THRALL_DIALOGUE_3, 10000);
                        break;
                    case EVENT_THRALL_DIALOGUE_3:
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                            DoCast(thrall, SPELL_DIALOGUE_3_THRALL, true);
                        events.ScheduleEvent(EVENT_THRALL_EVENT_UNFOLD, 5000);
                        break;
                    case EVENT_THRALL_EVENT_UNFOLD:
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                        {
                            DoCast(thrall, SPELL_EVENTS_UNFOLD_THRALL, true);
                            thrall->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        }
                        break;
                    case EVENT_DEATHWING_OUTRO:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->AI()->Talk(TALK_GAUNTLET_OUTRO_DEATHWING, NULL, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_MAP);
                        events.ScheduleEvent(EVENT_DEATHWING_OUTRO_2, 14000);
                        break;
                    case EVENT_DEATHWING_OUTRO_2:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->AI()->Talk(TALK_GAUNTLET_OUTRO_DEATHWING_2, NULL, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_MAP);
                        events.ScheduleEvent(EVENT_DEATHWING_OUTRO_3, 14000);
                        break;
                    case EVENT_DEATHWING_OUTRO_3:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->AI()->Talk(TALK_GAUNTLET_OUTRO_DEATHWING_3, NULL, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_MAP);
                        events.ScheduleEvent(EVENT_YSERA_OUTRO, 12000);
                        break;
                    case EVENT_YSERA_OUTRO:
                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                            ysera->AI()->Talk(TALK_GAUNTLET_ULTRAXION_SUMMON_YSERA, NULL, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_MAP);
                        events.ScheduleEvent(EVENT_SUMMON_ULTRAXION, 6000);
                        break;
                    case EVENT_SUMMON_ULTRAXION:
                        me->SummonCreature(NPC_ULTRAXION, ultraxionPositions[0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                        break;
                    case EVENT_OUTRO_ALEXSTRAZA:
                        if (Creature* alexstraza = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                            alexstraza->AI()->Talk(TALK_OUTRO_ALEXSTRASZA);
                        events.ScheduleEvent(EVENT_OUTRO_THRALL, 9000);
                        break;
                    case EVENT_OUTRO_THRALL:
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                            thrall->AI()->Talk(TALK_OUTRO_TRAHLL);
                        events.ScheduleEvent(EVENT_OUTRO_START_CINEMATIC, 12000);
                        break;
                    case EVENT_OUTRO_START_CINEMATIC:
                    {
                        DoCast(me, SPELL_PLAY_MOVIE_DEATHWING_1, true);

                        if (GameObject* hordeShip = instance->GetGameObject(DATA_HORDE_SHIP_CINEMATIC))
                            hordeShip->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

                        if (GameObject* allianceShip = instance->GetGameObject(DATA_ALLIANCE_SHIP_CINEMATIC))
                            allianceShip->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

                        if (GameObject* battleShip = instance->GetGameObject(DATA_ALLIANCE_BATTLE_SHIP))
                            battleShip->SetDestructibleState(GO_DESTRUCTIBLE_INTACT);

                        // summon part is a workaround because we don't get TARGET_DEST_NEARBY_ENTRY working correctly
                        if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                        {
                            thrall->SetVisible(false);
                            thrall->SummonCreature(NPC_OUTRO_THRALL, thrall->GetHomePosition());
                        }

                        if (Creature* alex = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                        {
                            alex->SetVisible(false);
                            alex->SummonCreature(NPC_OUTRO_ALEXSTRAZA, alex->GetHomePosition());
                        }

                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                        {
                            ysera->SetVisible(false);
                            ysera->SummonCreature(NPC_OUTRO_YSERA, ysera->GetHomePosition());
                        }

                        if (Creature* noz = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                        {
                            noz->SetVisible(false);
                            noz->SummonCreature(NPC_OUTRO_NOZDORMU, noz->GetHomePosition());
                        }

                        if (Creature* dragonSoul = instance->GetCreature(DATA_THE_DRAGON_SOUL_ULTRAXION))
                        {
                            dragonSoul->SetVisible(false);
                            dragonSoul->SummonCreature(NPC_OUTRO_THE_DRAGON_SOUL, dragonSoul->GetHomePosition());
                        }

                        me->SetVisible(false);
                        me->SummonCreature(NPC_OUTRO_KALECGOS, me->GetHomePosition());
                        me->SummonCreature(NPC_SKY_CAPTAIN_SWAYZE, skyCaptainPos);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        private:
            InstanceScript* instance;
            EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_kalecgosAI(creature);
    }
};

class npc_ds_ultraxion_gauntlet : public CreatureScript
{
public:
    npc_ds_ultraxion_gauntlet() : CreatureScript("npc_ds_ultraxion_gauntlet") { }

    struct npc_ds_ultraxion_gauntletAI : public Scripted_NoMovementAI
    {
        npc_ds_ultraxion_gauntletAI(Creature* creature) : Scripted_NoMovementAI(creature), summons(creature)
        {
            instance = me->GetInstanceScript();
        }

        void EnterEvadeMode(){}
        void AttackStart(Unit* target) {}
        void EnterCombat(Unit* attacker) {}
        void Reset() {}

        void DoAction(int32 const action)
        {
            if (action == ACTION_START_ULTRAXION_GAUNTLET)
            {
                killCount = 0;
                for (uint8 i = 0; i < 18; i++)
                    me->SummonCreature(twilightAssaulterPos[i].npcId, twilightAssaulterPos[i].x, twilightAssaulterPos[i].y, twilightAssaulterPos[i].z, twilightAssaulterPos[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                events.ScheduleEvent(EVENT_CALL_DRAKE, 21000);
                events.ScheduleEvent(EVENT_CALL_DRAKE_2, 22000);
                events.ScheduleEvent(EVENT_CALL_DRAKE_3, 23000);
                events.ScheduleEvent(EVENT_PING_PLAYER, 20000);
            }
            else if (action == ACTION_GAUNTLET_FAILED)
            {
                events.Reset();
                summons.DespawnAll();
                me->DespawnCreaturesInArea(NPC_TWILIGHT_FLAME_TARGET, 300.0f);

                if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                {
                    thrall->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    thrall->InterruptNonMeleeSpells(false);
                    thrall->RemoveAllAuras();
                    thrall->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                    thrall->NearTeleportTo(thrall->GetHomePosition());
                }

                if (Creature* alex = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                {
                    alex->InterruptNonMeleeSpells(false);
                    alex->RemoveAllAuras();
                    alex->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                    alex->NearTeleportTo(alex->GetHomePosition());
                }

                if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                {
                    kalecgos->InterruptNonMeleeSpells(false);
                    kalecgos->RemoveAllAuras();
                    kalecgos->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                    kalecgos->NearTeleportTo(kalecgos->GetHomePosition());
                }

                if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                {
                    nozdormu->InterruptNonMeleeSpells(false);
                    nozdormu->RemoveAllAuras();
                    nozdormu->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                    nozdormu->NearTeleportTo(nozdormu->GetHomePosition());
                }

                if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                {
                    ysera->InterruptNonMeleeSpells(false);
                    ysera->RemoveAllAuras();
                    ysera->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, 0);
                    ysera->NearTeleportTo(ysera->GetHomePosition());
                }

                if (Creature* dragonSoul = instance->GetCreature(DATA_THE_DRAGON_SOUL_ULTRAXION))
                    dragonSoul->NearTeleportTo(dragonSoul->GetHomePosition());
            }
        }

        void JustSummoned(Creature* summon)
        {
            summon->SetCanFly(true);
            summon->SetDisableGravity(true);
            if (summon->GetEntry() == NPC_TWILIGHT_ASSAULTER)
                summon->CastSpell(summon, SPELL_TEMPERAMENT, true);
            summons.Summon(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer)
        {
            if (++killCount >= 15)
            {
                if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                    kalecgos->AI()->DoAction(ACTION_ULTRAXION_GAUNTLET_FINISHED);
                if (Creature* thrall = instance->GetCreature(DATA_THRALL_ULTRAXION))
                    thrall->AI()->DoAction(ACTION_ULTRAXION_GAUNTLET_FINISHED);
                events.Reset();
                summons.DespawnAll();
                me->DespawnCreaturesInArea(NPC_TWILIGHT_FLAME_TARGET, 300.0f);
                return;
            }
            switch (summon->GetEntry())
            {
                case NPC_TWILIGHT_ASSAULTER:
                case NPC_TWILIGHT_ASSAULTER_2:
                case NPC_TWILIGHT_ASSAULTER_3:
                    me->SummonCreature(summon->GetEntry(), summon->GetHomePosition(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CALL_DRAKE:
                        DoCast(me, SPELL_CALL_DRAKE, true);
                        events.ScheduleEvent(EVENT_CALL_DRAKE, urand(16000, 19000));
                        break;
                    case EVENT_CALL_DRAKE_2:
                        DoCast(me, SPELL_CALL_DRAKE_2, true);
                        events.ScheduleEvent(EVENT_CALL_DRAKE_2, urand(16000, 19000));
                        break;
                    case EVENT_CALL_DRAKE_3:
                        DoCast(me, SPELL_CALL_DRAKE_3, true);
                        events.ScheduleEvent(EVENT_CALL_DRAKE_3, urand(16000, 19000));
                        break;
                    case EVENT_PING_PLAYER:
                        DoCastAOE(SPELL_PING, true);
                        events.ScheduleEvent(EVENT_PING_PLAYER, 1500);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        SummonList summons;
        uint8 killCount;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_ultraxion_gauntletAI(creature);
    }
};

class npc_ds_thrall : public CreatureScript
{
public:
    npc_ds_thrall() : CreatureScript("npc_ds_thrall") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
            if (creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                creature->AI()->DoAction(ACTION_START_ULTRAXION_GAUNTLET);
                player->CLOSE_GOSSIP_MENU();
            }
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
        {
            if ((instance->IsDone(DATA_HAGARA) || instance->GetData(DATA_LFR_PART) == LFR_PART_TWO) && !instance->IsDone(DATA_ULTRAXION))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You may begin your ritual, we will defend you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(13373, creature->GetGUID());
            }
        }
        return true;
    }

    struct npc_ds_thrallAI : public Scripted_NoMovementAI
    {
        npc_ds_thrallAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
            hagaraIntroCheck = false;
            gauntletDone = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (hagaraIntroCheck)
                return;

            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (Group* group = who->ToPlayer()->GetGroup())
                {
                    if (instance->GetData(DATA_LFR_PART) == LFR_PART_TWO)
                    {
                        hagaraIntroCheck = true;
                        return;
                    }
                }

                if (instance->IsDone(DATA_MORCHOK) && instance->IsDone(DATA_WARLORD_ZONOZZ) && instance->IsDone(DATA_YORSAHJ) && !instance->IsDone(DATA_HAGARA))
                {
                    if (me->GetDistance2d(who) <= 25.0f)
                    {
                        hagaraIntroCheck = true;
                        if (!instance->IsDone(DATA_HAGARA))
                            events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_THRALL, 1000);
                    }
                }
            }
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_START_ULTRAXION_GAUNTLET:
                    if (gauntletDone)
                    {
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            kalecgos->AI()->DoAction(ACTION_SKIP_GAUNTLET);
                    }
                    else
                    {
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->GetMotionMaster()->MovePoint(0, -1706.260f, -2357.546f, 360.397f, true, 40.0f);
                        events.ScheduleEvent(EVENT_DEATHWING_INTRO, 5000);
                    }
                    break;
                case ACTION_ULTRAXION_GAUNTLET_FINISHED:
                    gauntletDone = true;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    // Gauntlet Intro
                    case EVENT_DEATHWING_INTRO:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->AI()->Talk(TALK_GAUNTLET_INTRO_DEATHWING, NULL, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_MAP);
                        events.ScheduleEvent(EVENT_DEATHWING_INTRO_2, 11000);
                        break;
                    case EVENT_DEATHWING_INTRO_2:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->AI()->Talk(TALK_GAUNTLET_INTRO_DEATHWING_2, NULL, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_MAP);

                        if (Creature* gauntlet = instance->GetCreature(DATA_GAUNTLET_TRIGGER))
                            gauntlet->AI()->DoAction(ACTION_START_ULTRAXION_GAUNTLET);
                        events.ScheduleEvent(EVENT_DEATHWING_INTRO_3, 20000);
                        break;
                    case EVENT_DEATHWING_INTRO_3:
                        if (Creature* deathwing = instance->GetCreature(DATA_DEATHWING_ULTRAXION))
                            deathwing->GetMotionMaster()->MovePoint(0, -1596.760f, -2141.034f, 360.00f, true, 30.0f);

                        me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        me->SetDisableGravity(true);
                        me->AddUnitState(MOVEMENTFLAG_HOVER);
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5.0f, false);
                        me->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        DoCast(me, SPELL_WARD_OF_EARTH);

                        if (Creature* alexstrasza = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                        {
                            alexstrasza->AI()->Talk(TALK_GAUNTLET_INTRO_ALEXSTRASZA, NULL, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_MAP);
                            alexstrasza->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            alexstrasza->SetDisableGravity(true);
                            alexstrasza->AddUnitState(MOVEMENTFLAG_HOVER);
                            alexstrasza->GetMotionMaster()->MovePoint(0, alexstrasza->GetPositionX(), alexstrasza->GetPositionY(), alexstrasza->GetPositionZ() + 5.0f, false);
                            alexstrasza->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                            alexstrasza->CastSpell(alexstrasza, SPELL_WARD_OF_LIFE);
                        }

                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                        {
                            kalecgos->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            kalecgos->SetDisableGravity(true);
                            kalecgos->AddUnitState(MOVEMENTFLAG_HOVER);
                            kalecgos->GetMotionMaster()->MovePoint(0, kalecgos->GetPositionX(), kalecgos->GetPositionY(), kalecgos->GetPositionZ() + 5.0f, false);
                            kalecgos->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                            kalecgos->CastSpell(kalecgos, SPELL_WARD_OF_MAGIC);
                        }

                        if (Creature* nozdormu = instance->GetCreature(DATA_NOZDORMU_ULTRAXION))
                        {
                            nozdormu->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            nozdormu->SetDisableGravity(true);
                            nozdormu->AddUnitState(MOVEMENTFLAG_HOVER);
                            nozdormu->GetMotionMaster()->MovePoint(0, nozdormu->GetPositionX(), nozdormu->GetPositionY(), nozdormu->GetPositionZ() + 5.0f, false);
                            nozdormu->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                            nozdormu->CastSpell(nozdormu, SPELL_WARD_OF_TIME);
                        }

                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                        {
                            ysera->InterruptSpell(CURRENT_CHANNELED_SPELL);
                            ysera->SetDisableGravity(true);
                            ysera->AddUnitState(MOVEMENTFLAG_HOVER);
                            ysera->GetMotionMaster()->MovePoint(0, ysera->GetPositionX(), ysera->GetPositionY(), ysera->GetPositionZ() + 5.0f, false);
                            ysera->SetByteValue(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                            ysera->CastSpell(ysera, SPELL_WARD_OF_DREAM);
                        }

                        if (Creature* dragonsoul = instance->GetCreature(DATA_THE_DRAGON_SOUL_ULTRAXION))
                            dragonsoul->GetMotionMaster()->MovePoint(0, -1786.71f, -2393.264f, 355.0229f, false);
                        break;
                    // Hagara intro
                    case EVENT_PORTAL_DIALOGUE_THRALL:
                        DoCast(me, SPELL_PORTAL_DIALOGUE_THRALL, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_ALEX, 14000);
                        break;
                    case EVENT_PORTAL_DIALOGUE_ALEX:
                        if (Creature* alex = instance->GetCreature(DATA_ALEXSTRASZA_ULTRAXION))
                            alex->CastSpell(alex, SPELL_PORTAL_DIALOGUE_ALEX, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_KALEC, 5200);
                        break;
                    case EVENT_PORTAL_DIALOGUE_KALEC:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            kalecgos->CastSpell(kalecgos, SPELL_PORTAL_DIALOGUE_KALEC, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_KALEC_2, 9600);
                        break;
                    case EVENT_PORTAL_DIALOGUE_KALEC_2:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            kalecgos->CastSpell(kalecgos, SPELL_PORTAL_DIALOGUE_KALEC_2, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_THRALL_2, 10000);
                        break;
                    case EVENT_PORTAL_DIALOGUE_THRALL_2:
                        DoCast(me, SPELL_PORTAL_DIALOGUE_THRALL_2, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_YSERA, 10000);
                        break;
                    case EVENT_PORTAL_DIALOGUE_YSERA:
                        if (Creature* ysera = instance->GetCreature(DATA_YSERA_ULTRAXION))
                            ysera->CastSpell(ysera, SPELL_PORTAL_DIALOGUE_YSERA, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_THRALL_3, 11500);
                        break;
                    case EVENT_PORTAL_DIALOGUE_THRALL_3:
                        DoCast(me, SPELL_PORTAL_DIALOGUE_THRALL_3, true);
                        events.ScheduleEvent(EVENT_PORTAL_DIALOGUE_KALEC_3, 6000);
                        break;
                    case EVENT_PORTAL_DIALOGUE_KALEC_3:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            kalecgos->CastSpell(kalecgos, SPELL_PORTAL_DIALOGUE_KALEC_3, true);
                        events.ScheduleEvent(EVENT_PORTAL_OPEN_EYE_OF_ETERNITY, 15000);
                        break;
                    case EVENT_PORTAL_OPEN_EYE_OF_ETERNITY:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            if (Creature* portal = me->FindNearestCreature(57377, 50.0f))
                                kalecgos->CastSpell(portal, SPELL_OPEN_EYE_OF_ETERNITY);
                        events.ScheduleEvent(EVENT_PORTAL_CORRECT_KALECGOS_ORIENTATION, 8000);
                        break;
                    case EVENT_PORTAL_CORRECT_KALECGOS_ORIENTATION:
                        if (Creature* kalecgos = instance->GetCreature(DATA_KALECGOS_ULTRAXION))
                            if (Creature* dragonSoul = instance->GetCreature(DATA_THE_DRAGON_SOUL_ULTRAXION))
                                kalecgos->SetFacingToObject(dragonSoul);

                        if (Creature* portal = me->FindNearestCreature(57377, 50.0f))
                        {
                            portal->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                            portal->CastSpell(portal, SPELL_EYE_OF_ETERNITY_PORTAL_VIS, true);
                            portal->CastSpell(portal, SPELL_EYE_OF_ETERNITY_ACTIVE, true);
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool hagaraIntroCheck;
        bool gauntletDone;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_thrallAI(creature);
    }
};

class npc_ds_twilight_assaulter : public CreatureScript
{
public:
    npc_ds_twilight_assaulter() : CreatureScript("npc_ds_twilight_assaulter") { }

    struct npc_ds_twilight_assaulterAI : public ScriptedAI
    {
        npc_ds_twilight_assaulterAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->SetReactState(REACT_PASSIVE);
            distance = 0.1f;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_START_TWILIGHT_EVENT)
            {
                if (me->GetEntry() == NPC_TWILIGHT_ASSAULTER)
                {
                    events.ScheduleEvent(EVENT_SUMMON_TWILIGHT_FLAME, 900);
                    DoCast(me, SPELL_TWILIGHT_FLAMES);
                }
                else
                {
                    if (Player* player = me->FindNearestPlayer(100.0f))
                        me->AI()->AttackStart(player);
                    events.ScheduleEvent(EVENT_TWILIGHT_BREATH, 3000);
                }
            }
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo)
        {
            if (spellInfo->HasEffect(SPELL_EFFECT_ATTACK_ME) || spellInfo->HasAura(SPELL_AURA_MOD_TAUNT) || spellInfo->HasEffect(SPELL_EFFECT_INTERRUPT_CAST))
            {
                events.CancelEvent(EVENT_SUMMON_TWILIGHT_FLAME);
                events.ScheduleEvent(EVENT_TWILIGHT_BREATH, 3000);
                me->RemoveAurasDueToSpell(SPELL_TEMPERAMENT);
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                me->GetMotionMaster()->MoveChase(attacker, 0.00f, 0.00f);
                me->SetReactState(REACT_AGGRESSIVE);
            }
        }

        void EnterEvadeMode() {}

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_TWILIGHT_FLAME:
                    {
                        Position pos(*me);
                        me->GetNearPosition(pos, distance, 0.0f);
                        pos.m_positionZ = 341.3549f;
                        me->SummonCreature(NPC_TWILIGHT_FLAME_TARGET, pos, TEMPSUMMON_MANUAL_DESPAWN);
                        distance += 12.0f;
                        if (distance <= 90.00f)
                            events.ScheduleEvent(EVENT_SUMMON_TWILIGHT_FLAME, 1000);
                        break;
                    }
                    case EVENT_TWILIGHT_BREATH:
                        DoCastVictim(SPELL_TWILIGHT_BREATH);
                        events.ScheduleEvent(EVENT_TWILIGHT_BREATH, 5000);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        float distance;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_twilight_assaulterAI(creature);
    }
};

class npc_ds_nethestrasz_taxi : public CreatureScript
{
public:
    npc_ds_nethestrasz_taxi() : CreatureScript("npc_ds_nethestrasz_taxi") { }

    struct npc_ds_nethestrasz_taxiAI : public PassiveAI
    {
        npc_ds_nethestrasz_taxiAI(Creature* creature) : PassiveAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            point = 0;
            summoner->CastSpell(me, SPELL_RIDE_DRAKE, true);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
        }

        void PassengerBoarded(Unit* player, int8 seatId, bool apply)
        {
            if (!apply)
                me->DespawnOrUnsummon(500);
            else
                me->GetMotionMaster()->MoveSmoothPath(wardenPathOnTheTop, wardenPathOnTopSize);
        }

        void MovementInform(uint32 type, uint32 point)
        {
            if (type == SPLINE_MOTION_TYPE && point == wardenPathOnTopSize-1)
                DoCast(me, SPELL_EJECT_ALL_PASSENGERS);
        }

        void UpdateAI(const uint32 diff)
        {
        }

    private:
        EventMap events;
        uint8 point;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_nethestrasz_taxiAI(creature);
    }
};

class npc_ds_thrall_outro : public CreatureScript
{
public:
    npc_ds_thrall_outro() : CreatureScript("npc_ds_thrall_outro") { }

    struct npc_ds_thrall_outroAI : public ScriptedAI
    {
        npc_ds_thrall_outroAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            events.ScheduleEvent(EVENT_OUTRO_ASPECTS_MOVE_TO_GUNSHIP, 25000);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == WAYPOINT_MOTION_TYPE && id == 5)
            {
                me->SetStandState(UNIT_STAND_STATE_KNEEL);
                events.ScheduleEvent(EVENT_OUTRO_START_GUNSHIP_VISUAL, 7000);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_OUTRO_ASPECTS_MOVE_TO_GUNSHIP:
                        if (Creature* ysera = me->FindNearestCreature(NPC_OUTRO_YSERA, 50.0f))
                            ysera->GetMotionMaster()->MovePath(ysera->GetEntry() * 10, false);
                        if (Creature* nozdormu = me->FindNearestCreature(NPC_OUTRO_NOZDORMU, 50.0f))
                            nozdormu->GetMotionMaster()->MovePath(nozdormu->GetEntry() * 10, false);
                        me->GetMotionMaster()->MovePoint(0, -1788.256f, -2392.104f, 341.354f);
                        events.ScheduleEvent(EVENT_OUTRO_TAKE_DRAGON_SOUL, 2000);
                        break;
                    case EVENT_OUTRO_TAKE_DRAGON_SOUL:
                        if (Creature* dragonSoul = me->FindNearestCreature(NPC_OUTRO_THE_DRAGON_SOUL, 10.0f))
                            dragonSoul->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                        events.ScheduleEvent(EVENT_OUTRO_THRALL_MOVE_TO_GUNSHIP, 5000);
                        break;
                    case EVENT_OUTRO_THRALL_MOVE_TO_GUNSHIP:
                        if (Creature* alex = me->FindNearestCreature(NPC_OUTRO_ALEXSTRAZA, 50.0f))
                            alex->GetMotionMaster()->MovePath(alex->GetEntry() * 10, false);
                        if (Creature* kalecgos = me->FindNearestCreature(NPC_OUTRO_KALECGOS, 50.0f))
                            kalecgos->GetMotionMaster()->MovePath(kalecgos->GetEntry() * 10, false);
                        me->GetMotionMaster()->MovePath(me->GetEntry() * 10, false);
                        break;
                    case EVENT_OUTRO_START_GUNSHIP_VISUAL:
                        DoCast(me, SPELL_CHARGED_DRAGON_SOUL, true);
                        if (Creature* dragonSoul = me->FindNearestCreature(NPC_OUTRO_THE_DRAGON_SOUL, 10.0f))
                        {
                            if (Creature* alex = me->FindNearestCreature(NPC_OUTRO_ALEXSTRAZA, 50.0f))
                            {
                                alex->SetFacingToObject(dragonSoul);
                                alex->CastSpell(dragonSoul, SPELL_CHARGE_DRAGON_SOUL_ALEX, true);
                            }
                            if (Creature* kalecgos = me->FindNearestCreature(NPC_OUTRO_KALECGOS, 50.0f))
                            {
                                kalecgos->SetFacingToObject(dragonSoul);
                                kalecgos->CastSpell(dragonSoul, SPELL_CHARGE_DRAGON_SOUL_KALECGOS, true);
                            }
                            if (Creature* nozdormu = me->FindNearestCreature(NPC_OUTRO_NOZDORMU, 50.0f))
                            {
                                nozdormu->SetFacingToObject(nozdormu);
                                nozdormu->CastSpell(dragonSoul, SPELL_CHARGE_DRAGON_SOUL_NOZDORMU, true);
                            }
                            if (Creature* ysera = me->FindNearestCreature(NPC_OUTRO_YSERA, 50.0f))
                            {
                                ysera->SetFacingToObject(dragonSoul);
                                ysera->CastSpell(dragonSoul, SPELL_CHARGE_DRAGON_SOUL_YSERA, true);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_thrall_outroAI(creature);
    }
};

class spell_ds_gen_dialogue : public SpellScriptLoader
{
public:
    spell_ds_gen_dialogue() : SpellScriptLoader("spell_ds_gen_dialogue") { }

    class spell_ds_gen_dialogue_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_gen_dialogue_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (GetHitUnit()->ToCreature())
                GetHitUnit()->ToCreature()->AI()->Talk(-GetSpellInfo()->Effects[effIndex].BasePoints, NULL, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_MAP);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_gen_dialogue_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_gen_dialogue_SpellScript();
    }
};

class spell_ds_call_dragon : public SpellScriptLoader
{
public:
    spell_ds_call_dragon() : SpellScriptLoader("spell_ds_call_dragon") {}

    class spell_ds_call_dragon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_call_dragon_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 spellId = GetSpellInfo()->Id;
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_VISUAL));
            targets.remove_if([spellId](WorldObject* target)
            {
                if (target->GetTypeId() != TYPEID_UNIT
                    || (spellId == SPELL_CALL_DRAKE && target->GetEntry() != NPC_TWILIGHT_ASSAULTER)
                    || (spellId == SPELL_CALL_DRAKE_2 && target->GetEntry() != NPC_TWILIGHT_ASSAULTER_2)
                    || (spellId == SPELL_CALL_DRAKE_3 && target->GetEntry() != NPC_TWILIGHT_ASSAULTER_3))
                    return true;
                return false;
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* gauntlet = instance->GetCreature(DATA_GAUNTLET_TRIGGER))
                {
                    Position pos(centerPosition);
                    float angle = gauntlet->GetAngle(GetHitUnit());
                    gauntlet->MoveBlink(pos, 60.0f, angle);
                    pos.m_positionZ = 354.210f;
                    GetHitUnit()->GetMotionMaster()->MovePoint(POINT_START_TWILIGHT_EVENT, pos, false);
                    GetHitUnit()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    GetHitUnit()->RemoveAurasDueToSpell(SPELL_TWILIGHT_VISUAL);
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_call_dragon_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_call_dragon_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_call_dragon_SpellScript();
    }
};

class spell_ds_twilight_flames : public SpellScriptLoader
{
public:
    spell_ds_twilight_flames() : SpellScriptLoader("spell_ds_twilight_flames") {}

    class spell_ds_twilight_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_twilight_flames_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
                targets.remove_if([caster](WorldObject* target)
            {
                if (!target->ToCreature() || !target->ToCreature()->ToTempSummon())
                    return true;

                if (target->ToCreature()->ToTempSummon()->GetSummoner() != caster)
                    return true;

                return false;
            });
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_twilight_flames_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_twilight_flames_SpellScript();
    }
};

class spell_ds_ping : public SpellScriptLoader
{
public:
    spell_ds_ping() : SpellScriptLoader("spell_ds_ping") { }

    class spell_ds_ping_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_ping_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                GetCaster()->ToCreature()->AI()->DoAction(ACTION_GAUNTLET_FAILED);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_ping_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_ping_SpellScript();
    }
};

class spell_ds_twilight_instability_trigger : public SpellScriptLoader
{
public:
    spell_ds_twilight_instability_trigger() : SpellScriptLoader("spell_ds_twilight_instability_trigger") { }

    class spell_ds_twilight_instability_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_twilight_instability_trigger_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_SHIFT_PHASING));

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_twilight_instability_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_twilight_instability_trigger_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_twilight_instability_trigger_SpellScript();
    }
};

// we currently dont know for what this spell is... we just know that it trigger the spellscript above...
class spell_ds_twilight_instability : public SpellScriptLoader
{
public:
    spell_ds_twilight_instability() : SpellScriptLoader("spell_ds_twilight_instability") { }

    class spell_ds_twilight_instability_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_twilight_instability_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_SHIFT_PHASING));

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (!GetCaster()->FindCurrentSpellBySpellId(sSpellMgr->GetSpellIdForDifficulty(SPELL_HOUR_OF_TWILIGHT, GetCaster())))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_TWILIGHT_INSTABILITY_DAMAGE, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_twilight_instability_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_twilight_instability_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_twilight_instability_SpellScript();
    }
};

class spell_ds_fading_light : public SpellScriptLoader
{
public:
    spell_ds_fading_light() : SpellScriptLoader("spell_ds_fading_light") { }

    class spell_ds_fading_light_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_fading_light_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                {
                    targets.clear();
                    return;
                }
            }

            Difficulty difficulty = GetCaster()->GetMap()->GetDifficulty();
            uint8 maxTargets = 1;
            if (difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                maxTargets = 2;
            else if (difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                maxTargets = 3;
            else if (difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                maxTargets = 5;

            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_SHIFT_PHASING));

            if (GetCaster()->getVictim())
                targets.remove(GetCaster()->getVictim());

            // videos : always target the main tank and random dps but never hit a heal spec
            // removed tank first due to targets.size() > maxTargets
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end() && targets.size() > maxTargets; )
            {
                WorldObject* target = (*itr);
                itr++;
                if (target->ToPlayer()->HasTankSpec())
                    targets.remove(target);
            }
            // heal then
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end() && targets.size() > maxTargets; )
            {
                WorldObject* target = (*itr);
                itr++;
                if (target->ToPlayer()->HasHealSpec())
                    targets.remove(target);
            }

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, maxTargets);
        }

        void Register()
        {
            if (m_scriptSpellId == 109075 || m_scriptSpellId == 110080 || m_scriptSpellId == 110079 || m_scriptSpellId == 110078)
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_fading_light_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_fading_light_SpellScript();
    }

    class spell_ds_fading_light_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_fading_light_AuraScript);

        void HandleAfterEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            int32 duration = urand(5000, 9500);
            GetAura()->SetMaxDuration(duration);
            GetAura()->SetDuration(duration);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                return;

            if (GetTarget()->HasAura(SPELL_TWILIGHT_SHIFT_PHASING))
                GetTarget()->CastSpell(GetTarget(), SPELL_FADING_LIGHT_KILL, true);
            else
            {
                if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                    if (Creature* ultraxion = instance->GetCreature(DATA_ULTRAXION))
                        GetTarget()->CastSpell(ultraxion, SPELL_FADED_INTO_TWILIGHT, true);
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_ds_fading_light_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            AfterEffectRemove += AuraEffectRemoveFn(spell_ds_fading_light_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_fading_light_AuraScript();
    }
};

class spell_ds_heroic_will : public SpellScriptLoader
{
public:
    spell_ds_heroic_will() : SpellScriptLoader("spell_ds_heroic_will") { }

    class spell_ds_heroic_will_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_heroic_will_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                return;

            if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                GetTarget()->CastSpell(GetTarget(), SPELL_TWILIGHT_SHIFT_PHASING, true);
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* target = GetTarget()->ToPlayer())
                if (target->GetPetGUID())
                    if (Unit* pet = ObjectAccessor::GetUnit(*target, target->GetPetGUID()))
                        pet->AddAura(GetId(), pet);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_ds_heroic_will_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_PACIFY_SILENCE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_ds_heroic_will_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_PACIFY_SILENCE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_heroic_will_AuraScript();
    }
};

class spell_ds_hour_of_twilight_damage : public SpellScriptLoader
{
public:
    spell_ds_hour_of_twilight_damage() : SpellScriptLoader("spell_ds_hour_of_twilight_damage") {}

    class spell_ds_hour_of_twilight_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_hour_of_twilight_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Difficulty difficulty = GetCaster()->GetMap()->GetDifficulty();
            uint8 minTargets = 1;
            if (difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                minTargets = 1;
            else if (difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                minTargets = 2;
            else if (difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                minTargets = 3;
            else if (difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                minTargets = 5;

            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_SHIFT_PHASING));

            if (targets.size() < minTargets)
            {
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                    if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                        return;

                if (GetCaster()->ToCreature())
                    GetCaster()->ToCreature()->AI()->DoAction(ACTION_TRIGGER_ERRUPTION);
            }
        }

        void HandleImmuneDamage(SpellEffIndex effIndex)
        {
            if (GetHitUnit()->IsImmunedToDamage(GetSpellInfo()) || GetHitUnit()->HasAura(19263))
                SetHitDamage(0);
        }

        void HandleForceCast(SpellEffIndex effIndex)
        {
            if (!GetHitUnit()->HasAura(SPELL_LOOMING_DARKNESS_HC))
                PreventHitDefaultEffect(effIndex);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_hour_of_twilight_damage_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_hour_of_twilight_damage_SpellScript::HandleImmuneDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(spell_ds_hour_of_twilight_damage_SpellScript::HandleForceCast, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_hour_of_twilight_damage_SpellScript();
    }
};

class spell_ds_last_defender_of_azeroth : public SpellScriptLoader
{
public:
    spell_ds_last_defender_of_azeroth() : SpellScriptLoader("spell_ds_last_defender_of_azeroth") {}

    class spell_ds_last_defender_of_azeroth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_last_defender_of_azeroth_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                if (target->ToPlayer() && target->ToPlayer()->HasTankSpec()) // possible exploit for cat ferals and deathknight with HasTankSpec but there is no better way to check this
                    return false;
                return true;
            });
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_last_defender_of_azeroth_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_last_defender_of_azeroth_SpellScript();
    }
};

class spell_ds_last_defender_of_azeroth_buff : public SpellScriptLoader
{
public:
    spell_ds_last_defender_of_azeroth_buff() : SpellScriptLoader("spell_ds_last_defender_of_azeroth_buff") { }

    class spell_ds_last_defender_of_azeroth_buff_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_last_defender_of_azeroth_buff_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetHitUnit() || !GetHitUnit()->ToPlayer())
                return;

            uint32 currentSpec = GetHitUnit()->ToPlayer()->GetPrimaryTalentTree(GetHitUnit()->ToPlayer()->GetActiveSpec());

            switch (currentSpec)
            {
                case TALENT_TREE_WARRIOR_PROTECTION:
                    GetHitUnit()->AddAura(SPELL_LAST_DEFENDER_WARRIOR, GetHitUnit());
                    break;
                case TALENT_TREE_PALADIN_PROTECTION:
                    GetHitUnit()->AddAura(SPELL_LAST_DEFENDER_PALADIN, GetHitUnit());
                    break;
                case TALENT_TREE_DEATH_KNIGHT_BLOOD:
                    GetHitUnit()->AddAura(SPELL_LAST_DEFENDER_DEATH_KNIGHT, GetHitUnit());
                    break;
                case TALENT_TREE_DRUID_FERAL_COMBAT:
                    GetHitUnit()->AddAura(SPELL_LAST_DEFENDER_DRUID, GetHitUnit());
                    break;
                default:
                    break;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_last_defender_of_azeroth_buff_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_last_defender_of_azeroth_buff_SpellScript();
    }
};

class spell_ds_lightning_cosmetic : public SpellScriptLoader
{
public:
    spell_ds_lightning_cosmetic() : SpellScriptLoader("spell_ds_lightning_cosmetic") {}

    class spell_ds_lightning_cosmetic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_lightning_cosmetic_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                return target->GetEntry() != NPC_LIGHTNING_COSMETIC_TARGET;
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_lightning_cosmetic_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_lightning_cosmetic_SpellScript();
    }
};

class spell_ds_timeloop : public SpellScriptLoader
{
public:
    spell_ds_timeloop() : SpellScriptLoader("spell_ds_timeloop") { }

    class spell_ds_timeloop_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_timeloop_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            amount = -1;
        }

        void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
        {
            if (Unit* target = GetTarget())
            {
                int32 remainingHealth = target->GetHealth() - dmgInfo.GetDamage();
                if (remainingHealth <= 0)
                {
                    int32 maxHealth = target->GetMaxHealth();
                    target->CastCustomSpell(target, SPELL_TIMELOOP_HEAL, &maxHealth, NULL, NULL, true, NULL, aurEff);
                    Remove(AURA_REMOVE_BY_DEFAULT);
                }
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ds_timeloop_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_ds_timeloop_AuraScript::Absorb, EFFECT_1);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_timeloop_AuraScript();
    }
};

class spell_ds_gen_play_movie : public SpellScriptLoader
{
public:
    spell_ds_gen_play_movie() : SpellScriptLoader("spell_ds_gen_play_movie") { }

    class spell_ds_gen_play_movie_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_gen_play_movie_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            GetHitUnit()->ToPlayer()->SendMovieStart(GetSpellInfo()->Effects[effIndex].BasePoints);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_gen_play_movie_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_gen_play_movie_SpellScript();
    }
};

class spell_ds_essence_of_dreams : public SpellScriptLoader
{
public:
    spell_ds_essence_of_dreams() : SpellScriptLoader("spell_ds_essence_of_dreams") { }

    class spell_ds_essence_of_dreams_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_essence_of_dreams_AuraScript);

        void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32& amplitude)
        {
            isPeriodic = true;
            amplitude = 1000;
        }

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
                {
                    if (aurEff->GetAmount() > 0)
                    {
                        int32 healAmount = aurEff->GetAmount() * 2;
                        aurEff->SetAmount(0);
                        caster->CastCustomSpell(caster, SPELL_ESSENCE_OF_DREAMS_HEAL, &healAmount, NULL, NULL, TRIGGERED_FULL_MASK, NULL, aurEff, caster->GetGUID());
                    }
                }
            }
        }

        void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetHealInfo() || !eventInfo.GetHealInfo()->GetSpellInfo())
                return;

            if (eventInfo.GetHealInfo()->GetSpellInfo()->IsAffectingArea() || (eventInfo.GetHitMask() & PROC_EX_INTERNAL_HOT)) // only direct heal count
                return;

            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
                aurEff->SetAmount(aurEff->GetAmount() + eventInfo.GetHealInfo()->GetHeal());
        }

        void Register()
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_ds_essence_of_dreams_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_TRANSFORM);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_essence_of_dreams_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_TRANSFORM);
            OnEffectProc += AuraEffectProcFn(spell_ds_essence_of_dreams_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_TRANSFORM);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_essence_of_dreams_AuraScript();
    }
};


class spell_ds_essence_of_dreams_heal : public SpellScriptLoader
{
public:
    spell_ds_essence_of_dreams_heal() : SpellScriptLoader("spell_ds_essence_of_dreams_heal") { }

    class spell_ds_essence_of_dreams_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_essence_of_dreams_heal_SpellScript);

        bool Load()
        {
            _targetCount = 0;
            return true;
        }

        void CountTargets(std::list<WorldObject*>& targets)
        {
            _targetCount = targets.size();
        }

        void HandleHeal(SpellEffIndex effIndex)
        {
            SetHitHeal(GetHitHeal() / _targetCount);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_essence_of_dreams_heal_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_essence_of_dreams_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }

        uint32 _targetCount;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_essence_of_dreams_heal_SpellScript();
    }
};

class go_ultraxion_heal_stones : public GameObjectScript
{
public:
    go_ultraxion_heal_stones() : GameObjectScript("go_ultraxion_heal_stones")
    {}

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->HasHealSpec())
        {
            if (notUsed)
                notUsed = false;

            return notUsed;
        }
        player->GetSession()->SendNotification("You are unable to use this currently.");
        return true;
    }

private:
    bool notUsed = true;
};

void AddSC_boss_ultraxion()
{
    new boss_ds_ultraxion();
    new npc_ds_kalecgos();
    new npc_ds_ultraxion_gauntlet();
    new npc_ds_thrall();
    new npc_ds_twilight_assaulter();
    new npc_ds_nethestrasz_taxi();
    new npc_ds_thrall_outro();
    new spell_ds_gen_dialogue();
    new spell_ds_call_dragon();
    new spell_ds_twilight_flames();
    new spell_ds_ping();
    new spell_ds_twilight_instability_trigger();
    new spell_ds_twilight_instability();
    new spell_ds_fading_light();
    new spell_ds_heroic_will();
    new spell_ds_hour_of_twilight_damage();
    new spell_ds_last_defender_of_azeroth();
    new spell_ds_last_defender_of_azeroth_buff();
    new spell_ds_lightning_cosmetic();
    new spell_ds_timeloop();
    new spell_ds_gen_play_movie();
    new spell_ds_essence_of_dreams();
    new spell_ds_essence_of_dreams_heal();
    new go_ultraxion_heal_stones();
};
