#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class PlayerArmor : public KdGameObject
{
public:
	PlayerArmor() {}
	~PlayerArmor() override {}

	//get set wpDef
	float GetWpDef() { return wpDef; }
	void SetWpDef(float _def) { wpDef = _def; }

	void SetMatrix(const Matrix& m) { worldMat = m; }
	//getSet modelName
	void SetModelName(const string& name) { modelName = name; }
	//set anispd
	void SetAniSpd(float _spd) { aniSpd = _spd; }



	void Init() override;
	void Update() override;
	void DrawLit() override;
	void DrawRenderTarget() override;
	void GenerateDepthMapFromLight() override;

	void SetAnime(string _aniName, bool _isLoop);

	void ChangeModel(string _modelName);


	void CallImgui() override;



private:

	shared_ptr<KdModelWork>  model = nullptr;
	string modelName = "";
	shared_ptr<KdAnimator> anime;

	float aniSpd = 0.4777;

	float gamsSpd = 1.0f;

	float wpDef = 10;


	


};

