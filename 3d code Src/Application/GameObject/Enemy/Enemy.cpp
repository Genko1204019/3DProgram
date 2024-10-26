#include"Enemy.h"
#include"Application/Scene/SceneManager.h"
#include"Application/StateMachine/EnemyState.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/GameObject/Moji/Moji.h"
#include"Application/Manager/PathManager.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"

bool Enemy::isWithinAtkFrame()
{
	if (atkFrame >= atkReadyFrame && atkFrame <= atkEndFrame) {
		return true;
	}
	else {
		return false;
	}
}

bool Enemy::isAtkEnable()
{
	if (atkFrame >= atkReadyFrame+1 && atkFrame <= atkEndFrame && !isAtkDelay) {
		return true;
	}
	else {
		return false;
	}
}

void Enemy::Init()
{
	objTag = GameTag::EnemyTag;
	drawType = eDrawTypeLit | eDrawTypeDepthOfShadow;

	InitModelInfo();
	InitAnimator();
	InitCollider();
	InitTexuture();
	InitStatus();
	InitState();
	
	//cal return point;

	pModel = make_shared<KdModelWork>();
	pModel->SetModelData(pModelName);

}

void Enemy::Update()
{

	returnPoint = PathManager::Instance().FindPointInMap(patrolPt[0].x, patrolPt[0].z); //error crash

	
	EquipWeapon();

	EnemyKnockBack();


	UpdateMatrix();
	UpdateAnimation();
	UpdateGravity();

	ColorManager();
	CounterManager();
	EnemyManager();

	CheckFacingPlayer();

	EnemyStateMachine();

	PlayerInfo();
	CameraInfo();
	GameInfo();
	WeaponInfo();

	EffectManager();

	EnemyHitStop();

	SectorRaySearch();

}

void Enemy::PostUpdate()
{
	KdCollider::RayInfo faceray;
	SetCollider(faceray, pos + Vector3(0, 1.4, 0), {}, moveVec, 2.1, KdCollider::TypeDamage);
	 SetDebugWire(faceray);
	

	GroundCollision();

	BodyCollision();

	DmgCollision();

	




	
}

void Enemy::DrawLit()
{

	KdShaderManager::Instance().m_StandardShader.DrawModel(*pModel, Matrix::CreateScale(10,10,10)* Matrix::CreateTranslation({}));

	KdShaderManager::Instance().m_StandardShader.DrawModel(*eModel, worldMat);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*equipModel, worldMat);

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*alertPoly,Matrix::CreateScale(0.1,0.28,0.1) * rotMat * transMat * Matrix::CreateTranslation(0,5.6,0), alertColor);
	
	Matrix lockPolyMat = Matrix::CreateScale(1) * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180)) * transMat * Matrix::CreateTranslation(0, 5.6, 0);
	
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*lockPoly, lockPolyMat);
}

void Enemy::GenerateDepthMapFromLight()
{

	KdShaderManager::Instance().m_StandardShader.DrawModel(*eModel, worldMat);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*equipModel, worldMat);

}

void Enemy::DrawBright()
{
	if (isLockOn) {
		KdShaderManager::Instance().m_StandardShader.DrawModel(*lockOnModel, lockOnMat);
	}



}

void Enemy::DrawSprite()
{
	hpBarRatio = hp / 100;

	hpBarRect = { 0,0,(int)(190 * hpBarRatio),20 };

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(0.4) * Matrix::CreateTranslation(pos2D.x , pos2D.y  , 0) );
	KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBarTex, 0, 0, hpBarRect.width, hpBarRect.height, &hpBarRect, &kWhiteColor, Math::Vector2{ 0.5, 0 }); //fine
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);



}

void Enemy::DrawMoji()
{
}

void Enemy::DrawUnLit()
{
	//KdShaderManager::Instance().m_StandardShader.DrawModel(*eModel, worldMat);

}

void Enemy::InitModelInfo()
{

	eModel = make_shared<KdModelWork>();
	equipModel = make_shared<KdModelWork>();
	lockOnModel = make_shared<KdModelWork>();

	switch (objType)
	{
	case eFighter:
		eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
		break;
	case eMage:
		eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
		break;
	case eWanderer:
		eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
		break;

	default:
		eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
		equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
		break;
	}

	lockOnModelName = "Asset/Models/_EnemyObj/LockOn/LockOn.gltf";


	eModel->SetModelData(eModelName);
	equipModel->SetModelData(equipModelName);
	lockOnModel->SetModelData(lockOnModelName);


	alertPoly = make_shared<KdSquarePolygon>();
	alertPoly->SetMaterial("Asset/Textures/exMark.png");
	alertPoly->SetScale(10);
	alertColor = { 1,1,1,alertAlpha };

	lockPoly = make_shared<KdSquarePolygon>();
	lockPoly->SetMaterial("Asset/Textures/marker.png");
	lockPoly->SetScale(4);


}

void Enemy::InitAnimator()
{
	eAnime = make_shared<KdAnimator>();
	equipAnime = make_shared<KdAnimator>();

	SetAnime(idleAni, true);

}

void Enemy::InitCollider()
{
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("EnemyCollision", eModel, KdCollider::TypeDamage);

	
	
	debugWire = std::make_unique<KdDebugWireFrame>();

}

void Enemy::InitTexuture()
{
	hpBarTex.Load("Asset/Textures/hpBarRed.png");
	hpBarFrameTex.Load("Asset/Textures/hpBarGrey.png");
}

void Enemy::InitStatus()
{
	hp = maxHp;
	

	uniqueID = GameManager::Instance().GetEnemyUniqueId();
	GameManager::Instance().SetEnemyUniqueId(uniqueID + 1);

	scale = { 1.5 ,1.5,1.5};

}

void Enemy::InitState()
{
	if (objType == EnemyType::eWanderer) {
		stateCnt = 0;
		PatrolTransition();

		//stateMachine.ChangeState(this, new ePatrolState());

	}
	else {
		//stateMachine.ChangeState(this, new eChaseState());
		SpawnTransition();

	}


}

void Enemy::SetAnime(string _aniName, bool _isLoop)
{
	eAnime->SetAnimation(eModel->GetData()->GetAnimation(_aniName), _isLoop);
	equipAnime->SetAnimation(equipModel->GetData()->GetAnimation(_aniName), _isLoop);
	const shared_ptr<EnemyWeapon> spWeapon = wpWeapon.lock();
	if(spWeapon) spWeapon->SetAnime(_aniName, _isLoop);
}

void Enemy::SetAnime(string _aniName, string _aniName2, bool _isLoop)
{
	eAnime->SetAnimation(eModel->GetData()->GetAnimation(_aniName), _isLoop);
	equipAnime->SetAnimation(equipModel->GetData()->GetAnimation(_aniName2), _isLoop);
}

void Enemy::ResetAnime()
{
	switch (nowState)
	{
	default:
		break;
	}

}

void Enemy::SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType)
{
	_rayObj.m_dir = _rayDir;
	_rayObj.m_range = _rayRange;
	_rayObj.m_pos = _rayPos;
	_rayObj.m_pos += _rayPosAdd;
	_rayObj.m_type = _hitType;
}

void Enemy::SetCollider(KdCollider::SphereInfo& _sphereObj, Vector3 _sphereCenter, float _sphereRadius, UINT _hitType)
{
	_sphereObj.m_sphere.Center = _sphereCenter;
	_sphereObj.m_sphere.Radius = _sphereRadius;
	_sphereObj.m_type = _hitType;
}

void Enemy::SetDebugWire(KdCollider::RayInfo& _rayObj, Color _rayColor)
{
	if (GameManager::Instance().GetCanDebugWire())  debugWire->AddDebugLine(_rayObj.m_pos, _rayObj.m_pos + _rayObj.m_dir * _rayObj.m_range, _rayColor);
}

void Enemy::SetDebugWire(KdCollider::SphereInfo& _sphereObj, Color _sphereColor)
{
	if (GameManager::Instance().GetCanDebugWire()) debugWire->AddDebugSphere(_sphereObj.m_sphere.Center, _sphereObj.m_sphere.Radius, _sphereColor);
}

void Enemy::HitGround(KdCollider::RayInfo& _rayObj, list<KdCollider::CollisionResult>& _retRayList)
{
	
}

void Enemy::GroundCollision()
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

void Enemy::BodyCollision()
{
	

	KdCollider::SphereInfo eSphere;
	std::list<KdCollider::CollisionResult> eRetList;
	SetCollider(eSphere, pos + Vector3(0, 1, 0), 1, KdCollider::TypeDamage);
	//SetDebugWire(eSphere);

	float maxOverlapMap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::EnemyTag) {
			shared_ptr<Enemy> spEnemy = static_pointer_cast<Enemy>(obj);
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

	





	//E to Map Player collision
	KdCollider::SphereInfo bodySphere;
	std::list<KdCollider::CollisionResult> retHitList;
	SetCollider(bodySphere, pos + Vector3(0, 1, 0), 1, KdCollider::TypeDamage | KdCollider::TypeGround);
	// SetDebugWire(bodySphere);
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
						if (obj->GetTag() == GameTag::PlayerTag) {
							hitDirMap.Normalize();
							pos.x += hitDirMap.x * maxOverlapMap;
							pos.z += hitDirMap.z * maxOverlapMap;

							if (objType == EnemyType::eWanderer) {
								if (isDizzy) {
									//GameManager::Instance().StartBattle();
								}
								else {
									//GameManager::Instance().StartBattle();
								}
							}

						}

						if (obj->GetTag() == GameTag::MapTag) {
							if (obj->GetType() == MapType::WallType) {
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


}

void Enemy::SectorRaySearch()
{

	
}

void Enemy::DmgCollision()
{
	KdCollider::SphereInfo bodySphere;
	SetCollider(bodySphere, pos + Vector3(0, 1.28, 0), 1.21, KdCollider::TypeBump | KdCollider::TypeDamage);
	std::list<KdCollider::CollisionResult> retHitList;
	SetDebugWire(bodySphere);

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::PlayerWeaponTag || obj->GetTag() == GameTag::PlayerProjectileTag) {

			if (obj->Intersects(bodySphere, &retHitList)) {
				float maxOverlap = 0;
				Math::Vector3 hitPos;
				bool isHit = true;

				for (auto& ret : retHitList) {
					if (maxOverlap < ret.m_overlapDistance) {
						maxOverlap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						isHit = true;
					}
				}

				if (isHit) {

					if (obj->GetTag() == GameTag::PlayerProjectileTag) {
						OnGetDmg(hitPos);
					}

					auto player = wpPlayer.lock(); //can't cast obj to player class as it's weapon but not player
					if (player->ReadyForAtk() && mutekiCnt < 0) {
						player->UpdateHitFrame();
						OnGetDmg(hitPos);
						
					}

				}

			}

			
		}
	}

}


void Enemy::UpdateMatrix()
{
	scaleMat = Matrix::CreateScale(scale);
	rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
	transMat = Matrix::CreateTranslation(pos);
	worldMat = scaleMat * rotMat * transMat;

	lockOnMat = Matrix::CreateScale(lockOnScale) * Matrix::CreateRotationY(DirectX::XMConvertToRadians(lockOnAngle)) * Matrix::CreateTranslation(pos + lockOnPosOffset);


}

void Enemy::UpdateAnimation()
{
	eAnime->AdvanceTime(eModel->WorkNodes(), aniSpd * gameSpd);
	equipAnime->AdvanceTime(equipModel->WorkNodes(), aniSpd * gameSpd);

	

}

void Enemy::UpdateRotation(Vector3 _faceDir)
{
	lockOnAngle += 2;

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



void Enemy::UpdateGravity()
{
	if (GetAsyncKeyState(VK_SPACE)) {
		pos.y += 1.14;
	}

	pos.y -= gravity;
	gravity += gravityPow;
}



void Enemy::ColorManager()
{
}

void Enemy::CounterManager()
{
	stateCnt -= 1 * gameSpd;
	stunCnt -= 1 * gameSpd;
	hitStopCnt -= 1 * gameSpd;
	mutekiCnt -= 1 * gameSpd;
	atkCD -= 1 * gameSpd;

	atkFrame += 1 * gameSpd * aniSpd;

	followCD -= 1 * gameSpd;

	alertCnt -= 1 * gameSpd;
	alertAlpha -= 0.01 * gameSpd;

	spawnCnt--;

	canCheckFacingCnt--;

}

void Enemy::EnemyManager()
{
}

void Enemy::EnemyKnockBack()
{
	if (canKnockBack) {
		getHitDir.Normalize();
		float knockbackPow = 10.f;
		knockBackPos = pos + getHitDir * knockbackPow;
		gravity = -0.35;
		isKnockBack = true;
		knockBackTimer = 0;

	}

	if (isKnockBack) {
		knockBackTimer++;
		pos.x = Utility::Lerp(pos.x, knockBackPos.x, 0.07);
		pos.z = Utility::Lerp(pos.z, knockBackPos.z, 0.07);

		if (knockBackTimer > 140) {
			isKnockBack = false;
		}

	}






}

void Enemy::EnemyStateMachine()
{
	stateMachine.Update(this);

}

void Enemy::EnemyIdle()
{

	AttackTransition();
	ChaseTransition();
	RandomPositionTransition();

	DeathTransition();

	UpdateRotation(dirToPlayer);


}

void Enemy::IdleTransition()
{

	if (stateCnt <= 0) {
		SetAnime(idleAni, true);
		stateMachine.ChangeState(this, new eIdleState());
		nowState = EnemyState::eIdle;
		stateCnt = Utility::Rnd(35, 70);
	}

}

void Enemy::EnemyChase()
{
	moveVec = dirToPlayer;

	if (stunCnt < 0) {
		
		pos.x += moveVec.x * moveSpd * gameSpd;
		pos.z += moveVec.z * moveSpd * gameSpd;

	}

	IdleTransition();
	DeathTransition();

	UpdateRotation(dirToPlayer);


}

void Enemy::ChaseTransition()
{
	if (stateCnt <= 0) {
		SetAnime(walkAni, true);
		stateMachine.ChangeState(this, new eChaseState());
		nowState = EnemyState::eChase;
		stateCnt = Utility::Rnd(210, 420);
	}

}

void Enemy::EnemyAttack()
{

	if (stunCnt > 0) return;

	if (canAtkAnime) {
		canAtkAnime = false;

		switch (atkOrder)
		{
		case 1:
			SetAnime(atkAni1, false);
			break;
		case 2:
			SetAnime(atkAni2, false);
			break;
		case 3:
			SetAnime(atkAni3, false);
			break;

		default:
			break;
		}
	}

	AtkDelay();


	if (eAnime->IsAnimationEnd()) {
		stateCnt = 0;
		isAttacking = false;
		DeathTransition();
		ChaseTransition();
	}


}

void Enemy::AttackTransition()
{
	if (stateCnt < 0 && distToPlayer < 7) {
		stateCnt = 700;
		atkOrder = 1;
		atkCD = 50;
		canAtkAnime = true;
		isAttacking = true;

		isAtkDelay = true;
		atkDelayCnt = 35;

		atkFrame = 0;
		atkReadyFrame = 21;
		atkEndFrame = 50;

		aniSpd = 1.f;

		stateMachine.ChangeState(this, new eAtkState());
		nowState = EnemyState::eAttack;
	}


}

void Enemy::AtkDelay()
{
	if (atkFrame > atkReadyFrame && isAtkDelay) {
		if (atkDelayCnt > 0) {
			aniSpd = 0.04f;  // Slow down the animation speed during the delay
			atkDelayCnt--;   // Decrease the delay counter
		}

		if (atkDelayCnt <= 0) {
			isAtkDelay = false;  // End the delay
			aniSpd = 1.f;       // Restore normal animation speed
		}
	}


}

void Enemy::EnemyRandomPosition()
{
}

void Enemy::RandomPositionTransition()
{
	if(stateCnt <= 0){
		stateCnt = 560;
		//nowState = EnemyState::;
		//stateMachine.ChangeState(this, new eRandomPositionState());

		randomTargetPos = {};


	}

}

void Enemy::EnemyMagicAtk()
{
}

void Enemy::MagicAtkTransition()
{
}

void Enemy::EnemyMagicHeal()
{
}

void Enemy::MagicHealTransition()
{
}


void Enemy::EnemyPatrol()
{
	


	//if(patrolOrder < 3)
	distToPatrolTarget = (Vector3(patrolTarget.x, 0, patrolTarget.z) - Vector3(pos.x, 0, pos.z)).Length();

	dirToPatrolTarget = patrolTarget - pos;
	dirToPatrolTarget.Normalize();
	moveVec = dirToPatrolTarget;
	pos += moveVec * 0.03 * gameSpd;

	if (distToPatrolTarget <= 0.3 || stateCnt <= 0) {
		patrolOrder = rand() % 3;
		patrolTarget = patrolPt[patrolOrder];
		stateCnt = Utility::Rnd(280, 420);
		canCheckFacingCnt = 77;
	}

	FollowTransition();

	UpdateRotation(dirToPatrolTarget);

}

void Enemy::PatrolTransition()
{
	if (stateCnt <= 0) {
		stateCnt = 280;
		nowState = EnemyState::ePatrol;
		stateMachine.ChangeState(this, new ePatrolState());
		SetAnime(walkAni, true);
		patrolOrder = 0;
		patrolTarget = patrolPt[patrolOrder];
		followCD = 400;

	}

}

void Enemy::EnemyFollow()
{


	alertColor = { 7,1,1,alertAlpha };

	moveVec = dirToPlayer;

	if (isAlert && alertCnt <= 0) {
		SetAnime(walkAni, true);
		isAlert = false;
	}

	if (isAlert)UpdateRotation(dirToPlayer);

	
	if (alertCnt > 0) return;
	

//	if (alertCnt <= 0 ) {
	if (stateCnt > 0) {
		if (distToPlayer > 7) {
			HomingAStartPath(PathManager::Instance().playerPoint.x, PathManager::Instance().playerPoint.y);
			UpdateRotation(dirToNextTarget);
		}
		else {
			pos.x += moveVec.x * moveSpd * gameSpd;
			pos.z += moveVec.z * moveSpd * gameSpd;
			UpdateRotation(dirToPlayer);
		}
	}
	else {
		//player QuestionMark
		HomingAStartPath(returnPoint.x, returnPoint.y);
		UpdateRotation(dirToNextTarget);
		
		distToReturnPt = (Vector3(pos.x, 0, pos.z) - Vector3(returnPoint.y * 6, 0, returnPoint.x * 6)).Length();
		if (distToReturnPt <= 7) {
			PatrolTransition();
		}

	}
		







}

void Enemy::FollowTransition()
{
	if ((isFacingPlayer && followCD < 0) || isDizzy) {
		stateMachine.ChangeState(this, new eFollowState());
		nowState = EnemyState::eFollow;
		isDizzy = false;
		isAlert = true;
		alertCnt = 70;
		stateCnt = 770;
		alertAlpha = 1;
		SetAnime(alertAni, false);
	}

}



void Enemy::HandleDeath()
{
	/*if (eAnime->IsAnimationEnd() && stateCnt <= 0) {
		SetExpired();

	}*/

}

void Enemy::DeathTransition()
{
	if (hp <= 0) {
		SetAnime(deathAni, false);
		stateCnt = 700;
		GameManager::Instance().SetBattleEnemyNum(GameManager::Instance().GetBattleEnemyNum() - 1);
		nowState = EnemyState::eDeath;
		stateMachine.ChangeState(this, new eDeathState());
	}

}

void Enemy::EnemyStun()
{
	if (eAnime->IsAnimationEnd() && stunCnt <= 0) {
		stateCnt = 0;
		IdleTransition();
	}
	

}

void Enemy::StunTransition()
{
	nowState = EnemyState::eStun;
	stateMachine.ChangeState(this, new eStunState());
	SetAnime(stunAni, false);
}

void Enemy::EnemySpawn()
{

	if (spawnCnt < 0 && !canSpawn) {
		SetAnime(spawnAni, false);
		canSpawn = true;
		KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { pos + Vector3(0,0.1,0) }, 0.77, 1.4, false);

	}

	if (eAnime->IsAnimationEnd() && stateCnt <= 0) {
		IdleTransition();
	}
}

void Enemy::SpawnTransition()
{	
	stateCnt = 350;
	canSpawn = false;
	spawnCnt = 140;
	stateMachine.ChangeState(this, new eSpawnState());
	nowState = EnemyState::eSpawn;
}

void Enemy::EnemyDizzy()
{
	if (stateCnt < 0) {
		FollowTransition();
	}
}

void Enemy::DizzyTransition()
{
	stateCnt = 210;
	nowState = EnemyState::eDizzy;
	stateMachine.ChangeState(this, new eDizzyState());
	KdEffekseerManager::GetInstance().Play("pipofm-healthring02.efkefc", { pos + Vector3(0,3,0) }, 1, 0.7, true);
	SetAnime("Hit_B", false);
	isDizzy = true;

	
	
}

void Enemy::EquipWeapon()
{
	if (!hasWeapon) {
		hasWeapon = true;
		shared_ptr<EnemyWeapon> weapon = make_shared<EnemyWeapon>();
		weapon->Init();
		SceneManager::Instance().AddObject(weapon);
		SetWeapon(weapon);
	}
	
}

void Enemy::PlayerInfo()
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

void Enemy::CameraInfo()
{
	//get camera
	shared_ptr<GameCamera> spCamera = wpCamera.lock();

	if (spCamera) {
		Vector3 tempPos = pos;
		tempPos += hpOffset;
		spCamera->WorkCamera()->ConvertWorldToScreenDetail(tempPos, pos2D); //z is left right , x is up down   0,3.5,-1
		//spCamera->WorkCamera()->ConvertWorldToScreenDetail(hpOffset2, pos2Doffset); //z is left right , x is up down   0,3.5,-1
		camPos = spCamera->GetPos();
		hasMoji = true;
	}
	

}

void Enemy::GameInfo()
{
	gameSpd = GameManager::Instance().GetGameSpd();
}

void Enemy::WeaponInfo()
{
	const shared_ptr<EnemyWeapon>spWeapon = wpWeapon.lock();
	if (spWeapon) {
		spWeapon->SetAniSpd(aniSpd);
		spWeapon->SetMatrix(worldMat);
		spWeapon->SetIsAtkEnable(isAtkEnable());
	}


}

void Enemy::CheckFacingPlayer()
{
	Vector3 forVec = moveVec; //to ppt
	forVec.Normalize();

	Vector3 toPlayerVec = playerPos - pos;
	toPlayerVec.Normalize();

	float dot = forVec.Dot(toPlayerVec);

	float angle = acos(dot);
	angleToPlayer = DirectX::XMConvertToDegrees(angle);

	if(angleToPlayer < 45 && distToPlayer <= 14 && canCheckFacingCnt < 0) isFacingPlayer = true;
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
		SetDebugWire(searchRay, Color(1, 0, 0, 1));


		

	}

	
	

}



void Enemy::EffectManager()
{
	KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(effectId), pos + Vector3(0, 1.4, 0));

}

void Enemy::EnemyHitStop()
{
	isHitStop = false;
	/*if (isHitStop) {
		aniSpd = 0.01;
	}

	if (isHitStop && hitStopCnt < 0) {
		isHitStop = false;
		stunCnt = 35;
		aniSpd = 1;
		pos += getHitPos;
	}*/

	

}

void Enemy::HomingToTargetPoint(float pointX, float pointZ)
{
	Vector3 dirToA = Vector3(pointX, -70, pointZ) - pos;
	dirToA.Normalize();
	pos += dirToA * followSpd * gameSpd;
	
		


}

void Enemy::HomingAStartPath(float _targetX, float _targetY)
{

	PathManager::Instance().FindEnemyPoint(pos.x, pos.z, enemyPointX, enemyPointY);


	

	path = PathManager::Instance().PathFinding(enemyPointX, enemyPointY, _targetX, _targetY);

	if (!path.empty())
	{
		for (size_t i = 0; i < path.size(); ++i)
		{
			Vector3 nodePos = PathManager::Instance().mapPos[path[i].x][path[i].y];
			debugWire->AddDebugSphere(nodePos, 1.0f, kGreenColor); // Red sphere at each node
		}

		if (path.size() > 1) {
			nextTarget = PathManager::Instance().mapPos[path[1].x][path[1].y];
			dirToNextTarget = nextTarget - pos;
			dirToNextTarget.Normalize();
			HomingToTargetPoint(nextTarget.x, nextTarget.z);

		}

	}

}

void Enemy::OnGetDmg(Vector3 _hitPos)
{

	auto player = wpPlayer.lock();

	mutekiCnt = 35;

	shared_ptr<Moji> moji = make_shared<Moji>();
	moji->SetPosition(pos + Vector3(0,2.1,0));
	moji->SetCamera(wpCamera.lock());
	moji->SetDmgNum(wpPlayer.lock()->GetatkDmg());
	moji->Init();
	SceneManager::Instance().AddObject(moji);

	if (objType == EnemyType::eWanderer) {
		DizzyTransition();
		hp -= player->GetatkDmg();
		stunCnt = 35;
		getHitDir = Vector3(pos.x, 0, pos.z) - Vector3(playerPos.x, 0, playerPos.z);
		getHitDir.Normalize();
		float yaw = atan2(getHitDir.x, getHitDir.z);
		getHitPos = getHitDir * player->GetpushEnemyPow();
		KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { _hitPos + getHitPos }, 1, 0.7, false);
		KdEffekseerManager::GetInstance().Play("slashParticle2.efkefc", { _hitPos + getHitPos }, 1, 0.7, false);

		SetAnime(getHitAni, false);
		pos += getHitPos; // canceled hitstop

	}
	else {
		hp -= player->GetatkDmg();
		getHitDir = Vector3(pos.x, 0, pos.z) - Vector3(playerPos.x, 0, playerPos.z);
		getHitDir.Normalize();
		float yaw = atan2(getHitDir.x, getHitDir.z);
		getHitPos = getHitDir * player->GetpushEnemyPow();
		KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { _hitPos + getHitPos }, 1, 0.7, false);
		KdEffekseerManager::GetInstance().Play("slashParticle2.efkefc", { _hitPos + getHitPos }, 1, 0.7, false);

		SetAnime(getHitAni, false);
		pos += getHitPos; // canceled hitstop

		//add knockBack
		canKnockBack= true; 
		
		


	}

}

void Enemy::SpawnMoji()
{
}

void Enemy::OnHit()
{

	//create moji class
	//shared_ptr<Moji> moji = make_shared<Moji>();
	//moji->Init();
	//moji->SetCamera(wpCamera.lock());
	//moji->SetPosition(pos + Vector3(0, 2, 0));
	//moji->SetDmgNum(wpPlayer.lock()->GetatkDmg());
	//SceneManager::Instance().AddObject(moji);

	//getHitDir = Vector3(pos.x,0,pos.z) - Vector3(playerPos.x, 0, playerPos.z);
	//getHitDir.Normalize();
	//float rot = atan2(getHitDir.x, getHitDir.z);
	//getHitPos = getHitDir * wpPlayer.lock()->GetpushEnemyPow();
	//hp -= 5;
	//stunCnt = 35;
	//hitStopCnt = 21;
	//isHitStop = true;
	//KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { pos }, 1, 0.7, false);
	//KdEffekseerManager::GetInstance().Play("hitParticle.efkefc", { pos }, 1, 0.7, false);

	//

	//SetAnime(getHitAni, false);

}



void Enemy::CallImgui()
{
	ImGui::Begin("Enemy");
	ImGui::PushID(uniqueID);
	ImGui::Separator();

	//checkBox hasMoji
ImGui::Checkbox("hasMoji", &hasMoji);

	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "EnemyType : %s", EnemyTypeNames.at((EnemyType)objType));
	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "NowState : %s", EnemyStateNames.at((EnemyState)nowState));
	ImGui::Text("objType : %d", objType);
	ImGui::Text("NowState : %d", nowState);
	ImGui::Text("StateCnt : %f", stateCnt);
	ImGui::Text("aniSpd : %f", aniSpd);


	//show camPos
	ImGui::Text("camPos : %f, %f, %f", camPos.x, camPos.y, camPos.z);

	if (ImGui::CollapsingHeader("Status")) {
		ImGui::Text("hp : %f", hp);
		ImGui::Text("uniqueID : %d", uniqueID);
		ImGui::Text("storeID : %d", storeID);

		//ImGui::SliderFloat("hpOffsetx", &hpOffset.x, -10, 10);
		//ImGui::SliderFloat("hpOffsety", &hpOffset.y, -10, 10);
		//ImGui::SliderFloat("hpOffsetz", &hpOffset.z, -10, 10);
	}

	//================================================================================================

	if (ImGui::CollapsingHeader("PlayerInfo")) {
		ImGui::Checkbox("isLockOn", &isLockOn);
		ImGui::Checkbox("isFacingPlayer", &isFacingPlayer);
		ImGui::Text("angleToPlayer : %f", angleToPlayer);
		ImGui::Text("distToPlayer : %f", distToPlayer);
	}
	if (ImGui::CollapsingHeader("Move,Pos")) {
		ImGui::SliderFloat("aniSpd", &aniSpd, 0, 5);
		ImGui::SliderFloat("moveSpd", &moveSpd, 0, 5);
		ImGui::Text("Pos : %f, %f, %f", pos.x, pos.y, pos.z);

	}

	//================================================================================================

	if (ImGui::CollapsingHeader("Patrol")) {
		ImGui::Text("enemyPoint : %d", enemyPointX);
		ImGui::Text("enemyPoint : %d", enemyPointY);
		ImGui::Text("nextTarget : %f, %f, %f", nextTarget.x, nextTarget.y, nextTarget.z);
		ImGui::Text("returnPoint : %f, %f", returnPoint.x, returnPoint.y);
		ImGui::Text("distToReturnPt : %f", distToReturnPt);

		if (ImGui::CollapsingHeader("A* Path Details"))
		{
			if (path.empty())ImGui::Text("No Path Found");
			else
				for (size_t i = 0; i < path.size(); ++i) ImGui::Text("Path Node %zu: (X: %d, Y: %d)", i, path[i].x, path[i].y);
		}

		ImGui::Text("patrolTarget : %f, %f, %f", patrolTarget.x, patrolTarget.y, patrolTarget.z);
		ImGui::Text("isAlert : %d", isAlert);
		ImGui::Text("alertCnt : %f", alertCnt);
	}

	//================================================================================================

	if (ImGui::CollapsingHeader("Attack")) {
		ImGui::Text("AtkCD : %f", atkCD);
		ImGui::Text("AtkOrder : %d", atkOrder);
		ImGui::Text("AtkFrame : %f", atkFrame);
		//atkReady
		ImGui::Text("AtkReadyFrame : %f", atkReadyFrame);
		//end
		ImGui::Text("AtkEndFrame : %f", atkEndFrame);
		
		//checkBox isatkDelay
ImGui::Checkbox("isAtkDelay", &isAtkDelay);
//show atkDelayCnt
ImGui::Text("atkDelayCnt : %f", atkDelayCnt);
//isAttacking
ImGui::Checkbox("isAttacking", &isAttacking);

	}



	ImGui::PopID();  // End unique context
	ImGui::End;

	
	/*if (ImGui::CollapsingHeader((EnemyTypeNames.at((EnemyType)objType)))) {

	}*/
}
