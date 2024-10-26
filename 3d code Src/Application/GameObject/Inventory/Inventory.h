#pragma once

using namespace std;
using namespace DirectX::SimpleMath;


struct UiElement
{
	std::shared_ptr<KdTexture> tex;
	Math::Rectangle rect;
	Vector2 pos;
	Vector2 scale;
	Color color = { 1.0f,1.0f,1.0f,1.0f };
};

class Inventory : public KdGameObject
{
public:


	void Init() override;
	void Update() override;
	void DrawSprite() override;
	void DrawMoji() override;

	void DrawUi(const UiElement& ui);

	string itemDescription = "Item Description";



private:

	Math::Rectangle inventoryFrameRect = { 0,0,1280,720 };
	Vector2 inventoryFramePos = { 640,360 };
	Vector2 inventoryFrameScale = { 1.0f,1.0f };
	string inventoryFrameTexName = "Data/Texture/Inventory/InventoryFrame.png";
	Color inventoryFrameColor = { 1.0f,1.0f,1.0f,1.0f };

	UiElement inventoryFrame {make_shared<KdTexture>(inventoryFrameTexName), inventoryFrameRect, inventoryFramePos, inventoryFrameScale,inventoryFrameColor};





private:
	Inventory() {};
	~Inventory() override{}
public:
	static Inventory& Instance()
	{
		static Inventory instance;
		return instance;
	}

};

