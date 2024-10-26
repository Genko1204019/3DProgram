#include"PlayerArmor.h"

void PlayerArmor::Init()
{
	objTag = GameTag::PlayerArmorTag;

	model = make_shared<KdModelWork>();
	model->SetModelData(modelName);

	anime = make_shared<KdAnimator>();

	debugWire = make_unique<KdDebugWireFrame>();


	

}

void PlayerArmor::Update()
{
	if (GameManager::Instance().GetIsInventoryOpen()) {
		anime->AdvanceTime(model->WorkNodes(), aniSpd);
	}
	else {
		anime->AdvanceTime(model->WorkNodes(), aniSpd * gameSpd);
	}

	gameSpd = GameManager::Instance().GetGameSpd();



}

void PlayerArmor::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);

}

void PlayerArmor::DrawRenderTarget()
{
	Matrix inventoryPlayerMat = Matrix::CreateScale(2.1) * Matrix::CreateTranslation(worldMat.Translation()) * Matrix::CreateTranslation(0, 3.5, 0);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, inventoryPlayerMat);
}

void PlayerArmor::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);

}

void PlayerArmor::SetAnime(string _aniName, bool _isLoop)
{
	anime->SetAnimation(model->GetData()->GetAnimation(_aniName), _isLoop);

}

void PlayerArmor::ChangeModel(string _modelName)
{
	modelName = _modelName;
	model->SetModelData(modelName);
	model->CalcNodeMatrices();
}

void PlayerArmor::CallImgui()
{
}
