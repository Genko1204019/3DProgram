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

	void UpdateRotateByMouse();

	void CounterManager();

	void EnterDialogMode();
	void EndDialogMode();

	void EnemyLockOn();

	void PlayerInfo();
	void EnemyInfo();
	void UpdateMatrix();

	void ScreenShake();
	void StartScreenShake(float _duration = 0.5f);

	void StartBattleCutScene();

	void StartDialogCutScene();

	void StartWinCutScene();

	void PlanCameraMove(Vector3 _startPos, Vector3 _endPos, Vector3 _startRot, Vector3 _endRot, float _duration);
	void DoCameraMove();

	void CameraTwist();
	void StartCameraTwist();

	void ApplyBlurEffect();

	void StartStandStill();
	void ReturnToFollow();

	void OpenInventoryMode();
	void InventoryToFollowMode();

	KdTexture tex; 

	//weak_ptr enemy
	weak_ptr<BattleEnemy> wpEnemy;
	weak_ptr<Player> wpPlayer;

	//Vector3 rot = { 0,0,0 };

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
	Vector3 camToEnemyXZ = {};

	bool isLockOn = false;

	Matrix alphaMat = {};

	float focusDist = 14;
	float focusForRange = 14;
	float focusBackRange = 28;

	Vector3 posOffset = { 0, 4, -11.4 };
	//Vector3 posOffset = { 0, 4, -14 };

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
	float moveDuration = 2.0f;						//カメラの移動総合時間

	int cameraMode = CamMode::FollowMode;
	float cutSceneCnt = 0;
	int cutSceneProcess = 0;


	Vector3 targetPos = {}; //for camMove
	Vector3 targetRot = {};

	bool isCamMoving = false;

	float time = 0;


	float twistAngle = 0;
	float twistDuration = 1.5f; // Duration of the twist in seconds
	float twistElapsed = 0.0f;  // Time elapsed since the twist started
	bool isTwisting = false;    // Flag to check if twisting is active
	float camFov = 60;

	Vector3 standStillPos = { 6,-7,14 };
	float standStillDuration = 210;
	float standStillCnt = 0;
	bool isStandStill = false;

	Vector3 dialogPos = { 0,0,0 };

	float camBrightness = 1.0f;

	Vector3 observeEnemyPos = { 7,-11,14 };

	Vector3 observePlayerPos = { 7, -11, 14 };


private:

	


};

