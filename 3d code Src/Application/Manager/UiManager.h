#pragma once
#include"json.hpp"
#include"UiElement.h"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;


class Player;

enum UiLayer {
	LowBackground ,
	Background ,
	TopBackground,
	Middle,
	Font,
	TopFont,
	TopMost,
};

enum UiTag
{
	noTags = 0,
	FixUi,
	HpBar,
	MpBar,
	StaminaBar,
	StrikeBar,
	StartScene,
	EndScene,
	Inventory,
	BloodEffect,
	DialogFrame,
	DialogArrow,
	CutScene,
	InventorySelectSlot,
	MessageBoxFrame,
	InventoryBackGround,
	InventoryMarker,
	StaminaFrame,
	HpBuffer,
	StageProgressBar,
	itemMenuFrame,
	StageProgressFrame,
	BattleSlogan,
	WinSlogan,
	MiniMapBack,
	PlayerMapIcon,
	GameTitle,
};

class UiManager : public KdGameObject {
public:

	//set player
	void SetPlayer(const std::shared_ptr<Player>& player) {wpPlayer = player;}

	void Init() override;
	void Update() override;
	void DrawSprite() override;
	void DrawMoji() override;

	void DrawRenderTarget() override;

	void DrawInventory();

	void DrawMiniMap();

	void SaveUiSettings(const std::string& filePath);
	void LoadUiSettings(const std::string& filePath);
	void AddUiElement(const std::string& name, const std::string& texPath);

	void HandleStatusBars();

	void StartBloodScreen();

	void ControlDialogFrame();
	void OpenDialogFrame();
	void CloseDialogFrame();

	void ControlInvetoryRelated();

	void ControlMessageBox();
	

	void DrawUi(UiElement& _ui);

	void DrawUiTitle(UiElement& _ui);

	void DrawUiMiniMap(UiElement& _ui);


	void CallImgui() override;

	
	bool showBattleStart = false;
	bool showBattleWin = false;
	float bloodAlpha = 0;



private:

	const std::array<UiLayer, 7> DrawOrder = {
	UiLayer::LowBackground,
	UiLayer::Background,
	UiLayer::TopBackground,
	UiLayer::Middle,
	UiLayer::Font,
	UiLayer::TopFont,
	UiLayer::TopMost
	};



	KdTexture miniTex;

	std::vector<UiElement> uiElements;

	weak_ptr<Player> wpPlayer;

	string settingPath = "Data/UiSetting.json";

	bool isCutScene = false;
	bool isDialog = false;

	bool hasUi = true;

	float dialogFrameAlpha = 0;
	bool isDialogOpen = false;

	Vector3 originPos;
	bool isOriginSet = false;

	float messageBoxAlpha = 0;


	Vector2 invenslotOffset = { -30,-10.3 };
	Vector2 invenMarkerOffset = { -21,0 };


	bool isTypingMode = false;



	UiManager() {}
	~UiManager() override {}
public:
	static UiManager& Instance() {
		static UiManager instance;
		return instance;
	}
};

