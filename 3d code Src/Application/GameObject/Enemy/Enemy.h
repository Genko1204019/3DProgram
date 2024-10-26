//Enemy.h file
#pragma once 

using namespace std;
using namespace DirectX::SimpleMath;

class Player;
class EnemyWeapon;	



class Enemy : public KdGameObject
{
public:
	Enemy(){}
	~Enemy() override {}

	void SetPlayer(const std::shared_ptr<Player>& player) { wpPlayer = player; }
	void SetEnemy(const std::shared_ptr<Enemy>& enemy) { wpEnemy = enemy; }
	void SetObj(const std::shared_ptr<KdGameObject>& obj) { wpObj = obj; }
	void SetWeapon(const std::shared_ptr<EnemyWeapon>& weapon) { wpWeapon = weapon; }

	void SetIsLockOn(bool _isLockOn) { isLockOn = _isLockOn; }
	bool GetIsLockOn() { return isLockOn; }
	 void SetUniqueID(int _uniqueID) { uniqueID = _uniqueID; }
	 int GetUniqueID() { return uniqueID; }
	 void SetStoreID(int _storeID) { storeID = _storeID; }
	 int GetStoreID() { return storeID; }
	void SetPatrolPt(Vector3 _ptA, Vector3 _ptB, Vector3 _ptC) { patrolPt[0] = _ptA; patrolPt[1] = _ptB; patrolPt[2] = _ptC; }

	void SetReturnPoint(Vector2 _point) { returnPoint = _point; }

	//get set hp
	float GetHp() { return hp; }
	void SetHp(float _hp) { hp = _hp; }

	bool isWithinAtkFrame();
	bool isAtkEnable();

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawBright() override;
	void DrawSprite() override;
	void DrawMoji() override;

	void DrawUnLit() override;

	//Init,
	void InitModelInfo(); 
	void InitAnimator();
	void InitCollider();
	void InitTexuture();
	void InitStatus();
	void InitState();

	//SetAnimation
	void SetAnime(string _aniName, bool _isLoop);
	void SetAnime(string _aniName, string _aniName2, bool _isLoop);
	void ResetAnime();

	//Collision
	void SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType);
	void SetCollider(KdCollider::SphereInfo& _sphereObj, Vector3 _sphereCenter, float _sphereRadius, UINT _hitType);

	void SetDebugWire(KdCollider::RayInfo& _rayObj, Color _rayColor = { 1,0,0,1 });
	void SetDebugWire(KdCollider::SphereInfo& _sphereObj, Color _sphereColor = { 1,1,1,1 });

	void HitGround(KdCollider::RayInfo& _rayObj, list<KdCollider::CollisionResult>& _retRayList);

	void GroundCollision();
	void BodyCollision();
	void SectorRaySearch();

	void DmgCollision();

	//Update
	void UpdateMatrix();
	void UpdateAnimation();
	void UpdateRotation(Vector3 _faceDir);
	void UpdateGravity();

	void ColorManager();
	void CounterManager();
	void EnemyManager();

	void EnemyKnockBack();

	void EnemyStateMachine();

	void EnemyIdle();
	void IdleTransition();

	void EnemyChase();
	void ChaseTransition();

	void EnemyAttack();
	void AttackTransition();
	void AtkDelay();
	void AtkMove();

	void EnemyRandomPosition();
	void RandomPositionTransition();

	void EnemyMagicAtk();
	void MagicAtkTransition();

	void EnemyMagicHeal();
	void MagicHealTransition();

	void EnemyPatrol();
	void PatrolTransition();

	void EnemyFollow();
	void FollowTransition();

	void HandleDeath();
	void EnemyDeath();
	void DeathTransition();

	void EnemyStun();
	void StunTransition();

	void EnemySpawn();
	void SpawnTransition();

	void EnemyDizzy();
	void DizzyTransition();




	void EquipWeapon();

	//targetInfo
	void PlayerInfo();
	void CameraInfo();
	void GameInfo();
	void WeaponInfo();

	void CheckFacingPlayer();

	//Effect 
	void EffectManager();

	void EnemyHitStop();

	void HomingToTargetPoint(float pointX, float pointZ);
	void HomingAStartPath(float _targetX, float _targetY);
	vector<PathNode> path;

	void OnGetDmg(Vector3 _hitPos);
	void SpawnMoji();
	void OnHit() override;
	
	void CallImgui() override;

	Vector3 camPos = {};

	bool hasMoji = false;

	string pModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
	shared_ptr<KdModelWork>  pModel = nullptr;


private:

	//Model
	shared_ptr<KdModelWork>  eModel = nullptr;
	shared_ptr<KdModelWork>  equipModel = nullptr;
	shared_ptr<KdModelWork>  lockOnModel = nullptr;
	shared_ptr<KdSquarePolygon> lockPoly = nullptr;
	shared_ptr<KdSquarePolygon>  alertPoly = nullptr;

	string eModelName = "";
	string equipModelName = "";
	string lockOnModelName = "";
	string alertModelName = "";

	//Anime
	shared_ptr<KdAnimator> eAnime;
	shared_ptr<KdAnimator> equipAnime;
	string walkAni = "Running_A";
	string idleAni = "Idle";
	string stunAni = "Hit_B";

	string spawnAni = "Spawn_Ground_Skeletons";
	string deathAni = "Death_B";

	//string atkAni1 = "1H_Melee_Attack_Slice_Diagonal";
	string atkAni1 = "2H_Melee_Attack_Spin";
	string atkAni2 = "1H_Melee_Attack_Chop";
	string atkAni3 = "2H_Melee_Attack_Spin";
	string spellAni = "Spellcasting";

	string getHitAni = "Hit_B";

	string alertAni = "Taunt";

	//weakPtr obj
	weak_ptr<Player> wpPlayer;
	weak_ptr<Enemy> wpEnemy;
	weak_ptr<KdGameObject> wpObj;
	weak_ptr<EnemyWeapon> wpWeapon;

	

	//Matrix
	Matrix scaleMat = {}, rotMat = {}, transMat = {};
	Matrix lockOnMat = {};

	//Status
	int uniqueID = 0;
	int storeID = -99;
	float hp = 100.f;
	float maxHp = 100.f;

	//Drawing
	Color eColor = { 1,1,1,1 };
	float eAlpha = 1.f;
	Vector3 hpBarOffset = { 0,3.5,-1 };
	float hpBarRatio = 1.f;

	//HpBar & Moji
	Vector3 pos2D = {};
	Vector3 hpOffset = { 0,2.5f,0 };
	Math::Rectangle hpBarRect = { 0,0,190,20 };
	Math::Rectangle hpBarFrameRect = { 0,0,190,20 };
	KdTexture hpBarTex;
	KdTexture hpBarFrameTex;

	//Scale
	Vector3 scale = { 1,1,1 };	

	//Rotation
	float rotAngle = 0.f;

	//Animation
	float aniSpd = 1.f;

	//Move & Position
	float moveSpd = 0.077f; //0.1 0.056
	float gravity = 0.01f;
	float gravityPow = 0.01;
	Vector3 moveVec = { 0,0,1 };
	Vector3 backVec = { 0,0,-1 };
	float followSpd = 0.14f;

	//PlayerInfo
	Vector3 playerPos = {};
	Vector3 dirToPlayer = {};
	float distToPlayer = 0.f;
	float angleToPlayer = 0.f;

	//gameInfo

	//Collider info
	float colRadius = 1.f;

	//Patrol
	Vector3 patrolPt[3] = { {},{},{} };
	Vector3 patrolA = {}, patrolB = {} , patrolC = {};
	Vector3 patrolTarget = {};
	Vector3 dirToPatrolTarget = {};
	float distToPatrolTarget = 0.f;
	int patrolOrder = 0;

	Vector3 dirToNextTarget = {}; //astar dir

	//lock-on
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

	//muteki
	float mutekiCnt = 0;

	//Attack
	bool isAttacking = false;
	bool canAtkAnime = false;
	float atkCD = 0;
	int atkOrder = 0;
	float atkFrame = 0;
	float atkReadyFrame = 0;
	float atkEndFrame = 0;
	float atkDmg = 1;
	float atkMoveCnt = 0;

	float atkDelayCnt = 0;
	bool isAtkDelay = false;

	




	//knockBack
	bool canKnockBack = false;
	bool isKnockBack = false;
	float knockBackTimer = 0;
	float knockBackDuration = 0;
	Vector3 knockBackPos = {};
	








	//State
	int nowState = EnemyState::eIdle;
	float stateCnt = 0;

	//StateMachine
	StateMachine stateMachine;

	Vector3 playerMoveVec = {};
	bool isFacingPlayer = false;
	bool isRayHitPlayer = false;
	float canCheckFacingCnt = 0;
	
	//Effect
	int effectId = 0;

	//hpOffset2
	Vector3 hpOffset2 = { 0,0,0 };
	Vector3 pos2Doffset = { 0,0,0 };


	//EnemyFollow
	bool isAlert = false;
	float alertCnt = 0;
	float alertAlpha = 0;
	float followCD = 0;
	Color alertColor = { 7,0,0,1 };


	//EnemyDizzy 
	bool isDizzy = false;
	float dizzyCnt = 0;


	//EnemySpawn
	float spawnCnt = 0;
	bool canSpawn = false;




	//Patrol Logic
	Vector2 enemyPoint = { 0,0 };
	int enemyPointX = 0;
	int enemyPointY = 0;
	Vector3 nextTarget;
	Vector2 returnPoint = { 0,0 };
	float distToReturnPt = 100;

	//Equip Weapon
	bool hasWeapon = false;


	//Random Position
	Vector3 battleRangeMax = { 0,0,0 };
	Vector3 battleRangeMin = { 0,0,0 };

	Vector3 randomTargetPos = { 0,0,0 };

};

