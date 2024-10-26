#include"EnemyProjectile.h"
#include"Application/GameObject/Player/Player.h"

void EnemyProjectile::Init()
{
	
	

}

void EnemyProjectile::Update()
{

	UpdateMatrix();
	UpdateMagicBallAtk();;
	UpdateMagicRangeAtk();
	UpdateSlahWaveAtk();

	lifeSpan--;
	if (lifeSpan < 0) Death();
	

}

void EnemyProjectile::PostUpdate()
{
	if (objType == eProjectileType::MagicType || objType == eProjectileType::SlashWaveType) {
		BodyCollision();
	}


}

void EnemyProjectile::DrawLit()
{
	
	


	//KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);



}

void EnemyProjectile::GenerateDepthMapFromLight()
{
}

void EnemyProjectile::InitMagicBallAtk()
{
	objTag = GameTag::EnemyProjectileTag;
	objType = eProjectileType::MagicType;

	effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("infinHealPurple.efkefc",pos,0.42,1,false);

	model = make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/_ProjectileObj/HItBox/magicBall.gltf");

	lifeSpan = 770;

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("ProjectileCollision", model, KdCollider::TypeDamage);

	debugWire = make_unique<KdDebugWireFrame>();


}

void EnemyProjectile::InitRangeMagicAtk()
{
	objTag = GameTag::EnemyProjectileTag;
	objType = eProjectileType::MagicRangeType;

	outerRange = targetRange;
	atkRange = 0;

	lifeSpan = 210;


}

void EnemyProjectile::InitSlashWaveAtk()
{
	objTag = GameTag::EnemyProjectileTag;
	objType = eProjectileType::SlashWaveType;

	lifeSpan = 140;

	effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("slashWave2.efkefc", pos + Vector3(0,-0.7,0), 1, 1, true);

	model = make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/_ProjectileObj/HItBox/BossSlashWaveBox.gltf");

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("ProjectileCollision", model, KdCollider::TypeDamage);

}

void EnemyProjectile::BodyCollision()
{
	KdCollider::SphereInfo bodySphere(KdCollider::TypeGround, pos, 0.7);
	std::list<KdCollider::CollisionResult> retBodyList;
	//debugWire->AddDebugSphereByType(bodySphere);

	float overlap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;
	Vector3 hitPos;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::StageTag) {
			if (obj->Intersects(bodySphere, &retBodyList)) {
				Utility::CalOverlap(retBodyList, overlap, isHit, hitPos, hitDir);
			
			}
		}
		
	}

	if (isHit) {
		Death();
	}

	


}

void EnemyProjectile::Death()
{
	SetExpired();
	KdEffekseerManager::GetInstance().Play("slashParticle2.efkefc", pos, 1, 1, false);
	KdEffekseerManager::GetInstance().StopEffectById(effectId);

	KdAudioManager::Instance().PlayMagicExplosionSE();
}

void EnemyProjectile::UpdateMatrix()
{
	if (objType == eProjectileType::SlashWaveType) {
		DirectX::SimpleMath::Vector3 forwardVec(0.0f, 0.0f, 1.0f);
		moveVec.Normalize();
		float angle = atan2(moveVec.x, moveVec.z);
		float offsetAngle = angle + DirectX::XM_PI / 2.0f;

		Matrix scaleMat = Matrix::CreateScale(4.9, 0.7, 7);
		Matrix rotMat = Matrix::CreateRotationY(offsetAngle);
		Matrix transMat = Matrix::CreateTranslation(pos);
		worldMat = scaleMat * rotMat * transMat;
	}
	else {
		Matrix scaleMat = Matrix::CreateScale(0.71f);
		Matrix transMat = Matrix::CreateTranslation(pos);
		worldMat = scaleMat * transMat;
	}
}

void EnemyProjectile::UpdateMagicBallAtk()
{
	if (objType != eProjectileType::MagicType) return;

	preLaunchCnt--;
	if (isOriginSet) launchCnt--;

	if (!isOriginSet && preLaunchCnt < 0) {
		Vector3 toOrigin = originPos - pos;
		toOrigin.Normalize();
		pos += toOrigin * moveSpdOrigin;
		float length = (originPos - pos).Length();

		if (length < 0.1) isOriginSet = true;

	}

	if (isOriginSet && launchCnt < 0) {

		if (!isTargetSet) {
			targetPos = wpPlayer.lock()->GetPosition() + Vector3(0,0.7,0);
			isTargetSet = true;
		}

		Vector3 toTarget = targetPos - pos;
		toTarget.Normalize();
		pos += toTarget * moveSpdAtk;
		float length = (targetPos - pos).Length();
		if (length < 0.28) arriveCnt++;
		if (arriveCnt > 21) Death();

	}

	KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(effectId), pos);


}

void EnemyProjectile::UpdateMagicRangeAtk()
{
	if (objType != eProjectileType::MagicRangeType) return;

	if (atkRange < targetRange) atkRange += 0.04;
	KdShaderManager::Instance().WorkAmbientController().AddConeLight(pos + Vector3(0,2.1,0), {0,0,0}, targetRange, atkRange, {3,3,3});

	//if (atkRange >= targetRange) 	SetExpired();


}

void EnemyProjectile::UpdateSlahWaveAtk()
{
	if (objType != eProjectileType::SlashWaveType) return;

	//move toward moveVec
	pos += moveVec * 0.56
		
		;

	Vector3 forward = { 1.0f, 0.0f, 0.0f };

	Vector3 moveDirection = moveVec;
	moveDirection.Normalize();

	float dot = forward.Dot(moveDirection);
	float angle = acosf(dot);

	Vector3 axis = forward.Cross(moveDirection);
	if (axis.LengthSquared() > 0.0f) {
		axis.Normalize();
	}

	Vector3 posOffset = pos + Vector3(0, -0.7, 0);
	KdEffekseerManager::GetInstance().SetRotationById(effectId, axis, angle);
	KdEffekseerManager::GetInstance().SetPosById(effectId, posOffset);


}

void EnemyProjectile::OnHit()
{
	Death();

}

void EnemyProjectile::DrawBright()
{
}

void EnemyProjectile::CallImgui()
{
	ImGui::Begin("EnemyProjectile");

	ImGui::Text("pos : %f, %f, %f", pos.x, pos.y, pos.z);
	//show objType
	ImGui::Text("objType : %d", objType);
	ImGui::End();








	

}
