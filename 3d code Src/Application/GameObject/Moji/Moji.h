//Moji.h file

#pragma once 


using namespace std;
using namespace DirectX::SimpleMath;

class Moji : public KdGameObject
{
public:
	Moji(){}
	~Moji() override {}

	//get set dmgNum
	void SetDmgNum(float _dmgNum) { dmgNum = _dmgNum; }
	float GetDmgNum() { return dmgNum; }

	//set mojiText mojiPos	
	void SetMojiText(string _mojiText) { mojiText = _mojiText; }
	void SetMojiPos(Vector2 _mojiPos) { mojiPos = _mojiPos; }

	void Init() override;
	void Update() override;
	void DrawSprite() override;
	void DrawMoji() override;

	void InitDmgMoji();
	void InitMessageMoji();

	void UpdateDmgMoji();
	void UpdateMessageMoji();

	void DrawDmgMoji();
	void DrawMessageMoji();

	void CallImgui() override;

	

private:
	KdTexture tex;

	Color mojiColor = { 1,1,1,1 };
	float mojiAlpha = 1;

	Vector3 pos2D = { 0,0,0 };
	Vector2 mojiPos = { 0,0 };

	Vector3 mojiOffset = { 0,1,0 };
	Vector3 mojiDir = { 1,1,0 };
	string mojiText = "Hello";

	float dmgNum = 1;
	int lifeSpan = 60;




};

