#include"PlayerWeapon.h"
#include"Application/Scene/SceneManager.h"

void PlayerWeapon::Init()
{

	objTag = GameTag::PlayerWeaponTag;

	hitBoxModel = make_shared<KdModelWork>();
	hitBoxModel->SetModelData(hitBoxModelName);
	
	wpModel = make_shared<KdModelWork>();
	wpModel->SetModelData(wpModelName);
	wpAnime = make_shared<KdAnimator>();
	
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("PlayerWeaponCollision", wpModel, KdCollider::TypeDamage);

	tPoly.SetMaterial("Asset/Textures/sword_effect03.png");
	tPoly.SetLength(49);
	tPoly.SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);

	debugWire = std::make_unique<KdDebugWireFrame>();

	SetAnime("Idle", false);


}

void PlayerWeapon::Update()
{

	if (GameManager::Instance().GetIsInventoryOpen()) {
		wpAnime->AdvanceTime(wpModel->WorkNodes(), aniSpd);
	}
	else {
		wpAnime->AdvanceTime(wpModel->WorkNodes(), aniSpd * gameSpd);
	}

	Matrix matt = Matrix::CreateTranslation(worldMat.Translation());


	//const KdModelWork::Node* _pNode = wpModel->FindWorkNode("AttachPoint2"); 

	//Vector3 swordPos = _pNode->m_worldTransform.Translation();
	////Matrix swordWorldMat = Matrix::CreateScale(2.1,1,1)* wpRotMat* Matrix::CreateTranslation(worldMat.Translation() + swordPos + polyOffset);
	//Matrix swordWorldMat = Matrix::CreateScale(2.1, 1, 1) * wpRotMat * _pNode->m_worldTransform * Matrix::CreateTranslation(worldMat.Translation()  + polyOffset);
	//newSwordMat = _pNode->m_worldTransform * worldMat;

	//tPoly.AddPoint(Matrix::CreateScale(3, 0.1, 0.1) * newSwordMat);


	if (isWithAtkFrame) {

		if (slashEff) {
			slashEff = false;
			slashEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
			
			/*switch (playerAtkOrder)
			{
			case 1:
				KdEffekseerManager::GetInstance().Play("blueLeftSlash.efkefc", playerAtkMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 2:
				KdEffekseerManager::GetInstance().Play("blueRightSlash.efkefc", playerAtkMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 3:
				KdEffekseerManager::GetInstance().Play("blueDownSlash.efkefc", playerAtkMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 4:
				KdEffekseerManager::GetInstance().Play("blueRightSlash.efkefc", playerAtkMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			default:
				break;
			}*/

		}

	}
	
	

	//Vector3 forward = { -1.0f, 0.0f, 0.0f };

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

	//KdShaderManager::Instance().m_StandardShader.DrawModel(*hitBoxModel, worldMat);

	//KdShaderManager::Instance().m_StandardShader.DrawPolygon(tPoly);

	if (isWithAtkFrame) {

	}

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

//	//imgu begin pWeapon
//	ImGui::Begin("PlayerWeapon");
//
//	//isWithAtkFrame
//ImGui::Checkbox("isWithAtkFrame", &isWithAtkFrame);
//
//	//slider float forward x y z
//ImGui::SliderFloat("forward.x", &forward.x, -1.0f, 1.0f);
//	ImGui::SliderFloat("forward.y", &forward.y, -1.0f, 1.0f);
//	ImGui::SliderFloat("forward.z", &forward.z, -1.0f, 1.0f);
//
//	//imgui end
//	ImGui::End();
//
//	//show aniSp
//	ImGui::SliderFloat("WeaponAniSpeed", &aniSpd, 0.0f, 2.0f);
//
//	//slider float polyOffset
//	ImGui::SliderFloat("PolyOffsetX", &polyOffset.x, -10.0f, 10.0f);
//	ImGui::SliderFloat("PolyOffsetY", &polyOffset.y, -10.0f, 10.0f);
//	ImGui::SliderFloat("PolyOffsetZ", &polyOffset.z, -10.0f, 10.0f);


}
