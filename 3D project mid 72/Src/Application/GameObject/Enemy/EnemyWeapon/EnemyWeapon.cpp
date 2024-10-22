#include"EnemyWeapon.h"

void EnemyWeapon::Init()
{

	objTag = GameTag::EnemyWeaponTag;

	wpModel = make_shared<KdModelWork>();
	wpModel->SetModelData(wpModelName);

	wpAnime = make_shared<KdAnimator>();

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("EnemyWeaponCollision", wpModel, KdCollider::TypeDamage);


	debugWire = std::make_unique<KdDebugWireFrame>();

}

void EnemyWeapon::Update()
{
	wpAnime->AdvanceTime(wpModel->WorkNodes(), aniSpd * gameSpd);

	wpPos = worldMat.Translation();

	GameInfo();

	/*const KdModelWork::Node* _pNode = wpModel->FindWorkNode("AttachPoint1");
	Vector3 swordPos = _pNode->m_worldTransform.Translation();
	Matrix swordWorldMat =  enemyRotMat * _pNode->m_worldTransform * Matrix::CreateTranslation(worldMat.Translation());
	Matrix newSwordMat = _pNode->m_worldTransform * worldMat;
	debugWire->AddDebugSphere(newSwordMat.Translation(), 3, { 1,0,0,1 });*/

}

void EnemyWeapon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*wpModel, Matrix::CreateScale(wpScale) * worldMat);

}

void EnemyWeapon::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*wpModel, Matrix::CreateScale(wpScale) * worldMat);

}

void EnemyWeapon::SetAnime(string _aniName, bool _isLoop)
{
	wpAnime->SetAnimation(wpModel->GetData()->GetAnimation(_aniName), _isLoop);

}

void EnemyWeapon::GameInfo()
{
	gameSpd = GameManager::Instance().GetGameSpd();
}

void EnemyWeapon::SmashEffect()
{
	const KdModelWork::Node* _pNode = wpModel->FindWorkNode("AttachPoint1");
	Vector3 swordPos = _pNode->m_worldTransform.Translation();
	Matrix swordWorldMat = enemyRotMat * _pNode->m_worldTransform * Matrix::CreateTranslation(worldMat.Translation());
	Matrix newSwordMat = _pNode->m_worldTransform * worldMat;

	KdEffekseerManager::GetInstance().Play("BossHitGround.efkefc", { newSwordMat.Translation() + Vector3(0,-0.35,0)}, 2.1, 0.56, false);


}

void EnemyWeapon::CallImgui()
{
	
}
