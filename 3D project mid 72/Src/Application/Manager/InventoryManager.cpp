#include"InventoryManager.h"
#include"Application/GameObject/Item/Item.h"
#include"Application/Scene/SceneManager.h"
#include"Application/Manager/InputManager.h"
#include"Application/GameObject/Player/Player.h"

#include"Application/Manager/UiManager.h"

void InventoryManager::Init()
{
	tex = make_shared<KdTexture>();
	tex->Load("Asset/Textures/background_01.png");



}

void InventoryManager::Update()
{
	if (!isGetAllItem) {
		isGetAllItem = true;
		GetAllItemFromitemSpawnListAtBegin();
	}


	if (!GameManager::Instance().GetIsInventoryOpen()) return;

	DoSpawnItem();

	InventoryMenu();


	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::EnterKey)) {
		UseSelectedItem(filteredInventory);
	}

	HandleItemStatus();


}

void InventoryManager::DrawSprite()
{

	KdShaderManager::Instance().m_spriteShader.DrawTex(tex, 0, 0);

}

void InventoryManager::DrawMoji()
{
	ShowInventory();
}


void InventoryManager::LoadItemsFromJson(const std::string& filename)
{

	std::ifstream file(filename);
	json jsonData;
	file >> jsonData;

	for (auto& itemData : jsonData["items"])
	{
		std::shared_ptr<Item> item = std::make_shared<Item>();

		// General properties
		item->modelPath = itemData["modelPath"].get<std::string>();
		item->itemName = itemData["itemName"].get<std::string>();
		item->itemDescription = itemData["itemDescription"].get<std::string>();
		item->descriptionSlot1 = itemData["descriptionSlot1"].get<std::string>();
		item->descriptionSlot2 = itemData["descriptionSlot2"].get<std::string>();
		item->descriptionSlot3 = itemData["descriptionSlot3"].get<std::string>();
		item->itemScale = Vector3(itemData["itemScale"][0], itemData["itemScale"][1], itemData["itemScale"][2]);
		item->rarity = itemData["rarity"].get<float>();

		std::string itemType = itemData["type"].get<std::string>();

		if (itemType == "potion")
		{
			item->healAmount = itemData["healAmount"].get<float>();
			item->manaAmount = itemData["manaAmount"].get<float>();
			item->atkUpAmount = itemData["atkUpAmount"].get<float>();
			item->potionId = itemData["potionId"].get<int>();
			item->SetType(ItemType::PotionItem);
		}
		else if (itemType == "weapon")
		{
			float randomValue = rand() % 5 - 2;
			item->atkPow = itemData["atkPow"].get<float>() + randomValue;
			item->atkSpd = itemData["atkSpd"].get<float>();
			item->skillSet = itemData["skillSet"].get<int>();
			item->comboSet = itemData["comboSet"].get<int>();
			item->weaponId = itemData["weaponId"].get<int>();
			item->SetType(ItemType::WeaponItem);
		}
		else if (itemType == "armor")
		{
			item->defUp = itemData["defUp"].get<float>();
			item->atkUp = itemData["atkUp"].get<float>();
			item->hpUp = itemData["hpUp"].get<float>();
			item->manaUp = itemData["manaUp"].get<float>();
			item->staminaUp = itemData["staminaUp"].get<float>();
			item->armorId = itemData["armorId"].get<int>();
			item->SetType(ItemType::ArmorItem);
		}

		itemSpawnList.push_back(item);
	}

}

void InventoryManager::SpawnItemByType(int type,Vector3 _spawnPos)
{

	std::vector<std::shared_ptr<Item>> filteredItems;

	for (const auto& item : itemSpawnList)
	{
		if (item->GetType() == type)
		{
			filteredItems.push_back(item);
		}
	}

	if (!filteredItems.empty())
	{
		float totalRarity = 0;
		for (const auto& item : filteredItems)
		{
			totalRarity += item->GetRarity();
		}

		float randomValue = Utility::Rnd(0, totalRarity);
		float raritySum = 0;

		for (const auto& item : filteredItems)
		{
			raritySum += item->GetRarity();
			if (randomValue <= raritySum)
			{
				item->SetPosition(_spawnPos);
				item->Init();
				SceneManager::Instance().AddObject(item);
				break;
			}
		}
	}


}

void InventoryManager::RandomSpawnItem(Vector3 _spawnPos)
{

	float randomValue = Utility::Rnd(0, 100);

	if (randomValue < 20) 
	{
		SpawnItemByType(ItemType::PotionItem, _spawnPos);
	}
	else if (randomValue < 70)
	{
		SpawnItemByType(ItemType::WeaponItem, _spawnPos + Vector3(1.4,0,-1));
	}
	else 
	{
		SpawnItemByType(ItemType::ArmorItem, _spawnPos);
	}

}

void InventoryManager::DoSpawnItem()
{

	//spawnCd--;

	//if (InputManager::Instance().IsKeyJustPressed(GKey)) {
	//	itemSpawnList.clear();
	//	LoadItemsFromJson("Data/itemList JP.json");

	//	//RandomSpawnItem();
	//}

}

void InventoryManager::DoRandomSpawnItem(Vector3 _spawnPos)
{
	itemSpawnList.clear();
	LoadItemsFromJson("Data/itemList JP.json");

	RandomSpawnItem(_spawnPos);
}

void InventoryManager::HandleItemStatus()
{
	//loop through all potion item , if quantity is 0 remove from inventory
	for (auto it = inventory.begin(); it != inventory.end();)
	{
		if ((*it)->quantity <= 0)
		{
			it = inventory.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void InventoryManager::AddItem(std::shared_ptr<Item> item)
{
	/*inventory.push_back(item); 
	item->SetExpired();*/

	if (item->isPickUp == false) {
		item->isPickUp = true;
		inventory.push_back(item);
	}

}

void InventoryManager::ShowInventory()
{
	
	const int windowSize = 5;  // Number of items to show at a time

	// Ensure the selected item is always visible
	int startIndex = selectedItemIndex - (windowSize / 2);
	if (startIndex < 0) {
		startIndex = 0;  // Don't go below the first item
	}

	// Ensure we don't go beyond the number of available items
	int endIndex = startIndex + windowSize;
	if (endIndex > filteredInventory.size()) {
		endIndex = filteredInventory.size();  // Don't go past the end of the inventory
		startIndex = max(0, endIndex - windowSize);  // Adjust startIndex if we are near the end
	}

	// Draw the items within the window
	for (int i = startIndex; i < endIndex; i++) {
		auto item = filteredInventory[i];
		Vector2 itemPos(-560, (i - startIndex) * 35 + 7);  // Adjust y position based on visible items

		// Highlight the selected item
		//std::string displayText = (i == selectedItemIndex ? "> " : "  ") + item->itemName + " x " + std::to_string(item->quantity) + (item->isEquip ? "E" : "  ");
		//std::string displayText = item->itemName + "x" + std::to_string(item->quantity) ;
		std::string displayText = item->itemName;
		KdShaderManager::Instance().m_spriteShader.DrawString(itemPos.x, itemPos.y, displayText.c_str(), { 1, 1, 1, 1 }, 0.567);
		string equipText = item->isEquip ? "E" : "  ";
		KdShaderManager::Instance().m_spriteShader.DrawString(itemPos.x + 242, itemPos.y, equipText.c_str(), { 7,0,0, 1 }, 0.7,1);

		if (i == selectedItemIndex) seleteditemPos = itemPos;

		//show rarity
		/*switch (item->rareType)
		{
		case ItemRarity::Normal:
			KdShaderManager::Instance().m_spriteShader.DrawString(itemPos.x-35, itemPos.y, "Normal", kWhiteColor);
			break;
		case ItemRarity::Rare:
			KdShaderManager::Instance().m_spriteShader.DrawString(itemPos.x - 35, itemPos.y, "Rare", kPurpleColor);
			break;
		case ItemRarity::Legendary:
			KdShaderManager::Instance().m_spriteShader.DrawString(itemPos.x - 35, itemPos.y, "Epic", kYellowColor);
			break;

		default:
			break;
		}*/

		float descSlotSpace = 49.0f;
		//show des and des slot 123
		if (i == selectedItemIndex) {
			KdShaderManager::Instance().m_spriteShader.DrawString(descPos.x, descPos.y, item->itemDescription.c_str(), kWhiteColor, 0.56);
			string desc = item->descriptionSlot1.c_str() + std::to_string((int)item->atkPow);
			if (item->GetType() == ItemType::WeaponItem) KdShaderManager::Instance().m_spriteShader.DrawString(descPos.x, descPos.y - descSlotSpace * 1, desc.c_str(), kWhiteColor, 0.7);
			else KdShaderManager::Instance().m_spriteShader.DrawString(descPos.x, descPos.y- descSlotSpace * 1, item->descriptionSlot1.c_str(), kWhiteColor, 0.7);
			KdShaderManager::Instance().m_spriteShader.DrawString(descPos.x, descPos.y - descSlotSpace * 2, item->descriptionSlot2.c_str(), kWhiteColor, 0.7);
			KdShaderManager::Instance().m_spriteShader.DrawString(descPos.x, descPos.y - descSlotSpace * 3, item->descriptionSlot3.c_str(), kWhiteColor, 0.7);

		}



		float space = 77;
		KdShaderManager::Instance().m_spriteShader.DrawString(invenMenuPos.x, invenMenuPos.y, "全部", { 1,1,1,1 }, 0.49);
		KdShaderManager::Instance().m_spriteShader.DrawString(invenMenuPos.x + space, invenMenuPos.y, "武器", { 1,1,1,1 }, 0.49);
		KdShaderManager::Instance().m_spriteShader.DrawString(invenMenuPos.x + space * 2, invenMenuPos.y, "防具", { 1,1,1,1 }, 0.49);
		KdShaderManager::Instance().m_spriteShader.DrawString(invenMenuPos.x + space * 3, invenMenuPos.y, "消耗品", { 1,1,1,1 }, 0.49);
		

	}



}

void InventoryManager::ScrollInventory(int direction, const std::vector<std::shared_ptr<Item>>& filteredInventory)
{

	if (filteredInventory.empty()) return;

	if (direction == -1) {
		if (selectedItemIndex > 0)	  selectedItemIndex += direction;

	}
	else if (direction == 1) {
		if (selectedItemIndex < filteredInventory.size() - 1)selectedItemIndex += direction;
	}


}

void InventoryManager::UseSelectedItem(std::vector<std::shared_ptr<Item>>& filteredInventory)
{
	if (filteredInventory.empty()) return;

	auto selectedItem = filteredInventory[selectedItemIndex];
	auto player = wpPlayer.lock();
	if (!player) return; 

	if (selectedItem->canConsume)
	{
		KdAudioManager::Instance().PlayUseItemSE();
		player->UsePotion(selectedItem->potionId, *selectedItem);
		/*player->Sethp(player->Gethp() + selectedItem->healAmount);
		if (player->Gethp() > player->GetmaxHp()) player->Sethp(player->GetmaxHp());*/

		selectedItem->quantity--;

		if (selectedItem->quantity <= 0)
		{
			auto it = std::find(inventory.begin(), inventory.end(), selectedItem);
			if (it != inventory.end())
			{
				inventory.erase(it);
			}

			if (selectedItemIndex >= filteredInventory.size())
			{
				selectedItemIndex = filteredInventory.size() - 1;
			}
		}
	}

	else if (selectedItem->canEquip)
	{
		if (!selectedItem->isEquip)
		{
			KdAudioManager::Instance().PlayEquipItemSE();

			if (selectedItem->GetType() == ItemType::WeaponItem) {
				auto it = std::find(inventory.begin(), inventory.end(), selectedItem);
				if (it != inventory.end())
				{
					//set all weapon to not equip
					for (auto& item : inventory)
					{
						if (item->GetType() == ItemType::WeaponItem)
						{
							item->isEquip = false;
						}
					}
					(*it)->isEquip = true;
					player->ChangeWeapon(selectedItem->weaponId, selectedItem->skillSet, selectedItem->modelPath, selectedItem->atkPow);
				}
			}
			else if (selectedItem->GetType() == ItemType::ArmorItem) {
				auto it = std::find(inventory.begin(), inventory.end(), selectedItem);
				if (it != inventory.end())
				{
					//set all weapon to not equip
					for (auto& item : inventory)
					{
						if (item->GetType() == ItemType::ArmorItem)
						{
							item->isEquip = false;
						}
					}
					(*it)->isEquip = true;
					player->ChangeArmor(selectedItem->armorId, selectedItem->modelPath, selectedItem->defUp);
				}
			}
			
		}
		else {
			KdAudioManager::Instance().PlayUiDenieSE();
		}
		
	}

}

void InventoryManager::FilterInventory()
{

	filteredInventory.clear();

	for (auto& item : inventory) {
		switch (currentViewMode) {
		case FullInventory:
			filteredInventory.push_back(item);
			break;
		case PotionInventory:
			if (item->GetType() == ItemType::PotionItem) filteredInventory.push_back(item);
			break;
		case WeaponInventory:
			if (item->GetType() == ItemType::WeaponItem) filteredInventory.push_back(item);
			break;
		case ArmorInventory:
			if (item->GetType() == ItemType::ArmorItem) filteredInventory.push_back(item);
			break;
		}
	}

	if (currentViewMode == FullInventory) {
		std::sort(filteredInventory.begin(), filteredInventory.end(), [](const std::shared_ptr<Item>& a, const std::shared_ptr<Item>& b) {
			return a->GetType() < b->GetType();
			});
	}

}

void InventoryManager::InventoryMenu()
{
	OpenInventory();
	FilterInventory();  

	addcd--;

	if (selectedItemIndex >= filteredInventory.size()) {
		selectedItemIndex = filteredInventory.size() - 1;
	}

	if (GetAsyncKeyState('W') & 0x8000 && addcd < 0) {
		addcd = 14;
		ScrollInventory(1, filteredInventory);
		KdAudioManager::Instance().PlayUiHoverSE();
	}
	else if (GetAsyncKeyState('S') & 0x8000 && addcd < 0) {
		addcd = 14;
		ScrollInventory(-1, filteredInventory);
		KdAudioManager::Instance().PlayUiHoverSE();
	}




}

void InventoryManager::OpenInventory()
{
	InventoryViewMode previousViewMode = currentViewMode; 

	//currentViewMode = FullInventory;  

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::RightKey)) {
		if (currentViewMode == FullInventory) currentViewMode = WeaponInventory;
		else if (currentViewMode == PotionInventory) currentViewMode = FullInventory;
		else if (currentViewMode == WeaponInventory) currentViewMode = ArmorInventory;
		else if (currentViewMode == ArmorInventory) currentViewMode = PotionInventory;
		KdAudioManager::Instance().PlayUiHoverSE();

	}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::LeftKey)) {
		if (currentViewMode == FullInventory) currentViewMode = PotionInventory;
		else if (currentViewMode == PotionInventory) currentViewMode = ArmorInventory;
		else if (currentViewMode == WeaponInventory) currentViewMode = FullInventory;
		else if (currentViewMode == ArmorInventory) currentViewMode = WeaponInventory;
		KdAudioManager::Instance().PlayUiHoverSE();

	}

	if (previousViewMode != currentViewMode) {
		//selectedItemIndex = 0;  
		selectedItemIndex = filteredInventory.size() - 1;
	}


	switch (currentViewMode)
	{	
	case FullInventory:
		ItemMenuFramePos = { -579,166 };
		break;
	case PotionInventory:
		ItemMenuFramePos = { -579 + 80 * 3,166 };
		break;
	case WeaponInventory:
		ItemMenuFramePos = { -579 +80 * 1,166 };
		break;
	case ArmorInventory:
		ItemMenuFramePos = { -579 + 80 * 2,166 };
		break;
	default:
		break;
	}


}

void InventoryManager::GetAllItemFromitemSpawnListAtBegin()
{
	itemSpawnList.clear();
	LoadItemsFromJson("Data/itemList JP.json");

	for (auto& item : itemSpawnList)
	{
		// Initialize the item if necessary
		item->Init();

		// Add the item to the inventory
		AddItem(item);
	}

	itemSpawnList.clear();

}

void InventoryManager::CallImgui()
{
	ImGui::Begin("Inventory");

	//sldier float invenMenuPos x y
	ImGui::SliderFloat("invenMenuPosX", &invenMenuPos.x, -700, 700);
	ImGui::SliderFloat("invenMenuPosY", &invenMenuPos.y, -420, 420);


	//sldier decsPos xy
	ImGui::SliderFloat("decsPos x", &descPos.x, -1280, 1280);
	ImGui::SliderFloat("decsPos y", &descPos.y, -720, 720);

	for (int i = 0; i < filteredInventory.size(); i++)
	{
		auto item = filteredInventory[i];

		if (i == selectedItemIndex)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "> %s", item->itemName.c_str());
		}
		else
		{
			ImGui::Text("%s", item->itemName.c_str());
		}

		ImGui::Text("Description: %s", item->itemDescription.c_str());
		ImGui::Text("Quantity: %d", item->quantity);
		ImGui::Text("Rarity: %.1f", item->rarity);

		ImGui::Checkbox("Can Equip", &item->canEquip);
		ImGui::Checkbox("Can Consume", &item->canConsume);
		ImGui::Checkbox("Is Stackable", &item->isStackable);
		ImGui::Checkbox("Is Deletable", &item->isDeletable);
		ImGui::Checkbox("Is Equipped", &item->isEquip);

		//show descriptionSlot1
		ImGui::Text("Description Slot 1: %s", item->descriptionSlot1.c_str());

		ImGui::Text("Item Type: %d", item->GetType());

		if (item->canConsume)
		{
			ImGui::Text("Heal Amount: %.1f", item->healAmount);
			ImGui::Text("Mana Amount: %.1f", item->manaAmount);
		}

		if (item->GetType() == ItemType::WeaponItem)
		{
			ImGui::Text("Attack Power: %.1f", item->atkPow);
			ImGui::Text("Attack Speed: %.1f", item->atkSpd);
			ImGui::Text("Weapon ID: %d", item->weaponId);
		}

		if (item->GetType() == ItemType::ArmorItem)
		{
			ImGui::Text("Defense Up: %.1f", item->defUp);
			ImGui::Text("HP Up: %.1f", item->hpUp);
			ImGui::Text("Armor ID: %d", item->armorId);
		}

		ImGui::Separator();
	}

	ImGui::End();
}