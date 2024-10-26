#pragma once

class Player;

class MapObj : public KdGameObject
{
public:
	MapObj() {}
	~MapObj() override {}

	//set get pos2D
	//void SetPos2D(const Vector2& pos) { pos2D = pos; }
	//Vector2 GetPos2D() { return pos2D; }

	//setIsWalkable
	void SetIsWalkable(bool _isWalkable) { isWalkable = _isWalkable; }

	//setPlayer
	void SetPlayer(const shared_ptr<Player>& player) { wpPlayer = player; }


	//set roty

	void SetRotY(int _rotY) { rotY = _rotY; }

	//set rotangle
	void SetRotAngle(int _rotAngle) { rotAngle = _rotAngle; }


	void Init() override;
	void Update() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void DrawUnLit() override;

	void PostUpdate() override;

	void DrawBright() override;	

	void DrawSprite() override;

	void DrawMini() override;


	void InitMapInfo();
	void InitModelInfo();
	void InitCollider();
	void InitMatrix();

	void InitNPC();

	void BottleCollision();

	void UpdateAnimation();
	void AddPointLight();

	void CalDistPlayerToNPC();

	void CameraInfo();

	void ClearFireEff();



	void OnHit() override;



	void CallImgui() override;

	bool isSpawn = false;

	

private:

	shared_ptr<KdTexture> iconTex;

	shared_ptr<KdModelWork>  model = nullptr;
	shared_ptr<KdModelWork>  subModel = nullptr;

	shared_ptr<KdAnimator> anime;

	weak_ptr<Player> wpPlayer;

	shared_ptr<KdTexture> miniMapTex;
	Math::Rectangle miniMapRect = { 0,0,8,8 };

	shared_ptr<KdTexture> coverTex;
	Math::Rectangle coverRect = { 0,0,8,8 };

	shared_ptr<KdSquarePolygon> chestPoly;


	string modelName = "";

	Matrix scaleMat = {}, rotMat = {}, transMat = {};
	Vector3 scale = { 1,1,1 };
	//Vector2 pos2D = { 0,0 };
	Vector3 texPos2D = { 0,0,0 };

	Color objColor = { 1,1,1,1 };
	float objAlpha = 1.0f;

	//MiniMap
	Color coverColor = { 1,1,1,1 };
	float coverAlpha = 1.0f;

	float xOffset = -500; //300
	float yOffset = 200; //100 //42
	float scaleFactor = 28.0f / 6.0f; // 32 / 6 : 32 is 2d box size(16*2 each rectangle chip) , 6 is 3d box size (unchnage)

	bool isTransparent = false;

	Vector2 miniMapPos2D = { 0,0 };

	Color miniMapColor = { 1,1,1,1 };
	float miniMapAlpha = 1.0f;
	//float rotY = 0;

	bool isWalkable = false;
	bool hasModel = false;
	bool hasAnime = false;
	bool hasTriger = false;
	bool hasPlayEff = false;

	int rotY = -1;
	int rotAngle = 0;

	float aniSpd = 1;
	float triggerResetCnt = 100;


	Vector3 posOffset = {}; //to adjust spike pos

	bool canTriggerDialog = false;
	float dist = 3;

	bool isfireEff1 = false;
	bool isfireEff2 = false;

	int fireEff1Id = 0;
	int fireEff2Id = 0;


};
