//InventoryManager.h file:
#pragma once
#include"json.hpp"

using json = nlohmann::json;
using namespace std;
using namespace DirectX::SimpleMath;

class Item;
class Player;

class InventoryManager : public KdGameObject
{
public:

	//get selected itemPos
	Vector2 GetSelectedItemPos() { return seleteditemPos; }

	//get ItemMenuFramePos
	Vector2 GetItemMenuFramePos() { return ItemMenuFramePos; }

	//set player	
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	void Init() override;
	void Update() override;

	void DrawSprite() override;

	void DrawMoji() override;

	void CallImgui() override;

	void LoadItemsFromJson(const std::string& filename);
	void SpawnItemByType(int type, Vector3 _spawnPos);
	void RandomSpawnItem(Vector3 _spawnPos);

	void DoRandomSpawnItem(Vector3 _spawnPos);

	void HandleItemStatus();

	void AddItem(std::shared_ptr<Item> item);

	void ShowInventory();
	void ScrollInventory(int direction, const std::vector<std::shared_ptr<Item>>& filteredInventory);
	void UseSelectedItem(std::vector<std::shared_ptr<Item>>& filteredInventory);

	void FilterInventory();

	void InventoryMenu();

	void OpenInventory();

	void GetAllItemFromitemSpawnListAtBegin();







	std::vector<std::shared_ptr<Item>> itemSpawnList; //Item SpawnList

	std::vector<std::shared_ptr<Item>> inventory;
	std::vector<std::shared_ptr<Item>> filteredInventory;

	int selectedItemIndex = 0;
	
	int addcd = 60;


	InventoryViewMode currentViewMode = FullInventory;


	bool isGetAllItem = false;


private:


	//weak_ptr player
	weak_ptr<Player> wpPlayer;

	shared_ptr<KdTexture> tex;
	Math::Rectangle rect = { 0,0,1280,720 };

	Vector2 seleteditemPos = {};

	Vector2 descPos = { 270,184 };

	Vector2 invenMenuPos = { -576,183 };

	Vector2 ItemMenuFramePos = { 0,0 };


private:
	InventoryManager() {}
	~InventoryManager() override {}

public:
	static InventoryManager& Instance()
	{
		static InventoryManager instance;
		return instance;
	}
};

