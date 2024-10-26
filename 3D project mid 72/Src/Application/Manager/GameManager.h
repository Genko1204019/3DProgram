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

	
	//get set entityNum
	void SetEntityNum(int _entityNum) { entityNum += _entityNum; }


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

	void SetCanLoadBattleEnemy(bool _canLoadBattleEnemy) { canLoadBattleEnemy = _canLoadBattleEnemy; }
	bool GetCanLoadBattleEnemy() { return canLoadBattleEnemy; }

	void SetIsMiniMap(bool _isMiniMap) { isMiniMap = _isMiniMap; }
	bool GetIsMiniMap() { return isMiniMap; }

	void SetIsGameStart(bool _isGameStart) { isGameStart = _isGameStart; }
	bool GetIsGameStart() { return isGameStart; }

	bool GetCanDebugWire() { return canDebugWire; }


	void SetCanFadeScene(bool _canFade) { canFadeScene = _canFade; }
	bool GetCanFadeScene() { return canFadeScene; }

	//get set enemyUniqueId
	void SetEnemyUniqueId(int _enemyUniqueId) { enemyUniqueId = _enemyUniqueId; }
	int GetEnemyUniqueId() { return enemyUniqueId; }

	//battleEnemyNum
	void SetBattleEnemyNum(int _battleEnemyNum) { battleEnemyNum = _battleEnemyNum; }
	int GetBattleEnemyNum() { return battleEnemyNum; }

	//get set floorNum
	void SetFloorNum(int _floorNum) { floorNum = _floorNum; }
	int GetFloorNum() { return floorNum; }

	//get set isStrikeSkill
	void SetIsStrikeSkill(bool _isStrikeSkill) { isStrikeSkill = _isStrikeSkill; }
	bool GetIsStrikeSkill() { return isStrikeSkill; }

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

	//Get set MojiSpawnPos
	void SetMojiSpawnPos(Vector2 _mojiSpawnPos) { mojiSpawnPos = _mojiSpawnPos; }
	Vector2 GetMojiSpawnPos() { return mojiSpawnPos; }

	//get set playerScrollX Y 
	void SetPlayerScrollX(float _playerScrollX) { playerScrollX = _playerScrollX; }
	float GetPlayerScrollX() { return playerScrollX; }

	void SetPlayerScrollY(float _playerScrollY) { playerScrollY = _playerScrollY; }
	float GetPlayerScrollY() { return playerScrollY; }

	//get set isImgui
	void SetIsImgui(bool _isImgui) { isImgui = _isImgui; }
	bool GetIsImgui() { return isImgui; }

	//get set isEndBattle
	void SetIsEndBattle(bool _isEndBattle) { isEndBattle = _isEndBattle; }
	bool GetIsEndBattle() { return isEndBattle; }

	//get set endBattleCnt
	void SetEndBattleCnt(float _endBattleCnt) { endBattleCnt = _endBattleCnt; }
	float GetEndBattleCnt() { return endBattleCnt; }



	//setplayer
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	void Init() override;
	void Update() override;

	void DrawSprite() override;

	void DrawMoji() override;

	void StartBattle();
	void EndBattle();

	void GoToNextFloor();

	void UpdateSceneFade();

	void UpdateFogLight();
	void UpdateSound();

	void UpdateStageProgress();

	void UpdateDeltaTime();


	void ClearAllBattleEnemy();


	void DebugKey() override;

	void CallImgui() override;


	
	
	Vector3 inventoryOriginPos = { 0,0,0 };

	
	


private:

	//Bool
	bool isBattleMode = false;
	bool canNextFloor = true;
	bool canLoadBattleEnemy = false;
	bool isMiniMap = false;
	bool isGameStart = false;
	bool canDebugWire = false;
	bool isInventoryOpen = false;
	bool isTypingMode = false;
	bool isStrikeSkill = false;
	bool isImgui = false;

	//Time
	float deltaTime = 0;
	float globalTime = 0;
	std::chrono::high_resolution_clock::time_point prevTime;
	std::chrono::high_resolution_clock::time_point currentTime;

	//Fade Scene
	KdTexture fadeTex;
	Color fadeColor = { 1,1,1,1 };
	float fadeAlpha = 1;
	bool canFadeScene = false;

	//Player
	weak_ptr<Player> wpPlayer;
	Vector3 playerMapReturnPos = {};
	Vector3 battlePlayerPos = { 7.7 ,77,10  };
	float playerScrollX = 0;
	float playerScrollY = 0;

	//Floor
	int floorNum = 1;
	int stageProgress = 0;
	int stageProgressMax = 100;

	//Enemy & Battle
	int battleEnemyNum = 0;
	int enemyUniqueId = 0;
	bool canWandererDie = false;
	bool isEndBattle = false;
	float endBattleCnt = 70;

	//height fog
	float topVal = -2.1; //1 , -2.1
	float botVal = 0.5;
	float hfogDist = 49;
	Color hfogColor = { 0,0,0, 1.0f };
	float dfogDensity = 0.1;

	//LIght & Shader
	Vector3 lightDir = { -1,-1,-1 };
	Color lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color ambientColor = { 0.21,0.23,0.39, 1.0f };//0,0,1,1 worldStop

	//Moji
	Vector2 mojiSpawnPos = { -630,0 };

	int entityNum = 1;

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