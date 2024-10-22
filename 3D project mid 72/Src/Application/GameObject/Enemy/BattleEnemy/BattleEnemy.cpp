#include "BattleEnemy.h"
#include"Application/StateMachine/EnemyState.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/GameObject/Moji/Moji.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"
#include"Application/GameObject/Projectile/EnemyProjectile.h"

void BattleEnemy::Init()
{
	InitBattleEnemy();
	InitAtkInfo();

	EnemyBase::Init();

	InitState();


	EquipWeapon();

	

}

void BattleEnemy::Update()
{
	EnemyBase::Update();

	BattleCounterManager();
//	CameraInfo();
	WeaponInfo();

	HpBarBuffer();

	//FindNodePoint();


}

void BattleEnemy::PostUpdate()
{
	EnemyBase::PostUpdate();

	DmgCollision();

	CameraInfo();

}

void BattleEnemy::DrawLit()
{
	EnemyBase::DrawLit();
	//KdShaderManager::Instance().m_StandardShader.DrawModel(*lockOnModel, worldMat, kWhiteColor, emitRate);

	if (isLockOn) {
		lockOnAngle+=2.8;
		Matrix lockOnMat = Matrix::CreateScale(3.5, 3.5, 3.5) * Matrix::CreateRotationY(DirectX::XMConvertToRadians(lockOnAngle)) * transMat * Matrix::CreateTranslation(0,0.4,0);
		KdShaderManager::Instance().m_StandardShader.DrawModel(*lockOnModel, lockOnMat, kRedColor, emitRate);
	}
	
}

void BattleEnemy::GenerateDepthMapFromLight()
{
	EnemyBase::GenerateDepthMapFromLight();
}

void BattleEnemy::DrawSprite()
{



	if (hp <= 0) return;
	if (nowState == EnemyState::eSpawn) return;

	Color hpColor = { 1,1,1,hpBarAlpha };
	bufferColor = { 0,1,0.23,hpBarAlpha };

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(frameScale.x, frameScale.y, frameScale.z) * Matrix::CreateTranslation(pos2D.x, pos2D.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBaseFrameTex, 0, 0, hpBaseFrameRect.width, hpBaseFrameRect.height, &hpBaseFrameRect, &hpColor, { 0,0.5 }); //fine
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);


	hpBufferRatio = hpBufferMax / hpBufferDefault;
	Math::Rectangle hpBufferRect = { 0,0,(int)(276 * hpBufferRatio),30 };

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(barScale.x, barScale.y, barScale.z) * Matrix::CreateTranslation(pos2D.x, pos2D.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBarTex, 0, 0, hpBufferRect.width, hpBufferRect.height, &hpBufferRect, &bufferColor, { 0,0.5 }); //fine

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	hpBarRatio = hp / maxHp;
	hpBarRect = { 0,0,(int)(276 * hpBarRatio),30 };

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(barScale.x, barScale.y, barScale.z) * Matrix::CreateTranslation(pos2D.x, pos2D.y, 0));
	KdShaderManager::Instance().m_spriteShader.DrawTex(&hpBarTex, 0, 0, hpBarRect.width, hpBarRect.height, &hpBarRect, &hpColor, { 0,0.5 }); //fine
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);


	


}

void BattleEnemy::DrawMoji()
{
}

void BattleEnemy::DrawMini()
{
	//Math::Rectangle enemyRect = { 0,0,32,32 };
	//miniMapPos2D.x = scaleFactor * pos.x + xOffset;
	//miniMapPos2D.y = scaleFactor * pos.z + yOffset;

	//Matrix sMat = Matrix::CreateScale(100, 100, 100);
	//Matrix tmat = Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().scrollX, miniMapPos2D.y - GameManager::Instance().scrollY, 0);
	//Matrix fMat = sMat * tmat;
	//KdShaderManager::Instance().m_spriteShader.SetMatrix(fMat);	

	//KdShaderManager::Instance().m_spriteShader.DrawTex(enemyIconTex, 0, 0, enemyRect.width, enemyRect.height, &enemyRect, &kWhiteColor, { 0.5,0.5 }); //fine
	//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);


}

void BattleEnemy::InitBattleEnemy()
{
	eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
	equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
	lockOnModelName = "Asset/Models/_EnemyObj/LockOn/LockOn.gltf";

	lockOnModel = std::make_shared<KdModelWork>();
	lockOnModel->SetModelData(lockOnModelName);

	hpBarTex.Load("Asset/Textures/gauge_A_red_l_s.png");
	hpBaseFrameTex.Load("Asset/Textures/gauge_A_l_s.png");

	hpBarFrameTex.Load("Asset/Textures/gauge_A_red_l.png");



	if (objType == EnemyType::eBoss) {
		maxHp = 400;
		hp = 400;
		hpBufferDefault = 400;
		hpBufferMax = 400;
	}

}

void BattleEnemy::InitAtkInfo()
{
	switch (objType)
	{
	case EnemyType::eFighter:
		atkOrderMax = 1;
		break;
	case EnemyType::eBoss:
		atkOrderMax = 3;
		break;
	default:
		break;
	}
}

void BattleEnemy::InitState()
{
	SpawnTransition();


}

void BattleEnemy::CameraInfo()
{
	shared_ptr<GameCamera> spCamera = wpCamera.lock();
	if (spCamera) {
		
		Vector3 headWorldPos = {};
		if(objType == EnemyType::eBoss) headWorldPos = GetPos() + Vector3(0, 7.7f, 0);
		else headWorldPos = GetPos() + Vector3(0, 4.2f, 0);
		spCamera->WorkCamera()->ConvertWorldToScreenDetail(headWorldPos, pos2D);
		pos2D.x += hpOffset.x;

	}
}

void BattleEnemy::WeaponInfo()
{
	const shared_ptr<EnemyWeapon>spWeapon = wpWeapon.lock();
	if (spWeapon) {
		spWeapon->SetAniSpd(aniSpd);
		spWeapon->SetMatrix(worldMat);
		spWeapon->isAtkEnable = isAtkEnable();
		spWeapon->SetEnemyPos(pos);
		spWeapon->enemyRotMat = rotMat;
	}
}



void BattleEnemy::BattleCounterManager()
{
	hitStopCnt -= 1 * gameSpd;
	stunCnt -= 1 * gameSpd;

	atkCd -= 1 * gameSpd;
	atkFrame += 1 * gameSpd * aniSpd;
	atkWaitCnt -= 1 * gameSpd;

	spawnCnt--;

	showHpCnt -= 1 * gameSpd;

}

void BattleEnemy::DmgCollision()
{
	if (hp <= 0) return;

	KdCollider::SphereInfo bodySphere;
	SetCollider(bodySphere, pos + Vector3(0, 1.49, 0), 1.21, KdCollider::TypeBump | KdCollider::TypeDamage);
	std::list<KdCollider::CollisionResult> retHitList;
	//debugWire->AddDebugSphereByType(bodySphere);

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

					if (obj->GetTag() == GameTag::PlayerProjectileTag && mutekiCnt < 0) {
						if(obj->GetType() == pProjectileType::FlySlashType || obj->GetType() == pProjectileType::FireBallType)OnGetDmg(hitPos,true,42);
						else OnGetDmg(hitPos, false, wpPlayer.lock()->GetatkDmg());
						obj->OnHit();
					}
					else {
						auto player = wpPlayer.lock(); //can't cast obj to player class as it's weapon but not player
						if (player->ReadyForAtk() && mutekiCnt < 0) {
							player->OnGiveDmg();
							OnGetDmg(hitPos, false, wpPlayer.lock()->GetatkDmg());
						}
					}

					

				}

			}


		}
	}
}

void BattleEnemy::HpBarBuffer()
{
	hpBufferCd--;



	while (hpBuffer > 0 && hpBufferCd < 0) {
		if(objType == EnemyType::eBoss) hpBufferCd = 1;
		else  hpBufferCd = 2;
		hpBufferMax -= 1;
		hpBuffer -= 1;
	}

	if (showHpCnt > 0 || isLockOn) {
		hpBarAlpha = 1;
	}
	else {
		hpBarAlpha -= 0.035;
	}


}

void BattleEnemy::EnemySpawn()
{
	if (spawnCnt < 0 && !canSpawn) {
		if(objType == EnemyType::eBoss) 	SetAnime("Spawn_Ground_Skeletons", false);
		else								SetAnime("Skeletons_Awaken_Floor_Long", false);


		canSpawn = true;
		darkSpawnEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
		KdEffekseerManager::GetInstance().Play("darkSpawn.efkefc", { pos + Vector3(0,0.7,0) }, 0.77, 1.1, false);
		//pos.y = -14;

	}

	if (eAnime->IsAnimationEnd() && stateCnt <= 0) {
		IdleTransition();
	}
}

void BattleEnemy::SpawnTransition()
{
	stateCnt = 350;
	canSpawn = false;
	spawnCnt = 140;
	stateMachine.ChangeState(this, new eSpawnState());
	nowState = EnemyState::eSpawn;
	SetAnime("Death_A_Pose", false);


	//pos.y = -201;	
}

void BattleEnemy::ExitSpawn()
{
	KdEffekseerManager::GetInstance().StopEffectById(darkSpawnEffId);
}

void BattleEnemy::EnemyStun()
{

	DeathTransition();
	UpdateRotation(dirToPlayer);


	if (eAnime->IsAnimationEnd() && stunCnt <= 0) {
		stateCnt = 0;


		switch (objType)
		{
		case EnemyType::eFighter:
			if (Utility::Rnd(0, 100) < 77)  IdleTransition();
			else 							BackJumpTransition(dirToPlayer, 7);
			break;
		case EnemyType::eMage:
			if (Utility::Rnd(0, 100) < 30)  IdleTransition();
			else 							BackJumpTransition(dirToPlayer, 7.7);
			break;

		default:
			break;
		}

		//rand 30 % to backJump
		/*if (Utility::Rnd(0, 100) < 77) {
			IdleTransition();
		}
		else {
			BackJumpTransition(dirToPlayer, 7);

		}*/
	

	}
	
}

void BattleEnemy::StunTransition()
{
	if (objType == EnemyType::eBoss) return;

	aniSpd = 1;
	nowState = EnemyState::eStun;
	stateMachine.ChangeState(this, new eStunState());
	SetAnime(stunAni, false);
}

void BattleEnemy::EnemyBackJump()
{
	
	//float lerpSpeed = 2.0f;  // Speed at which the enemy moves to the target
	//jumpProgress += lerpSpeed * GameManager::Instance().GetDeltaTime();
	//pos = Vector3::Lerp(pos, backJumpTargetPos, jumpProgress);

	/*if (eAnime->IsAnimationEnd() && pos.y <-14 && jumpProgress >= 1.0f) {
		IdleTransition();
	}*/

	DeathTransition();

	UpdateRotation(dirToPlayer);

	float distToTarget = Vector3::Distance(pos, backJumpTargetPos);
	 
	if (distToTarget > 0.21 && stateCnt > 0) {
		float lerpFactor = 0.1f;
		pos.x = Utility::Lerp(pos.x, backJumpTargetPos.x, 0.1f);
		pos.z = Utility::Lerp(pos.z, backJumpTargetPos.z, 0.1f);
	}
	else {
		stateCnt = 0;
		IdleTransition();

	}



}

void BattleEnemy::BackJumpTransition(Vector3 _dir, float _pow)
{
	stateCnt = 210;
	aniSpd = 1;
	nowState = EnemyState::eBackJump;
	stateMachine.ChangeState(this, new eBackJumpState());
	SetAnime(backJumpAni, false);
	Vector3 backJumpDir = Vector3(pos.x, 0, pos.z) - Vector3(playerPos.x, 0, playerPos.z);
	backJumpDir.Normalize();
	backJumpDir = -backJumpDir;
	backJumpTargetPos = pos + (-_dir) * _pow;

	gravity = -0.21;

	 
}

void BattleEnemy::EnemyBlowAway()
{
	
	distToBlowTarget = Vector3::Distance(pos, blowTargetPos);
	if (distToBlowTarget > 0.28f && stateCnt > 0 && !hasBlowAway) {
		float lerpFactor = 0.1f; // Lerp speed factor, can be adjusted
		pos.x = Utility::Lerp(pos.x, blowTargetPos.x, lerpFactor);
		pos.z = Utility::Lerp(pos.z, blowTargetPos.z, lerpFactor);
		blowPow = std::max(blowPow * 0.95f, 0.1f); // Reduce blowPow with a clamp to prevent it from going too low
	}
	else {
		if (!hasBlowAway) {
			hasBlowAway = true;
			stateCnt = 140;
			aniSpd = 0.7;
			SetAnime(blowAwayAniStand, false);
		}

		if (hasBlowAway && eAnime->IsAnimationEnd()) {
			//stateCnt = 0;
			IdleTransition();
		}

	}

}

void BattleEnemy::BlowAwayTransition(Vector3 _blowDir, float _blowPow, float _gravity)
{
	stateCnt = 70;
	SetAnime(blowAwayAni, false);
	stateMachine.ChangeState(this, new eBlowAwayState());
	nowState = EnemyState::eBlowAway;

	blowPow = _blowPow;
	blowDir = _blowDir;
	blowDir.Normalize();
	gravity = _gravity;
	blowTargetPos = pos + blowDir * blowPow;
	mutekiCnt = 70;
	hasBlowAway = false;

	aniSpd = 3.5;

	
	
	
}

void BattleEnemy::EnemyIdle()
{
	DeathTransition();
	UpdateRotation(dirToPlayer);

	switch (objType)
	{
	case EnemyType::eFighter:
		ChaseTransition();
		//AttackTransition();
		break;
	case EnemyType::eMage:
		MageWalkTransition();
		//MageRangeAtkTransition();

		break;
	case EnemyType::eBoss:
		ChaseTransition();
		//BossJumpAtkTransition();
		break;

	default:
		break;
	}
	




}

void BattleEnemy::IdleTransition()
{
	if (stateCnt <= 0) {
		SetAnime(idleAni, true);
		stateMachine.ChangeState(this, new eIdleState());
		nowState = EnemyState::eIdle;
		stateCnt = Utility::Rnd(21, 70);
	}
}

void BattleEnemy::EnemyChase()
{
	UpdateRotation(dirToPlayer);

	moveVec = dirToPlayer;

	if (stunCnt < 0) {
		pos.x += moveVec.x * moveSpd * gameSpd;
		pos.z += moveVec.z * moveSpd * gameSpd;
	}

	if (objType == EnemyType::eBoss) { if (distToPlayer < 5.6) stateCnt = 0; }
	else { if (distToPlayer < 2) stateCnt = 0; }

	switch (objType)
	{
	case EnemyType::eFighter:
		//IdleTransition();
		DeathTransition();
		AttackTransition();
		break;
	case EnemyType::eBoss:
		AttackTransition();
		//BossSummonTransition();
		//BossJumpAtkTransition();
		break;
	default:
		break;
	}
	
}

void BattleEnemy::ChaseTransition()
{
	if (stateCnt <= 0 && distToPlayer > 3.5) {
		SetAnime(walkAni, true);
		stateMachine.ChangeState(this, new eChaseState());
		nowState = EnemyState::eChase;
		stateCnt = Utility::Rnd(210, 420);
	}
}

void BattleEnemy::EnemyAttack()
{
	if (stunCnt > 0) return;

	if (objType == EnemyType::eFighter) {
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
	else if (objType == EnemyType::eBoss) {

		DeathTransition();



		if (atkCd < 0 && !canAtkAnime) {
			canAtkAnime = true;
			isAtkDelay = true;
			atkDelayCnt = 35;
			atkFrame = 0;
			atkReadyFrame = 21;
			atkEndFrame = 50;
			isSmashEff = false;
		}

		if (canAtkAnime) {

			switch (atkOrder)
			{
			case 1:
				SetAnime("1H_Melee_Attack_Chop", false);
				break;
			case 2:
				SetAnime("1H_Melee_Attack_Chop", false);
				break;
			case 3:
				SetAnime("1H_Melee_Attack_Chop", false);
				break;

			default:
				break;
			}

			canAtkAnime = false;
			atkCd = 114;
			atkOrder++;
			

		}

		AtkDelay();

		if (eAnime->IsAnimationEnd() && atkOrder > 3) {
			stateCnt = 0;
			isAttacking = false;
			//IdleTransition();
			BossJumpAtkTransition();

		}

		if (!isSmashEff && atkFrame > 42) {
			isSmashEff = true;
			wpWeapon.lock()->SmashEffect();
			KdAudioManager::Instance().PlayBossSmashSE();
			wpCamera.lock()->isScreenShake = true;
			
		}

	}
	
}

void BattleEnemy::AttackTransition()
{
	if (stateCnt < 0 || distToPlayer < 7) {
		stateCnt = 770; //490
		atkOrder = 1;
		atkCd = 50;
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

		isSmashEff = false;
	}
}

void BattleEnemy::AtkDelay()
{
	if (atkFrame > atkReadyFrame && isAtkDelay) {
		if (atkDelayCnt > 0) {
			aniSpd = 0.04f;  
			atkDelayCnt--;   

			if (objType == EnemyType::eBoss) {
				Vector3 leftOffset(-dirToPlayer.z, 0, dirToPlayer.x); // Perpendicular to dirToPlayer
				float offsetStrength = 0.5f;
				Vector3 offsetDir = dirToPlayer + leftOffset * offsetStrength;
				offsetDir.Normalize();
				UpdateRotation(offsetDir);

			}

		}

		if (atkDelayCnt <= 0) {
			isAtkDelay = false; 
			aniSpd = 1.f; 
			KdAudioManager::Instance().PlaySwordSE();
		}
	}
	else if (atkFrame < atkReadyFrame) {
		if (objType == EnemyType::eBoss) {
			Vector3 leftOffset(-dirToPlayer.z, 0, dirToPlayer.x); // Perpendicular to dirToPlayer
			float offsetStrength = 0.5f;
			Vector3 offsetDir = dirToPlayer + leftOffset * offsetStrength;
			offsetDir.Normalize();
			UpdateRotation(offsetDir);

		}
	}

	

}

void BattleEnemy::AtkMove()
{
}

void BattleEnemy::HandleDeath()
{
	if (eAnime->IsAnimationEnd() && !hasDeadAni) {
		hasDeadAni = true;
		GameManager::Instance().SetBattleEnemyNum(GameManager::Instance().GetBattleEnemyNum() - 1);
		aniSpd = 1;
	}

}

void BattleEnemy::DeathTransition()
{
	if (hp <= 0) {
		SetAnime(deathAni, false);
		stateCnt = 700;
		aniSpd = 1;
		nowState = EnemyState::eDeath;
		stateMachine.ChangeState(this, new eDeathState());
		if (isLockOn) {
			wpPlayer.lock()->SetisLockOn(false);
			wpPlayer.lock()->SetdistToLockEnemy(100);
			isLockOn = false;


		}

	}
}



void BattleEnemy::MageWalk()
{
	Vector3 dir = walkTarget - pos;
	dir.Normalize();
	pos += dir * moveSpd * gameSpd;

	//check if reached target
	if (Vector3::Distance(pos, walkTarget) < 1.5 || stateCnt < 0) {
		MageAtkTransition();
	}

	UpdateRotation(dir);


}

void BattleEnemy::MageWalkTransition()
{
	if (stateCnt < 0) {
		stateCnt = Utility::Rnd(210,350);
		stateMachine.ChangeState(this, new eMageWalkState());
		nowState = EnemyState::eMageWalk;

		walkTarget.x = Utility::Rnd(walkRangeXMin, walkRangeXMax);
		walkTarget.y = pos.y;
		walkTarget.z = Utility::Rnd(walkRangeZMin, walkRangeZMax);

		SetAnime(walkAni, true);
	}
}

void BattleEnemy::MageAtk()
{

	if (atkCd > 0) {
		Vector3 effPos = { pos + Vector3(0,0.3,0) };
		KdEffekseerManager::GetInstance().SetPosById(mageAtkEffid, effPos);
	}

	if (atkCd < 0) {

		KdAudioManager::Instance().PlayMageAtkSE();

		KdEffekseerManager::GetInstance().StopEffectById(mageAtkEffid);	
		mageAtkEffid = -1;

		SetAnime("Spellcast_Summon", false);

		Vector3 headPosition = pos + Vector3(0, 2.8f, 0);

		float totalSpread = 110.0f * DirectX::XM_PI / 180.0f;  
		float halfSpread = totalSpread / 2.0f;

		float spawnDistance = 4.9;

		float angleStep = totalSpread / (3 - 1); 


		for (int i = 0; i < 3; i++) {
			shared_ptr<EnemyProjectile> magicBall = make_shared<EnemyProjectile>();

			float angle = -halfSpread + i * angleStep;
			float xOffset = spawnDistance * sinf(angle) * 0.5f; 
			float yOffset = spawnDistance * cosf(angle); 
			Vector3 spawnOffset = Vector3(xOffset, yOffset, 0);
			Vector3 spawnPosition = headPosition + spawnOffset; 
			magicBall->SetPosition(pos + Vector3(0,1,1.4));
			magicBall->SetOriginPos(spawnPosition);
			magicBall->SetPreLaunchCnt(70 * i);
			magicBall->SetLaunchCnt((3 - i) * 100);
			magicBall->SetPlayer(wpPlayer.lock());
			magicBall->InitMagicBallAtk();
			magicBall->Init();
			SceneManager::Instance().AddObject(magicBall);
			atkCd = 770;
		}
		
		//stateCnt = 0;
	}



	if (stateCnt < 0) {
		IdleTransition();
	}

	UpdateRotation(dirToPlayer);



}

void BattleEnemy::MageAtkTransition()
{
	//if (stateCnt < 0) {
		stateCnt = 700;
		atkCd = 210;
		stateMachine.ChangeState(this, new eMageAtkState());
		nowState = EnemyState::eMageAtk;

		SetAnime("Spellcasting", true);

		mageAtkEffid = KdEffekseerManager::GetInstance().m_nextUniqueId;
		KdEffekseerManager::GetInstance().Play("NA_v2_3d_Magic Circle_dark.efkefc", { pos + Vector3(0,0.3,0) }, 1.4, 1, false);

	//}
}

void BattleEnemy::ExitMageAtk()
{
	aniSpd = 1;
	if (mageAtkEffid != -1) KdEffekseerManager::GetInstance().StopEffectById(mageAtkEffid);
	mageAtkEffid = -1;
}

void BattleEnemy::MageRangeAtk()
{
	if (atkCd < 0) {
		/*atkCd = 210;
		shared_ptr<EnemyProjectile> magicRange = make_shared<EnemyProjectile>();
		magicRange->SetPosition(playerPos);
		magicRange->Init();
		magicRange->InitRangeMagicAtk();
		magicRange->SetType(eProjectileType::MagicRangeType);
		SceneManager::Instance().AddObject(magicRange);*/
	}

}

void BattleEnemy::MageRangeAtkTransition()
{
	if (stateCnt < 0) {
		stateCnt = 490;
		atkCd = Utility::Rnd(140, 270);
		stateMachine.ChangeState(this, new eMageRangeAtkState());
		nowState = EnemyState::eMageRangeAtk;
	}

}



void BattleEnemy::BossAtk()
{
}

void BattleEnemy::BossAtkTransition()
{
	
}

void BattleEnemy::BossMove()
{
}

void BattleEnemy::BossMoveTransition()
{
}

void BattleEnemy::BossSummon()
{
	
	//move to SpinTargetPosA
	Vector3 dir;

	if (spinProgress == 1) {
		dir = spinTargetA - pos;
		dir.Normalize();

		if (Vector3::Distance(pos, spinTargetA) < 0.21) {
			spinProgress++;
			spinTargetPlayer = playerPos;
		}
	}

	if (spinProgress == 3) {
		dir = spinTargetB - pos;
		dir.Normalize();

		if (Vector3::Distance(pos, spinTargetB) < 0.21) {
			spinProgress++;
			spinTargetPlayer = playerPos;
		}
	}

	if (spinProgress == 5) {
		dir = spinTargetC - pos;
		dir.Normalize();

		if (Vector3::Distance(pos, spinTargetC) < 0.21) {
			spinProgress++;
			spinTargetPlayer = { 0,0,0 };
		}
	}



	if (spinProgress % 2 == 0) {
		dir = spinTargetPlayer - pos;
		dir.Normalize();
		if (Vector3::Distance(pos, spinTargetPlayer) < 0.21) {
			spinProgress++;
		}

	}
	
	

	if (spinProgress >= 6) {
		dir = spinTargetD - pos;
		dir.Normalize();
		if (Vector3::Distance(pos, spinTargetD) < 0.21 && !isSpinFinish) {
			isSpinFinish = true;
			stateCnt = 280;
			dizzyEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
			KdEffekseerManager::GetInstance().Play("pipofm-healthring02.efkefc", { pos + Vector3(0,5.6,0) }, 2.1, 1.4, false);
			SetAnime("Sit_Floor_Idle", true);
			KdAudioManager::Instance().StopWpSpinSE();
			KdAudioManager::Instance().PlayDizzySE();
		}
		Vector3 tempPos = pos + Vector3(0, 5.6, 0);
		KdEffekseerManager::GetInstance().SetPosById(dizzyEffId, tempPos);

		if (stateCnt < 0 && !isStandUp) {
			SetAnime("Sit_Floor_StandUp", false);
			isStandUp = true;
		}

		if (isStandUp && eAnime->IsAnimationEnd()) {
			IdleTransition();
		}

	}
	else {
		KdAudioManager::Instance().PlayWpSpinSE();
	}

	if(!isSpinFinish) pos += dir * moveSpd * gameSpd * 2.1;


	

}

void BattleEnemy::BossSummonTransition()
{
	//if (hp > 50) return;

	if (stateCnt < 0) {
		stateCnt = 4900;
		atkCd = Utility::Rnd(210, 220);
		stateMachine.ChangeState(this, new eBossSummonState());
		nowState = EnemyState::eBossSummon;
		SetAnime("2H_Melee_Attack_Spinning", true);
		spinProgress = 1;
		isSpinFinish = false;
		isStandUp = false;
	}

}

void BattleEnemy::ExitSummon()
{
	if (dizzyEffId != -1) KdEffekseerManager::GetInstance().StopEffectById(dizzyEffId);
	dizzyEffId = -1;
}

void BattleEnemy::BossJumpAtk()
{

	//homing toward centerPos until reach the target

	if (hasReachedtarget) {
		absorbEffCnt--;
		Vector3 tempPos = { pos + Vector3(0,4.2,0) };
		KdEffekseerManager::GetInstance().SetPosById(absorbEffId, tempPos);
	}

	Vector3 dir = centerPos - pos;
	dir.Normalize();
	float length = Vector3::Distance(pos, centerPos);
	
	if (length > 0.28 && !hasReachedtarget) {
		pos += dir * moveSpd * gameSpd;
		UpdateRotation(dir);
	}
	else if (length <= 0.28 && !hasReachedtarget) {
		SetAnime("Spellcast_Raise", false);
		hasReachedtarget = true;
		atkCd = 0;
		absorbEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
		KdEffekseerManager::GetInstance().Play("absorbPurple.efkefc", { pos + Vector3(0,4.2,0) }, 0.77, 0.7, false);
	}
	else if (atkCd < 0 && absorbEffCnt < 0) {

		if (absorbEffId != -1) {
			KdEffekseerManager::GetInstance().StopEffectById(absorbEffId);
			absorbEffId = -1;
		}

		SetAnime("Spellcast_Raise", false);
		atkCd = 140;
		KdAudioManager::Instance().PlayPlayerSlashWaveSE();

		// Create 5 projectiles evenly distributed along 360 degrees
		/*const int numProjectiles = 5;
		const float angleIncrement = 270.0f / numProjectiles;*/

		/* int numProjectiles = GameManager::Instance().testNum;
		 float angleIncrement = GameManager::Instance().testAngle / numProjectiles;*/

		/*int numProjectiles;
		if (Utility::Rnd(0, 100) < 50) numProjectiles = 5;
		else numProjectiles = 4;
		float angleIncrement = 360 / numProjectiles;*/

		if (numProjectiles == 4) numProjectiles = 5;
		else numProjectiles = 4;
		float angleIncrement = shotAngle / numProjectiles;

		for (int i = 0; i < numProjectiles; ++i) {
			// Calculate the angle for this projectile
			float angle = i * angleIncrement;

			// Convert angle to radians for trigonometric functions
			float radians = DirectX::XMConvertToRadians(angle);

			// Calculate direction vector using sine and cosine
			DirectX::SimpleMath::Vector3 moveVec = {
				cosf(radians),
				0.0f,  // Assuming the attack is on the XZ plane
				sinf(radians)
			};

			// Normalize the direction vector (in case the length is not 1)
			moveVec.Normalize();

			// Create the projectile
			shared_ptr<EnemyProjectile> slashWave = make_shared<EnemyProjectile>();
			slashWave->SetType(eProjectileType::SlashWaveType);
			slashWave->InitSlashWaveAtk();
			slashWave->SetPosition(pos + Vector3(0,1.4,0));
			slashWave->SetMoveVec(moveVec);
			SceneManager::Instance().AddObject(slashWave);
		}

	}
	else {
		UpdateRotation(dirToPlayer);

	}

	/*if (atkCd > 0) {
		if (pos.y < 14) {
			pos.y += 0.5 * gameSpd;
			gravity = 0;
			if (pos.y >= 14) pos.y = 14;
		}
		
	}

	if (atkCd <= 0) {
		
		if (pos.y > -14.5) pos.y -= 0.5 * gameSpd;
		if (pos.y <= -14.5) {
			pos.y = -14.5;
			if (!isWaitCnt) {
				KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { worldMat.Translation() }, 4, 1, false);
				atkWaitCnt = 210;
				isWaitCnt = true;
			}		
		}
	}

	if (atkWaitCnt < 0 && isWaitCnt) {
		isWaitCnt = false;
		atkCd = 210;
	}*/

	BossSummonTransition();

}

void BattleEnemy::BossJumpAtkTransition()
{
	if (stateCnt <= 0) {
		stateCnt = 770;
		atkCd = Utility::Rnd(210,220);
		stateMachine.ChangeState(this, new eBossJumpAtkState());
		nowState = EnemyState::eBossJumpAtk;
		SetAnime(walkAni, true);
		hasReachedtarget = false;

		absorbEffCnt = 210;

		numProjectiles = 4;
		shotAngle = 360;
	}
}

void BattleEnemy::BossSlashWaveAtkTransition()
{
	if (stateCnt <= 0) {
		stateCnt = 490;
		atkCd = Utility::Rnd(210, 220);
		stateMachine.ChangeState(this, new eBossJumpAtkState());
		nowState = EnemyState::eBossSlashWaveAtk;
		SetAnime(backJumpAni, false);
	}
}

void BattleEnemy::OnGetDmg(Vector3 _hitPos , bool _isBlowAway, float dmgToGet)
{


	if (hp <= 0) return;

	if (GameManager::Instance().GetBattleEnemyNum() == 1) {
		dmgPredict = hp - dmgToGet;

		if (dmgPredict <= 0) {
			GameManager::Instance().SetGameSpd(0.14);
			GameManager::Instance().endBattleCnt == 350;
			GameManager::Instance().isEndBattle = true;

			KdAudioManager::Instance().battleBgm->Stop();
			KdAudioManager::Instance().PlayFinalHitSE();
		}
	}
	

	auto player = wpPlayer.lock();

	mutekiCnt = 35;
	showHpCnt = 140;

	shared_ptr<Moji> moji = make_shared<Moji>();
	moji->SetType(MojiType::DmgMoji);
	moji->SetPosition(pos + Vector3(0, 3.5, 0));
	moji->SetCamera(wpCamera.lock());
	float randomValue = rand() % 5 - 2;
	moji->SetDmgNum(dmgToGet + randomValue);
	moji->Init();
	SceneManager::Instance().AddObject(moji);

	hp -= dmgToGet;
	hpBuffer += dmgToGet;
	getHitDir = Vector3(pos.x, 0, pos.z) - Vector3(playerPos.x, 0, playerPos.z);
	getHitDir.Normalize();
	float yaw = atan2(getHitDir.x, getHitDir.z);

	if (objType == EnemyType::eBoss)	getHitPos = { 0,0,0 };
	else								getHitPos = getHitDir * player->GetpushEnemyPow();
	KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { _hitPos + getHitPos }, 1, 0.7, false);
	KdEffekseerManager::GetInstance().Play("hitParticleNew01.efkefc", { _hitPos + getHitPos }, 1, 1.4, false);
	//hitParticleNew01.efkefc slashParticle2
	
	if (_isBlowAway) {
		BlowAwayTransition(getHitDir, 7.7, -0.49);
	}
	else {
		pos += getHitPos;
		StunTransition();
	}
	
	KdAudioManager::Instance().PlaySwordHitSE();

	
	 
	//BlowAwayTransition(getHitDir, 7.7, -0.49);

	//add knockBack
	//canKnockBack = true;

	


	
}

void BattleEnemy::CallImgui()
{
	ImGui::Begin("BattleEnemy");
	ImGui::PushID(uniqueID);
	ImGui::Separator();


	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "EnemyType : %s", EnemyTypeNames.at((EnemyType)objType));
	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "NowState : %s", EnemyStateNames.at((EnemyState)nowState));
	ImGui::Text("objType : %d", objType);
	ImGui::Text("NowState : %d", nowState);
	ImGui::Text("StateCnt : %f", stateCnt);
	//sliderfloat aniSpd
	ImGui::SliderFloat("aniSpd", &aniSpd, 0, 4);


	//show spinProgress
	ImGui::Text("spinProgress : %d", spinProgress);


	//show rotangle
	ImGui::SliderFloat("rotAngle", &rotAngle, -180, 180);

	//slider float hpOffset.xyz
	
	////slider float barScale x y z
	//ImGui::SliderFloat("barScale.x", &barScale.x, 0, 5);
	//ImGui::SliderFloat("barScale.y", &barScale.y, 0, 5);
	//ImGui::SliderFloat("barScale.z", &barScale.z, 0, 5);
	////frameScale
	//ImGui::SliderFloat("frameScale.x", &frameScale.x, 0, 5);
	//ImGui::SliderFloat("frameScale.y", &frameScale.y, 0, 5);
	//ImGui::SliderFloat("frameScale.z", &frameScale.z, 0, 5);
	//
	////show hpoffset.x
	//ImGui::SliderFloat("hpOffset.x", &hpOffset.x, -500, 500);

	////color4 bufferColor
	//ImGui::ColorEdit4("bufferColor", (float*)&bufferColor);

	//show backJumpTargetPos
	ImGui::Text("backJumpTargetPos : %f, %f, %f", backJumpTargetPos.x, backJumpTargetPos.y, backJumpTargetPos.z);


	//show NodePos
	//ImGui::Text("NodePos : %f, %f, %f", nodePos.x, nodePos.y, nodePos.z);

	//show jumpProgress
	ImGui::Text("jumpProgress : %f", jumpProgress);



	//show hpBuffer related 
	ImGui::Text("hpBuffer : %f", hpBuffer);
	ImGui::Text("hpBufferMax : %f", hpBufferMax);
	ImGui::Text("hpBufferCd : %f", hpBufferCd);


	if (ImGui::CollapsingHeader("Status")) {
		ImGui::Text("hp : %f", hp);
		ImGui::Text("uniqueID : %d", uniqueID);
		ImGui::Text("storeID : %d", storeID);
	}

	if (ImGui::CollapsingHeader("PlayerInfo")) {
		ImGui::Checkbox("isLockOn", &isLockOn);
		ImGui::Checkbox("isFacingPlayer", &isFacingPlayer);
		ImGui::Text("angleToPlayer : %f", angleToPlayer);
		ImGui::Text("distToPlayer : %f", distToPlayer);
		ImGui::Text("Pos : %f, %f, %f", playerPos.x, playerPos.y, playerPos.z);
		ImGui::Text("Pos : %f, %f, %f", dirToPlayer.x, dirToPlayer.y, dirToPlayer.z);

	}

	if (ImGui::CollapsingHeader("Move,Pos")) {
		ImGui::SliderFloat("moveSpd", &moveSpd, 0, 5);
		ImGui::Text("Pos : %f, %f, %f", pos.x, pos.y, pos.z);

	}

	if (ImGui::CollapsingHeader("Attack")) {
		ImGui::Text("AtkCD : %f", atkCd);
		ImGui::Text("AtkOrder : %d", atkOrder);
		ImGui::Text("AtkFrame : %f", atkFrame);
		ImGui::Checkbox("isAtkDelay", &isAtkDelay);
		ImGui::Text("atkDelayCnt : %f", atkDelayCnt);
		ImGui::Checkbox("isAttacking", &isAttacking);
	}

	ImGui::PopID();  // End unique context
	ImGui::End;

}
