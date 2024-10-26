#include "PlayerShield.h"

void PlayerShield::Init()
{

	objTag = GameTag::PlayerShieldTag;

	modelName = "Asset/Models/_WeaponObj/Shield/ShieldNormal.gltf";
	model = make_shared<KdModelWork>();
	model->SetModelData(modelName);

	anime = make_shared<KdAnimator>();
	SetAnime("Idle", false);

	debugWire = make_unique<KdDebugWireFrame>();


}

void PlayerShield::Update()
{

	anime->AdvanceTime(model->WorkNodes(), aniSpd * gameSpd);

	gameSpd = GameManager::Instance().GetGameSpd();


}

void PlayerShield::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);

}

void PlayerShield::DrawRenderTarget()
{
	Matrix inventoryPlayerMat = Matrix::CreateScale(1.4) * worldMat * Matrix::CreateTranslation(0, 3.5, 0);
	//KdShaderManager::Instance().m_StandardShader.DrawModel(*model, inventoryPlayerMat);
}

void PlayerShield::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);

}

void PlayerShield::ChangeModel(string _modelName)
{
	modelName = _modelName;
	model->SetModelData(modelName);
	model->CalcNodeMatrices();

}

void PlayerShield::SetAnime(string _aniName, bool _isLoop)
{
	anime->SetAnimation(model->GetData()->GetAnimation(_aniName), _isLoop);

}

void PlayerShield::CallImgui()
{
}
