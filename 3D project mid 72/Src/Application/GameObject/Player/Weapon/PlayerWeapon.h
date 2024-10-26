#pragma once

using namespace std;
using namespace DirectX::SimpleMath;


class PlayerWeapon : public KdGameObject
{
public:
	PlayerWeapon(){}
	~PlayerWeapon() override {}

	//get set wpDmg
	float GetWpDmg() { return wpDmg; }
	void SetWpDmg(float _dmg) { wpDmg = _dmg; }

	//get set wpId skillId
	int GetWpId() { return wpId; }
	void SetWpId(int _wpId) { wpId = _wpId; }

	int GetSkillId() { return skillId; }
	void SetSkillId(int _skillId) { skillId = _skillId; }

	//set worldMat
	void SetMatrix(const Matrix& m) { worldMat = m; }

	//get set anisp
	float GetAniSpd() { return aniSpd; }
	void SetAniSpd(float _spd) { aniSpd = _spd; }

	//get set isPlayerAttacking
	bool GetIsPlayerAttacking() { return isPlayerAttacking; }
	void SetIsPlayerAttacking(bool _isAttacking) { isPlayerAttacking = _isAttacking; }

	//get set wpRotMat
	Matrix GetWpRotMat() { return wpRotMat; }
	void SetWpRotMat(Matrix _rotMat) { wpRotMat = _rotMat; }

	void Init() override;
	void Update() override;
	void PostUpdate() override;
	void DrawLit() override;
	void DrawRenderTarget() override;
	void GenerateDepthMapFromLight() override;

	void SetAnime(string _aniName, bool _isLoop);

	void ResetWeapon(int _wpId, int _skillId, string _modelPath);

	void GameInfo();


	void CallImgui() override;

	bool slashEff = false;
	int slashEffId = 0;

	Vector3 atkVec = {};

	Vector3 forward = {1,0,0};
	Matrix playerWorldMat = {};

	bool isWithAtkFrame = false;





private:

	shared_ptr<KdModelWork>  wpModel = nullptr;

	string wpModelName = "Asset/Models/_WeaponObj/Sword/SwordNormal.gltf";

	shared_ptr<KdAnimator> wpAnime;

	float aniSpd = 0.4777;

	bool isPlayerAttacking = false;

	Matrix wpRotMat = {};
	Matrix newSwordMat = {};

	int wpId = 1; //for classing combo set 
	int skillId = 1; //for classing skill

	float wpDmg = 10;




};

