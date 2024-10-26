#include "EnemyBase.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"
#include"Application/GameObject/Player/Player.h"


void EnemyBase::Init()
{
	objTag = GameTag::EnemyTag;
	drawType = eDrawTypeLit | eDrawTypeDepthOfShadow;

	InitModelInfo();
	InitAnimator();
	InitCollider();
	InitStatus();

}

void EnemyBase::Update()
{
	UpdateMatrix();
	UpdateAnimation();
	UpdateGravity();
	UpdateStateMachine();
	UpdateHpRotation();

	UpdateEnemyCounter();


	GameInfo();
	PlayerInfo();
	CheckFacingPlayer();

}

void EnemyBase::PostUpdate()
{
	GroundCollision();
	EnemyCollision();
	BodyCollision();
}

void EnemyBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*eModel, worldMat,kWhiteColor, emitRate);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*equipModel, worldMat, kWhiteColor);


}

void EnemyBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*eModel, worldMat, kWhiteColor);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*equipModel, worldMat, kWhiteColor);

}

void EnemyBase::SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType)
{
	_rayObj.m_dir = _rayDir;
	_rayObj.m_range = _rayRange;
	_rayObj.m_pos = _rayPos;
	_rayObj.m_pos += _rayPosAdd;
	_rayObj.m_type = _hitType;
}



void EnemyBase::InitModelInfo()
{

	if (objType == EnemyType::eBoss) {
		eModelName = "Asset/Models/_EnemyObj/Boss/BossBody/BossSke.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Boss/BossEquip/BossEquip.gltf";
		wpModelName = "Asset/Models/_EnemyObj/Boss/BossWeapon/BossWeapon2.gltf";

		scale = { 3.5,3.5,3.5 };
	}

	if (objType == EnemyType::eMage) {
		eModelName = "Asset/Models/_EnemyObj/Mage/MageBody/MageBody2.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Mage/MageEquip/MageEquip.gltf";
		wpModelName = "Asset/Models/_EnemyObj/Mage/MageWp/MageWp.gltf";

	}

	if (objType == EnemyType::eWanderer) {
		eModelName = "Asset/Models/_EnemyObj/Wander/WanderBody/WanderBody.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Wander/WanderEquip/WanderEquip.gltf";
		wpModelName = "Asset/Models/_EnemyObj/Fighter/FighterWp/FighterWp.gltf";
		scale = {1.9,1.9,1.9 };
	}

	if (objType == EnemyType::eHealer) {
		eModelName = "Asset/Models/_EnemyObj/Healer/HealerBody/HealerBody.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Healer/HealerEquip/HealerEquip.gltf";
		//wpModelName = "Asset/Models/_EnemyObj/Healer/HealerWp/HealerWp.gltf";
		wpModelName = "Asset/Models/_EnemyObj/Fighter/FighterWp/FighterWp.gltf";

	}

	if (objType == EnemyType::eFighter) {
		eModelName = "Asset/Models/_EnemyObj/Fighter/FighterBody/FighterBody.gltf";
		wpModelName = "Asset/Models/_EnemyObj/Fighter/FighterWp/FighterWp.gltf";
	}

	eModel = std::make_shared<KdModelWork>();
	equipModel = std::make_shared<KdModelWork>();

	eModel->SetModelData(eModelName);
	equipModel->SetModelData(equipModelName);

}

void EnemyBase::InitAnimator()
{
	eAnime = make_shared<KdAnimator>();
	equipAnime = make_shared<KdAnimator>();
	SetAnime(idleAni, true);

}

void EnemyBase::InitCollider()
{
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("EnemyCollision", eModel, KdCollider::TypeDamage);
	debugWire = std::make_unique<KdDebugWireFrame>();

}

void EnemyBase::InitStatus()
{
	hp = maxHp;

	uniqueID = GameManager::Instance().GetEnemyUniqueId();
	GameManager::Instance().SetEnemyUniqueId(uniqueID + 1);

}



void EnemyBase::UpdateMatrix()
{
	scaleMat = Matrix::CreateScale(scale.x, scale.y, scale.z);
	rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));

	transMat = Matrix::CreateTranslation(pos);
	worldMat = scaleMat * rotMat  * transMat;

}

void EnemyBase::UpdateAnimation()
{
	eAnime->AdvanceTime(eModel->WorkNodes(), aniSpd * gameSpd);
	equipAnime->AdvanceTime(equipModel->WorkNodes(), aniSpd * gameSpd);
}

void EnemyBase::UpdateGravity()
{
	pos.y -= gravity;
	gravity += gravityPow;

}

void EnemyBase::UpdateRotation(Vector3 _faceDir)
{
	Matrix nowRot = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
	Vector3 nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, -nowRot);
	Vector3 toVec;
	toVec = _faceDir;

	float dot = nowVec.Dot(toVec);
	dot = std::clamp(dot, -1.0f, 1.0f);
	long double ang = DirectX::XMConvertToDegrees(acos(dot));
	if (ang >= 0.1f) {
		if (ang > 5) ang = 5.0f;
		Math::Vector3 cross = toVec.Cross(nowVec);
		cross.Normalize();
		if (cross.y >= 0) {
			rotAngle -= ang * 1;
			if (rotAngle <= 0) rotAngle += 360;
		}
		else {
			rotAngle += ang * 1;
			if (rotAngle >= 360) rotAngle -= 360;
		}
	}

}

void EnemyBase::UpdateHpRotation()
{
	Matrix nowRot = Matrix::CreateRotationY(DirectX::XMConvertToRadians(hpRotAngle));
	Vector3 nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, -nowRot);
	Vector3 toVec;
	toVec = dirToPlayer;

	float dot = nowVec.Dot(toVec);
	dot = std::clamp(dot, -1.0f, 1.0f);
	long double ang = DirectX::XMConvertToDegrees(acos(dot));
	if (ang >= 0.1f) {
		if (ang > 5) ang = 5.0f;
		Math::Vector3 cross = toVec.Cross(nowVec);
		cross.Normalize();
		if (cross.y >= 0) {
			hpRotAngle -= ang * 1;
			if (hpRotAngle <= 0) hpRotAngle += 360;
		}
		else {
			hpRotAngle += ang * 1;
			if (hpRotAngle >= 360) hpRotAngle -= 360;
		}
	}
}

void EnemyBase::GroundCollision()
{
	const float stepHigh = 0.28f;
	KdCollider::RayInfo ray(KdCollider::TypeGround, pos + Vector3(0, stepHigh, 0), { 0,-1,0 }, gravity + stepHigh);
	std::list<KdCollider::CollisionResult> retRayList;
	float maxOverlap = 0;
	Vector3 hitPos;
	Vector3 hitDir;
	bool isHit = false;

	for (auto& mapObj : SceneManager::Instance().GetObjList()) {
		if (mapObj->Intersects(ray, &retRayList)) {
			Utility::CalOverlap(retRayList, maxOverlap, isHit, hitPos, hitDir);

			if (isHit) {
				pos = hitPos + Math::Vector3(0, -0.14f, 0);
				gravity = 0;
			}

		}
	}
}

void EnemyBase::EnemyCollision()
{
	KdCollider::SphereInfo eSphere(KdCollider::TypeDamage, pos + Vector3(0, 1.4, 0), 1.4);
	std::list<KdCollider::CollisionResult> eRetList;
	//debugWire->AddDebugSphereByType(eSphere);

	float overlapMap = 0;
	bool isHit = false;
	Vector3 hitDir;
	Vector3 hitPos;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::EnemyTag) {
			shared_ptr<EnemyBase> spEnemy = static_pointer_cast<EnemyBase>(obj);
			if (spEnemy->GetUniqueID() != uniqueID) {
				spEnemy->Intersects(eSphere, &eRetList);
			}
		}
	}

	Utility::CalOverlap(eRetList, overlapMap, isHit, hitPos, hitDir);

	if (isHit) {
		//hitDir.y = 0;
		//pos.x += hitDir.x * overlapMap;
		//pos.z += hitDir.z * overlapMap;
		//Math::Vector3 _nowPos = GetPos();
		//SetPos(pos);

		Utility::HitPositionAdjust2D(pos, hitDir, overlapMap);

	}
}

void EnemyBase::BodyCollision()
{
	KdCollider::SphereInfo bodySphere(KdCollider::TypeDamage | KdCollider::TypeGround, pos + Vector3(0, 1.4, 0), 1.4);
	std::list<KdCollider::CollisionResult> retHitList;
	float overlap = 0;
	bool isHit = false;
	Vector3 hitDir;
	Vector3 hitPos;
	//debugWire->AddDebugSphereByType(bodySphere);

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->Intersects(bodySphere, &retHitList)) {
			Utility::CalOverlap(retHitList, overlap, isHit, hitPos, hitDir);

			if (isHit) {
				if (obj->GetTag() != MapTag && obj->GetTag() != StageTag && obj->GetTag() != PlayerTag) return;
				if (hp <= 0) return;
				Utility::HitPositionAdjust2D(pos, hitDir, overlap);
			}
		}
	}


}

void EnemyBase::CheckFacingPlayer()
{
	Vector3 forVec = moveVec; //to ppt
	forVec.Normalize();
	Vector3 toPlayerVec = playerPos - pos;
	toPlayerVec.Normalize();
	float dot = forVec.Dot(toPlayerVec);
	float angle = acos(dot);
	angleToPlayer = DirectX::XMConvertToDegrees(angle);

	if (angleToPlayer < 45 && distToPlayer <= 14 && canCheckFacingCnt < 0) isFacingPlayer = true;
	else isFacingPlayer = false;


	const int numRays = 140;
	const float sectorAngle = 45;
	const float rayRange = 14.0f;
	Vector3 rayStartPos = pos + Vector3(0, 1.4f, 0);
	float angleStep = sectorAngle / (numRays - 1);
	Vector3 Dir = moveVec;
	Dir.Normalize();
	float halfSector = sectorAngle / 2.0f;

	for (int i = 0; i < numRays; ++i)
	{
		float angleOffset = -halfSector + (i * angleStep);
		Matrix rotation = Matrix::CreateRotationY(DirectX::XMConvertToRadians(angleOffset));
		Vector3 rayDir = Vector3::TransformNormal(Dir, rotation);
		KdCollider::RayInfo searchRay;
		SetCollider(searchRay, pos, {}, rayDir, rayRange, KdCollider::TypeDamage | KdCollider::TypeGround);
		//SetDebugWire(searchRay, Color(1, 0, 0, 1));
		
	}
}

void EnemyBase::UpdateEnemyCounter()
{
	stateCnt -= 1 * gameSpd;
	mutekiCnt -= 1 * gameSpd;

	canCheckFacingCnt -= 1 * gameSpd;



}

void EnemyBase::UpdateStateMachine()
{
	stateMachine.Update(this);
}

void EnemyBase::EquipWeapon()
{
	shared_ptr<EnemyWeapon> weapon = make_shared<EnemyWeapon>();
	weapon->SetModelName(wpModelName);
	weapon->Init();
	//if (objType == EnemyType::eBoss) weapon->SetWpScale({ 0.77,0.77,0.77 });
	SceneManager::Instance().AddObject(weapon);
	SetWeapon(weapon);
}

void EnemyBase::SetAnime(string _aniName, bool _isLoop)
{
	eAnime->SetAnimation(eModel->GetData()->GetAnimation(_aniName), _isLoop);
	equipAnime->SetAnimation(equipModel->GetData()->GetAnimation(_aniName), _isLoop);
	const shared_ptr<EnemyWeapon> spWeapon = wpWeapon.lock();
	if (spWeapon) spWeapon->SetAnime(_aniName, _isLoop);
}

void EnemyBase::PlayerInfo()
{
	const shared_ptr<Player> spPlayer = wpPlayer.lock();

	if (spPlayer) {
		playerPos = spPlayer->GetPosition();
	}

	distToPlayer = (Vector3(playerPos.x, 0, playerPos.z) - Vector3(pos.x, 0, pos.z)).Length();
	dirToPlayer = playerPos - pos;
	dirToPlayer.Normalize();
}

void EnemyBase::GameInfo()
{
	gameSpd = GameManager::Instance().GetGameSpd();

}
