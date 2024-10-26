#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class PlayerProjectile : public KdGameObject
{
public:
	PlayerProjectile(){}
	~PlayerProjectile() override {}


	//set rot.Y
	void SetRotY(float _rotY) { rot.y = _rotY; }
	
	//movevec
	void SetMoveVec(Vector3 _moveVec) { moveVec = _moveVec; }
	Vector3 GetMoveVec() { return moveVec; }
	//

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void InitFlySlash();
	void InitMagicBall();

	void InitFireBall();


	//void InitTrailPoly();

	void UpdateMatrix();

	void BodyCollision();

	void MoveNormal();
	void MoveMissile();
	void UpdateEffect();

	void HandleDeath();
	void Death();

	void OnHit() override;

	void DrawBright() override;

	void CallImgui() override;



private:

//Model
	shared_ptr<KdModelWork> model = nullptr;

	shared_ptr<KdTrailPolygon> trailPoly = nullptr;

	Matrix scaleMat = {}, rotMat = {}, transMat = {};

	Vector3 scale = {1,1,1}, rot = {};


	Vector3 moveVec = { 1,0.0f,0.0f };

	int effectId = 0;

	float lifeSpan = 210;

	bool isMissile = false;
	bool lifeFlg = true;


	//Beizer
	Math::Vector3 startPt, midPt, controlPt, endPt;

	float gravity = 0.01f;
	float gravityPow = 0.01;

	float bouncePow = 0.21f;
	float bouncePowDegenerate = 0.03f;


	float moveSpd = 0.42f;
	float moveSpdDegenerate = 0.004f;

	int maxPass = 1;
	int wpPass = 0;




};

