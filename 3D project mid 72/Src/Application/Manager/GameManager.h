#pragma once
#include"json.hpp"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;

class Player;
class GameCamera;

class GameManager : public KdGameObject
{
public:

	//get set fadeAlpha 
	void SetFadeAlpha(float _fadeAlpha) { fadeAlpha = _fadeAlpha; }
	float GetFadeAlpha() { return fadeAlpha; }

	//get set canWandererDie
	void SetCanWandererDie(bool _canWandererDie) { canWandererDie = _canWandererDie; }
	bool GetCanWandererDie() { return canWandererDie; }

	//get set stageProgress
	void SetStageProgress(float _stageProgress) { stageProgress = _stageProgress; }
	float GetStageProgress() { return stageProgress; }

	//stageProgressMax
	void SetStageProgressMax(float _stageProgressMax) { stageProgressMax = _stageProgressMax; }
	float GetStageProgressMax() { return stageProgressMax; }
	
	//setter getter for all bool type
	 void SetIsBattleMode(bool _isBattleMode) { isBattleMode = _isBattleMode; }
	 bool GetIsBattleMode() { return isBattleMode; }

	 void SetCanNextFloor(bool _canNextFloor) { canNextFloor = _canNextFloor; }
	 bool GetCanNextFloor() { return canNextFloor; }

	 void SetCanLoadMapEnemy(bool _canLoadMapEnemy) { canLoadMapEnemy = _canLoadMapEnemy; }
	 bool GetCanLoadMapEnemy() { return canLoadMapEnemy; }

	 void SetCanLoadBattleEnemy(bool _canLoadBattleEnemy) { canLoadBattleEnemy = _canLoadBattleEnemy; }
	 bool GetCanLoadBattleEnemy() { return canLoadBattleEnemy; }

	 void SetIsMiniMap(bool _isMiniMap) { isMiniMap = _isMiniMap; }
	 bool GetIsMiniMap() { return isMiniMap; }

	 void SetIsGameStart(bool _isGameStart) { isGameStart = _isGameStart; }
	 bool GetIsGameStart() { return isGameStart; }

	 bool GetCanDebugWire() { return canDebugWire; }


	 void SetCanFade(bool _canFade) { canFade = _canFade; }
	 bool GetCanFade() { return canFade; }

	 void SetCanFadeFloor(bool _canFadeFloor) { canFadeFloor = _canFadeFloor; }
	 bool GetCanFadeFloor() { return canFadeFloor; }

	 //get set enemyUniqueId
	 void SetEnemyUniqueId(int _enemyUniqueId) { enemyUniqueId = _enemyUniqueId; }
	 int GetEnemyUniqueId() { return enemyUniqueId; }

	 //battleEnemyNum
	 void SetBattleEnemyNum(int _battleEnemyNum) { battleEnemyNum = _battleEnemyNum; }
	 int GetBattleEnemyNum() { return battleEnemyNum; }

	 //get set floorNum
	 void SetFloorNum(int _floorNum) { floorNum = _floorNum; }
	 int GetFloorNum() { return floorNum; }

	 //get set isGameStop
	  void SetIsGameStop(bool _isGameStop) { isGameStop = _isGameStop; }
	  bool GetIsGameStop() { return isGameStop; }


	  //get set polyScale
	  void SetPolyScale(Vector3 _polyScale) { polyScale = _polyScale; }
	  Vector3 GetPolyScale() { return polyScale; }

	  //get set isStrikeSkill
	  void SetIsStrikeSkill(bool _isStrikeSkill) { isStrikeSkill = _isStrikeSkill; }
	  bool GetIsStrikeSkill() { return isStrikeSkill; }

	  //get set canLoadBoss
	  void SetCanLoadBoss(bool _canLoadBoss) { canLoadBoss = _canLoadBoss; }
	  bool GetCanLoadBoss() { return canLoadBoss; }

	//get set isEditorMode
void SetIsEditorMode(bool _isEditorMode) { isEditorMode = _isEditorMode; }
bool GetIsEditorMode() { return isEditorMode; }

//get set isInventoryOpen
void SetIsInventoryOpen(bool _isInventoryOpen) { isInventoryOpen = _isInventoryOpen; }
bool GetIsInventoryOpen() { return isInventoryOpen; }

//get deltaTime
float GetDeltaTime() { return deltaTime; }
//get globalTime
float GetGlobalTime() { return globalTime; }

//get set isTypingMode
void SetIsTypingMode(bool _isTypingMode) { isTypingMode = _isTypingMode; }
bool GetIsTypingMode() { return isTypingMode; }



	//setplayer
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	void Init() override;
	void Update() override;

	void DrawSprite() override;

	void DrawMoji() override;

	void StartBattle();

	void StartEndBattle();
	void EndBattle();

	void GoToNextFloor();

	void FadeFloor();

	void PlayerInfo();


	void GameTimeManager();

	void StartBossBattle();

	void ManageStageProgress();

	void CalDeltaTime();

	void GetStringFromJson();
	bool hasReadJson = false;
	string jsonPath = "Data/GameData.json";
	string gameName = "";

	void SoundManagement();

	void ClearAllBattleEnemy();


	void DebugKey() override;

	void CallImgui() override;

	Color itemColor = { 1,1,1,1 };

	float controlX = 0;
	float controlY = 0;
	float controlZ = 0;

	bool canEnemyCollision = true;

	float effectAngle = 0;

	
	
	int eSearchrange = 0;


	float inventoryX = -350;
	float inventoryY = 210;
	float decriptX = 0;
	float decriptY = 0;

	float invenSpace = 35;

	KdRenderTargetChanger rtc;
	KdRenderTargetPack rtp;

	float hpY = 307;

	float camFov = 60;

	shared_ptr<KdTexture> miniMapTex[15][15];
	Math::Rectangle miniMapRect = { 0,0,8,8 };

	int mapHeight = 15;  //15-25
	int mapWidth = 15;   //15-25
	vector<std::vector<Vector3>> mapPos;
	vector<std::vector<int>> mapData;


	float stageProgress = 0;
	float stageProgressMax = 100;

	float sliderVal = 1; //for ususal slidering 

	float sliderA = 0;
	float sliderB = 0;

	int testNum = 1; //for test boxx atk
	float testAngle = 360;

	//for adjust moji
	float mojiMoveX = 1;
	float mojiMoveY = 1;

	float mojiMoveDegen = 0.1;

	float scrollX = 0;
	float scrollY = 0;

	Color sliderColor = { 1,1,1,1 };

	Vector3 inventoryOriginPos = { 0,0,0 };

	Vector2 mojiSpawnPos = { -630,0 };
	float alphaPow = 0.007;
	float mojiAdd = 0.21;


	bool isEndBattle = false;
	float endBattleCnt = 70;

	bool isImgui = false;

private:

	bool isBattleMode = false;
	bool canNextFloor = true;
	bool canLoadMapEnemy = false;
	bool canLoadBattleEnemy = false;
	bool canLoadBoss = false;
	bool isMiniMap = false;
	bool isGameStart = false;
	bool isGameStop = false;

	bool canFade = false;
	float fadeCnt = 0;
	bool canFadeFloor = false;

	float hpX = 190;

	int battleEnemyNum = 0;


	Math::Rectangle hpBarRect = { 0,0,190,20 };
	Math::Rectangle hpBarBackRect = { 0,0,190,20 };
	Math::Rectangle fadeRect = { 0,0,1280,720 };

	Color nColor = { 1,1,1,1 };

	Vector2 hpBarPos = { 0,0 };
	Vector2 hpBarScale = { 1,1 };
	Vector2 hpBarBackPos = { -600,350 };
	Vector2 hpBarBackScale = { -600,350 };


	KdTexture hpBarTex;
	KdTexture hpBarBackTex;
	KdTexture fadeTex;
	KdTexture startTex;

	Vector3 playerMapReturnPos = {};
	Vector3 battlePlayerPos = { 7.7 ,77,10  };

	Color fadeColor = { 1,1,1,1 };
	float fadeAlpha = 0;

	int floorNum = 1;

	//weak_ptr player
	weak_ptr<Player> wpPlayer;

	float playerHp = 10;

	int enemyUniqueId = 0;



	Color startColor = { 1,1,1,1 };
	float startAlpha = 1;

	int gameCrystal = 100;

	int debugCnt = 0;

	Vector3 polyScale = { 2.8,1.4,1 };

	bool canDebugWire = false;
	bool isEditorMode = false;

	bool isStrikeSkill = false;

	bool isInventoryOpen = false;

	float deltaTime = 0;
	float globalTime= 0;
	std::chrono::high_resolution_clock::time_point prevTime;
	std::chrono::high_resolution_clock::time_point currentTime;

	bool isTypingMode = false;

	bool canWandererDie = false;


private:
	GameManager() {}
	~GameManager() override {}

public:
	static GameManager& Instance()
	{
		static GameManager instance;
		return instance;
	}
};