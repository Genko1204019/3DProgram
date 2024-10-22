#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class Player;

class EnemyProjectile : public KdGameObject
{
public:
	EnemyProjectile(){}
	~EnemyProjectile() override {}

	void SetOriginPos(Vector3 _originPos) { originPos = _originPos; }
	//set preLaunchCnt
	void SetPreLaunchCnt(float _preLaunchCnt) { preLaunchCnt = _preLaunchCnt; }
	//set launchCnt
	void SetLaunchCnt(float _launchCnt) { launchCnt = _launchCnt; }
	//set player
	void SetPlayer(const std::shared_ptr<Player>& player) { wpPlayer = player; }

	//set movevec
	void SetMoveVec(Vector3 _moveVec) { moveVec = _moveVec; }


	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawBright() override;

	void InitMagicBallAtk();
	void InitRangeMagicAtk();
	void InitSlashWaveAtk();

	void BodyCollision();

	void Death();

	void UpdateMatrix();
	void UpdateMagicBallAtk();
	void UpdateMagicRangeAtk();
	void UpdateSlahWaveAtk();

	void OnHit() override;


	void CallImgui() override;

private:

	shared_ptr<KdModelWork>  model = nullptr;

	Vector3 moveVec = { 1,0.0f,0.0f };

	float moveSpdOrigin = 0.1f;
	float moveSpdAtk = 0.42;

	int effectId = 0;

	Vector3 originPos = { 0,0,0 };
	Vector3 targetPos = { 0,0,0 };

	bool isOriginSet = false;
	bool isTargetSet = false;

	float preLaunchCnt = 0;
	float launchCnt = 0;

	float lifeSpan = 0;

	//weakPtr player
	weak_ptr<Player> wpPlayer;

	//magic range atk
	float outerRange = 0;
	float atkRange = 0;
	float targetRange = 7;

	float arriveCnt = 0;

};

