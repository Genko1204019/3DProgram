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

	EnemyCounterManager();

	CheckFacingPlayer();

	GameInfo();
	PlayerInfo();
	CameraInfo();
	WeaponInfo();

}

void EnemyBase::PostUpdate()
{
	GroundCollision();
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

void EnemyBase::SetCollider(KdCollider::SphereInfo& _sphereObj, Vector3 _sphereCenter, float _sphereRadius, UINT _hitType)
{
	_sphereObj.m_sphere.Center = _sphereCenter;
	_sphereObj.m_sphere.Radius = _sphereRadius;
	_sphereObj.m_type = _hitType;
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

	//if (objType == EnemyType::eFighter) {
	//	eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
	//	equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
	//}

	/*switch (objType)
	{
	case EnemyType::eBoss:
		eModelName = "Asset/Models/Enemy/EnemyBoss/EnemyBoss.gltf";
		equipModelName = "Asset/Models/Enemy/Empty/Empty.gltf";
		break;
	case EnemyType::eFighter:
		eModelName = "Asset/Models/_EnemyObj/EnemyFighter/FighterBody.gltf";
		equipModelName = "Asset/Models/_EnemyObj/EnemyFighter/FighterEquip.gltf";
		break;
	case EnemyType::eMage:
		eModelName = "Asset/Models/Enemy/EnemyMage/MageBody.gltf";
		equipModelName = "Asset/Models/Enemy/EnemyMage/MageEquip.gltf";
		break;	
	case EnemyType::eHealer:
		eModelName = "Asset/Models/Enemy/EnemyHealer/EnemyHealer.gltf";
		equipModelName = "Asset/Models/Enemy/EnemyHealer/GoblinWeapon.gltf";
		break;
	case EnemyType::eWanderer:
		eModelName = "Asset/Models/Enemy/EnemyWander/EnemyWander.gltf";
		equipModelName = "Asset/Models/Enemy/EnemyWander/RangeWeapon.gltf";
		break;
	default:
		break;
	}*/


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
	KdCollider::RayInfo ray;
	const float enableStepHigh = 0.28f;
	SetCollider(ray, pos, Vector3(0, enableStepHigh, 0), Vector3::Down, gravity + enableStepHigh, KdCollider::TypeGround);
	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& mapObj : SceneManager::Instance().GetObjList()) {
		if (mapObj->Intersects(ray, &retRayList)) {
			float maxOverlap = 0;
			Math::Vector3 hitPos;
			bool isHit = false;

			for (auto& ret : retRayList) {
				if (maxOverlap < ret.m_overlapDistance) {
					maxOverlap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					isHit = true;
				}
			}

			if (isHit) {
				pos = hitPos + Math::Vector3(0, -0.14f, 0);
				gravity = 0;
			}

		}
	}
}

void EnemyBase::BodyCollision()
{
	
	KdCollider::SphereInfo eSphere;  //hit with only other enemy
	std::list<KdCollider::CollisionResult> eRetList;

	SetCollider(eSphere, pos + Vector3(0, 1.4, 0), 1.4, KdCollider::TypeDamage);

	//SetDebugWire(eSphere);
	//debugWire->AddDebugSphereByType(eSphere);

	float maxOverlapMap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::EnemyTag) {
			shared_ptr<EnemyBase> spEnemy = static_pointer_cast<EnemyBase>(obj);
			if (spEnemy->GetUniqueID() != uniqueID) {
				spEnemy->Intersects(eSphere, &eRetList);
			}
		}
	}

	for (auto& ret : eRetList) {
		if (maxOverlapMap < ret.m_overlapDistance) {
			maxOverlapMap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			isHit = true;
		}
	}

	if (isHit) {
		hitDir.y = 0;
		pos.x += hitDir.x * maxOverlapMap;
		pos.z += hitDir.z * maxOverlapMap;
		Math::Vector3 _nowPos = GetPos();
		SetPos(pos);

	}

	//hit with all object map player 
	KdCollider::SphereInfo bodySphere;
	std::list<KdCollider::CollisionResult> retHitList;
	if (objType == EnemyType::eWanderer) SetCollider(bodySphere, pos + Vector3(0, 1.4, 0), 1, KdCollider::TypeDamage | KdCollider::TypeGround);
	else SetCollider(bodySphere, pos + Vector3(0, 1.4, 0), 1, KdCollider::TypeDamage | KdCollider::TypeGround);

	//debugWire->AddDebugSphereByType(bodySphere);

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->Intersects(bodySphere, &retHitList)) {
			float maxOverlapMap = 0;
			bool isHit = false;
			Math::Vector3 hitDirMap;

			for (auto& ret : retHitList) {
				if (maxOverlapMap < ret.m_overlapDistance) {
					maxOverlapMap = ret.m_overlapDistance;
					hitDirMap = ret.m_hitDir;
					isHit = true;

					if (isHit) {
						if (obj->GetTag() == GameTag::PlayerTag && nowState != EnemyState::eWanderDead) {
							if (hp > 0) {
								hitDirMap.Normalize();
								pos.x += hitDirMap.x * maxOverlapMap;
								pos.z += hitDirMap.z * maxOverlapMap;

							}
							
							

						}

						if (obj->GetTag() == GameTag::MapTag) {
							if (obj->GetType() == MapType::WallType) {
								hitDirMap.Normalize();
								pos.x += hitDirMap.x * maxOverlapMap;
								pos.z += hitDirMap.z * maxOverlapMap;
							}

						}

						if (obj->GetTag() == GameTag::StageTag) {
							hitDirMap.Normalize();
							pos.x += hitDirMap.x * maxOverlapMap;
							pos.z += hitDirMap.z * maxOverlapMap;
						
						}

					}

				}
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

void EnemyBase::EnemyCounterManager()
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
		distToPlayer = (Vector3(playerPos.x, 0, playerPos.z) - Vector3(pos.x, 0, pos.z)).Length();
	}

	distToPlayer = (Vector3(playerPos.x, 0, playerPos.z) - Vector3(pos.x, 0, pos.z)).Length();
	dirToPlayer = playerPos - pos;
	dirToPlayer.Normalize();
}





void EnemyBase::GameInfo()
{
	gameSpd = GameManager::Instance().GetGameSpd();

}
