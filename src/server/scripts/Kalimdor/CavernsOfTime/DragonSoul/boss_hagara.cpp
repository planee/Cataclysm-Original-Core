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
#include "GameObjectAI.h"
#include "dragonsoul.h"
#include "LFGMgr.h"
#include "Group.h"

enum Texts
{
    TALK_INTRO                          = -56614,
    TALK_INTRO_2                        = -56615,
    TALK_INTRO_3                        = -56616,
    TALK_INTRO_4                        = -56617,
    TALK_INTRO_5                        = -55646,
    TALK_AGGRO                          = -55647,
    TALK_ICE_LANCE                      = 55658,
    TALK_ICE_TOMB                       = 55652,
    TALK_ICE_WAVE                       = 55654,
    TALK_CRYSTAL_DESTROYED              = 55661,
    TALK_LIGHTING_STORM                 = 55656,
    TALK_OVERLOAD                       = 55668,
    TALK_FEEDBACK                       = 55664,
    TALK_KILLING_A_PLAYER               = 55648,
    TALK_DEATH                          = -55715
};

enum Spells
{
    SPELL_RIDE_VEHICLE                  = 98750,
    SPELL_ICE_LANCE_SUMMON              = 105297,
    SPELL_ICE_LANCE_TARGETING           = 105285,
    SPELL_ICE_LANCE_VISUAL              = 105269,
    SPELL_ICE_TOMB_SELECTOR             = 104448,
    SPELL_ICE_TOMB_MISSILE              = 104451,
    SPELL_SHATTERED_ICE                 = 105289,
    SPELL_FOCUSED_ASSAULT               = 107851,
    SPELL_FROZEN_TEMPEST                = 105256,
    SPELL_LIGHTNING_STORM               = 105409,
    SPELL_WATERY_ENTRENCHMENT_VISUAL    = 105259,
    SPELL_WATERY_ENTRENCHMENT_DEBUFF    = 110317,
    SPELL_ICE_WAVE                      = 105265,
    SPELL_FROSTFLAKE                    = 109325,
    SPELL_FROSTFLAKE_AREATRIGGER        = 109337,
    SPELL_FROSTFLAKE_SNARE              = 110316,
    SPELL_STORM_PILLARS                 = 109557,
    SPELL_FEEDBACK                      = 108934,
    SPELL_FROST_AXES                    = 109671,
    SPELL_LIGHTNING_AXES                = 109670,
    SPELL_HAGARA_FACING                 = 109474,

    // binding crystal
    SPELL_SPAWN_EFFECT                  = 70618,
    SPELL_CRYSTALLINE_TETHER            = 105311,
    SPELL_CRYSTALLINE_OVERLOAD          = 105312,

    // Collapsing Icicle
    SPELL_ICICLE_SUMMON                 = 109315,
    SPELL_ICICLE_FALL_TRIGGER           = 69426,
    SPELL_ICICLE_FALL_VISUAL            = 69428,

    // crystal conductor
    SPELL_CONDUCTOR_INACTIVE            = 105343,
    SPELL_CONDUCTOR_ACTIVE              = 109180,
    SPELL_CRYSTALLINE_TETHER_STORM      = 105482,
    SPELL_LIGHTNING_CONDUIT             = 105369,
    SPELL_LIGHTNING_CONDUIT_TRIGGER     = 105371,
    SPELL_LIGHTNING_CONDUIT_UNK         = 105377,
    SPELL_LIGHTNING_CONDUIT_DAMAGE      = 108569,
    SPELL_OVERLOAD_2                    = 105487,

    // bound lightning elemental
    SPELL_PERMANENT_FEIGN_DEATH         = 70628,
    SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP = 105367, // hit npc 56165
    SPELL_OVERLOAD                      = 105481, // hit npc 56165
};

enum Events
{
    // pre event
    EVENT_START_INTRO_EVENT             = 1,
    EVENT_SUMMON_GROUP_PORTAL_ONE       = 2,
    EVENT_SUMMON_GROUP_ONE              = 3,
    EVENT_SUMMON_GROUP_PORTAL_TWO       = 4,
    EVENT_SUMMON_GROUP_TWO              = 5,
    EVENT_SUMMON_GROUP_PORTAL_THREE     = 6,
    EVENT_SUMMON_GROUP_THREE            = 7,
    EVENT_SUMMON_GROUP_PORTAL_FOUR      = 8,
    EVENT_SUMMON_GROUP_FOUR             = 9,
    EVENT_FINISH_INTRO_EVENT            = 10,

    // Normal Phase
    EVENT_FOCUSED_ASSAULT               = 11,
    EVENT_ICE_LANCE_SUMMON              = 12,
    EVENT_SHATTERED_ICE                 = 13,
    EVENT_ICE_TOMB                      = 14,
    // Frost Phase
    EVENT_START_FROST_PHASE             = 15,
    EVENT_SUMMON_BINDING_CRYSTAL        = 16,
    EVENT_FROSTFLAKE                    = 17,
    EVENT_HANDLE_AREATRIGGERS           = 18,
    EVENT_SUMMON_ICICLE                 = 19,
    EVENT_SUMMON_ICE_WAVE               = 20,
    // Storm Phase
    EVENT_START_STORM_PHASE             = 21,
    EVENT_SUMMON_CRYSTAL_CONDUCTOR      = 22,
    EVENT_STORM_PILLARS                 = 23,
    // crystal conductor
    EVENT_LIGHTNING_CONDUIT             = 24,
    EVENT_REMOVE_JUMP_VISUAL            = 25,
    // ice tomb
    EVENT_CHECK_OWNER                   = 26
};

enum Phases
{
    PHASE_ALL       = 0,
    PHASE_INTRO     = 1,
    PHASE_NORMAL    = 2,
    PHASE_FROST     = 3,
    PHASE_STORM     = 4
};

enum Actions
{
    ACTION_START_INTRO_EVENT            = 1,
    ACTION_BINDING_CRYSTAL_DEATH        = 2,
    ACTION_CRYSTAL_CONDUCTOR_ACTIVATED  = 3,
    ACTION_FINISH_SPECIAL_PHASE         = 4
};

enum Weapons
{
    FROST_WEAPONS       = 75237,
    LIGHTNING_WEAPONS   = 75236,
};

enum visuals
{
    VISUAL_KIT_STORM_PHASE      = 21805,
    SPELL_VISUAL_STORM_PHASE    = 10604
};

enum Achievements
{
    CRITERIA_HOLDING_HANDS = 110520,
};

static const DragonSoulEventSpawns Portals[6] =
{
    { NPC_TRASH_TWILIGHT_PORTAL, 13546.790f, 13612.830f, 123.567f, 0.180f }, // wave one
    { NPC_TRASH_TWILIGHT_PORTAL, 13558.200f, 13581.200f, 123.567f, 0.925f }, // wave two
    { NPC_TRASH_TWILIGHT_PORTAL, 13557.400f, 13642.400f, 123.567f, 5.445f }, // wave two
    { NPC_TRASH_TWILIGHT_PORTAL, 13587.400f, 13570.299f, 123.567f, 1.605f }, // wave three
    { NPC_TRASH_TWILIGHT_PORTAL, 13588.000f, 13652.599f, 123.567f, 4.747f }, // wave three
    { NPC_TRASH_TWILIGHT_PORTAL, 13569.299f, 13612.099f, 122.502f, 3.089f }  // wave four
};

static const DragonSoulEventSpawns WaveOne[4] =
{
    { NPC_TWILIGHT_FROST_EVOKER, 13553.200f, 13607.900f, 123.484f, 5.904f },
    { NPC_TWILIGHT_FROST_EVOKER, 13554.200f, 13618.900f, 123.484f, 0.263f },
    { NPC_STORMBORN_MYRMIDON,    13555.000f, 13610.500f, 123.484f, 5.972f },
    { NPC_STORMBORN_MYRMIDON,    13555.000f, 13616.000f, 123.484f, 0.248f }
};

static const DragonSoulEventSpawns WaveTwo[12] =
{
    { NPC_CORRUPTED_FRAGMENT, 13562.099f, 13584.400f, 123.567f, 0.820f },
    { NPC_CORRUPTED_FRAGMENT, 13559.200f, 13585.799f, 123.567f, 0.820f },
    { NPC_CORRUPTED_FRAGMENT, 13561.900f, 13581.299f, 123.567f, 0.820f },
    { NPC_CORRUPTED_FRAGMENT, 13560.200f, 13583.299f, 123.567f, 0.820f },
    { NPC_CORRUPTED_FRAGMENT, 13557.700f, 13583.500f, 123.567f, 0.820f },
    { NPC_CORRUPTED_FRAGMENT, 13559.700f, 13639.099f, 123.567f, 5.585f },
    { NPC_CORRUPTED_FRAGMENT, 13561.099f, 13641.900f, 123.567f, 5.585f },
    { NPC_CORRUPTED_FRAGMENT, 13557.400f, 13639.700f, 123.567f, 5.585f },
    { NPC_CORRUPTED_FRAGMENT, 13558.000f, 13642.299f, 123.567f, 5.585f },
    { NPC_CORRUPTED_FRAGMENT, 13560.799f, 13644.200f, 123.567f, 5.585f },
    { NPC_STORMBINDER_ADEPT,  13556.900f, 13643.500f, 123.567f, 5.550f },
    { NPC_STORMBINDER_ADEPT,  13557.900f, 13581.200f, 123.567f, 0.078f }
};

static const DragonSoulEventSpawns WaveThree[6] =
{
    { NPC_TWILIGHT_FROST_EVOKER, 13590.500f, 13571.400f, 123.536f, 1.340f },
    { NPC_TWILIGHT_FROST_EVOKER, 13585.000f, 13651.700f, 123.514f, 4.690f },
    { NPC_STORMBORN_MYRMIDON,    13584.099f, 13571.099f, 123.539f, 1.659f },
    { NPC_STORMBORN_MYRMIDON,    13591.000f, 13651.599f, 123.515f, 4.737f },
    { NPC_STORMBINDER_ADEPT,     13587.500f, 13571.299f, 123.528f, 1.580f },
    { NPC_STORMBINDER_ADEPT,     13588.000f, 13651.799f, 123.499f, 4.680f }
};

static const DragonSoulEventSpawns WaveFour[11] =
{
    { NPC_CORRUPTED_FRAGMENT, 13570.500f, 13602.900f, 122.503f, 5.131f },
    { NPC_CORRUPTED_FRAGMENT, 13569.300f, 13616.100f, 122.503f, 1.692f },
    { NPC_CORRUPTED_FRAGMENT, 13573.800f, 13610.600f, 122.503f, 0.087f },
    { NPC_CORRUPTED_FRAGMENT, 13573.000f, 13615.200f, 122.503f, 0.942f },
    { NPC_CORRUPTED_FRAGMENT, 13567.200f, 13606.200f, 122.503f, 4.014f },
    { NPC_CORRUPTED_FRAGMENT, 13569.700f, 13612.200f, 122.503f, 5.846f },
    { NPC_CORRUPTED_FRAGMENT, 13566.200f, 13614.100f, 122.503f, 3.385f },
    { NPC_CORRUPTED_FRAGMENT, 13566.500f, 13609.900f, 122.503f, 3.089f },
    { NPC_CORRUPTED_FRAGMENT, 13570.000f, 13608.200f, 122.503f, 2.321f },
    { NPC_CORRUPTED_FRAGMENT, 13572.900f, 13605.800f, 122.503f, 5.689f },
    { NPC_LIEUTENANT_SHARA,   13570.299f, 13612.200f, 122.494f, 6.244f }
};

Position const bindingCrystalPos[4] =
{
    { 13617.500f, 13580.920f, 123.567f, 0.0000f },
    { 13557.730f, 13580.650f, 123.567f, 0.0000f },
    { 13557.420f, 13643.130f, 123.567f, 0.0000f },
    { 13617.320f, 13643.450f, 123.567f, 0.0000f }
};

Position const crystalConductor25PlayerPos[4] =
{
    { 13587.280f, 13658.630f, 123.567f, 0.0000f },
    { 13541.830f, 13611.320f, 123.567f, 0.0000f },
    { 13587.390f, 13566.770f, 123.567f, 0.0000f },
    { 13633.020f, 13612.080f, 123.567f, 0.0000f }
};

Position const crystalConductor10PlayerNormalPos[4] =
{
    { 13609.000f, 13634.420f, 123.483f, 0.0000f },
    { 13608.676f, 13589.943f, 123.431f, 0.0000f },
    { 13566.217f, 13589.740f, 123.483f, 0.0000f },
    { 13566.325f, 13634.353f, 123.483f, 0.0000f }
};

Position const crystalConductor10PlayerHeroicPos[8] =
{
    { 13587.280f, 13658.630f, 123.567f, 0.0000f },
    { 13541.830f, 13611.320f, 123.567f, 0.0000f },
    { 13587.390f, 13566.770f, 123.567f, 0.0000f },
    { 13633.020f, 13612.080f, 123.567f, 0.0000f },
    { 13609.000f, 13634.420f, 123.483f, 0.0000f },
    { 13608.676f, 13589.943f, 123.431f, 0.0000f },
    { 13566.217f, 13589.740f, 123.483f, 0.0000f },
    { 13566.325f, 13634.353f, 123.483f, 0.0000f }
};

Position const centerPosition = { 13587.886f, 13612.227f, 122.419f, 0.0506f };

class MoveAroundPlatformEvent : public BasicEvent
{
public:
    MoveAroundPlatformEvent(Unit* hagara, Unit* wave, float radius, float orient, bool sens, float precision) : _hagara(hagara), _wave(wave), _radius(radius), _orient(orient), _sens(sens), _precision(precision)
    {
    }

    bool Execute(uint64 execTime, uint32 /*diff*/)
    {
        float x, y;
        _hagara->GetNearPoint2D(x, y, _radius, _orient);
        _wave->GetMotionMaster()->Clear();
        _wave->GetMotionMaster()->MovePoint(0, x, y, 125.5f, false);
        _wave->m_Events.AddEvent(new MoveAroundPlatformEvent(_hagara, _wave, _radius, _orient + (_sens ? _precision : -_precision), _sens, _precision), execTime + 530);
        return true;
    }

private:
    Unit* _hagara;
    Unit* _wave;
    float _radius;
    float _orient;
    bool _sens;
    float _precision;
};

class boss_ds_hagara : public CreatureScript
{
public:
    boss_ds_hagara() : CreatureScript("boss_ds_hagara") {}

    struct boss_ds_hagaraAI : public BossAI
    {
        boss_ds_hagaraAI(Creature* creature) : BossAI(creature, DATA_HAGARA)
        {
            introTrashCount = 0;
            nextIntroPhase = 1;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_1);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_POSSESS, true);
            me->SetVisible(false);
        }

        void Reset() override
        {
            CrystalCount = 0;
            me->SetReactState(REACT_AGGRESSIVE);
            me->InterruptNonMeleeSpells(false);
            me->SetControlled(false, UNIT_STATE_ROOT);
            
            if (instance->GetBossState(DATA_HAGARA) != SPECIAL)
                _Reset();

            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_SUMMIT, 150.00f))
                portal->SetVisible(true);

            if (urand(0, 1))
            {
                firstPhase = EVENT_START_FROST_PHASE;
                SetEquipmentSlots(false, FROST_WEAPONS, FROST_WEAPONS);
            }
            else
            {
                firstPhase = EVENT_START_STORM_PHASE;
                SetEquipmentSlots(false, LIGHTNING_WEAPONS, LIGHTNING_WEAPONS);
            }

            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
            {
                if (!me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
                    me->CastSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, true);
                me->SetLootMode(LOOT_MODE_LFR);
            }
            else if (instance->GetData(DATA_RAID_NERF) == RAID_NERF_ACTIVE && !me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
            {
                int32 currentNormalAndHcNerfAmount = -5;
                me->CastCustomSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, &currentNormalAndHcNerfAmount, &currentNormalAndHcNerfAmount, NULL, true);
                DoCast(me, SPELL_POWER_OF_THE_ASPECTS, true);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            switch (summon->GetEntry())
            {
                case NPC_HOVER_DISK:
                    DoCast(summon, SPELL_RIDE_VEHICLE, true);
                    break;
                case NPC_TWILIGHT_FROST_EVOKER:
                case NPC_STORMBORN_MYRMIDON:
                case NPC_CORRUPTED_FRAGMENT:
                case NPC_STORMBINDER_ADEPT:
                case NPC_LIEUTENANT_SHARA:
                    introTrashCount++;
                    break;
                case NPC_BINDING_CRYSTAL:
                    summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    summon->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                    summon->CastSpell(summon, SPELL_SPAWN_EFFECT, true);
                    summon->CastSpell(me, SPELL_CRYSTALLINE_TETHER, true);
                    break;
                case NPC_CRYSTAL_CONDUCTOR:
                    summon->CastSpell(summon, SPELL_CONDUCTOR_INACTIVE, true);
                    summon->CastSpell(me, SPELL_CRYSTALLINE_TETHER_STORM, true);
                    break;
                default:
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer) override
        {
            switch (summon->GetEntry())
            {
                case NPC_TWILIGHT_FROST_EVOKER:
                case NPC_STORMBORN_MYRMIDON:
                case NPC_CORRUPTED_FRAGMENT:
                case NPC_STORMBINDER_ADEPT:
                case NPC_LIEUTENANT_SHARA:
                    if (--introTrashCount == 0)
                    {
                        summons.DespawnEntry(NPC_TRASH_TWILIGHT_PORTAL);
                        switch (nextIntroPhase)
                        {
                            case 2:
                                events.ScheduleEvent(EVENT_SUMMON_GROUP_PORTAL_TWO, 2000);
                                break;
                            case 3:
                                events.ScheduleEvent(EVENT_SUMMON_GROUP_PORTAL_THREE, 2000);
                                break;
                            case 4:
                                events.ScheduleEvent(EVENT_SUMMON_GROUP_PORTAL_FOUR, 2000);
                                break;
                            case 5:
                                Talk(TALK_INTRO_5);
                                if (Creature* veh = me->GetVehicleCreatureBase())
                                {
                                    veh->GetMotionMaster()->MovePoint(0, 13587.886f, 13612.227f, 122.419f);
                                    veh->DespawnOrUnsummon(7000);
                                }
                                events.ScheduleEvent(EVENT_FINISH_INTRO_EVENT, 7500);
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                case NPC_BINDING_CRYSTAL:
                    summon->CastSpell(summon, SPELL_CRYSTALLINE_OVERLOAD, true);
                    me->RemoveAurasDueToSpell(SPELL_CRYSTALLINE_TETHER, summon->GetGUID());
                    me->AI()->DoAction(ACTION_BINDING_CRYSTAL_DEATH);
                    break;
                default:
                    break;
            }
            BossAI::SummonedCreatureDies(summon, killer);
        }

        void DoAction(int32 const actionId) override
        {
            switch (actionId)
            {
                case ACTION_START_INTRO_EVENT:
                    me->NearTeleportTo(13555.229f, 13628.706f, 136.382f, 5.925f);
                    me->SummonCreature(NPC_HOVER_DISK, 13555.229f, 13628.706f, 136.382f, 5.925f, TEMPSUMMON_MANUAL_DESPAWN);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetVisible(true);
                    events.SetPhase(PHASE_INTRO);
                    events.ScheduleEvent(EVENT_SUMMON_GROUP_PORTAL_ONE, 4000);
                    break;
                case ACTION_BINDING_CRYSTAL_DEATH:
                    if (++CrystalCount >= 4)
                    {
                        me->InterruptNonMeleeSpells(false);
                        me->RemoveAurasDueToSpell(SPELL_FROZEN_TEMPEST);
                    }
                    else
                        TalkBroadcastGroup(TALK_CRYSTAL_DESTROYED);
                    break;
                case ACTION_CRYSTAL_CONDUCTOR_ACTIVATED:
                {
                    if (++CrystalCount >= ( !Is25ManRaid() && IsHeroic() ? 8 : 4 ))
                    {
                        me->InterruptNonMeleeSpells(false);

                        uint8 AchievementCount = 0;
                        Map::PlayerList const& PlayerList = instance->instance->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                            if (Player* player = itr->getSource())
                                if (player->HasAura(sSpellMgr->GetSpellIdForDifficulty(SPELL_LIGHTNING_CONDUIT_DAMAGE, me)))
                                    AchievementCount++;

                        if (AchievementCount >= (Is25ManRaid() ? 25 : 10))
                            instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, CRITERIA_HOLDING_HANDS);
                    }
                    else
                        TalkBroadcastGroup(TALK_OVERLOAD);
                    break;
                }
                case ACTION_FINISH_SPECIAL_PHASE:
                {
                    bool wasInFrostPhase = events.IsInPhase(PHASE_FROST);
                    SetEquipmentSlots(false, (wasInFrostPhase ? LIGHTNING_WEAPONS : FROST_WEAPONS), (wasInFrostPhase ? LIGHTNING_WEAPONS : FROST_WEAPONS));
                    TalkBroadcastGroup(TALK_FEEDBACK);
                    summons.DespawnEntry(NPC_CRYSTAL_CONDUCTOR);
                    summons.DespawnEntry(NPC_LIGHTNING_ELEMENTAL);
                    summons.DespawnEntry(NPC_ICE_WAVE);
                    std::list<WorldObject*> targetList;
                    Trinity::AllWorldObjectsInRange objects(me, 100.0f);
                    Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
                    me->VisitNearbyObject(100.0f, searcher);
                    for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                        if (AreaTrigger* areatrigger = (*i)->ToAreaTrigger())
                            if (areatrigger->GetEntry() == SPELL_WATERY_ENTRENCHMENT_VISUAL || areatrigger->GetEntry() == SPELL_FROSTFLAKE_AREATRIGGER)
                                areatrigger->Remove();
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_CONDUIT_DAMAGE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FROSTFLAKE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FROSTFLAKE_SNARE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WATERY_ENTRENCHMENT_DEBUFF);
                    DoCast(me, SPELL_FEEDBACK, true);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetControlled(false, UNIT_STATE_ROOT);
                    events.SetPhase(PHASE_NORMAL);
                    events.ScheduleEvent(EVENT_ICE_LANCE_SUMMON, 12000, 0, PHASE_NORMAL);
                    events.ScheduleEvent(EVENT_FOCUSED_ASSAULT, 13000, 0, PHASE_NORMAL);
                    events.ScheduleEvent(EVENT_SHATTERED_ICE, 18500, 0, PHASE_NORMAL);
                    events.ScheduleEvent(wasInFrostPhase ? EVENT_START_STORM_PHASE : EVENT_START_FROST_PHASE, 62000, 0, PHASE_NORMAL);

                    if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
                        events.ScheduleEvent(EVENT_ICE_TOMB, 20000, 0, PHASE_NORMAL);

                    if (!wasInFrostPhase)
                        SendWeather(WEATHER_STATE_FINE);

                    break;
                }
                default:
                    break;
            }
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);

            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_SUMMIT, 150.00f))
                portal->SetVisible(false);

            Talk(TALK_AGGRO);
            events.ScheduleEvent(EVENT_FOCUSED_ASSAULT, 4000, 0, PHASE_NORMAL);
            events.ScheduleEvent(EVENT_ICE_LANCE_SUMMON, 10000, 0, PHASE_NORMAL);
            events.ScheduleEvent(EVENT_SHATTERED_ICE, 14000, 0, PHASE_NORMAL);
            events.ScheduleEvent(firstPhase, 30000, 0, PHASE_NORMAL);
            _EnterCombat();
        }

        void JustDied(Unit* killer) override
        {
            Talk(TALK_DEATH);
            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_SUMMIT, 150.00f))
                portal->SetVisible(true);

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            if (!players.isEmpty())
            {
                if (Group* group = players.begin()->getSource()->GetGroup())
                    if (group->isLFRGroup())
                        instance->FinishLfgDungeon(me);
            }

            _JustDied();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                TalkBroadcastGroup(TALK_KILLING_A_PLAYER);
        }

        void EnterEvadeMode()
        {
            me->SetControlled(false, UNIT_STATE_ROOT);
            _EnterEvadeMode();
            BossAI::EnterEvadeMode();
        }

        void SendWeather(WeatherState weather) const
        {
            WorldPacket data(SMSG_WEATHER, 9);
            data << uint32(weather);
            data << float(1.0f);
            data << uint8(0);
            SendPacketToPlayers(&data);
        }

        void SendPacketToPlayers(WorldPacket const* data) const
        {
            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            if (!players.isEmpty())
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->getSource())
                        if (player->GetAreaId() == me->GetAreaId())
                            player->GetSession()->SendPacket(data);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!events.IsInPhase(PHASE_INTRO) && !UpdateVictim())
                return;

            events.Update(diff);

            if (events.IsInPhase(PHASE_NORMAL) && me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_SUMMON_GROUP_PORTAL_ONE:
                        Talk(TALK_INTRO);
                        me->SummonCreature(Portals[0].npcId, Portals[0].x, Portals[0].y, Portals[0].z, Portals[0].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON_GROUP_ONE, 1000);
                        break;
                    case EVENT_SUMMON_GROUP_ONE:
                        nextIntroPhase = 2;
                        for (uint8 i = 0; i < 4; i++)
                            me->SummonCreature(WaveOne[i].npcId, WaveOne[i].x, WaveOne[i].y, WaveOne[i].z, WaveOne[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
                        break;
                    case EVENT_SUMMON_GROUP_PORTAL_TWO:
                        Talk(TALK_INTRO_2);
                        me->SummonCreature(Portals[1].npcId, Portals[1].x, Portals[1].y, Portals[1].z, Portals[1].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(Portals[2].npcId, Portals[2].x, Portals[2].y, Portals[2].z, Portals[2].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON_GROUP_TWO, 1000);
                        break;
                    case EVENT_SUMMON_GROUP_TWO:
                        nextIntroPhase = 3;
                        for (uint8 i = 0; i < 12; i++)
                            me->SummonCreature(WaveTwo[i].npcId, WaveTwo[i].x, WaveTwo[i].y, WaveTwo[i].z, WaveTwo[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
                        break;
                    case EVENT_SUMMON_GROUP_PORTAL_THREE:
                        Talk(TALK_INTRO_3);
                        me->SummonCreature(Portals[3].npcId, Portals[3].x, Portals[3].y, Portals[3].z, Portals[3].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        me->SummonCreature(Portals[4].npcId, Portals[4].x, Portals[4].y, Portals[4].z, Portals[4].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON_GROUP_THREE, 1000);
                        break;
                    case EVENT_SUMMON_GROUP_THREE:
                        nextIntroPhase = 4;
                        for (uint8 i = 0; i < 6; i++)
                            me->SummonCreature(WaveThree[i].npcId, WaveThree[i].x, WaveThree[i].y, WaveThree[i].z, WaveThree[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
                        break;
                    case EVENT_SUMMON_GROUP_PORTAL_FOUR:
                        Talk(TALK_INTRO_4);
                        me->SummonCreature(Portals[5].npcId, Portals[5].x, Portals[5].y, Portals[5].z, Portals[5].orientation, TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON_GROUP_FOUR, 1000);
                        break;
                    case EVENT_SUMMON_GROUP_FOUR:
                        nextIntroPhase = 5;
                        for (uint8 i = 0; i < 11; i++)
                            me->SummonCreature(WaveFour[i].npcId, WaveFour[i].x, WaveFour[i].y, WaveFour[i].z, WaveFour[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000);
                        break;
                    case EVENT_FINISH_INTRO_EVENT:
                        me->SetHomePosition(13587.886f, 13612.227f, 122.419f, 0.0506f);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_1);
                        events.SetPhase(PHASE_ALL);
                        instance->SetBossState(DATA_HAGARA, NOT_STARTED);
                        break;
                    case EVENT_FOCUSED_ASSAULT:
                        if (!IsHeroic())
                        {
                            me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()); // prevent interrupting SPELL_FOCUSED_ASSAULT with kiting on cast start
                            DoCastVictim(SPELL_FOCUSED_ASSAULT);
                        }
                        else
                        {
                            DoCastVictim(SPELL_FOCUSED_ASSAULT);
                            me->ClearUnitState(UNIT_STATE_CASTING);
                        }
                        events.ScheduleEvent(EVENT_FOCUSED_ASSAULT, 15000, 0, PHASE_NORMAL);
                        break;
                    case EVENT_ICE_LANCE_SUMMON:
                    {
                        TalkBroadcastGroup(TALK_ICE_LANCE);
                        for (int8 i = 0; i < 3; i++)
                        {
                            float angle = frand(0.0f, 2.0f * M_PI);
                            float tempAngle = me->NormalizeOrientation(angle);
                            Position pos(*me);
                            me->MoveBlink(pos, 100.0f, tempAngle);
                            me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_ICE_LANCE_SUMMON, true);
                        }
                        events.ScheduleEvent(EVENT_ICE_LANCE_SUMMON, 30000, 0, PHASE_NORMAL);
                        break;
                    }
                    case EVENT_SHATTERED_ICE:
                    {
                        Unit* caster = me;
                        std::list<WorldObject*> targetList;
                        Trinity::AllWorldObjectsInRange objects(me, 40.0f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects, GRID_MAP_TYPE_MASK_PLAYER);
                        me->VisitNearbyObject(40.0f, searcher);
                        targetList.remove_if([caster](WorldObject* target){
                            return !caster->isInFront(target);
                        });

                        if (targetList.empty())
                            DoCastRandom(SPELL_SHATTERED_ICE, 40.0f, false);
                        else
                        {
                            if (WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targetList))
                                DoCast(target->ToPlayer(), SPELL_SHATTERED_ICE);
                        }
                        events.ScheduleEvent(EVENT_SHATTERED_ICE, urand(10500, 14000), 0, PHASE_NORMAL);
                        break;
                    }
                    case EVENT_ICE_TOMB:
                        TalkBroadcastGroup(TALK_ICE_TOMB);
                        DoCast(me, SPELL_ICE_TOMB_SELECTOR);
                        break;
                    case EVENT_START_FROST_PHASE:
                        TalkBroadcastGroup(TALK_ICE_WAVE);
                        events.SetPhase(PHASE_FROST);
                        me->NearTeleportTo(me->GetHomePosition());
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetControlled(true, UNIT_STATE_ROOT); // prevent interrupt on target change seems hagara starts running if the tank is dead
                        DoCast(me, SPELL_FROZEN_TEMPEST, true);
                        icicleCount = 0;
                        events.ScheduleEvent(EVENT_SUMMON_BINDING_CRYSTAL, 1000, 0, PHASE_FROST);
                        events.ScheduleEvent(EVENT_SUMMON_ICICLE, 1000, 0, PHASE_FROST);
                        events.ScheduleEvent(EVENT_HANDLE_AREATRIGGERS, 5000, 0, PHASE_FROST);
                        events.ScheduleEvent(EVENT_SUMMON_ICE_WAVE, 5000, 0, PHASE_FROST);
                        if (IsHeroic())
                            events.ScheduleEvent(EVENT_FROSTFLAKE, 8000, 0, PHASE_FROST);
                        break;
                    case EVENT_SUMMON_BINDING_CRYSTAL:
                        CrystalCount = 0;
                        for (uint8 i = 0; i < 4; i++)
                            me->SummonCreature(NPC_BINDING_CRYSTAL, bindingCrystalPos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
                        break;
                    case EVENT_FROSTFLAKE:
                        DoCast(me, SPELL_FROSTFLAKE, true);
                        events.ScheduleEvent(EVENT_FROSTFLAKE, 5000, 0, PHASE_FROST);
                        break;
                    case EVENT_HANDLE_AREATRIGGERS:
                    {
                        std::list<WorldObject*> targetList;
                        std::list<AreaTrigger*> triggerList;
                        Trinity::AllWorldObjectsInRange objects(me, 100.0f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
                        me->VisitNearbyObject(100.0f, searcher);
                        for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                            if (AreaTrigger* areatrigger = (*i)->ToAreaTrigger())
                                triggerList.push_back(areatrigger);

                        if (!triggerList.empty())
                        {
                            Map::PlayerList const &PlList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                            {
                                if (Player* player = i->getSource())
                                {
                                    bool isInFrostFlake = false;
                                    for (AreaTrigger* trigger : triggerList)
                                    {
                                        if (trigger->GetEntry() == SPELL_FROSTFLAKE_AREATRIGGER)
                                        {
                                            if (trigger->GetExactDist2d(player) < 10.0f)
                                                isInFrostFlake = true;
                                        }
                                        else if (trigger->GetEntry() == SPELL_WATERY_ENTRENCHMENT_VISUAL)
                                        {
                                            if (trigger->GetExactDist2d(player) < 30.0f)
                                                player->AddAura(SPELL_WATERY_ENTRENCHMENT_DEBUFF, player);
                                            else
                                                player->RemoveAurasDueToSpell(SPELL_WATERY_ENTRENCHMENT_DEBUFF);
                                        }
                                    }
                                    if (!isInFrostFlake)
                                        player->RemoveAurasDueToSpell(SPELL_FROSTFLAKE_SNARE);
                                    else
                                        player->AddAura(SPELL_FROSTFLAKE_SNARE, player);
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_HANDLE_AREATRIGGERS, 500, 0, PHASE_FROST);
                        break;
                    }
                    case EVENT_SUMMON_ICICLE:
                    {
                        for (uint8 i = 0; i < 2; i++)
                        {
                            uint8 maxAttemps = 0;
                            while (maxAttemps < 20)
                            {
                                bool nearIce = false;
                                float angle = frand(0.0f, 2.0f * M_PI);
                                float dist = frand(34.90f, 47.38f);
                                Position pos(*me);
                                me->MoveBlink(pos, dist, angle);
                                std::list<Creature*> icicleTriggers;
                                GetCreatureListWithEntryInGrid(icicleTriggers, me, NPC_ICICLE_TRIGGER, 100.0f);
                                for (Creature* ice : icicleTriggers)
                                {
                                    if (ice->GetExactDist2d(&pos) < 10.0f)
                                    {
                                        nearIce = true;
                                        break;
                                    }
                                }

                                if (!nearIce)
                                {
                                    me->SummonCreature(NPC_ICICLE_TRIGGER, pos, TEMPSUMMON_MANUAL_DESPAWN);
                                    break;
                                }
                                maxAttemps++;
                            }

                        }
                        icicleCount++;
                        events.ScheduleEvent(EVENT_SUMMON_ICICLE, 1000, 0, PHASE_FROST);
                        break;
                    }
                    case EVENT_SUMMON_ICE_WAVE:
                    {
                        uint8 counter = 0;
                        for (uint8 i = 0; i < 16; i++)
                        {
                            float orientation = 0.0f;
                            Position pos = me->GetHomePosition();
                            if (i < 4)
                                orientation = 0.02622f;
                            else if (i >= 4 && i < 8)
                                orientation = 1.64728f;
                            else if (i >= 8 && i < 12)
                                orientation = 3.26521f;
                            else if (i >= 12)
                                orientation = 4.77945f;

                            float distance = 0.0f;
                            switch (++counter)
                            {
                                case 1:
                                    distance = 27.83f;
                                    break;
                                case 2:
                                    distance = 34.16f;
                                    break;
                                case 3:
                                    distance = 40.49f;
                                    break;
                                case 4:
                                    distance = 46.59f;
                                    counter = 0;
                                    break;
                            }

                            if (Creature* iceWave = me->SummonCreature(NPC_ICE_WAVE, pos, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                iceWave->CastSpell(iceWave, SPELL_ICE_WAVE, true);
                                iceWave->SetOrientation(orientation);
                                float x, y;
                                me->GetNearPoint2D(x, y, distance, orientation);
                                iceWave->GetMotionMaster()->Clear();
                                iceWave->GetMotionMaster()->MovePoint(0, x, y, 125.5f, false);
                                iceWave->m_Events.AddEvent(new MoveAroundPlatformEvent(me, iceWave, distance, orientation, false, 0.05f), iceWave->m_Events.CalculateTime(7000));
                            }
                        }
                        break;
                    }
                    case EVENT_START_STORM_PHASE:
                        CrystalCount = 0;
                        TalkBroadcastGroup(TALK_LIGHTING_STORM);
                        events.SetPhase(PHASE_STORM);
                        me->NearTeleportTo(me->GetHomePosition());
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetControlled(true, UNIT_STATE_ROOT); // prevent interrupt on target change seems hagara starts running if the tank is dead
                        me->SendPlaySpellVisualKit(VISUAL_KIT_STORM_PHASE, 0);
                        SendWeather(WHEATER_STATE_HEAVY_THUNDERSTORM);
                        DoCast(me, SPELL_LIGHTNING_STORM, true);
                        events.ScheduleEvent(EVENT_SUMMON_CRYSTAL_CONDUCTOR, 1000, 0, PHASE_STORM);
                        if (IsHeroic())
                            events.ScheduleEvent(EVENT_STORM_PILLARS, 4000, 0, PHASE_STORM);
                        break;
                    case EVENT_SUMMON_CRYSTAL_CONDUCTOR:
                        me->SendPlaySpellVisual(SPELL_VISUAL_STORM_PHASE, me, 0.00f, 0.00f, 0.00f, 0.00f);
                        me->SummonCreature(NPC_LIGHTNING_ELEMENTAL, 13628.44f, 13610.42f, 123.5672f, 0.00f, TEMPSUMMON_MANUAL_DESPAWN);
                        if (!Is25ManRaid())
                        {
                            if (IsHeroic())
                            {
                                for (uint8 i = 0; i < 8; i++)
                                    me->SummonCreature(NPC_CRYSTAL_CONDUCTOR, crystalConductor10PlayerHeroicPos[i], TEMPSUMMON_MANUAL_DESPAWN);
                            } else
                            {
                                for (uint8 i = 0; i < 4; i++)
                                    me->SummonCreature(NPC_CRYSTAL_CONDUCTOR, crystalConductor10PlayerNormalPos[i], TEMPSUMMON_MANUAL_DESPAWN);
                            }
                        }
                        else
                        {
                            for (uint8 i = 0; i < 4; i++)
                                me->SummonCreature(NPC_CRYSTAL_CONDUCTOR, crystalConductor25PlayerPos[i], TEMPSUMMON_MANUAL_DESPAWN);
                        }
                        break;
                    case EVENT_STORM_PILLARS:
                        DoCast(me, SPELL_STORM_PILLARS, true);
                        events.ScheduleEvent(EVENT_STORM_PILLARS, 3000, 0, PHASE_STORM);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        uint8 introTrashCount;
        uint8 nextIntroPhase;
        uint8 CrystalCount;
        uint8 icicleCount;
        uint32 firstPhase;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ds_hagaraAI(creature);
    }
};

class go_ds_focusing_iris : public GameObjectScript
{
public:
    go_ds_focusing_iris() : GameObjectScript("go_ds_focusing_iris") {}

    struct go_ds_focusing_irisAI : public GameObjectAI
    {
        go_ds_focusing_irisAI(GameObject* go) : GameObjectAI(go) {}

        bool GossipHello(Player* player)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (!instance->IsDone(DATA_HAGARA) && instance->GetBossState(DATA_HAGARA) != SPECIAL)
                    if (Creature* hagara = instance->GetCreature(DATA_HAGARA))
                    {
                        instance->SetBossState(DATA_HAGARA, SPECIAL);
                        hagara->AI()->DoAction(ACTION_START_INTRO_EVENT);
                        go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE | GO_FLAG_IN_USE);
                        go->Delete(3000);
                    }
            return false;
        }
    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_ds_focusing_irisAI(go);
    }
};

class npc_ds_ice_lance : public CreatureScript
{
public:
    npc_ds_ice_lance() : CreatureScript("npc_ds_ice_lance") {}

    struct npc_ds_ice_lanceAI : public Scripted_NoMovementAI
    {
        npc_ds_ice_lanceAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_ICE_LANCE_VISUAL, true);
            me->SetInCombatWithZone();
        }

        void EnterEvadeMode()
        {
            // this needs to be empty...
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                if (Player* player = me->FindNearestPlayer(100.0f))
                    DoCast(player, SPELL_ICE_LANCE_TARGETING);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_ice_lanceAI(creature);
    }
};

class npc_ds_ice_tomb : public CreatureScript
{
public:
    npc_ds_ice_tomb() : CreatureScript("npc_ds_ice_tomb") {}

    struct npc_ds_ice_tombAI : public Scripted_NoMovementAI
    {
        npc_ds_ice_tombAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_CHECK_OWNER, 1000);
        }

        void JustDied(Unit* killer)
        {
            uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_ICE_TOMB_MISSILE, me);
            if (Unit* owner = me->ToTempSummon()->GetSummoner())
            {
                owner->RemoveAurasDueToSpell(spellId);
                owner->RemoveAllGameObjects();
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_OWNER:
                        if (Unit* owner = me->ToTempSummon()->GetSummoner())
                        {
                            if (!owner->isAlive())
                            {
                                owner->RemoveAllGameObjects();
                                me->DespawnOrUnsummon();
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_OWNER, 1000);
                        }
                        else
                            me->DespawnOrUnsummon();
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
        return new npc_ds_ice_tombAI(creature);
    }
};

class npc_ds_crystal_conductor : public CreatureScript
{
public:
    npc_ds_crystal_conductor() : CreatureScript("npc_ds_crystal_conductor") {}

    struct npc_ds_crystal_conductorAI : public Scripted_NoMovementAI
    {
        npc_ds_crystal_conductorAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_CONDUCTOR_INACTIVE, true);
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo)
        {
            if (!me->HasAura(SPELL_CONDUCTOR_ACTIVE))
            {
                if (spellInfo->Id == SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP)
                {
                    if (Creature* hagara = instance->GetCreature(DATA_HAGARA))
                        hagara->AI()->DoAction(ACTION_CRYSTAL_CONDUCTOR_ACTIVATED);
                    me->InterruptNonMeleeSpells(false);
                    me->RemoveAurasDueToSpell(SPELL_CONDUCTOR_INACTIVE);
                    me->AddAura(SPELL_LIGHTNING_CONDUIT_TRIGGER, me);
                    DoCast(me, SPELL_CONDUCTOR_ACTIVE, true);
                    events.ScheduleEvent(EVENT_REMOVE_JUMP_VISUAL, 2000); // dont remove instant the visual link
                }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (me->HasAura(SPELL_CONDUCTOR_ACTIVE) && !me->HasAura(SPELL_LIGHTNING_CONDUIT_TRIGGER))
                me->AddAura(SPELL_LIGHTNING_CONDUIT_TRIGGER, me);

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_REMOVE_JUMP_VISUAL:
                        me->RemoveAurasDueToSpell(SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP);
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
        return new npc_ds_crystal_conductorAI(creature);
    }
};

class DelayedIcicleFallEvent : public BasicEvent
{
public:
    DelayedIcicleFallEvent(Creature* trigger, uint64 casterGUID) : _trigger(trigger), _casterGUID(casterGUID) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        _trigger->CastSpell(_trigger, SPELL_ICICLE_FALL_TRIGGER, true);
        _trigger->CastSpell(_trigger, SPELL_ICICLE_FALL_VISUAL, false);

        if (Creature* caster = ObjectAccessor::GetCreature(*_trigger, _casterGUID))
            caster->DespawnOrUnsummon();
        /// @todo: fix snowfall visual after collapse

        return true;
    }

private:
    Creature* _trigger;
    uint64 _casterGUID;
};

class npc_ds_icicle : public CreatureScript
{
public:
    npc_ds_icicle() : CreatureScript("npc_ds_icicle") { }

    struct npc_ds_icicleAI : public ScriptedAI
    {
        npc_ds_icicleAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner)
        {
            me->CastSpell(me, SPELL_ICICLE_SUMMON, true);
        }

        void JustSummoned(Creature* summon) override
        {
            summon->SetReactState(REACT_PASSIVE);
            summon->m_Events.AddEvent(new DelayedIcicleFallEvent(summon, me->GetGUID()), summon->m_Events.CalculateTime(3000));
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ds_icicleAI(creature);
    }
};

class npc_ds_lightning_elemental : public CreatureScript
{
public:
    npc_ds_lightning_elemental() : CreatureScript("npc_ds_lightning_elemental") {}

    struct npc_ds_lightning_elementalAI : public ScriptedAI
    {
        npc_ds_lightning_elementalAI(Creature* creature) : ScriptedAI(creature) {}

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->HealthBelowPctDamaged(1, damage))
            {
                damage = 0;
                me->SetHealth(me->GetMaxHealth());
                me->RemoveAllAuras();
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                if (Creature* crystal = me->FindNearestCreature(NPC_CRYSTAL_CONDUCTOR, 100.0f))
                {
                    DoCast(crystal, SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP, true);
                    DoCast(crystal, SPELL_OVERLOAD, true);
                }
                DoCast(me, SPELL_PERMANENT_FEIGN_DEATH, true);
                me->SetFlag(UNIT_FIELD_FLAGS, 571246592);
                me->SetFlag(UNIT_FIELD_FLAGS_2, 4196353);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_lightning_elementalAI(creature);
    }
};

class spell_ds_ice_lance : public SpellScriptLoader
{
public:
    spell_ds_ice_lance() : SpellScriptLoader("spell_ds_ice_lance") { }

    class spell_ds_ice_lance_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_ice_lance_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* channelTarget = ObjectAccessor::GetUnit(*GetCaster(), caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
                {
                    targets.remove_if([caster, channelTarget](WorldObject* target) 
                    { 
                        if (target == channelTarget)
                            return false;

                        return !target->IsInBetween(caster, channelTarget, 1.5f); 
                    });
                }

                if (!targets.empty())
                    targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));

                if (targets.size() > 1)
                    targets.resize(1);
            }
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
            if (GetHitUnit()->GetGUID() != GetCaster()->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_ICE_LANCE_TARGETING, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_ice_lance_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_CONE_ENEMY_104);
            OnEffectHitTarget += SpellEffectFn(spell_ds_ice_lance_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_ice_lance_SpellScript();
    }
};

class summonDelayedIceBlockEvent : public BasicEvent
{
public:
    summonDelayedIceBlockEvent(Unit* caster, uint64 targetGUID) : _caster(caster), _targetGUID(targetGUID) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (Unit* target = ObjectAccessor::GetUnit(*_caster, _targetGUID))
        {
            if (target->isAlive())
            {
                target->SummonCreature(NPC_ICE_TOMB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                target->SummonGameObject(GO_ICE_BLOCK_COLLISION, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 0.00f, 0.00f, 0.00f, 0.00f, DAY);
            }
        }
        return true;
    }

private:
    Unit* _caster;
    uint64 _targetGUID;
};

class spell_ds_ice_tomb_selector : public SpellScriptLoader
{
public:
    spell_ds_ice_tomb_selector() : SpellScriptLoader("spell_ds_ice_tomb_selector") { }

    class spell_ds_ice_tomb_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_ice_tomb_selector_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint8 targetCount = 2;
            if (Map* map = GetCaster()->GetMap())
            {
                if (map->Is25ManRaid())
                    targetCount = 5;
                else if (map->IsHeroic() && map->Is25ManRaid())
                    targetCount = 6;
            }

            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_ICE_TOMB_MISSILE));

            if (targets.size() > targetCount)
                targets.remove_if([](WorldObject* target){
                return target->ToPlayer() && target->ToPlayer()->HasTankSpec();
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, targetCount);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), SPELL_ICE_TOMB_MISSILE, true);
            GetCaster()->m_Events.AddEvent(new summonDelayedIceBlockEvent(GetCaster(), GetHitUnit()->GetGUID()), GetCaster()->m_Events.CalculateTime(7000));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_ice_tomb_selector_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_ice_tomb_selector_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_ice_tomb_selector_SpellScript();
    }
};

class spell_ds_focused_assault : public SpellScriptLoader
{
public:
    spell_ds_focused_assault() : SpellScriptLoader("spell_ds_focused_assault") { }

    class spell_ds_focused_assault_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_focused_assault_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            if (Unit* caster = GetCaster())
                if (Unit* target = ObjectAccessor::GetUnit(*caster, caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
                    caster->CastSpell(target, GetSpellInfo()->Effects[EFFECT_1].TriggerSpell, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_focused_assault_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_focused_assault_AuraScript();
    }
};

class spell_ds_frostflake : public SpellScriptLoader
{
public:
    spell_ds_frostflake() : SpellScriptLoader("spell_ds_frostflake") { }

    class spell_ds_frostflake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_frostflake_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetStackAmount() < 10)
                SetStackAmount(GetStackAmount() + 1);
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(GetTarget(), GetSpellInfo()->Effects[EFFECT_1].BasePoints, true);
        }

        void HandleOnDispel(DispelInfo* /*dispelData*/)
        {
            Remove(AURA_REMOVE_BY_ENEMY_SPELL);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_frostflake_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_ds_frostflake_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnDispel += AuraDispelFn(spell_ds_frostflake_AuraScript::HandleOnDispel);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_frostflake_AuraScript();
    }

    class spell_ds_frostflake_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_frostflake_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_FROSTFLAKE));

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_frostflake_SpellScript::FilterTargets, EFFECT_ALL, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_frostflake_SpellScript();
    }
};

class spell_ds_lightning_conduit_trigger : public SpellScriptLoader
{
public:
    spell_ds_lightning_conduit_trigger() : SpellScriptLoader("spell_ds_lightning_conduit_trigger") { }

    class spell_ds_lightning_conduit_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_lightning_conduit_trigger_AuraScript);

        void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32& amplitude)
        {
            isPeriodic = true;
            amplitude = 500;
        }

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
            {
                uint32 conduitDebuff = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIGHTNING_CONDUIT_DAMAGE, caster);
                Map::PlayerList const& players = caster->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* target = itr->getSource())
                        if (target->isAlive() && !target->HasAura(conduitDebuff, GetTarget()->GetGUID()))
                            if (caster->GetDistance2d(target) <= 10.0f)
                                caster->CastSpell(target, SPELL_LIGHTNING_CONDUIT_DAMAGE, true);
            }
        }

        void Register()
        {
            DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_ds_lightning_conduit_trigger_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_DUMMY);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_lightning_conduit_trigger_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ds_lightning_conduit_trigger_AuraScript();
    }
};

class spell_ds_lightning_conduit_periodic : public SpellScriptLoader
{
public:
    spell_ds_lightning_conduit_periodic() : SpellScriptLoader("spell_ds_lightning_conduit_periodic") { }

    class spell_ds_lightning_conduit_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_lightning_conduit_periodic_AuraScript);

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(GetTarget(), SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP, true);
        }

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            uint32 conduitDebuff = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIGHTNING_CONDUIT_DAMAGE, GetTarget());
            if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                instance->DoRemoveAurasDueToSpellOnPlayers(conduitDebuff, GetTarget()->GetGUID());

            if (Unit* caster = GetCaster())
                GetTarget()->RemoveAurasDueToSpell(SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP, caster->GetGUID());
        }

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            uint32 conduitDebuff = sSpellMgr->GetSpellIdForDifficulty(SPELL_LIGHTNING_CONDUIT_DAMAGE, GetTarget());

            if (Unit* caster = GetCaster())
            {
                if (GetTarget()->GetDistance2d(caster) > 10.0f || (caster->ToPlayer() && !caster->HasAura(conduitDebuff)))
                {
                    GetTarget()->RemoveAurasDueToSpell(SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP, caster->GetGUID());
                    Remove(AURA_REMOVE_BY_DEFAULT);
                    return;
                }
            }

            Map::PlayerList const& players = GetTarget()->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (Player* player = itr->getSource())
                    if (GetTarget()->GetDistance2d(player) <= 10.0f)
                        if (player->isAlive() && !player->HasAura(conduitDebuff))
                            GetTarget()->CastSpell(player, SPELL_LIGHTNING_CONDUIT_DAMAGE, true);

            std::list<Creature*> CreatureList;
            Trinity::AllCreaturesOfEntryInRange checker(GetTarget(), NPC_CRYSTAL_CONDUCTOR, 12.00f);
            Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(GetTarget(), CreatureList, checker);
            GetTarget()->VisitNearbyObject(12.00f, searcher);
            if (!CreatureList.empty())
                for (std::list<Creature*>::iterator iter = CreatureList.begin(); iter != CreatureList.end(); ++iter)
                    if (!(*iter)->HasAura(SPELL_CONDUCTOR_ACTIVE))
                        GetTarget()->CastSpell((*iter), SPELL_LIGHTNING_CONDUIT_VISUAL_JUMP, true);
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_ds_lightning_conduit_periodic_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_ds_lightning_conduit_periodic_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_lightning_conduit_periodic_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_lightning_conduit_periodic_AuraScript();
    }
};

class spell_ds_hagara_water_shield : public SpellScriptLoader
{
public:
    spell_ds_hagara_water_shield() : SpellScriptLoader("spell_ds_hagara_water_shield") { }

    class spell_ds_hagara_water_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_hagara_water_shield_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (!GetTarget() || !GetTarget()->ToCreature())
                return;

            GetTarget()->ToCreature()->AI()->DoAction(ACTION_FINISH_SPECIAL_PHASE);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_ds_hagara_water_shield_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_hagara_water_shield_AuraScript();
    }
};

class spell_ds_ice_shards_damage : public SpellScriptLoader
{
public:
    spell_ds_ice_shards_damage() : SpellScriptLoader("spell_ds_ice_shards_damage") {}

    class spell_ds_ice_shards_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_ice_shards_damage_SpellScript);

        void HandleDamage(SpellEffIndex effIndex)
        {
            if (GetCaster()->GetMapId() == 967) // this spell is currently used (sniffed) on two instances - in dragon soul it should deal 100k damage
                SetHitDamage(100000);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_ice_shards_damage_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_ice_shards_damage_SpellScript();
    }
};

class spell_ds_storm_pillars : public SpellScriptLoader
{
public:
    spell_ds_storm_pillars() : SpellScriptLoader("spell_ds_storm_pillars") { }

    class spell_ds_storm_pillars_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_storm_pillars_SpellScript);

        void FilterTarget(std::list<WorldObject*>& targets)
        {
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_storm_pillars_SpellScript::FilterTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_storm_pillars_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_storm_pillars_SpellScript();
    }
};

void AddSC_boss_hagara()
{
    new boss_ds_hagara();
    new go_ds_focusing_iris();
    new npc_ds_ice_lance();
    new npc_ds_ice_tomb();
    new npc_ds_icicle();
    new npc_ds_lightning_elemental();
    new npc_ds_crystal_conductor();
    new spell_ds_ice_lance();
    new spell_ds_ice_tomb_selector();
    new spell_ds_focused_assault();
    new spell_ds_frostflake();
    new spell_ds_lightning_conduit_trigger();
    new spell_ds_lightning_conduit_periodic();
    new spell_ds_hagara_water_shield();
    new spell_ds_ice_shards_damage();
    new spell_ds_storm_pillars();
};