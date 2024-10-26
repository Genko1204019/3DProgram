#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class Drop : public KdGameObject
{
public:
	Drop(){}
	~Drop() override {}

	//set moveVec
	void SetMoveVec(Vector3 _moveVec) { moveVec = _moveVec; }

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawBright() override;

	void PostUpdate() override;

	void CallImgui() override;

private:

	shared_ptr<KdModelWork> inModel = nullptr;
	shared_ptr<KdModelWork> outModel = nullptr;

	Vector3 inScale = { 1,1,1 };
	Vector3 outScale = { 1,1,1 };

	Color objColor = { 1,1,1,1 };

	Vector3 moveVec = {};

	float gravity = -0.01f;
	float gravityPow = 0.01;

	float bouncePow = 0.21f;
	float bouncePowDegenerate = 0.03f;

	float moveSpd = 0.28f;
	float moveSpdDegenerate = 0.004f;



};

