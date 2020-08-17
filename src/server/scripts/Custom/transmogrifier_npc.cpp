#include "ScriptPCH.h"

// Transmog settings
int64 cost = 0; // gold cost - leave at 0 for none
bool RequireToken = false; // change false to true if you want transmog to cost a token
uint32 TokenEntry = 0; // token item id
uint32 TokenAmount = 0; // token amount
bool AllowMixedArmorTypes = true; // plate to cloth etc..
bool AllowMixedWeaponTypes = true; // sword to mace etc..
bool AllowMixedInventoryTypes = false; // 2H to 1H etc..
bool AllowAllClassTypes = false; // Warr -> Pala etc..
bool playSound = true; // play sound on transmog
uint32 soundId = 3337;

// Transmog helpers
std::string GetItemLink(uint32 entry, WorldSession* session)
{
    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
    LocaleConstant loc_idx = session->GetSessionDbLocaleIndex();
    std::string name = temp->Name1;
    if (ItemLocale const* il = sObjectMgr->GetItemLocale(entry))
        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

    std::ostringstream oss;
    oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
        "|Hitem:" << entry << ":0:0:0:0:0:0:0:0:0|h[" << name << "]|h|r";

    return oss.str();
}

const char* GetSlotName(uint8 slot, WorldSession* /*session*/)
{
	switch (slot)
	{
	case EQUIPMENT_SLOT_HEAD: return  "Head";
	case EQUIPMENT_SLOT_SHOULDERS: return  "Shoulders";
	case EQUIPMENT_SLOT_BODY: return  "Shirt";
	case EQUIPMENT_SLOT_CHEST: return  "Chest";
	case EQUIPMENT_SLOT_WAIST: return  "Waist";
	case EQUIPMENT_SLOT_LEGS: return  "Legs";
	case EQUIPMENT_SLOT_FEET: return  "Feet";
	case EQUIPMENT_SLOT_WRISTS: return  "Wrists";
	case EQUIPMENT_SLOT_HANDS: return  "Hands";
	case EQUIPMENT_SLOT_BACK: return  "Back";
	case EQUIPMENT_SLOT_MAINHAND: return  "Main hand";
	case EQUIPMENT_SLOT_OFFHAND: return  "Off hand";;
	case EQUIPMENT_SLOT_RANGED: return  "Ranged";
	case EQUIPMENT_SLOT_TABARD: return  "Tabard";
	default: return NULL;
	}
}

bool IsRangedWeapon(uint32 Class, uint32 SubClass)
{
	return Class == ITEM_CLASS_WEAPON && (
		SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
		SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
		SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW);
}

bool CanTransmogrifyItemWithItem(Player* player, ItemTemplate const* target, ItemTemplate const* source)
{
	if (!target || !source)
		return false;

	if (source->ItemId == target->ItemId)
		return false;

	if (source->DisplayInfoID == target->DisplayInfoID)
		return false;

	if (source->Class != target->Class)
		return false;

	if (source->InventoryType == INVTYPE_BAG ||
		source->InventoryType == INVTYPE_RELIC ||
		source->InventoryType == INVTYPE_FINGER ||
		source->InventoryType == INVTYPE_TRINKET ||
		source->InventoryType == INVTYPE_AMMO ||
		source->InventoryType == INVTYPE_QUIVER)
		return false;

	if (target->InventoryType == INVTYPE_BAG ||
		target->InventoryType == INVTYPE_RELIC ||
		target->InventoryType == INVTYPE_FINGER ||
		target->InventoryType == INVTYPE_TRINKET ||
		target->InventoryType == INVTYPE_AMMO ||
		target->InventoryType == INVTYPE_QUIVER)
		return false;

	if (IsRangedWeapon(source->Class, source->SubClass) != IsRangedWeapon(target->Class, target->SubClass))
		return false;

	if (source->SubClass != target->SubClass && !IsRangedWeapon(target->Class, target->SubClass))
	{
		if (source->Class == ITEM_CLASS_ARMOR && !AllowMixedArmorTypes)
			return false;
		if (source->Class == ITEM_CLASS_WEAPON && !AllowMixedWeaponTypes)
			return false;
	}

	if (source->InventoryType != target->InventoryType)
	{
		if (source->Class == ITEM_CLASS_WEAPON && !((IsRangedWeapon(target->Class, target->SubClass) ||
			AllowMixedInventoryTypes ||
			((target->InventoryType == INVTYPE_WEAPON || target->InventoryType == INVTYPE_2HWEAPON) &&
			(source->InventoryType == INVTYPE_WEAPON || source->InventoryType == INVTYPE_2HWEAPON)) ||
			((target->InventoryType == INVTYPE_WEAPONMAINHAND || target->InventoryType == INVTYPE_WEAPONOFFHAND) &&
			(source->InventoryType == INVTYPE_WEAPON || source->InventoryType == INVTYPE_2HWEAPON)))))
			return false;
		if (source->Class == ITEM_CLASS_ARMOR &&
			!((source->InventoryType == INVTYPE_CHEST || source->InventoryType == INVTYPE_ROBE) &&
			(target->InventoryType == INVTYPE_CHEST || target->InventoryType == INVTYPE_ROBE)))
			return false;
	}

	if (!AllowAllClassTypes && source->Class != ITEM_CLASS_WEAPON &&
        (source->AllowableClass != player->getClassMask()))
        return false;

	return true;
}

// Only checks bags while can use an item from anywhere in inventory
void ShowTransmogItems(Player* player, Creature* creature, uint8 slot)
{
	WorldSession* session = player->GetSession();
	Item* oldItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
	if (oldItem)
	{
		std::set<uint32> oldItemId;
		uint32 limit = 0;
		uint32 price = 0;
		std::ostringstream ss;
		ss << std::endl;
		if (RequireToken)
			ss << std::endl << std::endl << TokenAmount << " x " << GetItemLink(TokenEntry, session);

		for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
		{
			if (limit >= 25)
				break;
			Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
			if (!newItem)
				continue;
			if (!CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
				continue;
			oldItemId.insert(newItem->GetEntry());
			++limit;
		}

		for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
		{
			Bag* bag = player->GetBagByPos(i);
			if (!bag)
				continue;
			for (uint32 j = 0; j < bag->GetBagSize(); ++j)
			{
				if (limit >= 25)
					break;
				Item* newItem = player->GetItemByPos(i, j);
				if (!newItem)
					continue;
				if (!CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
					continue;
				oldItemId.insert(newItem->GetEntry());
				++limit;
			}
		}

		for (auto oldEntry : oldItemId)
		{
			Item* newItem = player->GetItemByEntry(oldEntry);
			if (!newItem)
				continue;
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, GetItemLink(newItem->GetEntry(), session), slot, newItem->GetGUIDLow(), "Using this item for transmogrify will bind it to you and make it non-refundable and non-tradeable.\nDo you wish to continue?\n\n" + GetItemLink(newItem->GetEntry(), session) + ss.str(), price, false);
		}

		oldItemId.clear();
	}

	player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "Remove transmogrification", EQUIPMENT_SLOT_END + 2, slot, "Remove transmogrification from the slot?", 0, false);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Update menu", EQUIPMENT_SLOT_END, slot);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "<- Back", EQUIPMENT_SLOT_END + 1, 0);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
}

void UpdateItem(Player* player, Item* item)
{
    if (item->IsEquipped())
    {
        player->SetVisibleItemSlot(item->GetSlot(), item);
		if (player->IsInWorld())
		{
			item->SendUpdateToPlayer(player);
			if (playSound)
				player->PlayDirectSound(soundId);
		}
    }
}

void DeleteFakeEntry(Player* player, Item* item)
{
	if (player->transmogMap.erase(item->GetGUID()) != 0)
		UpdateItem(player, item);
}

void SetFakeEntry(Player* player, Item* item, uint32 entry)
{
	player->transmogMap[item->GetGUID()] = entry;
	UpdateItem(player, item);
}

std::string Transmogrify(Player* player, uint64 itemGUID, uint8 slot, bool no_cost)
{
	// slot of the transmogrified item
	if (slot >= EQUIPMENT_SLOT_END)
		return "";

	Item* itemTransmogrifier = NULL;
	// guid of the transmogrifier item, if it's not 0
	if (itemGUID)
	{
		itemTransmogrifier = player->GetItemByGuid(itemGUID);
		if (!itemTransmogrifier)
			return "Missing source item";
	}

	// transmogrified item
	Item* itemTransmogrified = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
	if (!itemTransmogrified)
		return "Missing destination item";

	if (!itemTransmogrifier) // reset look newEntry
	{
		itemTransmogrified->SetState(ITEM_CHANGED, player);
		UpdateItem(player, itemTransmogrified);
	}
	else
	{
		if (!CanTransmogrifyItemWithItem(player, itemTransmogrified->GetTemplate(), itemTransmogrifier->GetTemplate()))
			return "Invalid transmog items";

		if (!no_cost)
		{
			if (RequireToken)
			{
				if (player->HasItemCount(TokenEntry, TokenAmount))
					player->DestroyItemCount(TokenEntry, TokenAmount, true);
				else
					return "Not enough tokens";
			}

			if (cost) // 0 cost if reverting look
			{
				if (cost < 0)
					;
				else
				{
					if (!player->HasEnoughMoney(cost))
						return "Not enough money!";
					player->ModifyMoney(-cost, false);
				}
			}
		}

		SetFakeEntry(player, itemTransmogrified, itemTransmogrifier->GetEntry());

		itemTransmogrified->UpdatePlayedTime(player);

		itemTransmogrified->SetOwnerGUID(player->GetGUID());
		itemTransmogrified->SetNotRefundable(player);
		itemTransmogrified->ClearSoulboundTradeable(player);

		if (itemTransmogrifier->GetTemplate()->Bonding == BIND_WHEN_EQUIPED || itemTransmogrifier->GetTemplate()->Bonding == BIND_WHEN_PICKED_UP || itemTransmogrifier->GetTemplate()->Bonding == BIND_QUEST_ITEM || itemTransmogrifier->GetTemplate()->Bonding == BIND_WHEN_USE)
			itemTransmogrifier->SetBinding(true);

		itemTransmogrifier->SetOwnerGUID(player->GetGUID());
		itemTransmogrifier->SetNotRefundable(player);
		itemTransmogrifier->ClearSoulboundTradeable(player);

		itemTransmogrifier->SetState(ITEM_CHANGED, player);
		itemTransmogrified->SetState(ITEM_CHANGED, player);
		UpdateItem(player, itemTransmogrified);
	}

	return "Success!";
}

class Transmogrifier_NPC : public CreatureScript
{
public:
	Transmogrifier_NPC() : CreatureScript("Transmogrifier_NPC") { }

	bool OnGossipHello(Player *player, Creature * m_creature)
	{
		WorldSession* session = player->GetSession();
		for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
			if (const char* slotName = GetSlotName(slot, session))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, std::string(slotName), EQUIPMENT_SLOT_END, slot);

		player->SEND_GOSSIP_MENU(1, m_creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		WorldSession* session = player->GetSession();

		switch (sender)
		{
			case EQUIPMENT_SLOT_END: // Show items you can use
			{
				ShowTransmogItems(player, m_creature, action);
				break;
			}
			case EQUIPMENT_SLOT_END + 1:
			{
				OnGossipHello(player, m_creature);
				break;
			}
			case EQUIPMENT_SLOT_END + 2:
			{
				if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
				{
					uint32 fakeEntry = 0;
					if (!player->transmogMap.empty())
					{
						TransmogMapType::const_iterator it = player->transmogMap.find(newItem->GetGUID());
						if (it != player->transmogMap.end())
							fakeEntry = it->second;
					}
					if (fakeEntry)
					{
						DeleteFakeEntry(player, newItem);
						session->SendAreaTriggerMessage("Success!");
					}
					else
						session->SendNotification("Transmog not found!");
				}

				OnGossipHello(player, m_creature);
				break;
			}
			default: // Transmogrify
			{
				if (!sender && !action)
				{
					OnGossipHello(player, m_creature);
					return true;
				}
				// sender = slot, action = display
				std::string res = Transmogrify(player, MAKE_NEW_GUID(action, 0, HIGHGUID_ITEM), sender, RequireToken ? false : true);
				if (res == "Success!")
					session->SendAreaTriggerMessage("Success!");
				else
					session->SendNotification(res.c_str());

				OnGossipHello(player, m_creature);
				break;
			}
		}
		return true;
	}
};

class PS_Transmogrification : public PlayerScript
{
public:
	PS_Transmogrification() : PlayerScript("PS_Transmogrification") { }

	void OnSave(Player* player)
	{
		uint32 lowguid = player->GetGUIDLow();
		SQLTransaction trans = CharacterDatabase.BeginTransaction();
		trans->PAppend("DELETE FROM `custom_transmogrification` WHERE `Owner` = %u", lowguid);

		if (!player->transmogMap.empty())
		{
			// Only save items that are in inventory / bank / etc
			std::vector<uint64> items = player->GetTransmogItemList(player);
			for (std::vector<uint64>::const_iterator it = items.begin(); it != items.end(); ++it)
			{
				TransmogMapType::const_iterator it2 = player->transmogMap.find(*it);
				if (it2 == player->transmogMap.end())
					continue;

				trans->PAppend("REPLACE INTO custom_transmogrification (GUID, FakeEntry, Owner) VALUES (%u, %u, %u)", it2->first, it2->second, lowguid);
			}
		}

		if (trans->GetSize()) // basically never false
			CharacterDatabase.CommitTransaction(trans);
	}

	void OnLogin(Player* player)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT GUID, FakeEntry FROM custom_transmogrification WHERE Owner = %u", player->GetGUIDLow());

		if (result)
		{
			do
			{
				Field* field = result->Fetch();
				uint64 itemGUID = MAKE_NEW_GUID(field[0].GetUInt32(), 0, HIGHGUID_ITEM);
				uint32 fakeEntry = field[1].GetUInt32();
				// Only load items that are in inventory / bank / etc
				if ((sObjectMgr->GetItemTemplate(fakeEntry)) && player->GetItemByGuid(itemGUID))
				{
					player->transmogMap[itemGUID] = fakeEntry;
				}
			} while (result->NextRow());

			if (!player->transmogMap.empty())
			{
				for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
				{
					if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
					{
						player->SetVisibleItemSlot(slot, item);
						if (player->IsInWorld())
							item->SendUpdateToPlayer(player);
					}
				}
			}
		}
	}
};

class WS_Transmogrification : public WorldScript
{
public:
	WS_Transmogrification() : WorldScript("WS_Transmogrification") { }

	void OnStartup() override
	{
		CharacterDatabase.DirectExecute("DELETE FROM custom_transmogrification WHERE NOT EXISTS (SELECT 1 FROM item_instance WHERE item_instance.guid = custom_transmogrification.GUID)");
	}
};

void AddSC_Transmogrifier_NPC()
{
	new Transmogrifier_NPC(); // npc script transmog
	new WS_Transmogrification(); // world script transmog
	new PS_Transmogrification(); // player script transmog
}