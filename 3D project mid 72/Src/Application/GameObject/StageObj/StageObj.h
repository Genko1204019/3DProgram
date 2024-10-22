#pragma once
#include"json.hpp"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;

enum objDrawTag
{
	noDrawtags = 0,
	Lit = 1,
	UnLit = 2,
	Bright = 3,

};

enum StageObjTag
{
	noTagObj = 0,
	FixedObj,
	FadeObj,
	MoveObj,
	BattleObj,

};

class StageObj : public KdGameObject
{
public:
	StageObj() {}
	~StageObj() override 
	{
	}

	// Copy constructor
	StageObj(const StageObj& other) {
		*this = other;
	}

	// Copy assignment operator
	StageObj& operator=(const StageObj& other) {
		if (this != &other) {
			objName = other.objName;
			modelPath = other.modelPath;
			pos = other.pos;
			scale = other.scale;
			rot = other.rot;
			objColor = other.objColor;
			canFade = other.canFade;
			canCollide = other.canCollide;
			hasAnime = other.hasAnime;
			drawTag = other.drawTag;
			stageTag = other.stageTag;

			// Deep copy the model
			if (other.model) {
				model = std::make_shared<KdModelWork>(*other.model);
			}
			else {
				model.reset(); // Reset to nullptr if no model
			}
		}
		return *this;
	}

	
	void Init() override;

	void Update() override;

	void DrawLit() override;
	void GenerateDepthMapFromLight() override;
	void DrawUnLit() override;
	void DrawBright() override;

	void InitWithStageObj(StageObj sobj);

	void SetCollider();

	void CallImgui();

	shared_ptr<KdModelWork> model;

	shared_ptr<KdAnimator> anime;

	string objName;
	string modelPath;

	Vector3 pos;
	Vector3 scale;
	Vector3 rot;

	Color objColor = {1,1,1,1};

	bool canFade = false;
	bool canCollide = false;
	bool hasAnime = false;

	int drawTag = 0;
	int stageTag = 0;

	Vector3 emissive = { 0,0,0 };



private:

	
	

};


void to_json(json& j, const StageObj& obj);

void from_json(const json& j, StageObj& obj);



