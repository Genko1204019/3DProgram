//Player.h file

#include"json.hpp"

//occulation culling視錐台カリング
//trace polygon バラつき

#pragma once

class BattleEnemy;
class PlayerProjectile;

class PlayerWeapon;
class PlayerArmor;
class PlayerShield;
class Item;

using namespace std;
using namespace DirectX::SimpleMath;

using json = nlohmann::json;

struct AfterImage {
	Vector3 position;
	Matrix rotation;
	float opacity;
	shared_ptr<KdAnimator> afterAnimator = nullptr;
	shared_ptr<KdModelWork> afterModel = nullptr;
	float aftetAniSpd = 1;
};

class Player : public KdGameObject
{
public:
	Player(){}
	~Player() override {}


	int GetnowState() { return nowState; }
	void SetnowState(int _state) { nowState = _state; }
	bool GetisAttacking() { return isAttacking; }
	void SetisAttacking(bool _isAtk) { isAttacking = _isAtk; }
	bool GetisMoving() { return isMoving; }
	void SetisMoving(bool _isMove) { isMoving = _isMove; }
	bool GetisLockOn() { return isLockOn; }
	void SetisLockOn(bool _isLock) { isLockOn = _isLock; }
	float Gethp() { return hp; }
	void Sethp(float _hp) { hp = _hp; }
	float GetmaxHp() { return maxHp; }
	void SetmaxHp(float _maxHp) { maxHp = _maxHp; }
	Vector3 GetmoveVec() { return moveVec; }
	void SetmoveVec(Vector3 _moveVec) { moveVec = _moveVec; }
	Vector3 GetatkVec() { return atkVec; }
	void SetatkVec(Vector3 _atkVec) { atkVec = _atkVec; }
	Vector3 GetatkVecOld() { return atkVecOld; }
	void SetatkVecOld(Vector3 _atkVecOld) { atkVecOld = _atkVecOld; }
	float Getgravity() { return gravity; }
	void Setgravity(float _gravity) { gravity = _gravity; }
	float GetgravityPow() { return gravityPow; }
	void SetgravityPow(float _gravityPow) { gravityPow = _gravityPow; }
	float GetmoveSpd() { return moveSpd; }
	void SetmoveSpd(float _moveSpd) { moveSpd = _moveSpd; }
	float GetmoveSpdWalk() { return moveSpdWalk; }
	void SetmoveSpdWalk(float _moveSpdWalk) { moveSpdWalk = _moveSpdWalk; }
	float GetmoveSpdDash() { return moveSpdDash; }
	void SetmoveSpdDash(float _moveSpdDash) { moveSpdDash = _moveSpdDash; }
	int GetmoveId() { return moveId; }
	void SetmoveId(int _moveId) { moveId = _moveId; }
	Vector3 GetdashVec() { return dashVec; }
	void SetdashVec(Vector3 _dashVec) { dashVec = _dashVec; }
	float GetdashDuration() { return dashDuration; }
	void SetdashDuration(float _dashDuration) { dashDuration = _dashDuration; }
	float GetdashCd() { return dashCd; }
	void SetdashCd(float _dashCd) { dashCd = _dashCd; }
	bool GetisDashing() { return isDashing; }
	void SetisDashing(bool _isDash) { isDashing = _isDash; }
	float GetatkMoveDist() { return atkMoveDist; }
	void SetatkMoveDist(float _atkMoveDist) { atkMoveDist = _atkMoveDist; }
	float GethitFrame() { return hitFrame; }
	void SethitFrame(float _hitFrame) { hitFrame = _hitFrame; }
	float GetatkMoveCnt() { return atkMoveCnt; }
	void SetatkMoveCnt(float _atkMoveCnt) { atkMoveCnt = _atkMoveCnt; }
	int GetatkOrder() { return atkOrder; }
	void SetatkOrder(int _atkOrder) { atkOrder = _atkOrder; }
	float GetatkCD() { return atkCD; }
	void SetatkCD(float _atkCD) { atkCD = _atkCD; }
	float GetatkFrame() { return atkFrame; }
	void SetatkFrame(float _atkFrame) { atkFrame = _atkFrame; }
	float GetatkReadyFrame() { return atkReadyFrame; }
	void SetatkReadyFrame(float _atkReadyFrame) { atkReadyFrame = _atkReadyFrame; }
	float GetatkEndFrame() { return atkEndFrame; }
	void SetatkEndFrame(float _atkEndFrame) { atkEndFrame = _atkEndFrame; }
	float GetpushEnemyPow() { return pushEnemyPow; }
	void SetpushEnemyPow(float _pushEnemyPow) { pushEnemyPow = _pushEnemyPow; }
	float GetatkDmg() { return atkDmg; }
	void SetatkDmg(float _atkDmg) { atkDmg = _atkDmg; }
	float GetatkDmgAccumulator() { return atkDmgAccumulator; }
	void SetatkDmgAccumulator(float _atkDmgAccumulator) { atkDmgAccumulator = _atkDmgAccumulator; }
	float GetatkDmgShowcase() { return atkDmgShowcase; }
	void SetatkDmgShowcase(float _atkDmgShowcase) { atkDmgShowcase = _atkDmgShowcase; }
	float GetrotAngle() { return rotAngle; }
	void SetrotAngle(float _rotAngle) { rotAngle = _rotAngle; }
	Matrix GetnowRot() { return nowRot; }
	void SetnowRot(Matrix _nowRot) { nowRot = _nowRot; }
	Vector3 GetnowVec() { return nowVec; }
	void SetnowVec(Vector3 _nowVec) { nowVec = _nowVec; }
	Vector3 GettoVec() { return toVec; }
	void SettoVec(Vector3 _toVec) { toVec = _toVec; }
	Vector3 GetenemyPos() { return enemyPos; }
	void SetenemyPos(Vector3 _enemyPos) { enemyPos = _enemyPos; }
	Vector3 GetcamToEnemyVec() { return camToEnemyVec; }
	void SetcamToEnemyVec(Vector3 _camToEnemyVec) { camToEnemyVec = _camToEnemyVec; }

	//get set distToLockEnemy
	float GetdistToLockEnemy() { return distToLockEnemy; }
	void SetdistToLockEnemy(float _distToLockEnemy) { distToLockEnemy = _distToLockEnemy; }

	//get set mana stamina anger
float GetMp() { return mp; }
void SeMp(float _mana) { mp = _mana; }
float GetMaxMp() { return maxMp; }
void SetMaxMp(float _maxMana) { maxMp = _maxMana; }
float Getstamina() { return stamina; }
void Setstamina(float _stamina) { stamina = _stamina; }
float GetmaxStamina() { return maxStamina; }
void SetmaxStamina(float _maxStamina) { maxStamina = _maxStamina; }
float Getanger() { return anger; }
void Setanger(float _anger) { anger = _anger; }
float GetmaxAnger() { return maxAnger; }
void SetmaxAnger(float _maxAnger) { maxAnger = _maxAnger; }

	//get set hitStop
	bool GetisHitStop() { return isHitStop; }
	void SetisHitStop(bool _isHitStop) { isHitStop = _isHitStop; }
	//hitstop cnt
	float GethitStopCnt() { return hitStopCnt; }
	void SethitStopCnt(float _hitStopCnt) { hitStopCnt = _hitStopCnt; }

	//get set hpbuffer bufferMax
	float GethpBuffer() { return hpBuffer; }
	void SethpBuffer(float _hpBuffer) { hpBuffer = _hpBuffer; }

	float GethpBufferMax() { return hpBufferMax; }
	void SethpBufferMax(float _hpBufferMax) { hpBufferMax = _hpBufferMax; }


	//get set aniSpd
	float GetaniSpd() { return aniSpd; }
	void SetaniSpd(float _aniSpd) { aniSpd = _aniSpd; }


	void SetEnemy(const shared_ptr<BattleEnemy>& enemy) { wpEnemy = enemy; }
	void SetWeapon(const shared_ptr<PlayerWeapon>& weapon) { wpWeapon = weapon; }
	void SetArmor(const shared_ptr<PlayerArmor>& armor) { wpArmor = armor; }
	void SetShield(const shared_ptr<PlayerShield>& shield) { wpShield = shield; }

	bool isWithinAtkFrame();
	bool ReadyForAtk();
	bool isPressingMoveKey();

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawUnLit() override;
	void DrawSprite() override;
	void DrawMoji() override;

	void DrawMini() override;
	void UpdateMiniMapPos();

	void DrawRenderTarget() override;

	

	//Init
	void InitModelInfo();
	void InitAnimator();
	void InitCollider();
	void InitTexuture();
	void InitStatus();
	void InitPlayerStatusFromJson();

	//SetAnimation
	void SetAnime(string _aniName, bool _isLoop);
	void SetAnime(string _aniName, string _aniName2, bool _isLoop);
	void ResetAnime();
	void ResetAnimeName();

	//Update
	void UpdateMatrix();
	void UpdateAnimation();
	void UpdateRotation();
	void UpdateGravity();

	//TargetInfo
	void EnemyInfo();
	void CameraInfo();
	void GameInfo();
	void WeaponInfo();

	//Collision & Debug Wire
	void SetCollider(KdCollider::SphereInfo& _sphereObj,Vector3 _sphereCenter, float _sphereRadius, UINT _hitType);
	void SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType);

	void SetDebugWire(KdCollider::SphereInfo& _sphereObj, Color _sphereColor = { 1,0,0,1 });
	void SetDebugWire(KdCollider::RayInfo& _rayObj, Color _rayColor = { 1,1,1,1 });

	void GroundCollision();
	void BodyCollision();
	void DmgCollision();
	void ItemCollision();

	string swordName = "Sword";

	//Counter
	void CounterManager();

	void ManageMP();
	void ManageFog();

	//Dash
	void PlayerDash();
	void DashTransition();
	void EnterDash();
	void ExitDash();
	void CheckDashDirection();

	void MoveManager();

	//LockOn
	void PlayerLockOn();
	
	void PlayerHitStop();

	void EffectManager();
	void CreateEffect(int& _effectId, string _effecName, Vector3 _effPos, float _effScale, float _effSpd, bool _isLoop);
	void SetEffectPos(int _effecId, Vector3 _pos);
	int effCnt = 0;

	//PLayer Weapon
	void EquipAllGears();
	void ChangeArmor(int _armorId, string _modelPath, float _wpDef);
	void ChangeWeapon(int _wpId, int _skillId, string _modelPath, float _wpDmg);

	void PickUpItem();
	void UseItem();
	void UsePotion(int _potionId, Item& _item);

	void PlayerHpBarBuffer();

	//Action (State Machine) & StateTransition
	void StateMachineManager();
	
	void PlayerIdle();
	void IdleTransition();
	void EnterIdle();

	void PlayerMove();
	void MoveTransition();
	void EnterMove();
	void ExitMove();
	void PlayerFreeMove();
	void PlayerLockMove();

	void PlayerDefense();
	void DefenseTransition();
	void EnterDefense();

	void PlayerAttack();
	void AtkTransition();
	void EnterAtk();
	void ExitAtk();
	void SetAtkInfo(float _aniSpd, float _atkStartframe, float _atkEndFrame, float _atkDmg, float _atkMoveDist, float _pushEnemyPow);
	void SetAtkAngle();
	void AtkMove();
	void AtkManager();

	void AtkDelay();
	void AtkEffect();

	void ShotMagicBall();

	void PlayerMagicAtk();
	void PlayerMagicAtkTransition();
	void EnterMagicAtk();
	void ExitMagicAtk();

	void PlayerStrikeKill();
	void StrikeKillTransition();


	void PlayerStun();
	void StunTransition();
	void EnterStun();

	void PlayerShot();

	void PlayerBlowAway();
	void BlowAwayTransition(Vector3 _blowDir, float _blowPow, float _gravity, bool _canRotate = false);
	void EnterBlowAway();


	void OnGiveDmg();

	void TrailPoly();



	void PlayerSkill();
	void SkillTransition();
	void EnterSkill();
	void ExitSkill();
	void ReleaseSkill();
	void SpinSlashSkill();
	void FlySlashSkill();
	void MagicAtkSkill();

	void DashEffect();

	void OnHit() override;
	

	void DebugKey() override;


	void CallImgui() override;

	static const int afterNum = 7;
	AfterImage playerAfter[afterNum];

	float afterCnt = 0;



	float scrollX = 0;
	float scrollY = 0;

	float playerMiniPosX = 0;
	float playerMiniPosY = 0;

	/*float scrollXMax = 200;
	float scrollXMin = -200;
	float scrollYMax = 200;
	float scrollYMin = -200;*/

	float scrollXMax = 270;
	float scrollXMin = 94;
	float scrollYMax = 261;
	float scrollYMin = 68;

	Vector2  miniMapPos2D = { 0,0 };


private:

	KdTexture IconTex;
	Math::Rectangle iconRect = { 0,0,32,32 };

	shared_ptr<KdSquarePolygon> dissolvePoly;

	shared_ptr<KdModelWork> test = nullptr;


	shared_ptr<KdModelWork>  testModel = nullptr;
	shared_ptr<KdAnimator> testAnime;

	KdTrailPolygon tPoly;

	shared_ptr<KdSquarePolygon> renderPoly;


	//Model
	shared_ptr<KdModelWork>  bodyModel = nullptr;

	string pModelName = "Asset/Models/_PlayerObj/PlayerBody/PlayerBody.gltf";
	string shieldModelName = "Asset/Models/_PlayerObj/WarriorShield/WarriorShield.gltf";
	string armorModelName = "Asset/Models/_ArmorObj/EmptyHelmet/EmptyHelmet.gltf";

	//Anime
	shared_ptr<KdAnimator> bodyAnime;

	//Poly
	shared_ptr<KdSquarePolygon> poly;

	string idleAni = "Idle";
	string idleAniTwoHand = "2H_Melee_Idle";

	string stunAni = "Hit_B";

	string spawnAni = "Spawn_A";
	string deathAni = "Death_A";

	string atkAni1 = "2H_Melee_Attack_Chop";
	string atkAni2 = "2H_Melee_Attack_Stab";
	//string atkAni3 = "1H_Melee_Attack_Slice_Diagonal";
	string atkAni3 = "2H_Melee_Attack_Chop";
	string atkAni4 = "2H_Melee_Attack_Spin";

	string dashForAni = "Dodge_Forward";
	string dashBackAni = "Dodge_Backward";
	string dashLeftAni = "Dodge_Left";
	string dashRightAni = "Dodge_Right";

	string walkAni = "Running_A";
	string walkBackAni = "Walking_Backwards";
	string walkLeftAni = "Running_Strafe_Left";
	string walkRightAni = "Running_Strafe_Right";

	string defenseAni = "Blocking";
	string getHitAni = "Hit_B";

	string skillAni = "2H_Melee_Attack_Spinning";
	string skillPrepareAni = "2H_Melee_Attack_Spin";

	string cheerAni = "Spellcast_Raise";


	//weakPtr obj
	weak_ptr<BattleEnemy> wpEnemy;
	weak_ptr<PlayerWeapon> wpWeapon;
	weak_ptr<PlayerArmor> wpArmor;
	weak_ptr<PlayerShield> wpShield;

	//StateMachine
	StateMachine stateMachine;


	//Matrix
	Matrix scaleMat = {}, rotMat = {}, transMat = {};
	Matrix wpWorldMat = {};
	Matrix swordMat = {};

	Matrix inventoryPlayerMat = {};

	//Status
	float hp = 100.f;
	float maxHp = 100.f;
	float mp = 100;
	float maxMp = 100;
	float stamina = 100;
	float maxStamina = 100;
	float anger = 0;
	float maxAnger = 100;

	//HpBar & Moji
	Vector3 pos2D = {};

	//Rotation
	float rotAngle = 0.f;
	Matrix nowRot = {};
	Vector3 nowVec = {};
	Vector3 toVec = {};


	//Animation
	float aniSpd = 1.f;

	//Move & Position
	Vector3 moveVec = {};
	Vector3 atkVec = {};
	Vector3 atkVecOld = {};
	float gravity = 0.01f;
	float gravityPow = 0.01;
	bool isMoving = false;
	float moveSpd = 0.21f; //0.1
	float moveSpdWalk = 0.177f; //0.21
	float moveSpdDash = 0.49f;
	float moveSpdSkill = 0.14;
	int moveId = 0;

	//Dash
	Vector3 dashVec = {};
	float dashDuration = 14;
	float dashCd = 14;
	bool isDashing = false;

	//Attack
	Vector3 atkMovePos = {};
	Vector3 atkWpMovePos = {}; // for weapon's effect's pos
	bool isAttacking = false;
	bool canAtkAnime = false;
	float atkMoveDist = 2.1;
	float hitFrame = 0;
	float atkMoveCnt = 0;
	
	int atkOrder = 0;
	float atkCD = 0;
	float atkFrame = 0;
	float atkReadyFrame = 30; //14
	float atkEndFrame = 56; // 20
	float atkReadyAdjustFrame = 0;
	float atkEndAdjustFrame = 0;
	float pushEnemyPow = 1.4;
	float atkDmg = 10;
	int wpComboSet = 1;

	int atkCombo = 0;


	float atkDmgAccumulator = 0;
	float atkDmgShowcase = 0;

	float atkDelayCnt = 0;
	bool isAtkDelay = false;

	//defense
	bool isDefensing = false;
	float defenseDuration = 0;

	

	//LockOn
	bool isLockOn = false;
	float distToLockEnemy = 100;
	float currentlockDist = 0;

	//EnemyInfo
	Vector3 enemyPos = {};
	Vector3 camToEnemyVec = {};
	int enemyId = 0;

	//CamInfo
	Vector3 camPos = {};
	Matrix camRotMat = {};

	//Collider Info
	Vector3 swordOffset = {};

	//Game Info

	//stun & hitstop
	Vector3 getHitPos = {};
	Vector3 getHitDir = {};
	Vector3 atkEnemyPos = {};
	bool isHitStop = false;
	float hitStopCnt = 0;
	float stunCnt = 0;

	//State
	int nowState = PlayerState::pIdle;
	float stateCnt = 0;

	//ChangeWeapon
	float changeWeaponCnt = 0;
	int weaponId = 1;


	//ChangeWeapon
	bool isChangeWeapon = false;
	int currentWeapon = 0;
	int weaponSlotA = 0;
	int weaponSlotB = 0;
	int weaponDmg = 0;
	 
	string currentAnime = "";

	//Bit-Masking
	int m_nowKey = 0;
	int m_prevKey = 0;


	bool isHitEnemy = false;

	//Effect-Related
	int effectId = 0;
	int slashEffId = 0;

	bool canSlashEff = false;

	Matrix polyMat = {};

	float polyRot = 90;

	Vector3 swordBoxSize = {11.14 ,11.14 ,11.14 };


	//StrikeKill
	bool isStrikeKill = false;
	int strikeCombo = 0;
	int strikeCnt = 0;
	Vector3 strikeTargetPos = {};
	Vector3 strikeStandPos = {700,700,700};

	int strikeEffId = 0;
	int auraEffId = 0;
	int dahsEffId = 0;

	string auraEffName = "NA_aura_004.efkefc";


	bool hasEquipment = false;

	//LIght & Shader
	Vector3 lightDir = {-1,-1,-1 };
	Color lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color ambientColor = { 0.21,0.23,0.39, 1.0f };//0,0,1,1 worldStop
	Color playerColor = { 0.7, 0.7,  0.7,1.0f };

	//height fog
	float topVal = -2.1; //1 , -2.1
	float botVal = 0.5;
	float hfogDist = 49;
	Color hfogColor = { 0,0,0, 1.0f };

	float dfogDensity = 0.1;

	//point light 
	Color pointLightColor = { 1,1,1,1 };
	Vector3 pointLightPos = { 0,0,0 };
	float pointLightRadiu = 100;

	bool canAStar = false;

	


	//Player Skill
	int skillId = 1;
	float skillCnt = 0;
	bool isSkillReleased = false;
	float skillPrepareCnt = 0;

	float mutekiCnt = 0;

	//get Hit 
	
	//hp buffer
	float hpBuffer = 0;
	float hpBufferMax = 100;
	float hpBufferCd = 14;

	//staff atk
	bool isMagicWeapon = false;

	//for water shader 
	std::shared_ptr<KdTexture> m_normalTex;

	//circle light 
	float circleRangeAdd = 0;
	float circleRangeMax = 7;

	//blowAway
	Vector3 blowTargetPos = {};
	Vector3 blowDir = {};
	float distToBlowTarget = {};
	float blowPow = 7.7;
	float blowGravity = -0.35;
	float toBlowPow = 14; //21

	float aniFrame = 0; //control animation frame progress
	int absordEffId = 0;

	float scaleFactor = 28.0f / 6.0f;
	float xOffset = -500;
	float yOffset = 200; //100
	
	bool canRotate = false;

	float comboId = 1;
};



