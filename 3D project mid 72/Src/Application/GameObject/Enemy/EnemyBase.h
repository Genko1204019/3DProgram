#pragma	once

using namespace std;
using namespace DirectX::SimpleMath;

class Player;
class EnemyWeapon;

class EnemyBase : public KdGameObject
{
public:
	EnemyBase() {}
	~EnemyBase() override {}

	int GetUniqueID() { return uniqueID; }

	//get set nowState
	int GetNowState() { return nowState; }
	void SetNowState(int _state) { nowState = _state; }

	void SetPlayer(const std::shared_ptr<Player>& player) { wpPlayer = player; }
	void SetEnemy(const std::shared_ptr<BattleEnemy>& enemy) { wpEnemy = enemy; }
	void SetWeapon(const std::shared_ptr<EnemyWeapon>& weapon) { wpWeapon = weapon; }

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType);
	void SetCollider(KdCollider::SphereInfo& _sphereObj, Vector3 _sphereCenter, float _sphereRadius, UINT _hitType);
	void SetDebugWire(KdCollider::RayInfo& _rayObj, Color _rayColor)
	{
		if (GameManager::Instance().GetCanDebugWire())  debugWire->AddDebugLine(_rayObj.m_pos, _rayObj.m_pos + _rayObj.m_dir * _rayObj.m_range, _rayColor);
	}

	void InitModelInfo();
	void InitAnimator();
	void InitCollider();
	void InitStatus();

	void UpdateMatrix();
	void UpdateAnimation();
	void UpdateGravity();
	void UpdateRotation(Vector3 _faceDir);
	void UpdateHpRotation();

	void GroundCollision();
	void BodyCollision();

	void CheckFacingPlayer();

	void EnemyCounterManager();
	void UpdateStateMachine();

	void EquipWeapon();

	void SetAnime(string _aniName, bool _isLoop);

	void PlayerInfo();
	void GameInfo();
	virtual void CameraInfo() {}
	virtual void WeaponInfo() {}
protected:

	shared_ptr<KdModelWork>  eModel = nullptr;
	shared_ptr<KdModelWork>  equipModel = nullptr;

	shared_ptr<KdModelWork> testModel = nullptr;

	string eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
	string equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
	string wpModelName = "Asset/Models/_EnemyObj/Fighter/FighterWp/SkeWarAxe.gltf";


	shared_ptr<KdAnimator> eAnime;
	shared_ptr<KdAnimator> equipAnime;

	string wanderAni = "Walking_D_Skeletons";
	string walkAni = "Running_A";
	string idleAni = "Idle";
	string stunAni = "Hit_B";

	string spawnAni = "Spawn_Ground_Skeletons";
	string deathAni = "Death_B";

	string atkAni1 = "2H_Melee_Attack_Spin";
	string atkAni2 = "1H_Melee_Attack_Chop";
	string atkAni3 = "2H_Melee_Attack_Spin";
	string spellAni = "Spellcasting";

	string getHitAni = "Hit_B";
	string alertAni = "Taunt";

	string backJumpAni = "Taunt";

	//string blowAwayAni = "Sit_Floor_Idle";
	//string blowAwayAniStand = "Sit_Floor_StandUp";

	string blowAwayAni = "Lie_Idle";
	string blowAwayAniStand = "Lie_StandUp";


	weak_ptr<Player> wpPlayer;
	weak_ptr<BattleEnemy> wpEnemy;
	weak_ptr<KdGameObject> wpObj;
	weak_ptr<EnemyWeapon> wpWeapon;

	//Matrix
	Matrix scaleMat = {}, rotMat = {}, rotMatX,transMat = {};

	Vector3 scale = { 1.77,1.77,1.77 };
	float rotAngle = 0.f;
	float rotAngleZ = 0.f;
	float hpRotAngle = 0;

	//Status
	int uniqueID = 0;
	int storeID = -99;
	float hp = 100.f;
	float maxHp = 100.f;

	//Drawing
	Color eColor = { 1,1,1,1 };
	float eAlpha = 1.f;

	//Animation
	float aniSpd = 1.f;

	//Move & Position
	float moveSpd = 0.14f; //0.1 0.056 //0.077
	float gravity = 0.01f;
	float gravityPow = 0.01;
	Vector3 moveVec = { 0,0,1 };
	Vector3 backVec = { 0,0,-1 };

	//PlayerInfo
	Vector3 playerPos = {};
	Vector3 dirToPlayer = {};
	float distToPlayer = 0.f;
	float angleToPlayer = 0.f;

	Vector3 playerMoveVec = {};
	bool isFacingPlayer = false;

	//State
	int nowState = EnemyState::eIdle;
	float stateCnt = 0;
	float mutekiCnt = 0;

	float canCheckFacingCnt = 0;

	//StateMachine
	StateMachine stateMachine;

	//Emmis
	Vector3 emitRate = { 1,1,1 };


};