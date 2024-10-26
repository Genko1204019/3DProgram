#include "WanderEnemy.h"
#include"Application/StateMachine/EnemyState.h"
#include"Application/Manager/PathManager.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"
#include"Application/GameObject/Player/Player.h"
#include"Application/Manager/InventoryManager.h"


void WanderEnemy::Init()
{

	
	InitWanderEnemy();
	EnemyBase::Init();
	
	stateCnt = 0;
	PatrolTransition();

	enemyIconTex = make_shared<KdTexture>();
	enemyIconTex->Load("Asset/Textures/Minimap/enemyIcon.png");

	enemyIconAtkTex = make_shared<KdTexture>();
	enemyIconAtkTex->Load("Asset/Textures/Minimap/enemyIconAtk.png");


}

void WanderEnemy::Update()
{
	EnemyBase::Update();

	UpdateWanderCounter();
	UpdateEmmit();
	UpdatePatrolPoint();

	WeaponInfo();





}

void WanderEnemy::PostUpdate()
{
	EnemyBase::PostUpdate();
	DmgCollision();

}

void WanderEnemy::DrawLit()
{
	EnemyBase::DrawLit();

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*alertPoly, Matrix::CreateScale(0.1, 0.28, 0.1) * rotMat * transMat * Matrix::CreateTranslation(0, 5.6, 0), alertColor);

}

void WanderEnemy::GenerateDepthMapFromLight()
{
	EnemyBase::GenerateDepthMapFromLight();

}

void WanderEnemy::DrawMini()
{
	Math::Rectangle enemyRect = { 0,0,32,32 };
	miniMapPos2D.x = scaleFactor * pos.x + xOffset;
	miniMapPos2D.y = scaleFactor * pos.z + yOffset;

	Matrix sMat = Matrix::CreateScale(1, 1, 1);
	Matrix tmat = Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0);
	Matrix fMat = sMat * tmat;
	KdShaderManager::Instance().m_spriteShader.SetMatrix(fMat);	

	if(nowState == EnemyState::eFollow) KdShaderManager::Instance().m_spriteShader.DrawTex(enemyIconAtkTex, 0, 0, enemyRect.width, enemyRect.height, &enemyRect, &kWhiteColor, { 0.5,0.5 }); //fine
	else  KdShaderManager::Instance().m_spriteShader.DrawTex(enemyIconTex, 0, 0, enemyRect.width, enemyRect.height, &enemyRect, &kWhiteColor, { 0.5,0.5 });
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

}

void WanderEnemy::InitWanderEnemy()
{
	eModelName = "Asset/Models/_EnemyObj/SkeletonFighter/SkeletonFighter.gltf";
	equipModelName = "Asset/Models/_EnemyObj/Skeleton/SkeWarHelmet.gltf";
	alertPoly = make_shared<KdSquarePolygon>();
	alertPoly->SetMaterial("Asset/Textures/exMark.png");
	alertPoly->SetScale(10);
	alertColor = { 1,1,1,alertAlpha };
}

void WanderEnemy::WeaponInfo()
{
	const shared_ptr<EnemyWeapon>spWeapon = wpWeapon.lock();
	if (spWeapon) {
		spWeapon->SetAniSpd(aniSpd);
		spWeapon->SetMatrix(worldMat);
	}
}

void WanderEnemy::UpdateWanderCounter()
{
	canCheckFacingCnt--;
	followCD -= 1 * gameSpd;

	alertCnt -= 1 * gameSpd;
	alertAlpha -= 0.01 * gameSpd;
	


}

void WanderEnemy::UpdateEmmit()
{
	if (nowState == EnemyState::eFollow) emitRate = { 7.7,0.28,0.28 };
	else emitRate = { 0.7,0.7,0.7 };

}

void WanderEnemy::UpdatePatrolPoint()
{
	returnPoint = PathManager::Instance().FindPointInMap(patrolPt[1].x, patrolPt[1].z);
}



void WanderEnemy::DmgCollision()
{

	//KdCollider::SphereInfo bodySphere;
	KdCollider::SphereInfo bodySphere(KdCollider::TypeBump | KdCollider::TypeDamage, pos + Vector3(0, 1.28, 0), 1.21);
	std::list<KdCollider::CollisionResult> retHitList;
	float overlap = 0;
	Vector3 hitPos;
	Vector3 hitDir;
	bool isHit = true;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->GetTag() == GameTag::PlayerWeaponTag || obj->GetTag() == GameTag::PlayerProjectileTag) {
			if (obj->Intersects(bodySphere, &retHitList)) {		
				Utility::CalOverlap(retHitList, overlap, isHit, hitPos, hitDir);
			
				if (isHit) {
					auto player = wpPlayer.lock(); //can't cast obj to player class as it's weapon but not player
					if (player->ReadyForAtk() && mutekiCnt < 0) {
						player->UpdateHitFrame();
						OnGetDmg();

					}

				}


			}
		}
	}


}

void WanderEnemy::EnemyPatrol()
{
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

void WanderEnemy::PatrolTransition()
{
	if (stateCnt <= 0) {
		stateCnt = 280;
		nowState = EnemyState::ePatrol;
		stateMachine.ChangeState(this, new ePatrolState());
		SetAnime(wanderAni, true);
		patrolOrder = 1;
		patrolTarget = patrolPt[patrolOrder];
		followCD = 400;

	}
}

void WanderEnemy::EnemyFollow()
{

	alertColor = { 7,1,1,alertAlpha };

	moveVec = dirToPlayer;

	if (isAlert && alertCnt <= 0) {
		SetAnime(walkAni, true);
		isAlert = false;
		KdAudioManager::Instance().PlayMonsterVoiceSE();
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
		if (!giveUpFollow) {
			giveUpFollow = true;
			KdAudioManager::Instance().PlayEnemyGiveUpSE();
			moveSpd = 0.07;
		}

		//player QuestionMark
		HomingAStartPath(returnPoint.x, returnPoint.y);
		UpdateRotation(dirToNextTarget);

		distToReturnPt = (Vector3(pos.x, 0, pos.z) - Vector3(returnPoint.y * 6, 0, returnPoint.x * 6)).Length();
		if (distToReturnPt <= 7) {
			PatrolTransition();
		}

	}


}

void WanderEnemy::FollowTransition()
{
	if ((isFacingPlayer && followCD < 0) || isDizzy) {
		stateMachine.ChangeState(this, new eFollowState());
		nowState = EnemyState::eFollow;
		isDizzy = false;
		isAlert = true;
		alertCnt = 70;
		stateCnt = 1440;
		alertAlpha = 1;
		SetAnime(alertAni, false);

		KdAudioManager::Instance().PlayAlertSE();
		giveUpFollow = false;

		moveSpd = 0.1;
	}
}

void WanderEnemy::EnemyDizzy()
{
	Vector3 dizzyPos = pos + Vector3(0, 3, 0);
	KdEffekseerManager::GetInstance().SetPosById(dizzyeffId, dizzyPos);

	if (eAnime->IsAnimationEnd()) {
		SetAnime("Idle_B", false);
	}


	if (stateCnt < 0) {
		FollowTransition();
	}
}

void WanderEnemy::DizzyTransition()
{
	stateCnt = 350;
	nowState = EnemyState::eDizzy;
	stateMachine.ChangeState(this, new eDizzyState());
	//SetAnime("Hit_B", false);
	isDizzy = true;

	KdEffekseerManager::GetInstance().PlayById(dizzyeffId,"pipofm-healthring02.efkefc", { pos + Vector3(0,3,0) }, 1.4, 1.4, false);
	KdAudioManager::Instance().PlayDizzySE();
}

void WanderEnemy::ExitDizzy()
{
	KdEffekseerManager::GetInstance().StopEffectById(dizzyeffId);

}

void WanderEnemy::WanderDead()
{
	if (stateCnt < 0 && GameManager::Instance().GetCanWandererDie() && GameManager::Instance().GetFadeAlpha() <= 0) {
		GameManager::Instance().SetCanWandererDie(false);
		SetAnime("Death_A", false);
		aniSpd = 0.42;
		//objCollider->SetEnable(KdCollider::TypeDamage,false);
		
		
	}

	if (eAnime->IsAnimationEnd() && !timeToDrop) {
		timeToDrop = true;
		InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(0, 0.7, 0));

	}

}

void WanderEnemy::WanderDeadTransition()
{
	stateCnt = 140;
	nowState = EnemyState::eWanderDead;
	stateMachine.ChangeState(this, new eWanderDeadState());
	//SetAnime("Idle_B", false);
	hp = 0;

}

void WanderEnemy::HomingToTargetPoint(float pointX, float pointZ)
{
	Vector3 dirToA = Vector3(pointX, -70, pointZ) - pos;
	dirToA.Normalize();
	pos += dirToA * followSpd * gameSpd;
}

void WanderEnemy::HomingAStartPath(float _targetX, float _targetY)
{
	PathManager::Instance().FindEnemyPoint(pos.x, pos.z, enemyPointX, enemyPointY);

	path = PathManager::Instance().PathFinding(enemyPointX, enemyPointY, _targetX, _targetY);

	if (!path.empty())
	{
		for (size_t i = 0; i < path.size(); ++i)
		{
			Vector3 nodePos = PathManager::Instance().mapPos[path[i].x][path[i].y];
		 	if(GameManager::Instance().GetCanDebugWire())debugWire->AddDebugSphere(nodePos, 1.0f, kGreenColor); // Red sphere at each node
		}

		if (path.size() > 1) {
			nextTarget = PathManager::Instance().mapPos[path[1].x][path[1].y];
			dirToNextTarget = nextTarget - pos;
			dirToNextTarget.Normalize();
			HomingToTargetPoint(nextTarget.x, nextTarget.z);
		}
		else {
			stateCnt = 0;
		}

	}
	


}

void WanderEnemy::OnGetDmg()
{
	auto player = wpPlayer.lock();
	mutekiCnt = 35;

	if(nowState != EnemyState::eDizzy) DizzyTransition();
	SetAnime(getHitAni, false);


}

void WanderEnemy::CallImgui()
{
	ImGui::Begin("WanderEnemy");
	ImGui::PushID(uniqueID);
	ImGui::Separator();

	//distToReturnPt
	ImGui::Text("distToReturnPt : %f", distToReturnPt);

	//checkTime TO drop
	ImGui::Text("timeToDrop : %d", timeToDrop);
	
	//show moveVec
	ImGui::Text("moveVec : %f, %f, %f", moveVec.x, moveVec.y, moveVec.z);

	//show distToPlayer 
	ImGui::Text("distToPlayer : %f", distToPlayer);
	//show angleToPlayer
	ImGui::Text("angleToPlayer : %f", angleToPlayer);
	//show isFacingPlayer
	ImGui::Text("isFacingPlayer : %d", isFacingPlayer);
	//show canCheckFacingCnt
	ImGui::Text("canCheckFacingCnt : %d", canCheckFacingCnt);

		 
	//show distToPatrolTarget patrolOrder
	ImGui::Text("distToPatrolTarget : %f", distToPatrolTarget);
	ImGui::Text("patrolOrder : %d", patrolOrder);
	//show dirToPatrolTarget
	ImGui::Text("dirToPatrolTarget : %f, %f, %f", dirToPatrolTarget.x, dirToPatrolTarget.y, dirToPatrolTarget.z);



	
	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "EnemyType : %s", EnemyTypeNames.at((EnemyType)objType));
	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "NowState : %s", EnemyStateNames.at((EnemyState)nowState));
	ImGui::Text("objType : %d", objType);
	ImGui::Text("NowState : %d", nowState);
	ImGui::Text("StateCnt : %f", stateCnt);
	ImGui::Text("aniSpd : %f", aniSpd);

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

	//show scale vector3 
	ImGui::Text("Scale : %f, %f, %f", scale.x, scale.y, scale.z);

	//show rotAngle float
	ImGui::SliderFloat("rotAngle", &rotAngle, 0, 360);

	//show pos
	ImGui::Text("Pos : %f, %f, %f", pos.x, pos.y, pos.z);

	ImGui::PopID();  // End unique context
	ImGui::End;

}
