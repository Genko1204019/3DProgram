#pragma once
#include"Application/GameObject/Enemy/EnemyBase.h"

using namespace std;
using namespace DirectX::SimpleMath;

class WanderEnemy : public EnemyBase
{
public:
	WanderEnemy(){}
	~WanderEnemy() override {}

	void SetPatrolPt(Vector3 _ptA, Vector3 _ptB, Vector3 _ptC) { patrolPt[0] = _ptA; patrolPt[1] = _ptB; patrolPt[2] = _ptC; }


	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawMini() override;

	void InitWanderEnemy();

	void WeaponInfo() override;

	void DmgCollision();

	void UpdateWanderCounter();
	void UpdateEmmit();
	void UpdatePatrolPoint();

	//Action
	void EnemyPatrol();
	void PatrolTransition();

	void EnemyFollow();
	void FollowTransition();

	void EnemyDizzy();
	void DizzyTransition();
	void ExitDizzy();

	void WanderDead();
	void WanderDeadTransition();

	void HomingToTargetPoint(float pointX, float pointZ);
	void HomingAStartPath(float _targetX, float _targetY);

	void OnGetDmg();

	void CallImgui() override;



private:

	shared_ptr<KdSquarePolygon>  alertPoly = nullptr;



	//EnemyFollow
	bool isAlert = false;
	float alertCnt = 0;
	float alertAlpha = 0;
	float followCD = 0;
	Color alertColor = { 7,0,0,1 };

	//EnemyDizzy 
	bool isDizzy = false;
	float dizzyCnt = 0;
	int dizzyeffId = 0;

	//Patrol Logic
	Vector2 enemyPoint = { 0,0 };
	int enemyPointX = 0;
	int enemyPointY = 0;
	Vector3 nextTarget;
	Vector2 returnPoint = { 0,0 };
	float distToReturnPt = 100;

	//Patrol
	Vector3 patrolPt[3] = { {},{},{} };
	Vector3 patrolA = {}, patrolB = {}, patrolC = {};
	Vector3 patrolTarget = {};
	Vector3 dirToPatrolTarget = {};
	float distToPatrolTarget = 0.f;
	int patrolOrder = 0;
	Vector3 dirToNextTarget = {};
	vector<PathNode> path;

	float followSpd = 0.14f;



	//MiniMap
	shared_ptr<KdTexture> enemyIconTex = nullptr;
	shared_ptr<KdTexture> enemyIconAtkTex = nullptr;
	float xOffset = -500;
	float yOffset = 200;
	float scaleFactor = 28.0f / 6.0f;
	Vector2 miniMapPos2D = { 0,0 };

	bool timeToDrop = false;

	bool giveUpFollow = false;

};

