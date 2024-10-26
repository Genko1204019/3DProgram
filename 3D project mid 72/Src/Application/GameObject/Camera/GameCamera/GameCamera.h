#pragma once
#include "../CameraBase.h"

class BattleEnemy;
class Player;

using namespace std;
using namespace DirectX::SimpleMath;

enum CamMode
{
	NoMode =0,
	FollowMode,
	DialogMode,
	CutSceneMode,
	BattleSceneMode,
	StandStillMode,
	InventoryMode,
	WinCutSceneMode,

};

class GameCamera : public CameraBase
{
public:
	GameCamera() {};
	~GameCamera() override {};

	//get set camBrightness
	float GetCamBrightness() { return camBrightness; }
	void SetCamBrightness(float _brightness) { camBrightness = _brightness; }

	//set enemy
	void SetEnemy(const shared_ptr<BattleEnemy>& enemy) { wpEnemy = enemy; }
	//set player
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }

	void Init()				override;
	void Update()			override;
	void PreDraw()			override;

	void PostUpdate()		override;

	void CallImgui()		override;

	void UpdateCameraRotation();
	void EnemyLockOn();
	void RotateWithMouse();

	void CounterManager();

	void CameraEffect();
	
	void PlayerInfo();
	void EnemyInfo();
	void UpdateMatrix();

	void ScreenShake();
	void StartScreenShake(float _duration = 0.5f);

	void StartBattleCutScene();

	void StartWinCutScene();

	void PlanCameraMove(Vector3 _startPos, Vector3 _endPos, Vector3 _startRot, Vector3 _endRot, float _duration);
	void DoCameraMove();

	void CameraTwist();
	void StartCameraTwist();


	void StartStandStill();
	void ReturnToFollow();

	void OpenInventoryMode();

	void DebugKey();

	//weak_ptr enemy
	weak_ptr<BattleEnemy> wpEnemy;
	weak_ptr<Player> wpPlayer;

	//Matrix 
	Matrix scaleMat = {};
	Matrix rotMat = {};
	Matrix localPosMat = {};
	Matrix localRotMat = {};

	//Player info
	Vector3 playerPos = {};
	Matrix playerTransMat = {};

	//Enemy info
	Vector3 enemyPos = {};
	Matrix enemyTransMat = {};

	Vector3 camToEnemy = {};

	bool isLockOn = false;

	Matrix alphaMat = {};

	float focusDist = 14;
	float focusForRange = 14;
	float focusBackRange = 28;

	Vector3 posOffset = { 0, 4, -11.4 };
	Vector3 rotOffset = { 0, 0, 0 };

	Vector3 startPos = {};
	Vector3 endPos = {40,-40,-7};

	Vector3 startRot = {};
	Vector3 endRot = {20,20,0};

	float distToEndPos = 0;
	float distToEndRot = 0;

	bool isMouseMode = false;

	bool isScreenShake = false;
	float screenShakeTime = 0;
	float screenShakeDuration = 0.5f;  // How long the shake lasts (in seconds)
	float shakeIntensity = 0.2f;       // How intense the shake is

	Vector3 originalPosOffset = { 0, 4, -10 };
	Vector3 originalRotOffset = { 0, 0, 0 };

	float moveElapsed = 0;
	float moveDuration = 2.0f;						

	int nowState = CamMode::FollowMode;
	float cutSceneCnt = 0;
	int cutSceneProcess = 0;

	bool isCamMoving = false;
	Vector3 targetPos = {}; //for camMove
	Vector3 targetRot = {};

	float twistAngle = 0;
	float twistDuration = 1.5f; 
	float twistElapsed = 0.0f;  
	bool isTwisting = false;    
	float camFov = 60;

	Vector3 standStillPos = { 6,-7,14 };
	float standStillDuration = 210;
	float standStillCnt = 0;
	bool isStandStill = false;

	float camBrightness = 1.0f;

	Vector3 observeEnemyPos = { 7,-11,14 };
	Vector3 observePlayerPos = { 7, -11, 14 };



private:

	


};

