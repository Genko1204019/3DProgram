//Item.h file:

#pragma once

class Item : public KdGameObject
{
public:
	Item() {}
	~Item() override {}

	//get set rarity
	float GetRarity() { return rarity; }
	void SetRarity(float _rarity) { rarity = _rarity; }

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawBright() override;

	void DrawMoji() override;

	void ItemGetPickUP();

	void CallImgui() override;

	bool canEquip = false;
	bool canConsume = false;
	bool isStackable = false;
	bool isDeletable = true;
	bool isEquip = false;

	string modelPath = "Data/ModelData/Item/Item.gltf";
	string itemName = "No Name";
	string itemDescription = "No Description";
	string descriptionSlot1 = "";
	string descriptionSlot2 = "";
	string descriptionSlot3 = "";

	Vector3 itemScale = { 1,1,1 };


	//for potion Item
	float healAmount = 10.0f;
	float manaAmount = 0.0f;
	float atkUpAmount = 0.0f;
	float defUpAmount = 0.0f;
	int potionId = 0;

	//for weapon Item
	float atkPow = 1;
	float atkSpd = 1;
	int skillSet = 1;
	int comboSet = 1;
	int weaponId = 0;

	//for armor Item
	float defUp = 1;
	float atkUp = 1;
	float hpUp = 1;
	float manaUp = 1;
	float staminaUp = 1;
	float strikeUp = 1;
	int armorId = 0;

	Vector2 itemPos = { 0,0 };
	Vector2 itemPosScroll = { 0,0 };

	int quantity = 1;
	int potionType = 0;

	float rarity = 1;
	int rareType = ItemRarity::Normal;

	int auraEffId = 0;
	bool isEff = false;

	bool isPickUp = false;


private:

	shared_ptr<KdModelWork>  model = nullptr;
	
	float rotAngle = 0.0f;
	



};

