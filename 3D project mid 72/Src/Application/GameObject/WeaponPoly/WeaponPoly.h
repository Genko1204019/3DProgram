#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class WeaponPoly : public KdGameObject
{
public:
	WeaponPoly(){}
	~WeaponPoly() override {}

	//get Set polyMat
	void SetPolyMat(const Matrix& _polyMat) { polyMat = _polyMat; }
	Matrix GetPolyMat() { return polyMat; }

	void Init() override;
	void Update() override;
	void DrawUnLit() override;

	void DrawBright() override;

	void CallImgui() override;


private:

	shared_ptr<KdSquarePolygon> poly;

	KdTrailPolygon trail;

	Matrix polyMat = {};

	Color polyColor = { 1,1,1,1 };
	float polyAlpha = 1;

	Matrix scaleMat  = {};

	Vector3 scale = { 1,1,1 };



};

