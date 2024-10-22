#pragma once 
#include"json.hpp"
#include"Application/GameObject/StageObj/StageObj.h"

using namespace std;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;


class StageEditor : public KdGameObject
{
public:

	void Init() override;
	void Update() override;

	void DrawUnLit() override;
	void DrawLit() override;
	void DrawBright() override;

	void DrawStageObj(StageObj& _obj);

	void SaveStageSettings(const std::string& filePath);
	void LoadStageSettings(const std::string& filePath);

	void CallImgui();


	

private:

	string settingPath = "Data/StageSetting.json";


private:
	std::vector<StageObj> stageUnit;


	StageEditor() {}
	~StageEditor() override {}
public:
	static StageEditor& Instance() {
		static StageEditor instance;
		return instance;
	}
};

