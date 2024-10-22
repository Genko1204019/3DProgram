#include"PlayerProjectile.h"
#include"Application/Manager/GameManager.h"
#include"Application/Scene/SceneManager.h"

void PlayerProjectile::Init()
{

	objTag = GameTag::PlayerProjectileTag;
	


	switch (objType)
	{
	case pProjectileType::FlySlashType:
		InitFlySlash();
		break;
	case pProjectileType::MagicBallType:
		InitMagicBall();
		break;
	case pProjectileType::pRangeCircleType:
		InitFlySlash();
		break;
	case pProjectileType::FireBallType:
		InitFireBall();
		break;

	default:
		break;
	}



	objCollider = make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("bulletCol", model, KdCollider::TypeDamage);
	debugWire = make_unique<KdDebugWireFrame>();

}

void PlayerProjectile::Update()
{
	UpdateMatrix();
	UpdateEffect();

	MoveMissile();

	if (isMissile)	MoveMissile();
	else			MoveNormal();

	

	KdCollider::SphereInfo sphere(KdCollider::TypeGround, pos, 1);
	std::list<KdCollider::CollisionResult> retList;
	//debugWire->AddDebugSphereByType(sphere);

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() != GameTag::PlayerTag) {
			obj->Intersects(sphere, &retList);

		}
		
	}

	float overlap = 0;
	bool hit = false;
	Math::Vector3 normal = {};

	for (auto& ret : retList) {
		if (overlap < ret.m_overlapDistance) {
			hit = true;
			overlap = ret.m_overlapDistance;
			normal = ret.m_hitNormal;


		}
	}

	if (hit) {
	

	}


	UpdateRangeCircle();


}

void PlayerProjectile::PostUpdate()
{
	BodyCollision();
}

void PlayerProjectile::DrawLit()
{

	//KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat);
}

void PlayerProjectile::GenerateDepthMapFromLight()
{
}

void PlayerProjectile::InitFlySlash()
{
	model = std::make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/_ProjectileObj/HItBox/SlashSkillBox.gltf");

	effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("slashFly06.efkefc", { pos }, 1, 1, true);

	//slashWave2.efkefc

	scale = { 3.5,2.8,2.1 };

	maxPass = 4;

}

void PlayerProjectile::InitMagicBall()
{
	model = std::make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/_ProjectileObj/Bullet/Bullet.gltf");

	effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("magicBallLoop.efkefc", { pos }, 0.449, 0.7, true); //NA_healing_001

	scale = { 1,1,1 };
	maxPass = 1;
}

void PlayerProjectile::InitFireBall()
{
	model = std::make_shared<KdModelWork>();
	model->SetModelData("Asset/Models/_ProjectileObj/Bullet/Bullet.gltf");
	effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("fireBall.efkefc", { pos }, 0.49, 0.7, true); //NA_healing_001

	scale = { 1,1,1 };
	maxPass = 1;
}

void PlayerProjectile::UpdateRangeCircle()
{
	/*if (objType == pProjectileType::pRangeCircleType) {
		KdShaderManager::Instance().WorkAmbientController().AddConeLight(pos + Vector3(0, 2.1, 0), { 0,0,0 }, circleOuter, circleRad, { 3,3,3 });

		if (circleRad < circleOuter) circleRad += 0.05;
		else {
			KdShaderManager::Instance().WorkAmbientController().AddConeLight(pos + Vector3(0, 2.1, 0), { 0,0,0 }, 0, 0, { 3,3,3 });
			SetExpired();
		}

	}*/

}







void PlayerProjectile::UpdateMatrix()
{
	transMat = Matrix::CreateTranslation(pos);
	rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rot.y + 90));
	scaleMat = Matrix::CreateScale(scale);

	worldMat = scaleMat * rotMat * transMat;

}

void PlayerProjectile::BodyCollision()
{
	KdCollider::SphereInfo bodySphere(KdCollider::TypeGround, pos, 0.7);
	//debugWire->AddDebugSphereByType(bodySphere);
	std::list<KdCollider::CollisionResult> retBodyList;

	float overlap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;
	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::StageTag || obj->GetTag() == GameTag::MapTag) {
			if (obj->Intersects(bodySphere, &retBodyList)) {
				for (auto& ret : retBodyList) {
					if (overlap < ret.m_overlapDistance) {
						overlap = ret.m_overlapDistance;
						hitDir = ret.m_hitDir;
						isHit = true;

						if (obj->GetTag() == GameTag::MapTag && obj->GetType() == MapType::BottleType)	obj->OnHit();
						
					}
				}

			}
		}

	}

	if (isHit) {
		Death();
	}

}

void PlayerProjectile::MoveNormal()
{
}

void PlayerProjectile::MoveMissile()
{
	pos += moveVec * moveSpd;

}

void PlayerProjectile::UpdateEffect()
{
	




	//KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(effectId), pos + Vector3(0, 0, 0));
	
	Vector3 forward = { 1.0f, 0.0f, 0.0f };

	// Normalize the moveVec to get the direction
	Vector3 moveDirection = moveVec;
	moveDirection.Normalize();

	// Calculate the angle between the forward vector and the movement direction
	float dot = forward.Dot(moveDirection);
	float angle = acosf(dot); // Angle in radians

	// Calculate the rotation axis using the cross product
	Vector3 axis = forward.Cross(moveDirection);
	if (axis.LengthSquared() > 0.0f) {
		axis.Normalize(); // Normalize the axis of rotation
	}
	else {
		// If vectors are parallel, no rotation needed
		axis = { 0.0f, 1.0f, 0.0f }; // Default to Y-axis as fallback
	}
	
	KdEffekseerManager::GetInstance().SetRotationById(effectId, axis, angle);
	KdEffekseerManager::GetInstance().SetPosById(effectId, pos);


}

void PlayerProjectile::HandleDeath()
{
	lifeSpan--;

	if (lifeSpan <= 0 || !lifeFlg) {
		Death();
	}
}

void PlayerProjectile::Death()
{
	KdEffekseerManager::GetInstance().StopEffectById(effectId);
	if(objType == pProjectileType::FireBallType) KdEffekseerManager::GetInstance().Play("fireBallExplo2.efkefc", pos, 0.14, 0.56, false);
	else KdEffekseerManager::GetInstance().Play("slashParticle2.efkefc", pos, 1, 1, false);
	SetExpired();
}

void PlayerProjectile::OnHit()
{
	wpPass++;
	if (wpPass >= maxPass) {
		Death();
	}
	

}



void PlayerProjectile::DrawBright()
{
}

void PlayerProjectile::CallImgui()
{
	if (objType != pProjectileType::pRangeCircleType) return;
	//imgui begin
	ImGui::Begin("PlayerProjectile");

	//show pos	
	ImGui::Text("pos : %f, %f, %f", pos.x, pos.y, pos.z);

	//show cicrleRad circleOuter
	ImGui::Text("circleRad : %f", circleRad);
	ImGui::Text("circleOuter : %f", circleOuter);

	//end
	ImGui::End();

}
