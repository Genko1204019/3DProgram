#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

class EnemyWeapon : public KdGameObject
{
public:
	EnemyWeapon() {}
	~EnemyWeapon() override {}

	//get set scale 
	Vector3 GetWpScale() { return wpScale; }
	void SetWpScale(Vector3 _scale) { wpScale = _scale; }

	//set wpModelName
	void SetModelName(string _name) { wpModelName = _name; }

	//setEnemyPos get
	void SetEnemyPos(Vector3 _pos) { enemyPos = _pos; }
	Vector3 GetEnemyPos() { return enemyPos; }

	//get set anispd
	float GetAniSpd() { return aniSpd; }
	void SetAniSpd(float _spd) { aniSpd = _spd; }

	//set worldMat
	void SetMatrix(const Matrix& m) { worldMat = m; }

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void SetAnime(string _aniName, bool _isLoop);

	void GameInfo();

	void SmashEffect();

	void CallImgui() override;

	bool isAtkEnable = false;

	Vector3 enemyPos = {};

	Matrix enemyRotMat = {};

private:

	shared_ptr<KdModelWork>  wpModel = nullptr;
	string wpModelName = "Asset/Models/_EnemyObj/Fighter/FighterWp/FighterWp.gltf";
	shared_ptr<KdAnimator> wpAnime;

	float aniSpd = 0.4777;

	Vector3 wpPos = { 0,0,0 };

	Vector3 wpScale = { 1,1,1 };



};

