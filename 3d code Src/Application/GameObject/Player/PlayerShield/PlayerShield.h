#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class PlayerShield : public KdGameObject
{
public:
	PlayerShield(){}
	~PlayerShield() override {}

	void SetMatrix(const Matrix& m) { worldMat = m; }
	
	//set modelname
	void SetModelName(const string& name) { modelName = name; }
	void SetAniSpd(float _spd) { aniSpd = _spd; }

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void DrawRenderTarget() override;
	void GenerateDepthMapFromLight() override;

	void ChangeModel(string _modelName);

	void SetAnime(string _aniName, bool _isLoop);


	void CallImgui() override;

private:

	shared_ptr<KdModelWork>  model = nullptr;
	//string modelName = "Asset/Models/_PlayerObj/WarriorShield/WarriorShield.gltf";
	string modelName = "Asset/Models/_WeaponObj/Shield/ShieldNormal.gltf";

	shared_ptr<KdAnimator> anime;

	float aniSpd = 0.4777;

	float gmaeSpd = 1.0f;


};

