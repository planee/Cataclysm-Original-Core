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
#include "LFGMgr.h"
#include "Group.h"

enum Spells
{
    // Deathwing - 57962
    SPELL_CORRUPTED_BLOOD           = 106843,
    SPELL_DUMMY_NUKE                = 21912,
    SPELL_SUMMON_IMPALING_TENTACLE  = 106775,
    SPELL_TIME_ZONE_SUMMON          = 106761,
    SPELL_ELEMENTIUM_TERROR         = 106766,
    SPELL_SUMMON_ELEMENTIUM_TERROR  = 106765,
    SPELL_ACHIEVEMENT               = 111533,
    SPELL_ELEMENTIUM_METEOR         = 105599, // unused - hits elementium bolt
    SPELL_CORRUPTING_PARASITE       = 108597, // target selector for 108649
    SPELL_CONGAELING_BLOOD_SUMMON   = 109082,


    // Deathwing - 56173 - Head
    SPELL_ROOT                      = 105451,
    SPELL_SHARE_HEALTH              = 109547,
    SPELL_TRIGGER_ASPECT_BUFFS      = 106943,
    SPELL_DUMMY_NUKE_1              = 109738,
    SPELL_ASSAULT_ASPECTS           = 107018,
    SPELL_SUMMON_MUTATED_CORRUPTION = 106239, // cast on 56519 on active plattform
    SPELL_FACE_RIGHT                = 106512,
    SPELL_FACE_TRIGGER              = 106513,
    SPELL_FACE_MID_RIGHT            = 106511,
    SPELL_FACE_MID_LEFT             = 106510,
    SPELL_FACE_LEFT                 = 106509,
    SPELL_SLUMP                     = 106708,
    SPELL_TRIGGER_ASPECT_YELL       = 109727,
    SPELL_SLUMP_1                   = 110062,
    SPELL_DEATH                     = 110101,
    SPELL_CATACLYSM                 = 106523,
    SPELL_ELEMENTIUM_BOLT           = 105651,
    SPELL_ELEMENTIUM_BOLT_MARKER    = 106991,
    SPELL_ELEMENTIUM_BOLT_VIS       = 106242, // hit 56307
    SPELL_ELEMENTIUM_BLAST          = 105723,
    SPELL_ELEMENTIUM_BLAST_PERIODIC = 110628,
    SPELL_ELEMENTIUM_BLAST_TRANSFORM = 110663,

    // Ysera - 56100
    // SPELL_YSERAS_PRESENCE               = 106457,
    SPELL_CONCENTRATION_2           = 106643,
    SPELL_CONCENTRATION_8           = 110077,
    SPELL_YSERAS_PRESENCE_AURA      = 106456, // Aura on Ysera triggers 106457 on players
    SPELL_THE_DREAMER_AURA          = 106463, // Aura on Ysera triggers 106464 which provides spell 106466 to player
    SPELL_ENTER_THE_DREAM_TRIGGER   = 106464,
    // SPELL_DREAM                         = 106466,
    SPELL_YSARA_EXPOSE_WEAKNESS     = 106613,

    // Alexstrasza - 56099
    // SPELL_ALEXSTRASZAS_PRESENCE         = 106028,
    SPELL_ALEXSTRASZAS_PRESENCE_AURA  = 105825, // Aura on Alexstrasza triggers 106028
    SPELL_CAUTERIZE_BLISTERING        = 105565,
    SPELL_CAUTERIZE_BLISTERING_DAMAGE = 105569,
    SPELL_CAUTERIZE_DEATHWING_5_SEC   = 106860,
    SPELL_CAUTERIZE_DEATHWING_10_SEC  = 106888,
    SPELL_CAUTERIZE_DEATHWING_15_SEC  = 106889,
    SPELL_CAUTERIZE_PARASITE          = 108840,
    SPELL_CONCENTRATION_3             = 106641,
    SPELL_CONCENTRATION_7             = 110071,
    SPELL_ALEXSTRASZA_EXPOSE_WEAKNESS = 106588,

    // Nozdormu - 56102
    // SPELL_NOZDORMUS_PRESENCE            = 106027,
    SPELL_NOZDORMUS_PRESENCE_AURA   = 105823, // Aura on Nozdormu triggers 106027
    SPELL_TIME_ZONE_2               = 106919, // target 56307
    SPELL_TIME_ZONE_3               = 105799, // summon
    SPELL_CONCENTRATION_4           = 106642,
    SPELL_TIME_ZONE_MISSILE_PHASE_1 = 105799,
    SPELL_TIME_ZONE_MISSILE_PHASE_2 = 107055,
    SPELL_CONCENTRATION_5           = 110076,
    SPELL_NOZDORMU_EXPOSE_WEAKNESS  = 106600,

    // Time Zone - 56311
    SPELL_TIME_ZONE                 = 105831,
    SPELL_TIMEZONE_SLOW             = 105830,

    // Kalecgos - 56101
    // SPELL_KALECGOS_PRESENCE      = 106029,
    SPELL_CONCENTRATION             = 106644,
    SPELL_CONCENTRATION_6           = 110072,
    SPELL_KALECGOS_PRESENCE_AURA    = 106026, // Aura on Kalecgos triggers 106029
    SPELL_SPELLWEAVER_AURA          = 106039,
    SPELL_SPELLWEAVER_TRIGGER       = 106040,
    SPELL_KALECGOS_EXPOSE_WEAKNESS  = 106624,

    // Thrall - 56103
    SPELL_ASTRAL_RECALL             = 108537,
    SPELL_ASTRAL_RECALL_2           = 110063, // 02/03/2015 22:24:10.813
    SPELL_FIRE_DRAGON_SOUL_ASPECTS  = 110067, // 02/03/2015 22:24:10.813
    SPELL_TRIGGER_ASPECT_YELL_2     = 109708, // 02/03/2015 22:24:13.375
    SPELL_FIRE_DRAGON_SOUL          = 109971, // 02/03/2015 22:24:13.375
    SPELL_FIRE_DRAGEN_SOUL_TR       = 110065,
    SPELL_PLAY_MOVIE                = 107000, // 02/03/2015 22:24:42.500

    // Travel to the deck of The Skyfire - 57378
    SPELL_TELEPORT_VISUAL_ACTIVE    = 108203,

    // Tail Tentacle - 56844
    SPELL_REDUCE_PARRY_AND_BLOCK    = 110470,
    SPELL_IDLE                      = 106187,

    // Arm Tentacle - 56167
    SPELL_LIMB_EMERGE_VISUAL        = 107991,

    // Blistering Tentacle - 56188
    SPELL_BLISTERING_HEAT           = 105444,

    // Mutated Corruption - 56471
    SPELL_IMPALE_ASPECT_1           = 107026,
    SPELL_CRUSH                     = 106385,
    SPELL_IMPALE_ASPECT             = 107029,
    SPELL_CRUSH_TARGET              = 106382,
    SPELL_IMPALE                    = 106400,

    // Wing Tentacle - 56168
    SPELL_BURNING_BLOOD             = 105401,
    SPELL_SPAWN_BLISTERING_TENTACLES = 105551,
    SPELL_AGONIZING_PAIN            = 106548,
    SPELL_TRIGGER_CONCENTRATION     = 106940,
    SPELL_HEMORRHAGE                = 105863,

    // Cosmetic Tentacle - 57693
    SPELL_RIDE_VEHICLE_HARDCODED    = 46598,

    // Elementium Fragment - 56724
    SPELL_SHRAPNEL_MARK             = 106794,
    SPELL_BASE_VISUAL               = 106814,
    SPELL_SHRAPNEL                  = 106791,

    // Deathwing Arm R - 57686
    SPELL_SUMMON_COSMETIC_TENTACLE  = 108970,

    // Elementium Terror - 56710
    SPELL_TETANUS                   = 106728,

    SPELL_CARRYING_WINDS_1          = 106666,
    SPELL_CARRYING_WINDS_2          = 106669,
    SPELL_CARRYING_WINDS_3          = 106671,
    SPELL_CARRYING_WINDS_4          = 106673,
    SPELL_CARRYING_WINDS_5          = 106675,
    SPELL_CARRYING_WINDS_6          = 106677,

    // Regenerative Blood
    SPELL_DEGENERATIVE_BITE         = 105842,
    SPELL_REGENERATIVE_BLOOD_HEAL   = 105937,
    SPELL_ENERGIZE_BLOOD            = 109083,

    SPELL_CLAW_MARK                 = 106273,

    // Corrupting Parasite
    SPELL_PARASITE_BACKLASH         = 108787,
    SPELL_UNSTABLE_CORRUPTION       = 108813,
    SPELL_VEHICLE_SHRINK            = 108628,
    SPELL_CORRUPTING_PARASITE_PERIODIC  = 108601,

    // Congealing Blood
    SPELL_CONGEALING_BLOOD_SUMMON   = 109082,
    SPELL_CONGEALING_BLOOD_HEAL     = 109102
};

enum Events
{
    EVENT_IMPALE                = 1,
    EVENT_CRUSH                 = 2,
    EVENT_ASSAULT_ASPECTS       = 3,
    EVENT_CATACLYSM             = 4,
    EVENT_ELEMENTIUM_BOLT       = 5,
    EVENT_SHRAPNELL             = 6,
    EVENT_CHECK_PLAYER          = 7,
    EVENT_SUMMON_CORRUPTION     = 8,
    EVENT_HEMORRHAGE            = 9,
    EVENT_START_PHASE_2         = 10,
    EVENT_ELEMENTIUM_TERROR     = 11,
    EVENT_CHECK_DIST            = 12,
    EVENT_REGENERATE_BLOOD      = 13,
    EVENT_MOVE_EXPLOSION        = 14,
    EVENT_SUMMON_FRAGMENTS      = 15,
    EVENT_SUMMON_TIME_ZONE      = 16,
    EVENT_CORRUPTING_PARASITE   = 17,
    EVENT_PLATFORM_CHECK        = 18,
    EVENT_START_MOVEMENT        = 19,
    EVENT_BERSERK               = 20,
    EVENT_PLAYER_DISTANCE_CHECK = 21,
};

enum Actions
{
    ACTION_ACTIVATE_PLATFORM                = 1,
    ACTION_GET_PLAYERS                      = 2,
    ACTION_SUMMON_CORRUPTION                = 3,
    ACTION_OUTRO                            = 4,
    ACTION_ELEMENTIUM_BOLT                  = 5,
    ACTION_START_PHASE_2                    = 6,
    ACTION_FINISH_FIGHT                     = 7,
    ACTION_SUMMON_BLOOD                     = 8
};

enum Data
{
    DATA_PLAYERS = 1,
    DATA_IS_PLATFORM_FINISHED = 2
};

enum Phase
{
    PHASE_NORMAL    = 1,
    PHASE_HEAD      = 2,
};

enum AnimKits
{
    ANIM_UNK                = 1703, // tail tentacle, mutated corruption
    ANIM_UNK_2              = 1711, // tail tentacle, mutated corruption
    ANIM_BLISTERING_SUMMON  = 1732, // Blistering Tentacle
    ANIM_BLISTERING_SUMMON_2 = 1733, // Blistering Tentacle
    ANIM_UNK_5              = 1731, // Blistering Tentacle
    ANIM_UNK_6              = 1704, // Mutated Corruption
    ANIM_DEATHWING          = 1792, // deathwing summon
    ANIM_ELEMENTIUM_BOLT    = 1730
};

enum SpellVisualKits
{
    KIT_ARM_TENTACLE_L  = 22450,
    KIT_ARM_TENTACLE_R  = 22447,
    KIT_WING_TENTACLE_L = 56168,
    KIT_WING_TENTACLE_R = 22447
};

enum Texts
{
    // intro
    INTRO_THRALL                = -55768,
    INTRO_ALEXSTRASZA_1         = -55769,
    INTRO_ALEXSTRASZA_2         = -56771,
    INTRO_NOZDORMU              = -55770,

    // Deathwing
    DW_SAY_AGGRO                = -55860,
    DW_SAY_ELEMENTUIM_BOLT      = 55779, // 55780 55781
    DW_CHAT_ASSAULT_ASPECT      = -55853,
    DW_CHAT_ATTACK_ALEXSTRASZA  = -55848,
    DW_CHAT_ATTACK_NOZDORMU     = -55849,
    DW_CHAT_ATTACK_YSERA        = -55850,
    DW_CHAT_ATTACK_KALECGOS     = -55851,
    DW_CHAT_PHASE_2             = -56720,
    DW_CHAT_CATACLYSM           = -55535,
    DW_CHAT_ELEMENTIUM_BOLT     = -55826, //

    // Kalecgos
    KALECGOS_SAY_ASSAULT        = -56752,
    KALECGOS_SAY_CATACLYSM      = -55776,

    // Nozdormu
    NOZDORMU_SAY_ASSAULT        = -55786,
    NOZDORMU_SAY_CATACLYSM      = -55774,

    // Ysera
    YSERA_SAY_ASSAULT           = -55843,
    YSERA_SAY_CATACLYSM         = -56781,

    // Alexstrasza
    ALEXSTRASZA_SAY_ASSAULT     = -56772,
    ALEXSTRASZA_SAY_CATACLYSM   = -55773,

    // Tentacle
    TENTACLE_EMOTE_BLISTERING   = -54846,

    // Mutated Corruption
    MUTATED_EMOTE_IMPALE_ASPECT = -56722,

    // Phase 2
    SAY_PHASE_TWO_1             = -55785, // DW
    EMOTE_PHASE_TWO             = -56720, // DW
    SAY_PHASE_TWO_2             = -56774, // Alexstrasza
    SAY_PHASE_TWO_3             = -56753, // Kalecgos
    SAY_PHASE_TWO_4             = -56770, // Nozdormu
    SAY_PHASE_TWO_5             = -56777, // Ysera
    SAY_PHASE_TWO_6             = -56737, // Thrall
    SAY_PHASE_TWO_7             = -55845, // Thrall
    SAY_PHASE_TWO_8             = -55846, // Thrall
    SAY_PHASE_TWO_9             = -55844, // Thrall

    // Outro
    OUTRO_ALEXSTRASZA           = -56773,
    OUTRO_KALECGOS              = -56751,
    OUTRO_NOZDORMU              = -56769, //  Time: 02/03/2015 22:24:13.375
    OUTRO_YSERA                 = -56775
};

enum Points
{
    POINT_CHECK_TIME_ZONE   = 1,
    POINT_EXPLOSION         = 2,
    POINT_DEATHWING         = 3
};

Position const DeathwingSpawnPos = { -11903.930f, 11989.140f, -113.204f, 2.1642f };

static const DragonSoulEventSpawns DeathwingTentacles[9] =
{
    { NPC_DEATHWING_ARM_LEFT, -11967.130f, 11958.790f, -49.982f, 2.1642f },
    { NPC_DEATHWING_ARM_RIGHT, -11852.100f, 12036.370f, -49.982f, 2.1642f },
    { NPC_DEATHWING_WING_LEFT, -11913.760f, 11926.520f, -60.374f, 2.1642f },
    { NPC_DEATHWING_WING_RIGHT, -11842.180f, 11974.800f, -60.374f, 2.1642f },
    { NPC_DEATHWING_HEAD, -12063.490f, 12198.860f, -13.054f, 2.1642f },
    { NPC_ARM_TENTACLE, -12065.030f, 12127.180f, -3.294f, 2.3387f },
    { NPC_ARM_TENTACLE_2, -12005.800f, 12190.350f, -6.593f, 2.1293f },
    { NPC_WING_TENTACLE, -11941.150f, 12248.890f, 12.149f, 1.9896f },
    { NPC_WING_TENTACLE, -12097.790f, 12067.370f, 13.488f, 2.2165f }
};

struct AspectInfo
{
    uint32 facingSpell;
    uint32 presence;
    uint32 specialSpell;
    uint32 concentrationSpells[2];
    uint32 exposeWeakness;
    uint8 index;
    Position pos; // concentrationPos
    int32 texts[3]; // 0 = Assault 1 = Cataclysm 2 = aspect attack emote
};

Position const AlexstraszaConcPos = { -11965.687f, 12246.030f, 29.800f, 4.848f };
Position const NozdormuConcPos = { -12034.951f, 12224.680f, 26.702f, 5.158f };
Position const YseraConcPos = { -12094.142f, 12180.540f, 29.724f, 5.545f };
Position const KalecgosConcPos = { -12130.000f, 12076.657f, 34.33f, 5.953f };
Position const bloodEndPos = { -12083.970f, 12173.650f, -4.087f, 0.000f };
Position const triggerPos[3]
{
    { -12079.900f, 12150.800f, -2.652f, 0.0f },
    { -12098.600f, 12160.600f, -2.652f, 0.0f },
    { -12116.900f, 12170.900f, -2.652f, 0.0f }
};


static const std::map<uint32, AspectInfo> aspectInfo
{
    { NPC_ALEXSTRASZA, { SPELL_FACE_RIGHT, SPELL_ALEXSTRASZAS_PRESENCE_AURA, SPELL_CAUTERIZE_BLISTERING, { SPELL_CONCENTRATION_3, SPELL_CONCENTRATION_7 }, SPELL_ALEXSTRASZA_EXPOSE_WEAKNESS, 7, AlexstraszaConcPos, { ALEXSTRASZA_SAY_ASSAULT, ALEXSTRASZA_SAY_CATACLYSM, DW_CHAT_ATTACK_ALEXSTRASZA } } },
    { NPC_NOZDORMU, { SPELL_FACE_MID_RIGHT, SPELL_NOZDORMUS_PRESENCE_AURA, SPELL_TIME_ZONE_3, { SPELL_CONCENTRATION_4, SPELL_CONCENTRATION_5 }, SPELL_NOZDORMU_EXPOSE_WEAKNESS, 6, NozdormuConcPos, { NOZDORMU_SAY_ASSAULT, NOZDORMU_SAY_CATACLYSM, DW_CHAT_ATTACK_NOZDORMU } } },
    { NPC_YSERA, { SPELL_FACE_MID_LEFT, SPELL_YSERAS_PRESENCE_AURA, SPELL_THE_DREAMER_AURA, { SPELL_CONCENTRATION_2, SPELL_CONCENTRATION_8 }, SPELL_YSARA_EXPOSE_WEAKNESS, 5, YseraConcPos, { YSERA_SAY_ASSAULT, YSERA_SAY_CATACLYSM, DW_CHAT_ATTACK_YSERA } } },
    { NPC_KALECGOS, { SPELL_FACE_LEFT, SPELL_KALECGOS_PRESENCE_AURA, SPELL_SPELLWEAVER_AURA, { SPELL_CONCENTRATION, SPELL_CONCENTRATION_6 }, SPELL_KALECGOS_EXPOSE_WEAKNESS, 8, KalecgosConcPos, { KALECGOS_SAY_ASSAULT, KALECGOS_SAY_CATACLYSM, DW_CHAT_ATTACK_KALECGOS } } }
};

class boss_deathwing_madness : public CreatureScript
{
public:
    boss_deathwing_madness() : CreatureScript("boss_deathwing_madness") {}

    struct boss_deathwing_madnessAI : public BossAI
    {
        boss_deathwing_madnessAI(Creature* creature) : BossAI(creature, DATA_MADNESS_OF_DEATHWING)
        {
            firstTentacleAnim = true;
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->SetReactState(REACT_PASSIVE);
            me->SetUInt32Value(UNIT_FIELD_BYTES_0, 33554688);
        }

        void ApplyNerf(Creature* creatrure)
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
            {
                if (!creatrure->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
                    creatrure->CastSpell(creatrure, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, true);
            }
            else if (instance->GetData(DATA_RAID_NERF) == RAID_NERF_ACTIVE && !creatrure->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
            {
                int32 currentNormalAndHcNerfAmount = -5;
                creatrure->CastCustomSpell(creatrure, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, &currentNormalAndHcNerfAmount, &currentNormalAndHcNerfAmount, NULL, true);
                DoCast(creatrure, SPELL_POWER_OF_THE_ASPECTS, true);
            }
        }

        void EnterEvadeMode() override
        {
            RemoveEncounterFrame();
            for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                    summon->AI()->RemoveEncounterFrame();

            if (Creature* thrall = instance->GetCreature(NPC_DW_MADNESS_THRALL))
                thrall->AI()->Reset();
            summons.DespawnAll();
            me->DespawnCreaturesInArea(NPC_MUTETED_CORRUPTION, 800.0f);
            me->DespawnCreaturesInArea(NPC_REGENERATIVE_BLOOD, 800.0f);
            me->DespawnCreaturesInArea(NPC_TIME_ZONE, 800.0f);
            me->DespawnCreaturesInArea(NPC_ELEMENTIUM_FRAGMENT, 800.0f);
            for (auto itr : aspectInfo)
                if (Creature* aspect = instance->GetCreature(itr.first))
                    aspect->GetMotionMaster()->MoveTargetedHome();
            instance->SetBossState(DATA_MADNESS_OF_DEATHWING, FAIL);
            Reset();
            me->DespawnOrUnsummon();
        }

        uint64 GetTentacle(uint8 index) const
        {
            return tentacles[index];
        }

        void IsSummonedBy(Unit* summoner)
        {
            instance->SetData(DATA_MADNESS_PLATFORM_COUNT, 0);
            std::list<Creature*> creatureList;
            GetCreatureListWithEntryInGrid(creatureList, me, NPC_SUMMON_ENABLER, 800.0f);
            for (Creature* trigger : creatureList)
                trigger->Respawn(true);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC);
            me->PlayOneShotAnimKit(ANIM_DEATHWING);
            for (int32 i = 0; i < 9; i++)
            {
                Unit* summon = me->SummonCreature(DeathwingTentacles[i].npcId, DeathwingTentacles[i].x, DeathwingTentacles[i].y, DeathwingTentacles[i].z, DeathwingTentacles[i].orientation);
                tentacles[i] = summon->GetGUID();
            }
            events.SetPhase(PHASE_NORMAL);
            events.ScheduleEvent(EVENT_ASSAULT_ASPECTS, 4 * IN_MILLISECONDS);
            AddEncounterFrame();
            events.ScheduleEvent(EVENT_CHECK_PLAYER, 2 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_BERSERK, 15 * IN_MILLISECONDS * MINUTE);
            Talk(DW_SAY_AGGRO);
            EnterCombat(nullptr);
        }

        void JustSummoned(Creature* summon) override
        {
            int32 seat = 0;
            switch (summon->GetEntry())
            {
                case NPC_DEATHWING_ARM_LEFT:
                    seat = 1;
                    summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                    break;
                case NPC_DEATHWING_ARM_RIGHT:
                    summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                    break;
                case NPC_DEATHWING_WING_LEFT:
                    seat = 4;
                    summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                    break;
                case NPC_DEATHWING_WING_RIGHT:
                    seat = 2;
                    summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                    break;
                case NPC_DEATHWING_HEAD:
                    // seat = 5;
                    //summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                    break;
                case NPC_ARM_TENTACLE:
                    ApplyNerf(summon);
                    summon->SendPlaySpellVisualKit(KIT_ARM_TENTACLE_L, 0);
                    break;
                case NPC_ARM_TENTACLE_2:
                    ApplyNerf(summon);
                    summon->SendPlaySpellVisualKit(KIT_ARM_TENTACLE_R, 0);
                    break;
                case NPC_WING_TENTACLE:
                    ApplyNerf(summon);
                    summon->SendPlaySpellVisualKit(firstTentacleAnim ? KIT_WING_TENTACLE_L : KIT_WING_TENTACLE_R, 0);
                    firstTentacleAnim = false;
                    break;
                case NPC_ELEMENTIUM_TERROR:
                    summon->CastSpell(summon, SPELL_TETANUS, true);
                    break;
                default:
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer) override
        {
            summons.remove(summon->GetGUID());

            if (summon->GetEntry() == NPC_WING_TENTACLE || summon->GetEntry() == NPC_ARM_TENTACLE_2 || summon->GetEntry() == NPC_ARM_TENTACLE)
                events.CancelEvent(EVENT_CATACLYSM);

            if (!summons.HasEntry(NPC_WING_TENTACLE) && !summons.HasEntry(NPC_ARM_TENTACLE_2) && !summons.HasEntry(NPC_ARM_TENTACLE) && !events.IsInPhase(PHASE_HEAD))
            {
                Talk(DW_CHAT_PHASE_2, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                events.SetPhase(PHASE_HEAD);
                events.CancelEventGroup(PHASE_NORMAL);
                events.ScheduleEvent(EVENT_START_PHASE_2, 6500);
            }
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            RemoveEncounterFrame();
        }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo) override
        {
            if (spellInfo && spellInfo->Id == SPELL_AGONIZING_PAIN && events.IsInPhase(PHASE_NORMAL))
            {
                events.ScheduleEvent(EVENT_ASSAULT_ASPECTS, 6500, 0, PHASE_NORMAL);
                events.CancelEvent(EVENT_ELEMENTIUM_BOLT);
            }
        }

        void DoAction(int32 const actionId) override
        {
            switch (actionId)
            {
                case ACTION_ACTIVATE_PLATFORM:
                {
                    bool firstAspect = instance->GetData(DATA_MADNESS_PLATFORM_COUNT) == 1;
                    events.ScheduleEvent(EVENT_CATACLYSM, firstAspect ? 105500 : 120500, PHASE_NORMAL, PHASE_NORMAL);
                    events.ScheduleEvent(EVENT_ELEMENTIUM_BOLT, firstAspect ? 30500 : 45500, PHASE_NORMAL, PHASE_NORMAL);
                    events.ScheduleEvent(EVENT_SUMMON_CORRUPTION, firstAspect ? 1 * IN_MILLISECONDS : 5 * IN_MILLISECONDS, PHASE_NORMAL, PHASE_NORMAL);
                    if (IsHeroic())
                    {
                        firstParasite = true;
                        events.ScheduleEvent(EVENT_CORRUPTING_PARASITE, firstAspect ? 1000 : 11000, PHASE_NORMAL, PHASE_NORMAL);
                    }
                    break;
                }
                case ACTION_FINISH_FIGHT:
                    if (Creature* thrall = instance->GetCreature(NPC_DW_MADNESS_THRALL))
                        thrall->AI()->DoAction(ACTION_OUTRO);
                    JustDied(nullptr);
                    me->DespawnCreaturesInArea(NPC_MUTETED_CORRUPTION, 800.0f);
                    me->DespawnCreaturesInArea(NPC_REGENERATIVE_BLOOD, 800.0f);
                    me->DespawnCreaturesInArea(NPC_TIME_ZONE, 800.0f);
                    DoCast(SPELL_DEATH);
                    me->CastWithDelay(30000, (Unit*)NULL, SPELL_PLAY_MOVIE);
                    me->DespawnOrUnsummon(19000);
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT, 0, me);
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);

            _EnterCombat();
        }

        void JustDied(Unit* killer) override
        {
            me->LowerPlayerDamageReq(me->GetMaxHealth());
            instance->CompleteGuildCriteriaForGuildGroup(18487);

            RemoveEncounterFrame();
            if (GameObject* chest = instance->GetGameObject((instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR) ? DATA_MADNESS_CHEST_LFR : DATA_MADNESS_CHEST))
            {
                chest->SetRespawnTime(chest->GetRespawnDelay());
                chest->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                chest->SetLootRecipient(me->GetLootRecipient());
            }

            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_NORMAL)
            {
                Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
                {
                    if (Player* player = i->getSource())
                    {
                        player->ModifyCurrency(CURRENCY_TYPE_VALOR_POINTS, Is25ManRaid() ? 13500 : 11500);
                        player->ModifyCurrency(CURRENCY_TYPE_ESSENCE_OF_CORRUPTED_DEATHWING, 1);
                    }
                }
            }

            _JustDied();

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            if (!players.isEmpty())
            {
                if (Group* group = players.begin()->getSource()->GetGroup())
                    if (group->isLFRGroup())
                        instance->FinishLfgDungeon(me);
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            /*if (me->HasUnitState(UNIT_STATE_CASTING))
                return;*/

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_ASSAULT_ASPECTS:
                        Talk(DW_CHAT_ASSAULT_ASPECT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        DoCast(SPELL_ASSAULT_ASPECTS);
                        break;
                    case EVENT_CATACLYSM:
                    {
                        Talk(DW_CHAT_CATACLYSM, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        DoCast(SPELL_CATACLYSM);
                        if (Creature* currAspect = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ACTIVE_PLATFORM)))
                        {
                            uint32 spell = 0;
                            int32 text = 0;
                            auto itr = aspectInfo.find(currAspect->GetEntry());
                            if (itr != aspectInfo.end())
                            {
                                spell = itr->second.exposeWeakness;
                                text = itr->second.texts[1];
                            }
                            currAspect->AI()->DoCast(spell);
                            currAspect->AI()->Talk(text);
                        }
                        break;
                    }
                    case EVENT_ELEMENTIUM_BOLT:
                        Talk(DW_CHAT_ELEMENTIUM_BOLT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        TalkBroadcastGroup(DW_SAY_ELEMENTUIM_BOLT);
                        if (Creature* aspect = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ACTIVE_PLATFORM)))
                            aspect->AI()->DoAction(ACTION_ELEMENTIUM_BOLT);
                        DoCast(SPELL_ELEMENTIUM_BOLT);
                        break;
                    case EVENT_CHECK_PLAYER:
                        if (!me->GetMap()->GetAlivePlayerCountExceptGMs())
                            EnterEvadeMode();
                        events.ScheduleEvent(EVENT_CHECK_PLAYER, 2 * IN_MILLISECONDS);
                        break;
                    case EVENT_SUMMON_CORRUPTION:
                        DoCast(SPELL_SUMMON_MUTATED_CORRUPTION);
                        break;
                    case EVENT_START_PHASE_2:
                    {
                        RemoveEncounterFrame();
                        for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                        {
                            if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                            {
                                if (summon->GetEntry() == NPC_DEATHWING_HEAD)
                                {
                                    summon->SetMaxHealth(me->GetMaxHealth());
                                    summon->SetHealth(me->GetHealth());
                                    summon->AI()->DoAction(ACTION_START_PHASE_2);
                                    break;
                                }
                            }
                        }

                        Talk(EMOTE_PHASE_TWO, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IN_COMBAT | UNIT_FLAG_PET_IN_COMBAT);
                        me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO | UNIT_DYNFLAG_TAPPED_BY_PLAYER);
                        me->RemoveAllAuras();
                        DoCast(SPELL_SLUMP);
                        DoCast(SPELL_TRIGGER_ASPECT_BUFFS);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CUSTOM_SPELL_01);
                        me->DespawnCreaturesInArea(NPC_SUMMON_ENABLER, 800.0f);
                        uint8 cnt = Is25ManRaid() ? 3 : 1;
                        for (int i = 0; i < 3; i++)
                            if (Creature* summon = DoSummon(NPC_SUMMON_ENABLER, triggerPos[i], 30000, TEMPSUMMON_MANUAL_DESPAWN))
                                for (int i = 0; i < cnt; i++)
                                    summon->CastSpell(summon, SPELL_SUMMON_IMPALING_TENTACLE, false); // Elementium Fragment
                        events.ScheduleEvent(EVENT_PLAYER_DISTANCE_CHECK, 30500);
                        events.ScheduleEvent(EVENT_SUMMON_TIME_ZONE, 30500);
                        events.ScheduleEvent(EVENT_ELEMENTIUM_TERROR, 35500);
                        events.ScheduleEvent(EVENT_SUMMON_FRAGMENTS, 90000);
                        break;
                    }
                    case EVENT_ELEMENTIUM_TERROR:
                        for (int i = 0; i < 2; i++)
                            DoCast(SPELL_SUMMON_ELEMENTIUM_TERROR);
                        events.ScheduleEvent(EVENT_ELEMENTIUM_TERROR, 90000);
                        break;
                    case EVENT_SUMMON_FRAGMENTS:
                    {
                        uint8 cnt = Is25ManRaid() ? 3 : 1;
                        for (int i = 0; i < 3; i++)
                            if (Creature* summon = DoSummon(NPC_SUMMON_ENABLER, triggerPos[i], 30000, TEMPSUMMON_MANUAL_DESPAWN))
                                for (int i = 0; i < cnt; i++)
                                    summon->CastSpell(summon, SPELL_SUMMON_IMPALING_TENTACLE, false);
                        events.ScheduleEvent(EVENT_ELEMENTIUM_TERROR, 90000);
                        break;
                    }
                    case EVENT_SUMMON_TIME_ZONE:
                        if (Creature* noz = instance->GetCreature(NPC_NOZDORMU))
                            if (Creature* timeZoneTarget = noz->SummonCreature(NPC_TIME_ZONE_TARGET, -12094.860f, 12158.730f, -2.734f, 0.00f, TEMPSUMMON_TIMED_DESPAWN, 20000))
                                noz->CastSpell(timeZoneTarget, SPELL_TIME_ZONE_MISSILE_PHASE_2, true);
                        events.ScheduleEvent(EVENT_SUMMON_TIME_ZONE, 90000);
                        break;
                    case EVENT_CORRUPTING_PARASITE:
                        DoCast(me, SPELL_CORRUPTING_PARASITE, true);
                        if (firstParasite)
                            events.ScheduleEvent(EVENT_CORRUPTING_PARASITE, 60000, PHASE_NORMAL, PHASE_NORMAL);
                        firstParasite = false;
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_CATACLYSM, true);
                        break;
                    case EVENT_PLAYER_DISTANCE_CHECK:
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            if (Player* player = itr->getSource())
                                if (player->GetAreaId() == 5893)
                                    if (player->GetExactDist2d(-12093.537f, 12159.299f) >= 49.00f)
                                        player->NearTeleportTo(-12093.537f, 12159.299f, -2.7339f, 1.0372f);

                        events.ScheduleEvent(EVENT_PLAYER_DISTANCE_CHECK, 2000);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private:
        bool firstTentacleAnim;
        bool firstParasite;
        uint64 tentacles[9];
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_deathwing_madnessAI(creature);
    }
};

class npc_ds_madness_of_deathwing_head : public CreatureScript
{
public:
    npc_ds_madness_of_deathwing_head() : CreatureScript("npc_ds_madness_of_deathwing_head") { }

    struct npc_ds_madness_of_deathwing_headAI : public ScriptedAI
    {
        npc_ds_madness_of_deathwing_headAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            outroTriggered = false;
            below6PctCheck = false;
            below11PctCheck = false;
            below16PctCheck = false;
        }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_START_PHASE_2)
            {
                DoCast(me, SPELL_CORRUPTED_BLOOD);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                AddEncounterFrame();
            }
            else if (action == ACTION_SUMMON_BLOOD)
            {
                std::list<Creature*> targetList;
                GetCreatureListWithEntryInGrid(targetList, me, NPC_CONGAELING_BLOOD_TARGET, 200.0f);

                if (!targetList.empty() && targetList.size() > 1)
                    Trinity::Containers::RandomResizeList(targetList, 1);

                if (Creature* bloodTarget = targetList.front())
                    bloodTarget->AddAura(SPELL_CONGAELING_BLOOD_SUMMON, bloodTarget);
            }
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo) override
        {
            if (spellInfo->Id == SPELL_CONGEALING_BLOOD_HEAL)
            {
                if (HealthAbovePct(15))
                    below16PctCheck = false;

                if (HealthAbovePct(10))
                    below11PctCheck = false;

                if (HealthAbovePct(5))
                    below6PctCheck = false;
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (me->GetHealthPct() < 1.4f && !outroTriggered) // Fight finish at 1.4 %
            {
                RemoveEncounterFrame();
                outroTriggered = true;
                damage = 0;
                me->RemoveAllAuras();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                
                if (Creature* deathwing = instance->GetCreature(DATA_MADNESS_OF_DEATHWING))
                    deathwing->AI()->DoAction(ACTION_FINISH_FIGHT);
            }

            if (IsHeroic())
            {
                if (me->HealthBelowPctDamaged(16, damage) && !below16PctCheck)
                {
                    below16PctCheck = true;
                    me->AI()->DoAction(ACTION_SUMMON_BLOOD);
                    if (Creature* alex = instance->GetCreature(NPC_ALEXSTRASZA))
                    {
                        alex->CastSpell(alex, SPELL_CAUTERIZE_DEATHWING_5_SEC, true);
                        me->SetPower(POWER_ALTERNATE_POWER, 10);
                    }
                }

                if (me->HealthBelowPctDamaged(11, damage) && !below11PctCheck)
                {
                    below11PctCheck = true;
                    me->AI()->DoAction(ACTION_SUMMON_BLOOD);
                    if (Creature* alex = instance->GetCreature(NPC_ALEXSTRASZA))
                    {
                        alex->CastSpell(alex, SPELL_CAUTERIZE_DEATHWING_10_SEC, true);
                        me->SetPower(POWER_ALTERNATE_POWER, 20);
                    }
                }

                if (me->HealthBelowPctDamaged(6, damage) && !below6PctCheck)
                {
                    below6PctCheck = true;
                    me->AI()->DoAction(ACTION_SUMMON_BLOOD);
                    if (Creature* alex = instance->GetCreature(NPC_ALEXSTRASZA))
                    {
                        alex->CastSpell(alex, SPELL_CAUTERIZE_DEATHWING_15_SEC, true);
                        me->SetPower(POWER_ALTERNATE_POWER, 30);
                    }
                }
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            // this needs to be empty
        }

    private:
        InstanceScript* instance;
        bool outroTriggered;
        bool below16PctCheck;
        bool below11PctCheck;
        bool below6PctCheck;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_madness_of_deathwing_headAI(creature);
    }
};

// helper for shortened code
typedef boss_deathwing_madness::boss_deathwing_madnessAI MadnessAI;

class npc_ds_dragon_aspect : public CreatureScript
{
public:
    npc_ds_dragon_aspect() : CreatureScript("npc_ds_dragon_aspect") { }

    struct npc_ds_dragon_aspectAI : public ScriptedAI
    {
        npc_ds_dragon_aspectAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        Creature* GetLimbTentacle()
        {
            uint8 index = 0;
            uint64 guid = 0;
            auto itr = aspectInfo.find(me->GetEntry());
            if (itr != aspectInfo.end())
                index = itr->second.index;

            if (Unit* target = instance->GetCreature(DATA_MADNESS_OF_DEATHWING))
            {
                if (MadnessAI* madnessAI = CAST_AI(MadnessAI, target->GetAI()))
                    guid = madnessAI->GetTentacle(index);
            }

            if (Creature* limb = ObjectAccessor::GetCreature(*me, guid))
                return limb;

            return nullptr;
        }

        void Reset() override
        {
            me->RemoveAllAuras();
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetSpeed(MOVE_FLIGHT, 3.5f);
            me->m_Events.KillAllEvents(true);
            playerCnt = 0;
            uint32 presence = 0;
            uint32 special = 0;
            auto itr = aspectInfo.find(me->GetEntry());
            if (itr != aspectInfo.end())
            {
                presence = itr->second.presence;
                special = itr->second.specialSpell;
                DoCast(presence);
                if ((me->GetEntry() == NPC_KALECGOS) || (me->GetEntry() == NPC_YSERA))
                    DoCast(special);
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_TRIGGER_ASPECT_BUFFS)
            {
                me->InterruptNonMeleeSpells(true);
                uint32 presence = 0;
                uint32 special = 0;
                auto itr = aspectInfo.find(me->GetEntry());
                if (itr != aspectInfo.end())
                {
                    presence = itr->second.presence;
                    special = itr->second.specialSpell;
                    DoCast(presence);
                    if ((me->GetEntry() == NPC_KALECGOS) || (me->GetEntry() == NPC_YSERA))
                        DoCast(special);
                }
            }
            else if (spell->Id == SPELL_TRIGGER_CONCENTRATION)
            {
                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, NPC_SUMMON_ENABLER, 200.0f);
                Unit* temp = me;
                creatureList.remove_if([temp](WorldObject* target)
                {
                    if (Unit* trigger = temp->FindNearestCreature(NPC_PLATFORM_TRIGGER, 150.0f, true))
                        if (target->IsInBetween(temp, trigger, 30))
                            return false;
                    return true;
                });

                for (Creature* trigger : creatureList)
                    trigger->DespawnOrUnsummon();

                auto itr = aspectInfo.find(me->GetEntry());
                if (itr != aspectInfo.end())
                    me->GetMotionMaster()->MovePoint(0, itr->second.pos, false);
            }
            else if (spell->Id == SPELL_FIRE_DRAGON_SOUL_ASPECTS)
            {
                auto itr = aspectInfo.find(me->GetEntry());
                if (itr != aspectInfo.end())
                    DoCast(itr->second.concentrationSpells[1]);
            }
        }

        uint32 GetData(uint32 type) const override
        {
            if (type == DATA_PLAYERS)
                return uint32(playerCnt);

            if (type == DATA_IS_PLATFORM_FINISHED)
            {
                auto itr = aspectInfo.find(me->GetEntry());
                if (itr != aspectInfo.end())
                    return me->HasAura(itr->second.presence) ? 0 : 1;
            }

            return 0;
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_GET_PLAYERS:
                {
                    playerCnt = 0;

                    if (Unit* trigger = me->FindNearestCreature(NPC_PLATFORM_TRIGGER, 150.0f, true))
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            if (Player* player = itr->getSource())
                                if (player->IsInBetween(me, trigger, 40.0f))
                                    ++playerCnt;
                    }
                    break;
                }
                case ACTION_ACTIVATE_PLATFORM:
                {
                    me->DespawnCreaturesInArea(NPC_TIME_ZONE, 450.00f);
                    instance->SetData64(DATA_ACTIVE_PLATFORM, me->GetGUID());
                    if (Creature* limb = GetLimbTentacle())
                        limb->AI()->DoAction(ACTION_ACTIVATE_PLATFORM);
                    if (Creature* madness = instance->GetCreature(DATA_MADNESS_OF_DEATHWING))
                        madness->AI()->DoAction(ACTION_ACTIVATE_PLATFORM);
                    if (Creature* noz = instance->GetCreature(NPC_NOZDORMU))
                    {
                        if (noz->HasAura(SPELL_NOZDORMUS_PRESENCE_AURA))
                        {
                            Position pos;
                            switch (me->GetEntry())
                            {
                                case NPC_KALECGOS:
                                    pos = { -12121.710f, 12071.200f, 2.392f, 0.00f };
                                    break;
                                case NPC_ALEXSTRASZA:
                                    pos = { -11956.290f, 12268.920f, 1.414f, 0.00f };
                                    break;
                                case NPC_NOZDORMU:
                                    pos = { -12025.940f, 12218.560f, -6.081f, 0.00f };
                                    break;
                                case NPC_YSERA:
                                    pos = { -12094.860f, 12158.730f, -2.734f, 0.00f };
                                    break;
                                default:
                                    break;
                            }

                            if (Creature* timeZoneTarget = noz->SummonCreature(NPC_TIME_ZONE_TARGET, pos, TEMPSUMMON_TIMED_DESPAWN, 20000))
                                noz->CastWithDelay(5000, timeZoneTarget, SPELL_TIME_ZONE_MISSILE_PHASE_1, false);
                        }
                    }
                    int32 text = 0;
                    auto itr = aspectInfo.find(me->GetEntry());
                    if (itr != aspectInfo.end())
                        text = itr->second.texts[0];
                    Talk(text);
                    break;
                }
                case ACTION_ELEMENTIUM_BOLT:
                {
                    Position pos;
                    switch (me->GetEntry())
                    {
                        case NPC_KALECGOS:
                            pos = { -12139.794f, 12078.556f, 2.306f, 0.00f };
                            break;
                        case NPC_ALEXSTRASZA:
                            pos = { -11960.541f, 12284.416f, 1.298f, 0.00f };
                            break;
                        case NPC_NOZDORMU:
                            pos = { -12049.049f, 12233.996f, -6.150f, 0.00f };
                            break;
                        case NPC_YSERA:
                            pos = { -12108.411f, 12166.631f, -2.735f, 0.00f };
                            break;
                        default:
                            break;
                    }

                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_ELEMENTIUM_BOLT_MARKER, true);
                    break;
                }
                default:
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            me->RemoveAllAuras();

            if (me->GetEntry() == NPC_YSERA)
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ENTER_THE_DREAM_TRIGGER);

            auto itr = aspectInfo.find(me->GetEntry());
            if (itr != aspectInfo.end())
                DoCast(itr->second.concentrationSpells[0]);
        }

        bool CanAIAttack(Unit const* victim) const override
        {
            return victim->isCharmedOwnedByPlayerOrPlayer();
        }

        void UpdateAI(uint32 const diff) override
        {
            // this needs to be empty
        }

    private:
        InstanceScript* instance;
        EventMap events;
        uint8 playerCnt;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_dragon_aspectAI(creature);
    }
};

class npc_ds_thrall_madness : public CreatureScript
{
public:
    npc_ds_thrall_madness() : CreatureScript("npc_ds_thrall_madness") { }

    struct npc_ds_thrall_madnessAI : public Scripted_NoMovementAI
    {
        npc_ds_thrall_madnessAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
            intro = false;
        }

        void Reset() override
        {
            me->NearTeleportTo(me->GetHomePosition());
        }

        void MoveInLineOfSight(Unit* mover) override
        {
            if ((mover->GetTypeId() == TYPEID_PLAYER) && !intro)
            {
                intro = true;
                Talk(INTRO_THRALL);
                if (Creature* alexstrasza = instance->GetCreature(NPC_ALEXSTRASZA))
                {
                    alexstrasza->AI()->TalkWithDelay(10000, INTRO_ALEXSTRASZA_1);
                    alexstrasza->AI()->TalkWithDelay(17000, INTRO_ALEXSTRASZA_2);
                }
                if (Creature* noz = instance->GetCreature(NPC_NOZDORMU))
                    noz->AI()->TalkWithDelay(25000, INTRO_NOZDORMU);
            }
        }

        void sGossipSelect(Player* /*player*/, uint32 sender, uint32 action) override
        {
            if (instance->GetBossState(DATA_MADNESS_OF_DEATHWING) != IN_PROGRESS && !instance->IsDone(DATA_MADNESS_OF_DEATHWING))
            {
                if (!IsHeroic()
                    || (IsHeroic() && instance->IsDoneInHeroic(DATA_MORCHOK) && instance->IsDoneInHeroic(DATA_YORSAHJ) && instance->IsDoneInHeroic(DATA_WARLORD_ZONOZZ) && instance->IsDoneInHeroic(DATA_HAGARA) && instance->IsDoneInHeroic(DATA_ULTRAXION) && instance->IsDoneInHeroic(DATA_WARMASTER_BLACKHORN) && instance->IsDoneInHeroic(DATA_SPINE_OF_DEATHWING)))
                {
                    if (me->GetCreatureTemplate()->GossipMenuId == sender && !action)
                    {
                        me->SummonCreature(NPC_DW_MADNESS, DeathwingSpawnPos);
                        DoCast(SPELL_ASTRAL_RECALL);
                    }
                }
            }
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_OUTRO:
                    me->SetCanFly(true);
                    me->SetDisableGravity(true);
                    DoCast(SPELL_ASTRAL_RECALL_2);
                    DoCast(SPELL_TRIGGER_ASPECT_YELL_2);
                    if (Creature* alexstrasza = instance->GetCreature(NPC_ALEXSTRASZA))
                        alexstrasza->AI()->Talk(OUTRO_ALEXSTRASZA);
                    /*if (Creature* kalecgos = instance->GetCreature(NPC_KALECGOS))
                        kalecgos->AI()->TalkWithDelay(10000, OUTRO_KALECGOS);
                    if (Creature* noz = instance->GetCreature(NPC_NOZDORMU))
                        noz->AI()->TalkWithDelay(16000, OUTRO_NOZDORMU);
                    if (Creature* ysera = instance->GetCreature(NPC_YSERA))
                        ysera->AI()->TalkWithDelay(29000, OUTRO_YSERA);*/
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    me->CastWithDelay(1000, (Unit*)NULL, SPELL_FIRE_DRAGON_SOUL_ASPECTS);
                    me->CastWithDelay(1000, (Unit*)NULL, SPELL_FIRE_DRAGON_SOUL);
                    me->DespawnOrUnsummon(31000);
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
                    case 0:
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool intro;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_thrall_madnessAI(creature);
    }
};

class BloodSetInvisibleEvent : public BasicEvent
{
public:
    BloodSetInvisibleEvent(Creature* owner) : blood(owner) { }

    bool Execute(uint64 /*execTime*/, uint32 /*diff*/) override
    {
        blood->SetVisible(false);
        return true;
    }

private:
    Creature* blood;
};

class npc_ds_limb_tentacle : public CreatureScript
{
public:
    npc_ds_limb_tentacle() : CreatureScript("npc_ds_limb_tentacle") { }

    struct npc_ds_limb_tentacleAI : public Scripted_NoMovementAI
    {
        npc_ds_limb_tentacleAI(Creature* creature) : Scripted_NoMovementAI(creature), summons(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset() override
        {
            me->SetInCombatWithZone();
            me->RemoveAllAuras();
            DoCast(SPELL_REDUCE_PARRY_AND_BLOCK);
            DoCast(SPELL_IDLE);
            seat = 0;
            summons.DespawnAll();
            pct = 75;
        }

        void EnterEvadeMode() 
        {
            // this need to be empty
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(pct, damage) && pct > 0)
            {
                pct -= 25;
                seat = 0;
                for (int i = 0; i < 6; i++)
                    DoCast(SPELL_SPAWN_BLISTERING_TENTACLES);
                me->GetVehicleKit()->RelocatePassengers();
                Talk(TENTACLE_EMOTE_BLISTERING, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                if (Creature* alexstaza = instance->GetCreature(NPC_ALEXSTRASZA))
                    if (alexstaza->HasAura(SPELL_ALEXSTRASZAS_PRESENCE_AURA))
                        alexstaza->CastSpell(alexstaza, SPELL_CAUTERIZE_BLISTERING, false);
            }
        }


        void SummonedCreatureDamageTaken(Unit* /*attacker*/, Creature* summon, uint32& damage, SpellInfo const* spellProto) override
        {
            if (summon->GetEntry() == NPC_BLISTERING_TENTACLE) // temp fix until we figure out how we can block all aoe spells
            {
                uint32 cauterize = sSpellMgr->GetSpellIdForDifficulty(SPELL_CAUTERIZE_BLISTERING_DAMAGE, me);
                if (spellProto && spellProto->Id != cauterize && spellProto->IsTargetingArea())
                    damage = 1; // damage = 0 cause client display bugs...
            }
        }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_BLISTERING_TENTACLE)
            {
                summon->SetReactState(REACT_PASSIVE);
                summon->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &seat, NULL, NULL, TRIGGERED_FULL_MASK);
                ++seat;
                summon->CastSpell(summon, SPELL_BLISTERING_HEAT, true);
                summon->PlayOneShotAnimKit(RAND(ANIM_BLISTERING_SUMMON, ANIM_BLISTERING_SUMMON_2));
            }

            if (summon->GetEntry() != NPC_REGENERATIVE_BLOOD) // NPC_REGENERATIVE_BLOOD should not despawn on death
                summons.Summon(summon);

            summon->SetInCombatWithZone();
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer) override
        {
            if (summon->GetEntry() == NPC_REGENERATIVE_BLOOD)
                summon->m_Events.AddEvent(new BloodSetInvisibleEvent(summon), summon->m_Events.CalculateTime(2000));
            else
            {
                summon->DespawnOrUnsummon(1000);
                summons.RemoveNotExisting();
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            DoCast(SPELL_AGONIZING_PAIN);
            DoCast(SPELL_TRIGGER_CONCENTRATION);
            summons.DespawnAll();
            RemoveEncounterFrame();
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_ACTIVATE_PLATFORM:
                {
                    bool firstAspect = instance->GetData(DATA_MADNESS_PLATFORM_COUNT) == 1;
                    me->RemoveAurasDueToSpell(SPELL_IDLE);
                    DoCast(SPELL_BURNING_BLOOD);
                    events.ScheduleEvent(EVENT_HEMORRHAGE, firstAspect ? (IsHeroic() ? 45500 : 75500) : (IsHeroic() ? 60500 : 90500));
                    AddEncounterFrame();
                    break;
                }
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
                    case EVENT_HEMORRHAGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            for (uint8 i = 0; i < 6; i++)
                                DoCast(target, SPELL_HEMORRHAGE);
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
        int32 seat;
        uint8 pct;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_limb_tentacleAI(creature);
    }
};

class npc_ds_mutated_corruption : public CreatureScript
{
public:
    npc_ds_mutated_corruption() : CreatureScript("npc_ds_mutated_corruption") { }

    struct npc_ds_mutated_corruptionAI : public Scripted_NoMovementAI
    {
        npc_ds_mutated_corruptionAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            bool firstAspect = instance->GetData(DATA_MADNESS_PLATFORM_COUNT) == 1;
            events.ScheduleEvent(EVENT_IMPALE, firstAspect ? 12000 : 17500);
            events.ScheduleEvent(EVENT_CRUSH, 25 * IN_MILLISECONDS);
            AddEncounterFrame();
        }

        void JustDied(Unit* /*killer*/) override
        {
            RemoveEncounterFrame();
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_IMPALE:
                    {
                        std::list<WorldObject*> playerList;
                        Trinity::AllWorldObjectsInRange objects(me, 200.0f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, playerList, objects, GRID_MAP_TYPE_MASK_PLAYER);
                        me->VisitNearbyObject(200.0f, searcher);
                        playerList.sort(Trinity::ObjectDistanceOrderPred(me));
                        if (WorldObject* target = playerList.front())
                            DoCast(target->ToPlayer(), SPELL_IMPALE, false);
                        events.ScheduleEvent(EVENT_IMPALE, 35000);
                        break;
                    }
                    case EVENT_CRUSH:
                        DoCastRandom(SPELL_CRUSH_TARGET, 50.0f);
                        if (Unit* trigger = me->FindNearestCreature(NPC_CRUSH_TARGET, 50.0f))
                            DoCast(trigger, SPELL_CRUSH);
                        events.ScheduleEvent(EVENT_CRUSH, 25000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_mutated_corruptionAI(creature);
    }
};

class npc_ds_elementium_fragment     : public CreatureScript
{
public:
    npc_ds_elementium_fragment() : CreatureScript("npc_ds_elementium_fragment") { }

    struct npc_ds_elementium_fragmentAI : public Scripted_NoMovementAI
    {
        npc_ds_elementium_fragmentAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            DoCast(SPELL_BASE_VISUAL);
            events.ScheduleEvent(EVENT_SHRAPNELL, urand(5000, 10000));
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHRAPNELL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 800.0f, true, -SPELL_SHRAPNEL_MARK))
                        {
                            me->AddAura(SPELL_SHRAPNEL_MARK, target);
                            me->CastWithDelay(1000, target, SPELL_SHRAPNEL);
                        }
                        events.ScheduleEvent(EVENT_SHRAPNELL, urand(7000, 10000));
                        break;
                    default:
                        break;
                }
            }
            // DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_elementium_fragmentAI(creature);
    }
};

class npc_ds_elementium_bolt : public CreatureScript
{
public:
    npc_ds_elementium_bolt() : CreatureScript("npc_ds_elementium_bolt") { }

    struct npc_ds_elementium_boltAI : public ScriptedAI
    {
        npc_ds_elementium_boltAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            instance = me->GetInstanceScript();
        }

        void AttackStart(Unit* target) {}
        void EnterEvadeMode() {}

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            me->SetSpeed(MOVE_FLIGHT, me->GetCreatureTemplate()->speed_run);
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetReactState(REACT_PASSIVE);
            if (Creature* aspect = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ACTIVE_PLATFORM)))
            {
                Position pos;
                switch (aspect->GetEntry())
                {
                    case NPC_KALECGOS:
                        pos = { -12101.368f, 12072.304f, 8.371f, 0.00f };
                        break;
                    case NPC_ALEXSTRASZA:
                        pos = { -11955.240f, 12260.890f, 5.359f, 0.00f };
                        break;
                    case NPC_NOZDORMU:
                        pos = { -12026.630f, 12208.030f, 3.532f, 0.00f };
                        break;
                    case NPC_YSERA:
                        pos = { -12090.450f, 12151.620f, 5.359f, 0.00f };
                        break;
                    default:
                        break;
                }
                me->GetMotionMaster()->MovePoint(POINT_CHECK_TIME_ZONE, pos, false);
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case POINT_CHECK_TIME_ZONE:
                    events.ScheduleEvent(EVENT_MOVE_EXPLOSION, 100);
                    break;
                case POINT_EXPLOSION:
                    me->SetAIAnimKitId(ANIM_ELEMENTIUM_BOLT); // 1730 -> 0 at the same time (sniff)
                    me->SetAIAnimKitId(0);
                    DoCast(me, SPELL_ELEMENTIUM_BLAST, true);
                    DoCast(me, SPELL_ELEMENTIUM_BLAST_PERIODIC, true);
                    if (Creature* marker = me->FindNearestCreature(NPC_ELEMENTIUM_BOLT_TARGET, 800.0f))
                    {
                        marker->RemoveAurasDueToSpell(SPELL_ELEMENTIUM_BOLT_VIS);
                        DoCast(marker, SPELL_RIDE_VEHICLE_HARDCODED, true);
                    }
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* marker = me->FindNearestCreature(NPC_ELEMENTIUM_BOLT_TARGET, 800.0f))
            {
                marker->CastSpell(marker, SPELL_ELEMENTIUM_BLAST_TRANSFORM, true);
                marker->CastSpell(marker, SPELL_CLAW_MARK, true);
                marker->DespawnOrUnsummon(30000);
            }
            me->DespawnOrUnsummon(1000);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_EXPLOSION:
                        if (Creature* aspect = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ACTIVE_PLATFORM)))
                        {
                            Position pos;
                            switch (aspect->GetEntry())
                            {
                                case NPC_KALECGOS:
                                    pos = { -12139.360f, 12080.590f, 5.359f, 0.00f };
                                    break;
                                case NPC_ALEXSTRASZA:
                                    pos = { -11958.620f, 12278.810f, 2.414f, 0.00f };
                                    break;
                                case NPC_NOZDORMU:
                                    pos = { -12039.840f, 12231.580f, -3.067f, 0.00f };
                                    break;
                                case NPC_YSERA:
                                    pos = { -12110.740f, 12169.300f, -1.683f, 0.00f };
                                    break;
                                default:
                                    break;
                            }
                            float speed = (me->GetExactDist2d(pos.GetPositionX(), pos.GetPositionY()) / urand(8, 10)) / 7;
                            if (Creature* timeZone = me->FindNearestCreature(NPC_TIME_ZONE, 200.00f))
                                me->SetSpeed(MOVE_FLIGHT, speed);

                            me->GetMotionMaster()->MovePoint(POINT_EXPLOSION, pos, false);
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
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_elementium_boltAI(creature);
    }
};

class npc_ds_corrupted_parasite : public CreatureScript
{
public:
    npc_ds_corrupted_parasite() : CreatureScript("npc_ds_corrupted_parasite") { }

    struct npc_ds_corrupted_parasiteAI : public ScriptedAI
    {
        npc_ds_corrupted_parasiteAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void AttackStart(Unit* attacker) { }
        void EnterEvadeMode() { }

        void IsSummonedBy(Unit* summoner) override
        {
            // We don't use the vehicle shrink and player vehicle because the player vehicle fucked up the parasite vehicle passenger positions...
            // There is no visible difference because the parasite is invisible on the player vehicle
            summoner->CastSpell(summoner, SPELL_PARASITE_BACKLASH, true);
            DoCast(me, SPELL_UNSTABLE_CORRUPTION, false);

            // vehicle_template_accessory dont work...
            Position pos(*me);
            for (int32 i = 0; i < 4; i++)
                if (Creature* tentacle = me->SummonCreature(NPC_CORRUPTING_PARASITE_TENTACLE, pos, TEMPSUMMON_TIMED_DESPAWN, 11500))
                    tentacle->CastCustomSpell(me, SPELL_RIDE_VEHICLE_HARDCODED, &i, NULL, NULL, TRIGGERED_FULL_MASK);

            if (InstanceScript* instance = me->GetInstanceScript())
                if (Creature* alexstaza = instance->GetCreature(NPC_ALEXSTRASZA))
                    if (alexstaza->HasAura(SPELL_ALEXSTRASZAS_PRESENCE_AURA))
                        alexstaza->CastWithDelay(3000, alexstaza, SPELL_CAUTERIZE_PARASITE);
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
        }

        void JustDied(Unit* killer)
        {
            summons.DespawnAll();
        }

    private:
        SummonList summons;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_corrupted_parasiteAI(creature);
    }
};

class npc_ds_regenerative_blood : public CreatureScript
{
public:
    npc_ds_regenerative_blood() : CreatureScript("npc_ds_regenerative_blood") { }

    struct npc_ds_regenerative_bloodAI : public ScriptedAI
    {
        npc_ds_regenerative_bloodAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetByteValue(UNIT_FIELD_BYTES_0, 1, 4);
            me->setPowerType(POWER_ENERGY);
            me->SetPower(POWER_ENERGY, 0);
            me->SetReactState(REACT_AGGRESSIVE);
            DoCast(me, SPELL_DEGENERATIVE_BITE, true);
            events.ScheduleEvent(EVENT_REGENERATE_BLOOD, 1000);
            events.ScheduleEvent(EVENT_PLATFORM_CHECK, 1000);
            if (Unit* target = me->FindNearestPlayer(50.0f))
                AttackStart(target);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_REGENERATE_BLOOD:
                        if (me->GetPower(POWER_ENERGY) >= me->GetMaxPower(POWER_ENERGY))
                        {
                            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                            {
                                int32 healPct = 50;
                                me->CastCustomSpell(me, SPELL_REGENERATIVE_BLOOD_HEAL, &healPct, NULL, NULL, TRIGGERED_FULL_MASK);
                            }
                            else
                                DoCast(me, SPELL_REGENERATIVE_BLOOD_HEAL);
                            me->SetPower(POWER_ENERGY, 0);
                        }
                        else
                            DoCast(me, SPELL_ENERGIZE_BLOOD, true);
                        events.ScheduleEvent(EVENT_REGENERATE_BLOOD, 1000);
                        break;
                    case EVENT_PLATFORM_CHECK:
                    {
                        if (!me->isAlive() || !me->getVictim())
                            break;

                        Creature* myPlatform = me->FindNearestCreature(NPC_PLATFORM_TRIGGER, 100.0f);
                        Creature* victimPlatform = me->getVictim()->FindNearestCreature(NPC_PLATFORM_TRIGGER, 100.0f);

                        if (myPlatform != victimPlatform)
                        {
                            if (me->getVictim()->GetMotionMaster()->GetCurrentMovementGeneratorType() != EFFECT_MOTION_TYPE)
                            {
                                Position pos(*me->getVictim());
                                me->NearTeleportTo(pos);
                            }
                        }
                        events.ScheduleEvent(EVENT_PLATFORM_CHECK, 1000);
                        break;
                    }
                    default:
                        break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_regenerative_bloodAI(creature);
    }
};

class npc_ds_congealing_blood : public CreatureScript
{
public:
    npc_ds_congealing_blood() : CreatureScript("npc_ds_congealing_blood") { }

    struct npc_ds_congealing_bloodAI : public ScriptedAI
    {
        npc_ds_congealing_bloodAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void AttackStart(Unit* attacker) { }
        void EnterEvadeMode() { }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_START_MOVEMENT, 1000);
        }

        void JustDied(Unit* killer) override
        {
            me->DespawnOrUnsummon(500);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_DEATHWING)
                DoCast(me, SPELL_CONGEALING_BLOOD_HEAL, true);
            me->DespawnOrUnsummon(500);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_MOVEMENT:
                        me->GetMotionMaster()->MovePoint(POINT_DEATHWING, bloodEndPos);
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
        return new npc_ds_congealing_bloodAI(creature);
    }
};

// 107018
class spell_ds_assault_aspects : public SpellScriptLoader
{
public:
    spell_ds_assault_aspects() : SpellScriptLoader("spell_ds_assault_aspects") { }

    class spell_ds_assault_aspects_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_assault_aspects_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            for (auto const &itr : targets)
            {
                if (Creature* target = itr->ToCreature())
                {
                    target->AI()->DoAction(ACTION_GET_PLAYERS);
                    uint32 count = target->AI()->GetData(DATA_PLAYERS);
                    if (target->AI()->GetData(DATA_IS_PLATFORM_FINISHED) == 0)
                        valueMap.insert(std::make_pair(target->GetGUID(), count));
                }
            }
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            auto pr = std::max_element(valueMap.begin(), valueMap.end(),
                [](const std::pair<uint64, uint32>& p1, const std::pair<uint64, uint32>& p2) {
                return p1.second < p2.second; });

            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* target = ObjectAccessor::GetCreature((*GetCaster()), pr->first))
                {
                    instance->SetData(DATA_MADNESS_PLATFORM_COUNT, instance->GetData(DATA_MADNESS_PLATFORM_COUNT) + 1);
                    target->AI()->DoAction(ACTION_ACTIVATE_PLATFORM);
                    uint32 spell = 0;
                    int32 emote = 0;
                    auto itr = aspectInfo.find(target->GetEntry());
                    if (itr != aspectInfo.end())
                    {
                        spell = itr->second.facingSpell;
                        emote = itr->second.texts[2];
                    }
                    GetCaster()->ToCreature()->AI()->DoCast(spell);

                    if (Creature* deathwing = instance->GetCreature(DATA_MADNESS_OF_DEATHWING))
                        deathwing->AI()->Talk(emote, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                }
            }
        }

    private:
        std::map<uint64, uint32> valueMap;

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_assault_aspects_SpellScript::FilterTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHit += SpellEffectFn(spell_ds_assault_aspects_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_assault_aspects_SpellScript();
    }
};

// 105444
class spell_ds_blistering_heat : public SpellScriptLoader
{
public:
    spell_ds_blistering_heat() : SpellScriptLoader("spell_ds_blistering_heat") { }

    class spell_ds_blistering_heat_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_blistering_heat_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            GetAura()->SetStackAmount(GetStackAmount() + 1);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_blistering_heat_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_blistering_heat_AuraScript();
    }
};

// 105445
class spell_ds_blistering_heat_damage : public SpellScriptLoader
{
public:
    spell_ds_blistering_heat_damage() : SpellScriptLoader("spell_ds_blistering_heat_damage") { }

    class spell_ds_blistering_heat_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_blistering_heat_damage_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            int32 bonusamount = GetSpellInfo()->Effects[EFFECT_1].BasePoints;
            uint32 damage = GetEffectValue();
            if (Aura* aura = GetCaster()->GetAura(SPELL_BLISTERING_HEAT))
                SetHitDamage(AddPct(damage, aura->GetStackAmount() * bonusamount));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_blistering_heat_damage_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_blistering_heat_damage_SpellScript();
    }
};

// 105551 106775 106765
class spell_ds_summon_tentacle : public SpellScriptLoader
{
public:
    spell_ds_summon_tentacle() : SpellScriptLoader("spell_ds_summon_tentacle") { }

    class spell_ds_summon_tentacle_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_summon_tentacle_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), GetEffectValue(), true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_summon_tentacle_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_summon_tentacle_SpellScript();
    }
};

// 106239
class spell_ds_summon_tail : public SpellScriptLoader
{
public:
    spell_ds_summon_tail() : SpellScriptLoader("spell_ds_summon_tail") { }

    class spell_ds_summon_tail_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_summon_tail_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                if (Creature* limb = ObjectAccessor::GetCreature((*GetCaster()), instance->GetData64(DATA_ACTIVE_PLATFORM)))
                    if (Creature* trigger = limb->FindNearestCreature(NPC_CORRUPTION_TRIGGER, 200.0f))
                    {
                        targets.clear();
                        targets.push_back(trigger);
                    }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_summon_tail_SpellScript::FilterTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_summon_tail_SpellScript();
    }
};

// 106548
class spell_ds_agonizing_pain : public SpellScriptLoader
{
public:
    spell_ds_agonizing_pain() : SpellScriptLoader("spell_ds_agonizing_pain") { }

    class spell_ds_agonizing_pain_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_agonizing_pain_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            int32 damage = CalculatePct(GetHitCreature()->GetMaxHealth(), GetEffectValue());
            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_agonizing_pain_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_agonizing_pain_SpellScript();
    }
};

// 105408
class spell_ds_burning_blood : public SpellScriptLoader
{
public:
    spell_ds_burning_blood() : SpellScriptLoader("spell_ds_burning_blood") { }

    class spell_ds_burning_blood_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_burning_blood_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            int32 bonusamount = (100 - GetCaster()->GetHealthPct()) / 2;
            uint32 damage = GetEffectValue() * (bonusamount ? bonusamount : 1);
            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_burning_blood_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_burning_blood_SpellScript();
    }
};

// 106663 106668 106670 106672 106674 106676
class spell_ds_carrying_winds : public SpellScriptLoader
{
public:
    spell_ds_carrying_winds() : SpellScriptLoader("spell_ds_carrying_winds") { }

    class spell_ds_carrying_winds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_carrying_winds_SpellScript);

        void ModDestHeight(SpellEffIndex /*effIndex*/)
        {
            std::list<Creature*> creatureList;
            GetCreatureListWithEntryInGrid(creatureList, GetCaster(), NPC_JUMP_PAD, 200.0f);
            Unit* caster = GetCaster();
            creatureList.remove_if([caster](WorldObject* target)
            {
                return target->GetDistance2d(caster) < 50.0f && !target->HasInArc(M_PI, caster);
            });
            creatureList.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            creatureList.pop_front();
            if (creatureList.size() > 1)
                creatureList.resize(1);

            for (Creature* target : creatureList)
            {
                Position pos;
                target->GetNearPosition(pos, 5.0f, 0.0f);
                GetHitDest()->Relocate(pos);
            }
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_ds_carrying_winds_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_JUMP_DEST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_carrying_winds_SpellScript();
    }
};

// 106666, 106669, 106671, 106673,106675,106677
class spell_ds_carrying_winds_trigger : public SpellScriptLoader
{
public:
    spell_ds_carrying_winds_trigger() : SpellScriptLoader("spell_ds_carrying_winds_trigger") { }

    class spell_ds_carrying_winds_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_carrying_winds_trigger_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetCaster(), GetEffectValue(), false);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_carrying_winds_trigger_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_carrying_winds_trigger_SpellScript();
    }
};

// helper for shortened code
typedef npc_ds_dragon_aspect::npc_ds_dragon_aspectAI AspectAI;

// 106613, 106588, 106600, 106624
class spell_ds_expose_weakness : public SpellScriptLoader
{
public:
    spell_ds_expose_weakness() : SpellScriptLoader("spell_ds_expose_weakness") { }

    class spell_ds_expose_weakness_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_expose_weakness_SpellScript);

        void FilterTarget(WorldObject*& target)
        {
            if (AspectAI* aspectai = CAST_AI(AspectAI, GetCaster()->GetAI()))
                if (Creature* tentacle = aspectai->GetLimbTentacle())
                    target = tentacle;
        }

        void Register() override
        {
            OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_ds_expose_weakness_SpellScript::FilterTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_expose_weakness_SpellScript();
    }
};

// 106940
class spell_ds_trigger_concentration : public SpellScriptLoader
{
public:
    spell_ds_trigger_concentration() : SpellScriptLoader("spell_ds_trigger_concentration") { }

    class spell_ds_trigger_concentration_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_trigger_concentration_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            targets.clear();
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                if (Creature* aspect = ObjectAccessor::GetCreature((*GetCaster()), instance->GetData64(DATA_ACTIVE_PLATFORM)))
                    targets.push_back(aspect);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_trigger_concentration_SpellScript::FilterTarget, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_trigger_concentration_SpellScript();
    }
};

// -106043
class spell_ds_spellweave : public SpellScriptLoader
{
public:
    spell_ds_spellweave() : SpellScriptLoader("spell_ds_spellweave") { }

    class spell_ds_spellweave_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_spellweave_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            if (Player* caster = GetCaster()->ToPlayer())
                if (Unit* target = caster->GetSelectedUnit())
                    targets.remove(target);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_spellweave_SpellScript::FilterTarget, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_spellweave_SpellScript();
    }
};

// 106766
class spell_ds_select_random : public SpellScriptLoader
{
public:
    spell_ds_select_random() : SpellScriptLoader("spell_ds_select_random") { }

    class spell_ds_select_random_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_select_random_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            WorldObject* target = nullptr;
            if (!targets.empty())
                target = Trinity::Containers::SelectRandomContainerElement(targets);
            targets.clear();
            targets.push_back(target);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_select_random_SpellScript::FilterTarget, EFFECT_ALL, TARGET_DEST_NEARBY_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_select_random_SpellScript();
    }
};

// 106527
class spell_ds_cataclysm_aura : public SpellScriptLoader
{
public:
    spell_ds_cataclysm_aura() : SpellScriptLoader("spell_ds_cataclysm_aura") { }

    class spell_ds_cataclysm_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_cataclysm_aura_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetTarget()->GetMapId() != 967)
                Remove(AURA_REMOVE_BY_DEFAULT);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_cataclysm_aura_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_cataclysm_aura_AuraScript();
    }
};

// 105658
class spell_ds_elementium_bolt : public SpellScriptLoader
{
public:
    spell_ds_elementium_bolt() : SpellScriptLoader("spell_ds_elementium_bolt") { }

    class spell_ds_elementium_bolt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_elementium_bolt_SpellScript);

        void ModDestHeight(SpellEffIndex /*effIndex*/)
        {
            WorldLocation *offset = GetHitDest();
            const_cast<WorldLocation*>(GetExplTargetDest())->Relocate(-11960.88f, 12073.58f, 35.45797f);
            GetHitDest()->Relocate(-11960.88f, 12073.58f, 35.45797f);
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_ds_elementium_bolt_SpellScript::ModDestHeight, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_elementium_bolt_SpellScript();
    }
};

class spell_ds_corrupted_blood_periodic : public SpellScriptLoader
{
public:
    spell_ds_corrupted_blood_periodic() : SpellScriptLoader("spell_ds_corrupted_blood_periodic") { }

    class spell_ds_corrupted_blood_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_corrupted_blood_periodic_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                if (Creature* alex = instance->GetCreature(NPC_ALEXSTRASZA))
                    if (!alex->HasAura(SPELL_CAUTERIZE_DEATHWING_5_SEC) && !alex->HasAura(SPELL_CAUTERIZE_DEATHWING_10_SEC) && !alex->HasAura(SPELL_CAUTERIZE_DEATHWING_15_SEC))
                        GetTarget()->SetPower(POWER_ALTERNATE_POWER, std::max(int32((21 - GetTarget()->GetHealthPct())), 1));

            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_1))
                aurEff->SetAmount(std::max(GetTarget()->GetPower(POWER_ALTERNATE_POWER) * 100, 100));
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_corrupted_blood_periodic_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_corrupted_blood_periodic_AuraScript();
    }
};

// -105723
class spell_ds_elementium_blast : public SpellScriptLoader
{
public:
    spell_ds_elementium_blast() : SpellScriptLoader("spell_ds_elementium_blast") { }

    class spell_ds_elementium_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_elementium_blast_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            // cap damage at 50% at 30y range
            float distance = std::min(GetHitUnit()->GetDistance2d(GetCaster()), 30.0f);
            const int32 damage = GetHitDamage();
            int32 newDamage = int32(damage - damage * (distance * 1 / 60));
            SetHitDamage(newDamage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_elementium_blast_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_elementium_blast_SpellScript();
    }
};

class spell_ds_temp_area : public SpellScriptLoader
{
public:
    spell_ds_temp_area() : SpellScriptLoader("spell_ds_temp_area") { }

    class spell_ds_temp_area_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_temp_area_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target){ return target->GetTypeId() == TYPEID_PLAYER; });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_temp_area_SpellScript::FilterTarget, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_temp_area_SpellScript();
    }
};

class spell_ds_dream : public SpellScriptLoader
{
public:
    spell_ds_dream() : SpellScriptLoader("spell_ds_dream") { }

    class spell_ds_dream_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_dream_SpellScript);

        SpellCastResult CheckSpellOverride()
        {
            if (!GetCaster()->HasAura(SPELL_ENTER_THE_DREAM_TRIGGER))
                return SPELL_FAILED_SPELL_UNAVAILABLE;

            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_ds_dream_SpellScript::CheckSpellOverride);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_dream_SpellScript();
    }
};

class spell_ds_corrupting_parasite : public SpellScriptLoader
{
public:
    spell_ds_corrupting_parasite() : SpellScriptLoader("spell_ds_corrupting_parasite") { }

    class spell_ds_corrupting_parasite_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_corrupting_parasite_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) {
                return target->ToPlayer() && target->ToPlayer()->HasTankSpec();
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
            GetCaster()->CastSpell(GetHitUnit(), SPELL_CORRUPTING_PARASITE_PERIODIC, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_corrupting_parasite_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_corrupting_parasite_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_corrupting_parasite_SpellScript();
    }
};

class spell_ds_unstable_corruption : public SpellScriptLoader
{
public:
    spell_ds_unstable_corruption() : SpellScriptLoader("spell_ds_unstable_corruption") { }

    class spell_ds_unstable_corruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_unstable_corruption_SpellScript);

        void HandleDamage(SpellEffIndex effIndex)
        {
            SetEffectDamage(CalculatePct(GetCaster()->GetHealth(), GetSpellInfo()->Effects[effIndex].CalcValue(GetCaster())));
        }

        void Despawn()
        {
            if (GetCaster()->ToCreature())
                GetCaster()->ToCreature()->DespawnOrUnsummon(1000);
        }

        void Register()
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_ds_unstable_corruption_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnHit += SpellHitFn(spell_ds_unstable_corruption_SpellScript::Despawn);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_unstable_corruption_SpellScript();
    }
};

class spell_ds_corrupting_parasite_periodic : public SpellScriptLoader
{
public:
    spell_ds_corrupting_parasite_periodic() : SpellScriptLoader("spell_ds_corrupting_parasite_periodic") { }

    class spell_ds_corrupting_parasite_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_corrupting_parasite_periodic_AuraScript);

        void onPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
            {
                if (aurEff->GetTickNumber() > 1)
                {
                    int32 damage = aurEff->GetAmount();
                    damage += CalculatePct(aurEff->GetBaseAmount(), 45);
                    aurEff->SetAmount(damage);
                }
            }

        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Position pos(*GetTarget());
            GetTarget()->SummonCreature(NPC_CORRUPTING_PARASITE, pos, TEMPSUMMON_TIMED_DESPAWN, 20000);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_corrupting_parasite_periodic_AuraScript::onPeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            OnEffectRemove += AuraEffectRemoveFn(spell_ds_corrupting_parasite_periodic_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_corrupting_parasite_periodic_AuraScript();
    }
};

class spell_ds_congealing_blood : public SpellScriptLoader
{
public:
    spell_ds_congealing_blood() : SpellScriptLoader("spell_ds_congealing_blood") { }

    class spell_ds_congealing_blood_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_congealing_blood_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_congealing_blood_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_congealing_blood_SpellScript();
    }
};

class at_isle_jump : public AreaTriggerScript
{
public:
    at_isle_jump() : AreaTriggerScript("at_isle_jump") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* areaTrigger)
    {
        if (player->GetMotionMaster()->GetCurrentMovementGeneratorType() == EFFECT_MOTION_TYPE)
            return true;

        switch (areaTrigger->id)
        {
            case 7110:
            case 7111:
                player->CastSpell(player, SPELL_CARRYING_WINDS_1, false);
                break;
            case 7115:
            case 7116:
                if (!player->HasAura(106672))
                    player->CastSpell(player, SPELL_CARRYING_WINDS_2, false);
                break;
            case 7112:
            case 7113:
            case 7118:
                player->CastSpell(player, SPELL_CARRYING_WINDS_3, false);
                break;
            case 7117:
            case 7119:
                player->CastSpell(player, SPELL_CARRYING_WINDS_4, false);
                break;
            case 7114:
            case 7120:
                player->CastSpell(player, SPELL_CARRYING_WINDS_5, false);
                break;
            default:
                break;
        }
        return true;
    }
};

void AddSC_boss_deathwing_madness()
{
    new boss_deathwing_madness();
    new npc_ds_madness_of_deathwing_head();
    new npc_ds_dragon_aspect();
    new npc_ds_thrall_madness();
    new npc_ds_limb_tentacle();
    new npc_ds_elementium_fragment();
    new npc_ds_mutated_corruption();
    new npc_ds_elementium_bolt();
    new npc_ds_regenerative_blood();
    new npc_ds_corrupted_parasite();
    new npc_ds_congealing_blood();
    new spell_ds_assault_aspects();
    new spell_ds_blistering_heat();
    new spell_ds_blistering_heat_damage();
    new spell_ds_summon_tentacle();
    new spell_ds_summon_tail();
    new spell_ds_agonizing_pain();
    new spell_ds_burning_blood();
    new spell_ds_carrying_winds();
    new spell_ds_carrying_winds_trigger();
    new spell_ds_expose_weakness();
    new spell_ds_trigger_concentration();
    new spell_ds_spellweave();
    new spell_ds_cataclysm_aura();
    new spell_ds_elementium_bolt();
    new spell_ds_corrupted_blood_periodic();
    new spell_ds_elementium_blast();
    new spell_ds_temp_area();
    new spell_ds_dream();
    new spell_ds_corrupting_parasite();
    new spell_ds_unstable_corruption();
    new spell_ds_corrupting_parasite_periodic();
    new spell_ds_congealing_blood();
    new at_isle_jump();
};
