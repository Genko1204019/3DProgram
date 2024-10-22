#pragma once 
#include "../EnemyBase.h"

using namespace std;
using namespace DirectX::SimpleMath;

class BattleEnemy : public EnemyBase
{
public:
	BattleEnemy(){}
	~BattleEnemy() override {}

	//getHp
	float GetHp() { return hp; }
	//getisLockOn setisLockOn
	bool GetIsLockOn() { return isLockOn; }
	void SetIsLockOn(bool _isLockOn) { isLockOn = _isLockOn; }

	bool isAtkEnable()
	{
		if (atkFrame >= atkReadyFrame + 1 && atkFrame <= atkEndFrame && !isAtkDelay) {
			return true;
		}
		else if (!isSpinFinish && nowState == EnemyState::eBossSummon) {
			return true;
		}
		else {
			return false;
		}
	}


	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void DrawSprite() override;
	void DrawMoji() override;

	void DrawMini() override;

	void InitBattleEnemy();
	void InitAtkInfo();

	void InitState();

	void CameraInfo() override;
	void WeaponInfo() override;


	void BattleCounterManager();

	void DmgCollision();

	void HpBarBuffer();

	//Fighter
	void EnemySpawn();
	void SpawnTransition();
	void ExitSpawn();

	void EnemyStun();
	void StunTransition();

	void EnemyBackJump();
	void BackJumpTransition(Vector3 _dir, float _pow);

	void EnemyBlowAway();
	void BlowAwayTransition(Vector3 _blowDir, float _blowPow, float _gravity);
	Vector3 blowTargetPos = {};
	Vector3 blowDir = {};
	float distToBlowTarget = {};
	float blowPow = 7.7;
	float blowGravity = -0.49;
	float toBlowPow = 7.7;

	bool hasBlowAway = false;

	void EnemyIdle();
	void IdleTransition();

	void EnemyChase();
	void ChaseTransition();

	void EnemyAttack();
	void AttackTransition();
	void AtkDelay();
	void AtkMove();

	void HandleDeath();
	void DeathTransition();

	//Mage
	void MageWalk();
	void MageWalkTransition();

	void MageAtk();
	void MageAtkTransition();
	void ExitMageAtk();

	void MageRangeAtk();
	void MageRangeAtkTransition();



	//Boss
	void BossAtk();
	void BossAtkTransition();

	void BossMove();
	void BossMoveTransition();

	void BossSummon();
	void BossSummonTransition();
	void ExitSummon();

	void BossJumpAtk();
	void BossJumpAtkTransition();

	void BossSlashWaveAtk();
	void BossSlashWaveAtkTransition();

	void BossSpinAtk();
 

	void OnGetDmg(Vector3 _hitPos, bool _isBlowAway, float dmgToGet);

	void CallImgui() override;

private:

	//hpBar & Moji
	Vector3 hpBarOffset = { 0,3.5,-1 };
	float hpBarRatio = 1.f;

	float hpBufferRatio = 1.f;
	float hpBuffer = 0;
	float hpBufferCd = 0;
	float hpBufferMax = 100;
	float hpBufferDefault = 100;

	Vector3 pos2D = {};
//	Vector3 hpOffset = { -2.8 ,2.5f,0 };
	Vector3 hpOffset = { -78 ,4.2f,0 };
	Math::Rectangle hpBarRect = { 0,0,276,30 };
	Math::Rectangle hpBarFrameRect = { 0,0,276,30 };
	Math::Rectangle hpBaseFrameRect = { 0,0,276,30 };

	KdTexture hpBarTex;
	KdTexture hpBarFrameTex;
	KdTexture hpBaseFrameTex;

	shared_ptr<KdTexture> enemyIconTex = nullptr;

	//lock-on
	shared_ptr<KdModelWork>  lockOnModel = nullptr;
	string lockOnModelName = "Asset/Models/_EnemyObj/LockOn/LockOn.gltf";

	bool isLockOn = false;
	Vector3 lockOnScale = { 2,2,2 };
	Vector3 lockOnPosOffset = { 0, 0.28, 0 };
	float lockOnAngle = 0.f;

	//stun & hitstop
	Vector3 getHitPos = {};
	Vector3 getHitDir = {};
	bool isHitStop = false;
	float hitStopCnt = 0;
	float stunCnt = 0;
	bool isStun = false;

	//Attack
	bool isAttacking = false;
	bool canAtkAnime = false;
	float atkCd = 0;
	int atkOrder = 0;
	int atkOrderMax = 0;
	float atkFrame = 0;
	float atkReadyFrame = 0;
	float atkEndFrame = 0;
	float atkDmg = 1;
	float atkMoveCnt = 0;

	float atkWaitCnt = 0;
	bool isWaitCnt = false;

	float atkDelayCnt = 0;
	bool isAtkDelay = false;

	//knockBack
	bool canKnockBack = false;
	bool isKnockBack = false;
	float knockBackTimer = 0;
	float knockBackDuration = 0;
	Vector3 knockBackPos = {};

	//EnemySpawn
	float spawnCnt = 0;
	bool canSpawn = false;

	//MageWalk
	float walkRangeXMax = 35;
	float walkRangeXMin = -21;
	float walkRangeZMax = 35;
	float walkRangeZMin = 0;
	Vector3 walkTarget = {};

	//backJump
	Vector3 backJumpDir = {};
	Vector3 backJumpTargetPos = {};
	float jumpProgress = 0;

	Vector3 barScale = { 0.56,0.7,1 };
	Vector3 frameScale = { 0.56,0.7,1 };

	Color bufferColor = { 0,1,0.23,1 };

	float showHpCnt = 0;
	float hpBarAlpha = 0;

	float xOffset = -500; 
	float yOffset = 200; 
	float scaleFactor = 28.0f / 6.0f; 
	Vector2 miniMapPos2D = { 0,0 };

	int magicCircleEffId = -1;

	float mageAtkEffid = -1;

	float dmgPredict = 100;

	bool hasDeadAni = false;

	int darkSpawnEffId = -1;

	bool isSmashEff = false;

	Vector3 centerPos = {7.5,-14.6,21};

	bool hasReachedtarget = false;
	
	int numProjectiles = 5;
	float shotAngle = 360;

	int absorbEffCnt = 0;
	int absorbEffId = -1;

	Vector3 spinTargetA = { 5.9,-14.6,-2.8 };
	Vector3 spinTargetB = { 28.2,-14.6,30.7 };
	Vector3 spinTargetC = { -22.3,-14.6,20.8 };
	Vector3 spinTargetD = { 7.5 ,-14.6,28 }; // 7.5 ,-14.6,28
	Vector3 spinTargetPlayer = { 0,0,0 };
	int spinProgress = 1;
	bool isSpinFinish = false;
	//bool spinDizzyCnt = 0;
	int dizzyEffId = -1;

	bool isStandUp = false;

};

