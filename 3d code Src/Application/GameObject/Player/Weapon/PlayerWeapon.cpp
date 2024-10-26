#include"PlayerWeapon.h"
#include"Application/Scene/SceneManager.h"

void PlayerWeapon::Init()
{

	objTag = GameTag::PlayerWeaponTag;

	wpModel = make_shared<KdModelWork>();
	wpModel->SetModelData(wpModelName);
	wpAnime = make_shared<KdAnimator>();
	
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("PlayerWeaponCollision", wpModel, KdCollider::TypeDamage);

	debugWire = std::make_unique<KdDebugWireFrame>();

	SetAnime("Idle", false);


}

void PlayerWeapon::Update()
{
	//WpAnime
	if (GameManager::Instance().GetIsInventoryOpen()) wpAnime->AdvanceTime(wpModel->WorkNodes(), aniSpd);
	else wpAnime->AdvanceTime(wpModel->WorkNodes(), aniSpd * gameSpd);
	

	if (isWithAtkFrame) {

		if (slashEff) {
			slashEff = false;
			slashEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;

		}

	}
	
	Vector3 moveDirection = atkVec;
	moveDirection.Normalize();

	float dot = forward.Dot(moveDirection);
	float angle = acosf(dot); 

	Vector3 axis = forward.Cross(moveDirection);
	if (axis.LengthSquared() > 0.0f) {
		axis.Normalize(); 
	}
	//else {
	//	axis = { 0.0f, 1.0f, 0.0f }; // Default to Y-axis as fallback
	//}

	KdEffekseerManager::GetInstance().SetRotationById(slashEffId, axis, angle);
	KdEffekseerManager::GetInstance().SetSpeedById(slashEffId, 0.7*gameSpd);

	KdCollider::SphereInfo sphere(KdCollider::TypeDamage, newSwordMat.Translation(), 0.7);
	//debugWire->AddDebugSphereByType(sphere);


	GameInfo();

}

void PlayerWeapon::PostUpdate()
{

	

}

void PlayerWeapon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*wpModel, worldMat);

}

void PlayerWeapon::DrawRenderTarget()
{
	Matrix 	inventoryPlayerMat = Matrix::CreateScale(2.1) * Matrix::CreateTranslation(worldMat.Translation()) * Matrix::CreateTranslation(0, 3.5, 0);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*wpModel, inventoryPlayerMat);

}

void PlayerWeapon::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*wpModel, worldMat);

}

void PlayerWeapon::SetAnime(string _aniName, bool _isLoop)
{
	wpAnime->SetAnimation(wpModel->GetData()->GetAnimation(_aniName), _isLoop);
}

void PlayerWeapon::ResetWeapon(int _wpId, int _skillId, string _modelPath)
{
	wpModelName = _modelPath;
	wpId = _wpId;
	skillId = _skillId;

	wpModel->SetModelData(wpModelName);
	wpModel->CalcNodeMatrices();

}

void PlayerWeapon::GameInfo()
{
	gameSpd	 = GameManager::Instance().GetGameSpd();
}



void PlayerWeapon::CallImgui()
{


}
