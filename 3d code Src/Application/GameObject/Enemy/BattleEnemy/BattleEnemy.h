#pragma once 
#include "../EnemyBase.h"

using namespace std;
using namespace DirectX::SimpleMath;

class BattleEnemy : public EnemyBase
{
public:
	BattleEnemy(){}
	~BattleEnemy() override {}

	float GetHp() { return hp; }
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

	void WeaponInfo() override;
	void CameraInfo() override;

	void UpdateBattleCounter();

	void DmgCollision();

	void UpdateStatus();

	//Common
	void EnemySpawn();
	void SpawnTransition();
	void ExitSpawn();

	void EnemyIdle();
	void IdleTransition();

	void EnemyStun();
	void StunTransition();

	void EnemyBackJump();
	void BackJumpTransition(Vector3 _dir, float _pow);

	void EnemyBlowAway();
	void BlowAwayTransition(Vector3 _blowDir, float _blowPow, float _gravity);

	void EnemyChase();
	void ChaseTransition();

	void HandleDeath();
	void DeathTransition();

	//Fighter
	void EnemyAttack();
	void AttackTransition();
	void AtkDelay();
	void AtkMove();

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


	void OnGetDmg(Vector3 _hitPos, bool _isBlowAway, float dmgToGet);
	void CallImgui() override;

private:

	//hpBar & Moji
	KdTexture hpBarTex;
	KdTexture hpBarFrameTex;
	KdTexture hpBaseFrameTex;
	Vector3 pos2D = {};
	float hpBuffer = 0;
	float hpBufferCd = 0;
	float hpBufferMax = 100;
	float hpBufferDefault = 100;
	float displayHpCnt = 0;
	float hpBarAlpha = 0;

	//lock-on
	shared_ptr<KdModelWork>  lockOnModel = nullptr;
	string lockOnModelName = "Asset/Models/_EnemyObj/LockOn/LockOn.gltf";
	bool isLockOn = false;
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

	//EnemySpawn
	float spawnCnt = 0;
	bool canSpawn = false;

	//BlowAway
	Vector3 blowTargetPos = {};
	Vector3 blowDir = {};
	float distToBlowTarget = {};
	float blowPow = 7.7;
	float blowGravity = -0.49;
	float toBlowPow = 7.7;
	bool hasBlowAway = false;

	//MageWalk
	Vector3 walkTarget = {};

	//backJump
	Vector3 backJumpTargetPos = {};

	//Effect
	int mageAtkEffId = -1;
	int darkSpawnEffId = -1;
	int dizzyEffId = -1;
	int absorbEffId = -1;
	int absorbEffCnt = 0;
	bool isSmashEff = false;

	float dmgPredict = 100;
	bool hasDeadAni = false;


	//Boss
	bool hasReachedtarget = false;
	int spinProgress = 1;
	bool isSpinFinish = false;
	bool isStandUp = false;

};

