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

	float mojiScale = 1;
	Color mojiColor = { 1,1,1,1 };
	float mojiAlpha = 1;


	float dmgNum = 1;

	Vector3 pos2D = { 0,0,0 };
	int lifeSpan = 60;
	Vector3 move = { 0,0,0 };

	Vector3 mojiOffset = { 0,1,0 };
	Vector3 mojiDir = { 1,1,0 };
	int plusMinus = 0;

	float gravity = 0.01f;


	float timer = 0;

	string mojiPrefix = "を手に入れました";
	string mojiText = "Hello";
	Vector2 mojiPos = { 0,0 };

	KdTexture tex;
	Math::Rectangle messaageRect = { 0,0,1280,250 };

private:

	




};

