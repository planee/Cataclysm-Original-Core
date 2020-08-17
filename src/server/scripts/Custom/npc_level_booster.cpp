/// Game
#include "Chat.h"
#include "Creature.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"

static const std::vector<uint32> g_SpellsToNotLearn =
{
    31687, 92315, 11366, 44425, ///< Mage
    12294, 23880, 23881, 23885, 23922, ///< Warrior
    36554, 13877, 1329, 5374, 27576, ///< Rogue
    15407, 88625, 47757, 47758, 47540, ///< Priest
    20473, 31935, 85256, 82242, ///< Paladin
    17962, 30146, 31117, 30108, ///< Warlock
    18562, 33878, 33876, 78674, 81283, 81291, ///< Druid
    53301, 82928, 19434, 19577, ///< Hunter
    51490, 60103, 379, 974, ///< Shaman
    90355, 24450, 81291, 81283,
    16979, 49376, 49377,
    33395, 31707,
    55749, 90337, 90335, 20557, 50433, 97229, 17253, 93433, 50256, 24423, 16827, 50541, 50285,
    35387, 90363, 1742, 34889, 58943, 54644, 92380, 24604, 58604, 35290, 24844, 2649, 90327, 90339, 54680, 93434, 50479, 50245, 20555, 90347, 94019, 24550, 93435, 26090, 50318, 90364, 26064, 50518, 49966, 91644, 56626, 50519, 50498, 90361, 50271, 90328, 90309, 50274, 20558, 57386, 35346, 94022, 54706, 4167, 96201,
    6307, 7814, 30213, 3110, 30151, 17735, 3716, 83967,
};

class npc_level_booster : public CreatureScript
{
    public:
        npc_level_booster() : CreatureScript("npc_level_booster") { }

        enum eActions
        {
            ActionBoostTo20 = 1,
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            p_Player->ADD_GOSSIP_ITEM(0, "Boost me to level 20!", 0, eActions::ActionBoostTo20);
            p_Player->SEND_GOSSIP_MENU(p_Creature->GetEntry(), p_Creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action) override
        {
            p_Player->CLOSE_GOSSIP_MENU();

            ///if (p_Action == eActions::ActionBoostTo20)
            {
                if (p_Player->getLevel() >= 20)
                {
                    ChatHandler(p_Player->GetSession()).SendSysMessage("You are already 20!");
                    return false;
                }

                p_Player->GiveLevel(20);

                /// Riding
                p_Player->learnSpell(33388, false); // Apprentice Riding();
                p_Player->learnSpell(42776, false); ///< Spectral Tiger

                /// Copied from command
                ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(p_Player->getClass());
                if (!classEntry)
                    return true;

                uint32 family = classEntry->spellfamily;

                for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
                {
                    SkillLineAbilityEntry const* entry = sSkillLineAbilityStore.LookupEntry(i);
                    if (!entry)
                        continue;

                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(entry->spellId);
                    if (!spellInfo)
                        continue;

                    // skip server-side/triggered spells
                    if (spellInfo->SpellLevel == 0)
                        continue;

                    // skip wrong class/race skills
                    if (!p_Player->IsSpellFitByClassAndRace(spellInfo->Id))
                        continue;

                    // skip other spell families
                    if (spellInfo->SpellFamilyName != family)
                        continue;

                    // skip spells with first rank learned as talent (and all talents then also)
                    uint32 firstRank = sSpellMgr->GetFirstSpellInChain(spellInfo->Id);
                    if (GetTalentSpellCost(firstRank) > 0)
                        continue;

                    // skip broken spells
                    if (!SpellMgr::IsSpellValid(spellInfo, p_Player, false))
                        continue;

                    if (spellInfo->SpellLevel > p_Player->getLevel())
                        continue;

                    if (std::find(g_SpellsToNotLearn.begin(), g_SpellsToNotLearn.end(), spellInfo->Id) != g_SpellsToNotLearn.end())
                        continue;

                    p_Player->learnSpell(spellInfo->Id, false);
                }

                if (p_Player->getClass() == CLASS_HUNTER)
                {
                    p_Player->learnSpell(82243, false);
                    p_Player->learnSpell(674, false);

                    for (uint32 l_Spell : { 1462, 93321, 2641, 6991, 1515, 6197, 83242 })
                        p_Player->learnSpell(l_Spell, false);
                }

                if (p_Player->getClass() == CLASS_PALADIN)
                {
                    p_Player->learnSpell(82242, false);
                }

                if (p_Player->getClass() == CLASS_ROGUE)
                {
                    p_Player->learnSpell(82245, false);
                }

                if (p_Player->getRace() == RACE_WORGEN)
                {
                    p_Player->learnSpell(68976, true);        // Aberration
                    p_Player->learnSpell(68978, true);        // Flayer
                    p_Player->learnSpell(68992, true);        // Darkflight
                    p_Player->learnSpell(68975, true);        // Viciousness
                    p_Player->learnSpell(68996, true);        // Two Forms
                }

                p_Player->StoreNewItemInBestSlots(22243, 4);

                /// Gear
                switch (p_Player->getClass())
                {
                    case CLASS_WARRIOR:
                        p_Player->StoreNewItemInBestSlots(1460, 1); ///< 2H
                        p_Player->AddItem(15210, 1);                ///< 1H
                        p_Player->AddItem(3655, 1);                 ///< Shield
                        p_Player->StoreNewItemInBestSlots(2864, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(2854, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(2274, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(2857, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(2865, 1); ///< Leggings
                        p_Player->StoreNewItemInBestSlots(1446, 1); ///< Boots
                        break;

                    case CLASS_PALADIN:
                        p_Player->StoreNewItemInBestSlots(1387, 1); ///< 2H
                        p_Player->AddItem(15210, 1);                ///< 1H
                        p_Player->AddItem(15494, 1);                ///< Shield
                        p_Player->StoreNewItemInBestSlots(2864, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(2854, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(2274, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(2857, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(2865, 1); ///< Leggings
                        p_Player->StoreNewItemInBestSlots(1446, 1); ///< Boots
                        break;

                    case CLASS_HUNTER:
                        p_Player->StoreNewItemInBestSlots(3039, 1); ///< Bow
                        p_Player->StoreNewItemInBestSlots(1461, 1); ///< 2H
                        p_Player->StoreNewItemInBestSlots(2316, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(2317, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(14561, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(6467, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(1215, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(3022, 1); ///< Legs
                        p_Player->StoreNewItemInBestSlots(2315, 1); ///< Boots
                        break;

                    case CLASS_ROGUE:
                        p_Player->StoreNewItemInBestSlots(820, 2); ///< Daggers
                        p_Player->StoreNewItemInBestSlots(2316, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(2317, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(14561, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(6467, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(1215, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(3022, 1); ///< Legs
                        p_Player->StoreNewItemInBestSlots(2315, 1); ///< Boots
                        break;

                    case CLASS_SHAMAN:

                        p_Player->AddItem(15494, 1);                ///< Shield
                        p_Player->AddItem(31269, 2);
                        p_Player->StoreNewItemInBestSlots(4437, 1); /// Stave 2H
                        p_Player->StoreNewItemInBestSlots(4244, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(4244, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(15331, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(15012, 1); ///< Boots
                        p_Player->StoreNewItemInBestSlots(2986, 1); ///< Leggings
                        p_Player->StoreNewItemInBestSlots(14560, 1); ///< Boots

                        p_Player->StoreNewItemInBestSlots(2316, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(2317, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(14561, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(6467, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(1215, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(3022, 1); ///< Legs
                        p_Player->StoreNewItemInBestSlots(2315, 1); ///< Boots
                        break;

                    case CLASS_PRIEST:
                    case CLASS_MAGE:
                    case CLASS_WARLOCK:
                        p_Player->StoreNewItemInBestSlots(4437, 1); /// Stave 2H
                        p_Player->StoreNewItemInBestSlots(2969, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(3645, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(892, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(1299, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(2970, 1); ///< Pants
                        p_Player->StoreNewItemInBestSlots(1560, 1); ///< Boots
                        break;

                    case CLASS_DRUID:
                        p_Player->StoreNewItemInBestSlots(4437, 1); /// Stave 2H
                        p_Player->StoreNewItemInBestSlots(4244, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(4244, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(15331, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(15012, 1); ///< Boots
                        p_Player->StoreNewItemInBestSlots(2986, 1); ///< Leggings
                        p_Player->StoreNewItemInBestSlots(14560, 1); ///< Boots

                        p_Player->StoreNewItemInBestSlots(2316, 1); ///< Cloak
                        p_Player->StoreNewItemInBestSlots(2317, 1); ///< Chest
                        p_Player->StoreNewItemInBestSlots(14561, 1); ///< Bracers
                        p_Player->StoreNewItemInBestSlots(6467, 1); ///< Gloves
                        p_Player->StoreNewItemInBestSlots(1215, 1); ///< Belt
                        p_Player->StoreNewItemInBestSlots(3022, 1); ///< Legs
                        p_Player->StoreNewItemInBestSlots(2315, 1); ///< Boots
                        break;
                }
            }

            p_Player->SendTalentsInfoData(false);

            if (p_Player->GetTeamId() == TEAM_ALLIANCE)
            {
                p_Player->TeleportTo(0, -8833.379883f, 628.627991f, 94.006599f, 1.065350f);

                if (p_Player->getLevel() == 1)
                    p_Player->SetHomebind(WorldLocation(0, -8833.379883f, 628.627991f, 94.006599f, 1.065350f), 5170);
            }
            else
            {
                p_Player->TeleportTo(1, 1568.300049f, -4399.930176f, 16.168900f, 0.239387f);

                if (p_Player->getLevel() == 1)
                    p_Player->SetHomebind(WorldLocation(1, 1568.300049f, -4399.930176f, 16.168900f, 0.239387f), 5170);
            }

            p_Player->ModifyMoney(500000);

            return true;
        }
};

void AddSC_npc_level_booster()
{
    new npc_level_booster();
}