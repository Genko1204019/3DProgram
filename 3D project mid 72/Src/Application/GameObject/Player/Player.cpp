//Player.cpp file

#include"Player.h"
#include"Application/Scene/SceneManager.h"
#include"Application/GameObject/MapObj/MapObj.h"
#include"Application/StateMachine/PlayerState.h"
#include"Application/GameObject/Projectile/PlayerProjectile.h"
#include"Application/GameObject/WeaponPoly/WeaponPoly.h"

#include"Weapon/PlayerWeapon.h"
#include"PlayerArmor/PlayerArmor.h"
#include"PlayerShield/PlayerShield.h"

#include"Application/Manager/PathManager.h"
#include"Application/Manager/InputManager.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/GameObject/Item/Item.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"
#include"Application/Manager/UiManager.h"

#include"Application/Manager/MapLoader.h"
#include"Application/GameObject/Moji/Moji.h"

#include"Application/GameObject/Projectile/EnemyProjectile.h"

#include"Application/GameObject/Enemy/BattleEnemy/BattleEnemy.h"
#include"Application/GameObject/Enemy/WanderEnemy/WanderEnemy.h"

#include"Application/GameObject/Item/Item.h"


bool Player::isWithinAtkFrame()
{
	if (atkFrame >= atkReadyAdjustFrame && atkFrame <= atkEndAdjustFrame && !isMagicWeapon) {
		return true;
	}
	else {
		return false;
	}



}

bool Player::ReadyForAtk()
{
	if(isAttacking && isWithinAtkFrame()) return true;
	else if (isSkillReleased) return true;
	else return false;
}

bool Player::isPressingMoveKey()
{
	if (!GetAsyncKeyState('W') && (!GetAsyncKeyState('A')) && (!GetAsyncKeyState('S')) && (!GetAsyncKeyState('D'))) return false;
	else return true;
	
}

void Player::Init()
{

	IconTex.Load("Asset/Textures/playerHelmet.png");

	/*dissolvePoly = make_shared<KdSquarePolygon>();
	dissolvePoly->SetMaterial("Asset/Textures/Black01.png");
	dissolvePoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);*/

	//init normal
	m_normalTex = std::make_shared<KdTexture>();
	m_normalTex->Load("Asset/Textures/normalTex.png"); //!! error
	KdShaderManager::Instance().m_StandardShader.SetWaterNormalTexture(*m_normalTex); //send it to GPU


	test = std::make_shared<KdModelWork>();
	test->SetModelData("Asset/Models/_MapObj/ChestAni/ChestAni01.gltf");


	renderPoly = make_shared<KdSquarePolygon>();

	objTag = GameTag::PlayerTag;
	objType = PlayerType::WarriorPlayer;
	drawType = eDrawTypeLit | eDrawTypeDepthOfShadow;

	
	tPoly.SetMaterial("Asset/Textures/sword_effect.png");
	tPoly.SetLength(200);

	for (int i = 0; i < afterNum; i++) {
		playerAfter[i].afterAnimator = make_shared<KdAnimator>();
		playerAfter[i].afterModel = make_shared<KdModelWork>();
		playerAfter[i].afterModel->SetModelData(pModelName);
	}

	InitModelInfo();
	InitAnimator();
	InitCollider();
	InitTexuture();
	InitStatus();
	//InitPlayerStatusFromJson();

	stateMachine.ChangeState(this, new pIdleState());

		

	poly = make_shared<KdSquarePolygon>();
	poly->SetMaterial("Asset/Textures/sword_effect.png");





}

void Player::Update()
{
	//for (int i = 0; i < 17; ++i)
	//{
	//	Math::Vector3 position = pos + Math::Vector3(21 * i, 1.4, 0);
	//	Math::Vector3 direction = worldMat.Backward();
	//	float range = 14; // Example range
	//	float angle = DirectX::XMConvertToRadians(45); // Example angle in radians
	//	Math::Vector3 color = { 4,4,4 }; // Example color
	//	KdShaderManager::Instance().WorkAmbientController().AddConeLight(position, direction, range, angle, color);
	//}

	
	UpdateMiniMapPos();

	PlayerHpBarBuffer();

	PickUpItem();

	TrailPoly();

	EquipAllGears();



	KdShaderManager::Instance().WorkAmbientController().SetDirLight(lightDir, { lightColor.x,lightColor.y,lightColor.z });//add a bit z to create shadow,face and shine player in orange light in win scene //dir(default ) and color  (strong light)
	KdShaderManager::Instance().WorkAmbientController().SetAmbientLight(ambientColor); //color

	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true); // false & true or true & false
	KdShaderManager::Instance().WorkAmbientController().SetheightFog({hfogColor.x,hfogColor.y ,hfogColor.z }, topVal, botVal, hfogDist);
	
	

	isHitStop = false;
	//AttackManager();

	

	UpdateMatrix();
	UpdateRotation();
	UpdateAnimation();
	UpdateGravity();

	ManageMP();
	ManageFog();

	PlayerLockOn();

	EnemyInfo();
	CameraInfo();
	GameInfo();
	WeaponInfo();

	MoveManager();

	EffectManager();

	StateMachineManager();
	CounterManager();

	PlayerShot();

	DebugKey();



	

}

void Player::PostUpdate()
{


	GroundCollision();
	BodyCollision();
	DmgCollision();

	



}

void Player::DrawLit()
{

	//DashEffect();

	static Math::Vector2 offset = { 0.0f,0.0f };
	offset.x += 0.0001f;
	if (offset.x > 1) offset.x -= 1;
	offset.y += 0.0001f;
	if (offset.y > 1) offset.y -= 1;
	KdShaderManager::Instance().m_StandardShader.SetWaterUVOffset(offset);
	//KdShaderManager::Instance().m_StandardShader.SetUVTiling({ 5,5 });

	//KdShaderManager::Instance().m_StandardShader.SetWaterEnable(true);
	//KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld); //84
	//KdShaderManager::Instance().m_StandardShader.SetWaterEnable(false);

	//KdShaderManager::Instance().m_StandardShader.SetWaterEnable(true);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*bodyModel, Matrix::CreateScale(1, 1, 1) * worldMat, playerColor);
	//KdShaderManager::Instance().m_StandardShader.SetWaterEnable(false);

	//static float d = 0;
	//float move = 0.011f;
	//if(GetAsyncKeyState('1')) d += move;	
	//KdShaderManager::Instance().m_StandardShader.SetDissolve(d);
	//KdShaderManager::Instance().m_StandardShader.DrawPolygon(*dissolvePoly, Matrix::CreateScale(35, 35, 35)* worldMat);

}

void Player::GenerateDepthMapFromLight()
{

	KdShaderManager::Instance().m_StandardShader.DrawModel(*bodyModel, worldMat, playerColor);

}

void Player::DrawUnLit()
{

}

void Player::DrawSprite()
{

	if (GameManager::Instance().GetIsMiniMap()) {
		

	}


	//rtc.ChangeRenderTarget(rtp);
	//rtp.ClearTexture(Math::Color(0, 0, 0, 0));
	//KdShaderManager::Instance().m_StandardShader.DrawModel(*bodyModel,worldMat, playerColor);
	//rtc.UndoRenderTarget();
	//KdShaderManager::Instance().m_spriteShader.DrawTex(rtp.m_RTTexture, 0, 0);


}

void Player::DrawMoji()
{
	//KdShaderManager::Instance().m_spriteShader.DrawString(0, 0, "HPポーション　伝説の剣　帽子",kWhiteColor);
	//KdShaderManager::Instance().m_spriteShader.DrawString(0, 0, GameManager::Instance().gameName.c_str(), kWhiteColor);

		//KdShaderManager::Instance().m_spriteShader.DrawString(0, 0, "",kWhiteColor);

		/*if (GameManager::Instance().GetIsInventoryOpen()) {
			KdShaderManager::Instance().m_spriteShader.DrawString(0, -280, "ENTERキーで使用", kWhiteColor,7.7);

		}*/

	//char m = '伝';
	//KdShaderManager::Instance().m_spriteShader.DrawString(0, 0, m.c_str(), kWhiteColor);


}

void Player::DrawMini()
{
	if (GameManager::Instance().GetIsMiniMap()) {
		miniMapPos2D.x = scaleFactor * pos.x + xOffset;
		miniMapPos2D.y = scaleFactor * pos.z + yOffset;

		KdShaderManager::Instance().m_spriteShader.DrawBox(0,0, 1280, 720, &kBlackColor, true);
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

		//KdShaderManager::Instance().m_spriteShader.DrawBox(miniMapPos2D.x - GameManager::Instance().scrollX, miniMapPos2D.y - GameManager::Instance().scrollY, 14, 14, &kRedColor, true);

		Matrix sMat = Matrix::CreateScale(1, 1, 1);
		Matrix tmat = Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().scrollX, miniMapPos2D.y - GameManager::Instance().scrollY, 0);
		Matrix fMat = sMat * tmat;

		KdShaderManager::Instance().m_spriteShader.SetMatrix(fMat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&IconTex, 0, 0, iconRect.width, iconRect.height, &iconRect, &kWhiteColor, { 0.5,0.5 }); //fine





		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

		


	}
}

void Player::UpdateMiniMapPos()
{
	/*xOffset = GameManager::Instance().sliderA;
	yOffset = GameManager::Instance().sliderB;*/



	scrollX = pos.x * scaleFactor;
	scrollY = pos.z * scaleFactor;

	if (scrollX > scrollXMax) scrollX = scrollXMax;
	if (scrollX < scrollXMin) scrollX = scrollXMin;
	if (scrollY > scrollYMax) scrollY = scrollYMax;
	if (scrollY < scrollYMin) scrollY = scrollYMin;

	MapLoader::Instance().scrollX = scrollX;
	MapLoader::Instance().scrollY = scrollY;

	MapLoader::Instance().playerMiniPosX = playerMiniPosX;
	MapLoader::Instance().playerMiniPosY = playerMiniPosY;
}

void Player::DrawRenderTarget()
{
	inventoryPlayerMat = Matrix::CreateScale(2.1) * Matrix::CreateTranslation(worldMat.Translation()) * Matrix::CreateTranslation(0, 3.5, 0);
	KdShaderManager::Instance().m_StandardShader.DrawModel(*bodyModel, inventoryPlayerMat, playerColor);

}


void Player::InitModelInfo()
{

	bodyModel = make_shared<KdModelWork>();
	bodyModel->SetModelData(pModelName);



}

void Player::InitAnimator()
{

	bodyAnime = make_shared<KdAnimator>();

	SetAnime(idleAni, true);



}

void Player::InitCollider()
{

	

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("PlayerCollision", bodyModel, KdCollider::TypeDamage);


	debugWire = make_unique<KdDebugWireFrame>();

}

void Player::InitTexuture()
{
}

void Player::InitStatus()
{
	hp = maxHp;
	pos = { 0,-70,0 };

}

void Player::InitPlayerStatusFromJson()
{
	std::ifstream file("Data/PlayerData.json");
	json j;
	file >> j;

	std::string playerType;
	switch (objType) {
	case PlayerType::WarriorPlayer:
		playerType = "WarriorPlayer";
		break;
	case PlayerType::HealerPlayer:
		playerType = "HealerPlayer";
		break;
	default:
		return;
	}

	if (j.contains(playerType)) {
		hp = j[playerType]["hp"].get<float>();
		maxHp = j[playerType]["maxHp"].get<float>();
		pos = Vector3(j[playerType]["pos"][0], j[playerType]["pos"][1], j[playerType]["pos"][2]);
		scaleMat = Matrix::CreateScale(j[playerType]["scale"][0], j[playerType]["scale"][1], j[playerType]["scale"][2]);
		weaponId = j[playerType]["weaponId"].get<int>();
		moveSpd = j[playerType]["moveSpd"].get<float>();
		atkDmg = j[playerType]["atkDmg"].get<float>();
	}


	//KdRenderTargetPack rtPack;
	//KdRenderTargetChanger rtChanger;



}

void Player::SetAnime(string _aniName, bool _isLoop)
{

	const shared_ptr<PlayerWeapon>spWeapon = wpWeapon.lock();
	if(spWeapon) spWeapon->SetAnime(_aniName, _isLoop);

	const shared_ptr<PlayerArmor>spArmor = wpArmor.lock();
	if (spArmor) spArmor->SetAnime(_aniName, _isLoop);

	const shared_ptr<PlayerShield>spShield = wpShield.lock();
	if (spShield) spShield->SetAnime(_aniName, _isLoop);

	bodyAnime->SetAnimation(bodyModel->GetData()->GetAnimation(_aniName), _isLoop);


	for (int i = 0; i < afterNum; i++) {
		if (playerAfter[i].aftetAniSpd > 0) {
			playerAfter[i].afterAnimator->SetAnimation(playerAfter[i].afterModel->GetData()->GetAnimation(_aniName), _isLoop);
		}
	}


}

void Player::SetAnime(string _aniName, string _aniName2, bool _isLoop)
{
	bodyAnime->SetAnimation(bodyModel->GetData()->GetAnimation(_aniName), _isLoop);

}

void Player::ResetAnime()
{
	switch (nowState)
	{
	case PlayerState::pIdle:
		SetAnime(idleAni, true);
		break;
	case PlayerState::pMove:
		SetAnime(walkAni, true);
		break;
	default:
		break;
	}
}

void Player::ResetAnimeName()
{
	atkAni1 = "1H_Melee_Attack_Slice_Diagonal";
	atkAni2 = "1H_Melee_Attack_Chop.001";
	atkAni3 = "1H_Melee_Attack_Slice_Horizontal";
	atkAni4 = "1H_Melee_Attack_Chop.001";

}


void Player::PlayerStrikeKill()
{
	aniSpd = 3;

	if (strikeCnt <= 0 && strikeCombo < 21) {
		strikeCnt = 35;
		strikeCombo++;
		SetAnime(atkAni1, false);
	}

	if (strikeCombo >= 10) {
		//strikeEffId
	}

	SetEffectPos(auraEffId, pos + Vector3(0, 1, 0));

}

void Player::StrikeKillTransition()
{
	
	stateCnt = 770;
	strikeCombo = 0;
	strikeCnt = 0;
	pos = strikeStandPos;
	GameManager::Instance().SetIsStrikeSkill(true);
	CreateEffect(auraEffId,auraEffName,pos+Vector3(0,1,0),0.4,1,false);
	nowState = PlayerState::pStrikeKill;
	stateMachine.ChangeState(this, new pStrikeKillState());
}

void Player::PlayerStun()
{
	if (!bodyAnime->IsAnimationEnd() || stunCnt > 0)  return;
		MoveTransition();
		IdleTransition();
		AtkTransition();

	
}

void Player::StunTransition()
{

	if (nowState == PlayerState::pSkill) return;

	stunCnt = 35;
	mutekiCnt = 70;

	SetAnime(stunAni, false);
	nowState = PlayerState::pStun;
	stateMachine.ChangeState(this, new pStunState());



	KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { pos }, 1, 0.7, false);

	getHitDir = Vector3(pos.x, 0, pos.z) - Vector3(enemyPos.x, 0, enemyPos.z);
	getHitDir.Normalize();
	float rot = atan2(getHitDir.x, getHitDir.z);
	getHitPos = getHitDir * 1.4;

	KdAudioManager::Instance().PlayGetHitSE();


}

void Player::EnterStun()
{
}






void Player::UpdateMatrix()
{
	scaleMat = Matrix::CreateScale(1.56);
	//scaleMat = Matrix::CreateScale(1.5,2.1,1.5);

	rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
	transMat = Matrix::CreateTranslation(pos);

	worldMat = scaleMat * rotMat * transMat;
	wpWorldMat = Matrix::CreateScale(2) * rotMat * Matrix::CreateTranslation(pos + Vector3(0, -0.1, 0));


}

void Player::UpdateAnimation()
{
	if (GameManager::Instance().GetIsInventoryOpen()) {
		bodyAnime->AdvanceTime(bodyModel->WorkNodes(), aniSpd);
	}
	else{
		bodyAnime->AdvanceTime(bodyModel->WorkNodes(), aniSpd * gameSpd);
	}

	
}

void Player::UpdateRotation()
{

	//if(!isMoving || isAttacking || nowState == PlayerState::pAttack) return;
	if (!isMoving && !isDashing) return;

	if (isMoving || isDashing) {
		Matrix nowRot = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
		Vector3 nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, -nowRot);
		Vector3 toVec = {};

		if (isLockOn)	toVec = camToEnemyVec;
		else {
			if(nowState == PlayerState::pDefense || isDashing) toVec = atkVecOld;
			else toVec = moveVec;
		}

		float dot = nowVec.Dot(toVec);
		dot = std::clamp(dot, -1.0f, 1.0f);
		long double ang = DirectX::XMConvertToDegrees(acos(dot));
		if (ang >= 0.1f) {
			if (ang > 5) ang = 5.0f;
			Math::Vector3 cross = toVec.Cross(nowVec);
			cross.Normalize();
			if (cross.y >= 0) {
				rotAngle -= ang * 1.7; //1.7
				if (rotAngle <= 0) rotAngle += 360;
			}
			else {
				rotAngle += ang * 1.7;
				if (rotAngle >= 360) rotAngle -= 360;
			}
		}
	}

	


	

}

void Player::UpdateGravity()
{
	if (GetAsyncKeyState(VK_SPACE)) {
		pos.y += 2.14 * gameSpd;
	}

	
	//gravity
	if (!GameManager::Instance().GetIsStrikeSkill() && !GameManager::Instance().GetIsInventoryOpen()) {
		pos.y -= gravity;
		gravity += gravityPow;
	}


}

void Player::EnemyInfo()
{
	const shared_ptr<BattleEnemy>spEnemy = wpEnemy.lock();
	if (spEnemy) {
		enemyPos = spEnemy->GetPosition();
		//enemyId = spEnemy->enemyId;
	}
}

void Player::CameraInfo()
{
	const shared_ptr<GameCamera>spCam = wpCamera.lock();
	if (spCam)
	{
		camPos = spCam->GetPosition();
		camRotMat = spCam->GetRotationYMatrix();
		camToEnemyVec = spCam->camToEnemyXZ;

		spCam->WorkCamera()->ConvertWorldToScreenDetail(pos, pos2D);

	}
}

void Player::GameInfo()
{
	gameSpd = GameManager::Instance().GetGameSpd();
}

void Player::WeaponInfo()
{

	const shared_ptr<PlayerWeapon>spWeapon = wpWeapon.lock();
	if (spWeapon) {
		spWeapon->SetAniSpd(aniSpd);
		spWeapon->SetMatrix(worldMat);
		spWeapon->SetIsPlayerAttacking(isSkillReleased);
		spWeapon->SetWpRotMat(rotMat);
		spWeapon->playerWorldMat = worldMat;
		spWeapon->isWithAtkFrame = isWithinAtkFrame();
		spWeapon->playerAtkMovePos = atkWpMovePos;
		spWeapon->playerAtkOrder = atkOrder;
		atkDmg = spWeapon->GetWpDmg();
	}

	const shared_ptr<PlayerArmor>spArmor = wpArmor.lock();
	if (spArmor) {
		spArmor->SetMatrix(worldMat);
		spArmor->SetAniSpd(aniSpd);
	}

	const shared_ptr<PlayerShield>spShield = wpShield.lock();
	if (spShield) {
		spShield->SetMatrix(worldMat);
		spShield->SetAniSpd(aniSpd);
	}


}


void Player::SetCollider(KdCollider::SphereInfo& _sphereObj, Vector3 _sphereCenter, float _sphereRadius, UINT _hitType)
{
	_sphereObj.m_sphere.Center = _sphereCenter;
	_sphereObj.m_sphere.Radius = _sphereRadius;
	_sphereObj.m_type = _hitType;

}

void Player::SetCollider(KdCollider::RayInfo& _rayObj, Vector3 _rayPos, Vector3 _rayPosAdd, Vector3 _rayDir, float _rayRange, UINT _hitType)
{
	_rayObj.m_pos = _rayPos;
	_rayObj.m_pos += _rayPosAdd;
	_rayObj.m_dir = _rayDir;
	_rayObj.m_range = _rayRange;
	_rayObj.m_type = _hitType;
}

void Player::SetDebugWire(KdCollider::SphereInfo& _sphereObj, Color _sphereColor)
{
	if (GameManager::Instance().GetCanDebugWire()) 	debugWire->AddDebugSphere(_sphereObj.m_sphere.Center, _sphereObj.m_sphere.Radius, _sphereColor);
}

void Player::SetDebugWire(KdCollider::RayInfo& _rayObj, Color _rayColor)
{
	if (GameManager::Instance().GetCanDebugWire()) debugWire->AddDebugLine(_rayObj.m_pos, _rayObj.m_pos + _rayObj.m_dir * _rayObj.m_range, _rayColor);
}

void Player::GroundCollision()
{

	KdCollider::RayInfo ray;
	const float stepHigh = 0.28f;
	SetCollider(ray, pos, Vector3(0, stepHigh, 0), Vector3::Down, gravity + stepHigh, KdCollider::TypeGround);
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

				if (mapObj->GetType() == MapType::TrapType && mutekiCnt < 0) {
					mapObj->OnHit();
					mutekiCnt = 70;
					hp -= 10;
					hpBuffer += 10;
					UiManager::Instance().bloodAlpha = 1;
					Vector3 blowDir = pos - mapObj->GetPosition();
					blowDir.Normalize();
					BlowAwayTransition(blowDir, 2.8, -0.14);
				}

				
			}

		}
	}

}

void Player::BodyCollision()
{

	KdCollider::SphereInfo bodySphere;
	SetCollider(bodySphere, pos +Vector3(0,1.14,0), 1.f, KdCollider::TypeGround); // size used 0.5
	std::list<KdCollider::CollisionResult> retBodyList;
	//debugWire->AddDebugSphereByType(bodySphere,kGreenColor);

	for (auto& obj : SceneManager::Instance().GetObjList()) {
			if (obj->Intersects(bodySphere, &retBodyList)) {
				float overlap = 0;
				bool isHit = false;
				Math::Vector3 hitDir;
				Vector3 hitPos;

				for (auto& ret : retBodyList) {
					if (overlap < ret.m_overlapDistance) {
						overlap = ret.m_overlapDistance;
						hitDir = ret.m_hitDir;
						hitPos = ret.m_hitPos;
						isHit = true;

						if (isHit) {

							if (obj->GetTag() == GameTag::StageTag) {
								pos.x += hitDir.x * overlap;
								pos.z += hitDir.z * overlap;
							}

							if (obj->GetTag() == GameTag::MapTag) {
								if (obj->GetType() == MapType::GoalType) {
									if (GameManager::Instance().GetStageProgress() < 100 && effCnt < 0) {
										KdEffekseerManager::GetInstance().Play("ef01_50_MagicOne_Neutral.efkefc", { obj->GetPosition()  + Vector3(0,2.1,0)}, 1, 1, false);
										KdAudioManager::Instance().PlayUiDenieSE();
										effCnt = 77;
									}
									else GameManager::Instance().GoToNextFloor();
								}
								else if (obj->GetType() != MapType::WallType && obj->GetType() != MapType::GoalType && obj->GetType() != MapType::BottleType) {
									obj->OnHit();

									if (obj->GetType() == MapType::TrapType) {
										//cal blowDir , it is opposite of obj->GetPsition
										/*Vector3 blowDir = pos - obj->GetPosition();
										blowDir.Normalize();
										float blowPow = 2.8;
										BlowAwayTransition(blowDir, blowPow,-0.14);*/
									}

								}

						
								pos.x += hitDir.x * overlap;
								pos.z += hitDir.z * overlap;
							}

							else if (obj->GetTag() == GameTag::ItemTag) {
								shared_ptr<Item> item = static_pointer_cast<Item>(obj);

								if (item->isPickUp == false) {
									InventoryManager::Instance().AddItem(item);
									//item->ItemGetPickUP();
									//obj->SetExpired();

									//UiManager::Instance().OpenMessageBox({ -210,0 }, item->itemName);
									//create moji
									shared_ptr<Moji> messageMoji = make_shared<Moji>();
									messageMoji->SetType(MojiType::MessageMoji);
									messageMoji->SetMojiText(item->itemName);
									//messageMoji->SetMojiPos({ -630,0 });
									messageMoji->Init();
									SceneManager::Instance().AddObject(messageMoji);

									KdAudioManager::Instance().PlayPickUpSE();
								}
								
							}

							
						}

					}
				}


			}
	}



}

void Player::DmgCollision()
{
	
	KdCollider::SphereInfo dmgSphere(KdCollider::TypeDamage,pos + Vector3(0,1,0),1.f);
	std::list<KdCollider::CollisionResult> retDmgList;
	//debugWire->AddDebugSphereByType(dmgSphere, kRedColor);

	float overlap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
			if (obj->Intersects(dmgSphere, &retDmgList)) {
				for (auto& ret : retDmgList) {
					
					if (overlap < ret.m_overlapDistance) {
						overlap = ret.m_overlapDistance;
						hitDir = ret.m_hitDir;
						isHit = true;
					}

					if (isHit) {
						if (obj->GetTag() == GameTag::EnemyWeaponTag) {
							shared_ptr<EnemyWeapon> enemyWeapon = static_pointer_cast<EnemyWeapon>(obj);
							if (enemyWeapon->isAtkEnable) {

								if (isDefensing) {
									if (mutekiCnt < 0) {
										mutekiCnt = 35;
										KdEffekseerManager::GetInstance().Play("greenShield.efkefc", { pos + Vector3(0,2.1,0) }, 0.4, 2.1, false);
									}
								}
								else {
									OnHit();
								}

								/*if (mutekiCnt < 0) {
									hp -= 5;
									mutekiCnt = 35;
									Vector3 blowDir = pos - enemyWeapon->GetEnemyPos();
									blowDir.Normalize();
									BlowAwayTransition(blowDir, toBlowPow, blowGravity);
								}*/

							}
							
							

						}
						else if (obj->GetTag() == GameTag::EnemyProjectileTag) {
							//OnHit();
							if (mutekiCnt < 0) {
								hp -= 5;
								hpBuffer += 5;
								mutekiCnt = 35;
								Vector3 blowDir = pos - obj->GetPosition();
								blowDir.Normalize();
								BlowAwayTransition(blowDir, toBlowPow, blowGravity,true);
							}

							obj->OnHit();
						}
						else if (obj->GetTag() == GameTag::EnemyTag) {
							if (obj->GetType() == EnemyType::eWanderer) {
								shared_ptr<WanderEnemy> wanderEnemy = static_pointer_cast<WanderEnemy>(obj);
								if (wanderEnemy->GetNowState() == EnemyState::eFollow || wanderEnemy->GetNowState() == EnemyState::eDizzy) {
									wpCamera.lock()->StartCameraTwist();
									wanderEnemy->WanderDeadTransition();
									KdAudioManager::Instance().PlayEncounterSE();
								}
							}
						}
						


						
						
					}

					

				}
			}
	}


	


}

void Player::ItemCollision()
{
}

void Player::CounterManager()
{
	stateCnt -= 1.f * gameSpd;
	atkMoveCnt -= 1.f * gameSpd;
	atkCD--;
	//atkDelayCnt -= 1.f * gameSpd;


	stunCnt -= 1.f * gameSpd;

	dashCd--;
	dashDuration--;

	atkFrame += 1.f * gameSpd * aniSpd ;


	effCnt--;

	mutekiCnt-= 1.f * gameSpd;


	
	//strike
	strikeCnt--;


	if(stamina < maxStamina) stamina += 0.15f * gameSpd;

	//denfense
	defenseDuration -= 1.f * gameSpd;

	aniFrame++;

}

void Player::ManageMP()
{
	mp += 0.021;
	if (mp > maxMp) mp = maxMp;
	if (mp < 0) mp = 0;
}

void Player::ManageFog()
{
	if (GameManager::Instance().GetIsBattleMode())	topVal = 0;
	else if (GameManager::Instance().GetFloorNum() == 0) topVal = 0;
	else											topVal = 0.7;
	
}


void Player::EquipAllGears()
{
	if (!hasEquipment) {
		hasEquipment = true;
		shared_ptr<PlayerWeapon> weapon = make_shared<PlayerWeapon>();
		weapon->Init();
		weapon->SetWpId(1);
		weapon->SetSkillId(1);
		SceneManager::Instance().AddObject(weapon);
		SetWeapon(weapon);

		shared_ptr<PlayerShield> shield = make_shared<PlayerShield>();
		shield->SetModelName(shieldModelName);
		shield->Init();
		SceneManager::Instance().AddObject(shield);
		SetShield(shield);

		shared_ptr<PlayerArmor> armor = make_shared<PlayerArmor>();
		armor->SetModelName(armorModelName);
		armor->Init();
		SceneManager::Instance().AddObject(armor);
		SetArmor(armor);


	

	}

}

void Player::ChangeArmor(int _armorId, string _modelPath, float _wpDef)
{

	armorModelName = _modelPath;
	wpArmor.lock()->ChangeModel(armorModelName);
	wpArmor.lock()->SetWpDef(_wpDef);	
	KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { inventoryPlayerMat.Translation() }, 0.7, 1, false);
	SetAnime(cheerAni, false);

	
}

void Player::ChangeWeapon(int _wpId,int _skillId, string _modelPath, float _wpDmg)
{
	skillId = _skillId;
	wpWeapon.lock()->ResetWeapon(_wpId, _skillId, _modelPath);
	wpWeapon.lock()->SetWpDmg(_wpDmg);	
	KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { inventoryPlayerMat.Translation() }, 0.7, 1, false);
	SetAnime(cheerAni, false);

	if (_skillId == 2) { //2 = magic staff related
		isMagicWeapon = true;
	}
	else {
		isMagicWeapon = false;
	}



}

void Player::UsePotion(int _potionId, Item& _item)
{
	SetAnime("Use_Item", false);

	switch (_potionId)
	{
	case PotionType::HpPotionType:
		hp += _item.healAmount;
		if (hp > maxHp) hp = maxHp;
		KdEffekseerManager::GetInstance().Play("NA_v3_2.5d_shining_4UseItem.efkefc", { inventoryPlayerMat.Translation() + Vector3(0,2.8,0)}, 0.56, 0.77, false);
		break;
	case PotionType::MpPoitionType:
		mp += _item.healAmount;
		if (mp > maxMp) mp = maxMp;
		KdEffekseerManager::GetInstance().Play("NA_v3_2.5d_shining_4UseItem.efkefc", { inventoryPlayerMat.Translation() + Vector3(0,2.8,0) }, 0.56, 0.77, false);
		break;
	case PotionType::AtkPotionType:
		KdEffekseerManager::GetInstance().Play("NA_v3_2.5d_shining_4UseItem.efkefc", { inventoryPlayerMat.Translation() + Vector3(0,2.8,0) }, 0.56, 0.77, false);
		break;


	default:
		break;
	}
}

void Player::PickUpItem()
{



}

void Player::UseItem()
{
}




void Player::PlayerHpBarBuffer()
{
	hpBufferCd--;

	while (hpBuffer > 0 && hpBufferCd < 0) {
		hpBufferCd = 7;
		hpBufferMax-=1;
		hpBuffer-=1;
	}

}

void Player::StateMachineManager()
{
	stateMachine.Update(this);


}

void Player::PlayerDash()
{
	//PlayerFreeMove();

	if(isLockOn)pos += dashVec * moveSpd * gameSpd;
		else pos += atkVecOld * moveSpd * gameSpd;


	if (bodyAnime->IsAnimationEnd()) {
		AtkTransition();
		MoveTransition();
		IdleTransition();
	}
	
	
	

}

void Player::DashTransition()
{

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::ShiftKey) && dashCd <= 0 && stamina >= 20)
	{
		
		stateMachine.ChangeState(this, new pDashState());

		if (!isLockOn) {
			dahsEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
			KdEffekseerManager::GetInstance().Play("dashEff04.efkefc", { pos + Vector3(0,2.1,0) }, 1.4, 1.4, false);
		}
	
		//Vector3 forward = { GameManager::Instance().controlX,  GameManager::Instance().controlY, GameManager::Instance().controlZ};
		Vector3 forward = { -1,0,0 };

		Vector3 moveDirection = atkVecOld;
		moveDirection.Normalize();

		float dot = forward.Dot(moveDirection);
		float angle = acosf(dot);

		Vector3 axis = forward.Cross(moveDirection);
		if (axis.LengthSquared() > 0.0f) {
			axis.Normalize();
		}

		KdEffekseerManager::GetInstance().SetRotationById(dahsEffId, axis, angle);

		mutekiCnt = 35; // muteki dash time

		KdAudioManager::Instance().PlayDashSE();

	}



}

void Player::EnterDash()
{
	isDashing = true;
	dashCd = 20;
	dashDuration = 21;
	stamina -= 20;
	moveSpd = moveSpdDash;

	if (nowState == PlayerState::pAttack)
	{
		dashVec = atkVecOld;
		SetAnime(dashForAni, false);
	}
	else if (nowState == PlayerState::pMove)
	{
		dashVec = moveVec;
		if (isLockOn)	CheckDashDirection();
		else 			SetAnime(dashForAni, false);

	}
	else if (nowState == PlayerState::pIdle)
	{
		dashVec = atkVecOld;
		SetAnime(dashForAni, false);
	}
	else if (isLockOn) {
		dashVec = atkVec;
	}

	
	nowState = PlayerState::pDash;


}

void Player::ExitDash()
{
	moveSpd = moveSpdWalk;
	isDashing = false;
}

void Player::CheckDashDirection()
{
	switch (moveId)
	{
	case 1:
		SetAnime(dashForAni, false);
		break;
	case 2:
		SetAnime(dashBackAni, false);
		break;
	case 3:
		SetAnime(dashLeftAni, false);
		break;
	case 4:
		SetAnime(dashRightAni, false);
		break;
	default:
		break;
	}
}

void Player::PlayerMove()
{


	if (isLockOn)	PlayerLockMove();
	else			PlayerFreeMove();

	if (moveVec.LengthSquared() != 0) {
		moveVec.Normalize();
	}

	SkillTransition();
	DashTransition();
	IdleTransition();
	AtkTransition();
	DefenseTransition();

	KdAudioManager::Instance().PlayFootStepSE();


}

void Player::PlayerFreeMove()
{

	if (GameManager::Instance().GetIsInventoryOpen()) return;

	if (GetAsyncKeyState('W')) {
		moveVec += Math::Vector3::TransformNormal({ 0,0,1 }, camRotMat);
		atkVec = moveVec;
	}
	if (GetAsyncKeyState('S')) {
		moveVec += Math::Vector3::TransformNormal({ 0,0,-1 }, camRotMat);
		atkVec = moveVec;
	}
	if (GetAsyncKeyState('A')) {
		moveVec += Math::Vector3::TransformNormal({ -1,0,0 }, camRotMat);
		atkVec = moveVec;
	}
	if (GetAsyncKeyState('D')) {
		moveVec += Math::Vector3::TransformNormal({ 1,0,0 }, camRotMat);
		atkVec = moveVec;
	}

	pos += moveVec * moveSpd * gameSpd;


	

}

void Player::PlayerLockMove()
{

	if (GetAsyncKeyState('W') && moveVec == Vector3::Zero) {
		moveId = 1;
		moveVec += Math::Vector3::TransformNormal({ 0,0,1 }, camRotMat);
		atkVec = moveVec;
		if (bodyAnime->IsThisAnimationEnd(walkAni) && bodyAnime->IsThisAnimationEnd(dashForAni) && bodyAnime->IsThisAnimationEnd(walkLeftAni) && bodyAnime->IsThisAnimationEnd(walkRightAni)) {
			SetAnime(walkAni, false);
		}
	}
	if (GetAsyncKeyState('S')) {
		moveId = 2;
		moveVec += Math::Vector3::TransformNormal({ 0,0,-1 }, camRotMat);
		atkVec = moveVec;
		if (bodyAnime->IsThisAnimationEnd(walkBackAni) && bodyAnime->IsThisAnimationEnd(dashBackAni) && bodyAnime->IsThisAnimationEnd(walkLeftAni) && bodyAnime->IsThisAnimationEnd(walkRightAni)) {
			SetAnime(walkBackAni, false);
		}
	}
	if (GetAsyncKeyState('A')) {
		moveId = 3;
		moveVec += Math::Vector3::TransformNormal({ -1,0,0 }, camRotMat);
		atkVec = moveVec;
		if (bodyAnime->IsThisAnimationEnd(walkLeftAni) && bodyAnime->IsThisAnimationEnd(dashLeftAni)) {
			SetAnime(walkLeftAni, false);
		}

	}
	if (GetAsyncKeyState('D')) {
		moveId = 4;
		moveVec += Math::Vector3::TransformNormal({ 1,0,0 }, camRotMat);
		atkVec = moveVec;
	if (bodyAnime->IsThisAnimationEnd(walkRightAni) && bodyAnime->IsThisAnimationEnd(dashRightAni)) {
			SetAnime(walkRightAni, false);
		}

	}

	pos += moveVec * moveSpd * gameSpd;


}

void Player::MoveManager()
{
	moveVec = {};


	if(isLockOn) atkVecOld = camToEnemyVec;
	else {
		if (atkVec != Vector3::Zero) atkVecOld = atkVec;
	}


	atkVec = {};


}

void Player::PlayerLockOn()
{
	if (InputManager::Instance().IsKeyJustPressed(TKey)) {
		if (isLockOn) {
			ResetAnime();
			isLockOn = false;
			wpEnemy.lock()->SetIsLockOn(false);
			distToLockEnemy = 100;
		}
		else {
			for (auto& obj : SceneManager::Instance().GetObjList()) {
				if (obj->GetTag() == GameTag::EnemyTag && obj->GetType() != EnemyType::eWanderer) {
					auto enemy = static_pointer_cast<BattleEnemy>(obj);
					if (enemy->GetHp() > 0) {
						currentlockDist = Vector3::Distance(pos, enemy->GetPosition());
						if (currentlockDist < distToLockEnemy) {
							distToLockEnemy = currentlockDist;
							if (wpEnemy.lock()) wpEnemy.lock()->SetIsLockOn(false);
							SetEnemy(enemy);
							wpCamera.lock()->SetEnemy(enemy);
							enemy->SetIsLockOn(true);
							isLockOn = true;
						}
					}

				}
			}
		}

		


	}

}

void Player::PlayerHitStop()
{
	



}

void Player::EffectManager()
{
	/*if (GetAsyncKeyState('Q') && effCnt < 0) {
		effCnt = 60;
		effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
		KdEffekseerManager::GetInstance().Play("NA_healing_001.efkefc", { pos + Vector3(0,1.4,0) }, 1, 0.4, true);
	}
	KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(effectId), pos + Vector3(0, 1.4, 0));

	if (GetAsyncKeyState('U')) {
		KdEffekseerManager::GetInstance().StopEffectById(effectId);
	}*/

	
	KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(dahsEffId), pos + Vector3(0, 1.4, 0));


}

void Player::CreateEffect(int& _effectId, string _effecName, Vector3 _effPos, float _effScale, float _effSpd, bool _isLoop)
{
	_effectId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play(_effecName, _effPos, _effScale, _effSpd, _isLoop);

	
}

void Player::SetEffectPos(int _effecId, Vector3 _pos)
{
	KdEffekseerManager::GetInstance().SetPos(KdEffekseerManager::GetInstance().GetEffectHandleById(_effecId), _pos);
}

void Player::MoveTransition()
{
	if (isDefensing) return;
	if (!isPressingMoveKey()) return;
	if (GameManager::Instance().GetIsInventoryOpen()) return;


	if (nowState == pAttack) {
		if (bodyAnime->IsAnimationEnd() && !isAttacking)	stateMachine.ChangeState(this, new pMoveState());
	}
	
		else												stateMachine.ChangeState(this, new pMoveState());
		
	

}

void Player::EnterMove()
{
	aniSpd = 1;
	isMoving = true;
	moveSpd = moveSpdWalk;
	if (isLockOn) SetAnime(walkAni, false);
	else SetAnime(walkAni, true);

	nowState = PlayerState::pMove;


}

void Player::ExitMove()
{
	isMoving = false;


}


void Player::PlayerIdle()
{

	SkillTransition();
	MoveTransition();
	AtkTransition();
	DefenseTransition();
	DashTransition();

}

void Player::IdleTransition()
{
	if(isDefensing) return;
	if (isPressingMoveKey()) return;

	if (nowState == PlayerState::pAttack) {
		if (bodyAnime->IsAnimationEnd() && !isAttacking)	stateMachine.ChangeState(this, new pIdleState());		
	}
		else												stateMachine.ChangeState(this, new pIdleState());
	


	
}

void Player::EnterIdle()
{
	aniSpd = 1;
	if(nowState == PlayerState::pAttack) SetAnime(idleAni, true);
	else SetAnime(idleAni, true);

	nowState = PlayerState::pIdle;
}


void Player::PlayerAttack()
{

	AtkMove();
	AtkDelay();
	AtkEffect();
	SetAtkAngle();
	AtkManager();

	AtkTransition();
	DashTransition();
	MoveTransition();
	IdleTransition();
	DefenseTransition();


	

	if(!canAtkAnime) return;

	KdAudioManager::Instance().PlaySwordSE();

	/*
	single chop : 10 - 17
	single yoko : 5 - 14
	diagonal : 9 - 15
	*/


	switch (atkOrder)
	{
	case 1:
		SetAnime(atkAni1, false);
		SetAtkInfo(2.f, 10, 17, 11, 3.5, 1.7);
		break;
	case 2:
		SetAnime(atkAni2, false);
		SetAtkInfo(2.f, 12, 18, 11, 3.5, 1.7);
		break;
	case 3:
		SetAnime(atkAni3, false);
		SetAtkInfo(2.f, 10, 17, 11, 3.5, 1.7);
		break;
	case 4:
		SetAnime(atkAni4, false);
		SetAtkInfo(2.f, 5, 14, 11, 3.5, 1.7);
		break;
	default:
		break;
	}
	
	if (isMagicWeapon) {
		ShotMagicBall();
	}
	



}

void Player::AtkTransition()
{
	if (isDefensing || atkCD > 0 || isAttacking) return;

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::CKey)) {		
		stateMachine.ChangeState(this, new pAttackState());
		//KdEffekseerManager::GetInstance().Play("slashFix.efkefc", pos + Vector3(0,0,0), 1, 1, false);
		wpWeapon.lock()->slashEff = true;
		wpWeapon.lock()->atkVec = atkVecOld;
		ResetAnimeName();
	}


}

void Player::EnterAtk()
{
	isAttacking = true;
	canAtkAnime = true;
	atkCD = 7;
	atkOrder++;
	atkFrame = 0;
	atkMoveCnt = 7;
	aniSpd = 1.4;
	if (atkOrder > 4) atkOrder = 1;

	isAtkDelay = true;
	atkDelayCnt = 0;

	canSlashEff = true;

	nowState = PlayerState::pAttack;


}

void Player::ExitAtk()
{
	isAttacking = false;
	isAtkDelay = false;
	atkDelayCnt = 0;
	aniSpd = 1;
}

void Player::SetAtkInfo(float _aniSpd, float _atkStartframe, float _atkEndFrame, float _atkDmg, float _atkMoveDist, float _pushEnemyPow)
{
	//aniSpd = _aniSpd;
	atkFrame = _atkStartframe;
	atkEndFrame = _atkEndFrame;
	//atkDmg = _atkDmg;
	atkMoveDist = _atkMoveDist;
	pushEnemyPow = _pushEnemyPow;

	atkMovePos = pos + atkVecOld * atkMoveDist;
	atkWpMovePos = pos + atkVecOld * (atkMoveDist * 0.7);
	canAtkAnime = false;

}

void Player::SetAtkAngle()
{
	float atkAngle = atan2(atkVecOld.x, atkVecOld.z);
	rotAngle = DirectX::XMConvertToDegrees(atkAngle);
	rotAngle += 180;
	if (rotAngle >= 360) rotAngle -= 360;
}

void Player::AtkManager()
{

	atkReadyFrame = 28; //21
	atkEndFrame = 56;
	pushEnemyPow = 3.5;
	//atkDmg = 11;
	atkMoveDist = 2.1;


	atkReadyAdjustFrame = atkReadyFrame / aniSpd;
	atkEndAdjustFrame = atkEndFrame / aniSpd;

}

void Player::AtkMove()
{
	if (isAttacking && atkMoveCnt < 0 && atkFrame > atkReadyFrame) { // or > 21
		
		if (!isAtkDelay) {
			if (GameManager::Instance().GetIsBattleMode() && !isMagicWeapon) {
				pos.x = Utility::Lerp(pos.x, atkMovePos.x, 0.1f);
				pos.z = Utility::Lerp(pos.z, atkMovePos.z, 0.1f);
			}
			
		}
		

		//if(bodyAnime->IsAnimationEnd()) isAttacking = false;
		if (atkFrame > atkEndFrame) isAttacking = false;
		
		

	}

	

}

void Player::AtkDelay()
{

	// Start attack delay only when atkFrame is greater than atkReadyFrame
	if (atkFrame >= atkReadyFrame-2 && isAtkDelay) {
		if (atkDelayCnt > 0) {
			//aniSpd = 0.04f;  // Slow down the animation speed during the delay
			aniSpd = 0.14f;  // Slow down the animation speed during the delay
			atkDelayCnt--;   // Decrease the delay counter
		}

		if (atkDelayCnt <= 0) {
			isAtkDelay = false;  // End the delay
			aniSpd = 1.4f;       // Restore normal animation speed
		}
	}

}

void Player::AtkEffect()
{

	if (isWithinAtkFrame() && !isAtkDelay) {

		if (canSlashEff) {
			canSlashEff = false;
			slashEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;

			switch (atkOrder)
			{
			case 1:
				KdEffekseerManager::GetInstance().Play("blueLeftSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 2:
				KdEffekseerManager::GetInstance().Play("blueRightSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 3:
				KdEffekseerManager::GetInstance().Play("blueDownSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 4:
				KdEffekseerManager::GetInstance().Play("blueRightSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			default:
				break;
			}

		}


	}


	Vector3 forward = { 1.0f, 0.0f, 0.0f };

	Vector3 moveDirection = atkVecOld;
	moveDirection.Normalize();

	float dot = forward.Dot(moveDirection);
	float angle = acosf(dot);

	Vector3 axis = forward.Cross(moveDirection);
	if (axis.LengthSquared() > 0.0f) {
		axis.Normalize();
	}

	KdEffekseerManager::GetInstance().SetRotationById(slashEffId, axis, angle);
	KdEffekseerManager::GetInstance().SetSpeedById(slashEffId, 0.7 * gameSpd);



}

void Player::ShotMagicBall()
{
	//create playerProjectile
	shared_ptr<PlayerProjectile> magicBall = make_shared<PlayerProjectile>();
	magicBall->SetPosition(pos + Vector3(0, 2.8, 0));
	magicBall->SetMoveVec(atkVecOld);
	magicBall->SetType(pProjectileType::MagicBallType);
	magicBall->Init();
	SceneManager::Instance().AddObject(magicBall);

	mp -= 7;

}

void Player::PlayerMagicAtk()
{
}

void Player::PlayerMagicAtkTransition()
{
	if (isDefensing || atkCD > 0 || isAttacking) return;

	if (isMagicWeapon && InputManager::Instance().IsKeyJustPressed(KeyFlg::CKey)) {
		stateMachine.ChangeState(this, new pMagicAtkState());
		wpWeapon.lock()->atkVec = atkVecOld;

	}


}

void Player::EnterMagicAtk()
{

	isAttacking = true;
	canAtkAnime = true;
	atkCD = 7;
	atkFrame = 0;
	atkMoveCnt = 7;
	aniSpd = 1.4;
	atkOrder++;
	if (atkOrder > 4) atkOrder = 1;
	//isAtkDelay = true;
	//atkDelayCnt = 0;

	nowState = PlayerState::pMagicAtk;

	


}

void Player::ExitMagicAtk()
{
	isAttacking = false;
}


void Player::PlayerDefense()
{

	if (defenseDuration < 0) {
		isDefensing = false;
		stateCnt = 0;
		MoveTransition();
		IdleTransition();
		AtkTransition();
	}

}

void Player::DefenseTransition()
{

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::RButton)) {
		stateMachine.ChangeState(this, new pDefenseState());
	}

}

void Player::EnterDefense()
{
	isDefensing = true;
	defenseDuration = 35;
	SetAnime(defenseAni, false);

	nowState = PlayerState::pDefense;


}

void Player::PlayerShot()
{
	

}

void Player::PlayerBlowAway()
{
	////move to blowTargetPos
	//distToBlowTarget = Vector3::Distance(pos, blowTargetPos);
	//if (distToBlowTarget > 0.14f) {
	//	pos += blowDir * blowPow * gameSpd;
	//}
	//else {
	//	stateCnt = 0;
	//	IdleTransition();
	//	MoveTransition();
	//}

	 // Calculate the remaining distance to the target
	distToBlowTarget = Vector3::Distance(pos, blowTargetPos);

	// Apply a lerp for smoother movement
	if (distToBlowTarget > 0.28f && stateCnt > 0) {
		float lerpFactor = 0.1f; // Lerp speed factor, can be adjusted
		//pos = Vector3::Lerp(pos, blowTargetPos, lerpFactor * gameSpd);
		pos.x = Utility::Lerp(pos.x, blowTargetPos.x, lerpFactor);
		pos.z = Utility::Lerp(pos.z, blowTargetPos.z, lerpFactor);
		//blowPow = std::max(blowPow * 0.95f, 0.1f); // Reduce blowPow with a clamp to prevent it from going too low

		if (canRotate) {
			rotAngle += 21;
			if (rotAngle > 360) rotAngle = 0;
		}
		

	}
	else {
		stateCnt = 0;
		IdleTransition();
		MoveTransition();
	}
	
}

void Player::BlowAwayTransition(Vector3 _blowDir, float _blowPow, float _gravity, bool _canRotate)
{
	//if (mutekiCnt > 0) return;

	stateCnt = 70;
	stateMachine.ChangeState(this, new pBlowAwayState());
	blowPow = _blowPow;
	blowDir = _blowDir;
	blowDir.Normalize();
	gravity = _gravity;
	mutekiCnt = 140;
	//cal blowTargetPos
	blowTargetPos = pos + blowDir * blowPow;

	canRotate = _canRotate;

	KdAudioManager::Instance().PlayGetHitSE();


}

void Player::EnterBlowAway()
{
	SetAnime(getHitAni, false);
	nowState = PlayerState::pBlowAway;
}

void Player::OnGiveDmg()
{
	hitFrame = atkFrame;
	



}

void Player::TrailPoly()
{
	tPoly.AddPoint(transMat);

}



void Player::PlayerSkill()
{
	skillCnt-= 1.f * gameSpd;
	skillPrepareCnt -= 1.f * gameSpd;

	if (aniFrame > 21 && !isSkillReleased) aniSpd = 0.01;
	Vector3 effPos = pos + Vector3(0, 2.1, 0);
	KdEffekseerManager::GetInstance().SetPosById(absordEffId, effPos);

	if (!isSkillReleased && skillPrepareCnt > 0) {

		if (InputManager::Instance().IsKeyJustReleased(KeyFlg::QKey)) {
			aniSpd = 1.4;
			KdEffekseerManager::GetInstance().StopEffectById(absordEffId);
			absordEffId = -1;
			isSkillReleased = true;
			stateCnt = 0;
			ReleaseSkill();
		}

	}

	if (skillPrepareCnt <= 0 && !isSkillReleased) {
		//change back to idle state...
	}

	//PlayerFreeMove();


	if (isSkillReleased) {

		//isWithinAtkFrame();
		PlayerFreeMove();

		if (skillId == PlayerSkillType::SpinSlash) {
			KdAudioManager::Instance().PlayWpSpinSE();
		}

		if (skillCnt < 0) {
			isSkillReleased = false;
			IdleTransition();
			MoveTransition();
			KdAudioManager::Instance().StopWpSpinSE();
		}
	}


}

void Player::SkillTransition()
{

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::QKey)) {
		stateMachine.ChangeState(this, new pSkillState());

		

	
	}

}

void Player::EnterSkill()
{
	stateCnt = 7000;
	isSkillReleased = false;
	skillPrepareCnt = 350;
	SetAnime(skillPrepareAni, false);
	moveSpd = moveSpdSkill;

	nowState = PlayerState::pSkill;

	aniFrame = 0;
	aniSpd = 1;

	absordEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
	KdEffekseerManager::GetInstance().Play("absorbYellow.efkefc", pos + Vector3(0, 2.1, 0), 0.35, 1, false);

}

void Player::ExitSkill()
{
	if (absordEffId != -1) KdEffekseerManager::GetInstance().StopEffectById(absordEffId);
	isSkillReleased = false;

}

void Player::ReleaseSkill()
{
	

	//skillId = wpWeapon.lock()->GetSkillId();

	switch (skillId)
	{
	case PlayerSkillType::SpinSlash:
		SpinSlashSkill();
		mp -= 35;
		break;
	case PlayerSkillType::FlySlash:
		FlySlashSkill();
		mp -= 35;
		break;
	case PlayerSkillType::MagicAtk:
		MagicAtkSkill();
		mp -= 35;
		break;


		
	default:
		break;
	}

}

void Player::SpinSlashSkill()
{
	SetAnime(skillAni, true);
	skillCnt = 350;
}

void Player::FlySlashSkill()
{
	shared_ptr<PlayerProjectile> flySlash = make_shared<PlayerProjectile>();
	flySlash->SetPosition(pos + Vector3(0, 2.8, 0));
	flySlash->SetMoveVec(atkVecOld);
	flySlash->SetType(pProjectileType::FlySlashType);
	flySlash->SetRotY(rotAngle);
	flySlash->Init();
	SceneManager::Instance().AddObject(flySlash);
	skillCnt = 70;

	KdAudioManager::Instance().PlayPlayerSlashWaveSE();
}

void Player::MagicAtkSkill()
{
	shared_ptr<PlayerProjectile> flySlash = make_shared<PlayerProjectile>();
	flySlash->SetPosition(pos + Vector3(0, 2.8, 0));
	flySlash->SetMoveVec(atkVecOld);
	flySlash->SetType(pProjectileType::FireBallType);
	flySlash->SetRotY(rotAngle);
	flySlash->Init();
	SceneManager::Instance().AddObject(flySlash);
	skillCnt = 70;

	KdAudioManager::Instance().PlayPlayerSlashWaveSE();
}

void Player::DashEffect()
{
	//afterCnt--;
	//if (afterCnt <= 0) {
	//	afterCnt = 35;
	//	//if (isDashing) {
	//	// Store current position and rotation for afterimage
	//	AfterImage newImage;
	//	newImage.position = pos;
	//	newImage.rotation = rotMat;
	//	newImage.opacity = 1.0f;  // Start fully opaque
	//	afterImages.push_back(newImage);

	//	// Limit the number of afterimages stored
	//	if (afterImages.size() > maxAfterImages) {
	//		afterImages.erase(afterImages.begin());
	//	}

	//	// Update the opacity of each afterimage to create a fading effect
	//	for (auto& image : afterImages) {
	//		image.opacity -= (1.f / maxAfterImages);  // Decrease opacity over time
	//	}

	//	// Remove afterimages that are fully transparent
	//	afterImages.erase(std::remove_if(afterImages.begin(), afterImages.end(), [](const AfterImage& img) {
	//		return img.opacity <= 0.0f;
	//		}), afterImages.end());
	//	//	}
	//}

	//// Render each afterimage with reduced opacity
	//for (const auto& image : afterImages) {
	//	// Apply position and rotation
	//	Matrix modelMatrix = Matrix::CreateScale(1.49f) * image.rotation * Matrix::CreateTranslation(image.position);

	//	// Set transparency and draw the model
	//	Color afterImageColor = Color(playerColor.x, playerColor.y, playerColor.z, image.opacity);  // Use playerColor and apply opacity
	//	KdShaderManager::Instance().m_StandardShader.DrawModel(*bodyModel, modelMatrix, afterImageColor);
	//}

	afterCnt--;

	if (isDashing && afterCnt < 0) {
		// Iterate over the afterimage array
		for (int i = 0; i < afterNum; i++) {
			if (playerAfter[i].opacity <= 0) {
				// Calculate positions along the dash path
				playerAfter[i].position = pos + dashVec * (i * 1.0f);  // Spread the afterimages along the dash vector
				playerAfter[i].rotation = rotMat;

				// Set the animator's speed to 0 to freeze the animation at this point
				playerAfter[i].aftetAniSpd = 0;
				playerAfter[i].opacity = 1;
				afterCnt = 7;
			}
		

			
		}
	}

	
	
	for (int i = 0; i < afterNum; i++) {
		playerAfter[i].opacity -= 0.1;
		// Set the model matrix using position and rotation
		Matrix modelMatrix = Matrix::CreateScale(1.49f) * playerAfter[i].rotation * Matrix::CreateTranslation(playerAfter[i].position);

		// Set the transparency color
		Color afterImageColor = Color(playerColor.x, playerColor.y, playerColor.z, playerAfter[i].opacity);

		// Render the model using the frozen animator state
		KdShaderManager::Instance().m_StandardShader.DrawModel(*playerAfter[i].afterModel, modelMatrix, afterImageColor);
		playerAfter[i].afterAnimator->AdvanceTime(playerAfter[i].afterModel->WorkNodes(), playerAfter[i].aftetAniSpd);

	}


}

void Player::OnHit()
{
	if (mutekiCnt < 0) {
		hp -= 5;
		hpBuffer += 5;
		StunTransition();
		
	}
	

}

void Player::DebugKey()
{

	if (hp < 21) hp = 21;

	if (GameManager::Instance().GetIsEditorMode()) return;

	//if (GetAsyncKeyState('J') && stateCnt < 0) {
	//	stateCnt = 0;
	//	StrikeKillTransition();
	//	stateCnt = 60;

	//}

	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::VKey)) {
		InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(0, 3, 0));
	}

	//if (InputManager::Instance().IsKeyJustPressed(KeyFlg::FKey) && effCnt < 0) {
	//	//create class playerprojectile
	//	shared_ptr<PlayerProjectile> bullet = make_shared<PlayerProjectile>();
	//	bullet->SetPosition(pos + Vector3(0,2.8,0));
	//	bullet->SetMoveVec(atkVecOld);
	//	bullet->Init();
	//	SceneManager::Instance().AddObject(bullet);

	//	effCnt = 70;

	//	//KdEffekseerManager::GetInstance().Play("hitGround001.efkefc", { pos + Vector3(0,1,0) }, 0.7, 1, false);
	//	//KdEffekseerManager::GetInstance().Play("absorbYellow.efkefc", { pos + Vector3(0,1,0) }, 0.7, 1, true);
	//	//KdEffekseerManager::GetInstance().Play("EVFX02_15_SweepBlade.efkefc", { pos + Vector3(0,1,0) }, 0.7, 1, false);

	//}


	PathManager::Instance().FindPlayerPoint(pos.x, pos.z);


	//if (InputManager::Instance().IsKeyJustPressed(KeyFlg::GKey)) {
	//	KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { inventoryPlayerMat.Translation()}, 0.7, 1, false);

	//}

	//if (InputManager::Instance().IsKeyJustPressed(KeyFlg::XKey)) {
	//	//for (int i = 0; i < 70; i++) {
	//	
	//	//}

	//
	//	circleRangeAdd = Utility::Rnd(1, 4);
	//	circleRangeAdd = 0;
	//	//KdShaderManager::Instance().WorkAmbientController().AddConeLight(pos + Vector3(0, 2.1, 0), { 0,0,0 }, circleRangeMax, circleRangeAdd, { 3,3,3 });

	//	//KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 4,0,0,1 }, 10, pos, true);


	//	//shared_ptr<EnemyProjectile> magicRange = make_shared<EnemyProjectile>();
	//	//magicRange->SetPosition(pos);
	//	//magicRange->Init();
	//	//magicRange->InitRangeMagicAtk();
	//	//magicRange->SetType(eProjectileType::MagicRangeType);
	//	//SceneManager::Instance().AddObject(magicRange);

	//	//crreate player projectile class
	//	shared_ptr<PlayerProjectile> pcircle = make_shared<PlayerProjectile>();
	//	pcircle->SetType(pProjectileType::pRangeCircleType);
	//	pcircle->Init();
	//	pcircle->SetPosition(pos + Vector3(0, 2.1, 0));
	//	pcircle->SetMoveVec({ 0,0,0 });
	//	SceneManager::Instance().AddObject(pcircle);

	//}


	//KdShaderManager::Instance().WorkAmbientController().SetSingleLight(pos + Vector3(0, 2.1, 0), { 0,0,0 }, 14, 7, { 3,3,3 });

	//KdShaderManager::Instance().WorkAmbientController().SetSingleLightEnable(true);
	//KdShaderManager::Instance().WorkAmbientController().SetSingleLight(pos + Math::Vector3(0, 7, 0), worldMat.Backward(), 50, DirectX::XMConvertToRadians(30), { 3,3,3 }); //91 to circle {3,3,3} is strong white light
	//KdShaderManager::Instance().WorkAmbientController().SetSingleLightEnable(false);


	if (circleRangeAdd < circleRangeMax) circleRangeAdd += 0.01;

	/*KdShaderManager::Instance().WorkAmbientController().AddConeLight(pos + Vector3(0, 2.1, 0), { 0,0,0 }, circleRangeMax, circleRangeAdd, { 3,3,3 });
	circleRangeAdd += 0.005;*/

}




void Player::CallImgui()
{
	ImVec4 textColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);

	ImGui::Begin("Player");

	//blowpow blowgravity slider float 
	ImGui::SliderFloat("pblowPow", &toBlowPow, 0.0f, 21.0f);
	ImGui::SliderFloat("pblowGravity", &blowGravity, -0.1f, -7.f);


	//show scrollX scrollY
	ImGui::Text("scrollX : %f", scrollX);
	ImGui::Text("scrollY : %f", scrollY);

	//slider float scrollXMax min
	ImGui::SliderFloat("scrollXMax", &scrollXMax, -1000, 1000);
	//slider float scrollYMax min
	ImGui::SliderFloat("scrollYMax", &scrollYMax, -1000, 1000);

	//slider float scrollXMin
	ImGui::SliderFloat("scrollXMin", &scrollXMin, -1000.0f, 1000);
	//slider float scrollYMin
	ImGui::SliderFloat("scrollYMin", &scrollYMin, -1000, 1000);
	

	//slider float blowPow
	ImGui::SliderFloat("blowPow", &toBlowPow, 0.0f, 20.0f);
	//slider float blowGravity
	ImGui::SliderFloat("blowGravity", &blowGravity, -0.1f, -3.f);


	//show stateCnt 
	ImGui::Text("stateCnt : %f", stateCnt);


	//distToBlowTarget
	ImGui::Text("distToBlowTarget : %f", distToBlowTarget);

	//show mutekiCnt
	ImGui::Text("mutekiCnt : %f", mutekiCnt);

	//show atkAni1 string
	ImGui::Text("atkAni1 : %s", atkAni1.c_str());

	
	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),"NowState : %s", PlayerStateNames.at((PlayerState)nowState));
	ImGui::Text("nowState : %d", nowState);
	ImGui::Text("aniSpd : %f", aniSpd);

	//show hpbuffer buffermax

	ImGui::Text("hpBuffer : %f", hpBuffer);
	ImGui::Text("hpBufferMax : %f", hpBufferMax);

	//================================================================================================

	if (ImGui::CollapsingHeader("Status")) {
		ImGui::Text("Pos: %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);
		ImGui::Text("hp : %f", hp);
		ImGui::Text("stamina : %f", stamina);
		ImGui::Text("gravity : %f", gravity);

		ImGui::Checkbox("isLockOn", &isLockOn);
		//show currentlockDist and distToLockEnemy
		ImGui::Text("currentlockDist : %f", currentlockDist);
		ImGui::Text("distToLockEnemy : %f", distToLockEnemy);
	}

	if (ImGui::CollapsingHeader("Attack")) {
		//checkBox isWithinAtkFrame
		bool isW = isWithinAtkFrame();
		ImGui::Checkbox("isWithinAtkFrame", &isW);
		ImGui::Checkbox("isAttacking", &isAttacking);
		ImGui::Checkbox("isHitStop", &isHitStop);
		//check isAtkDelay
		ImGui::Checkbox("isAtkDelay", &isAtkDelay);
		//atkDelayCnt	
		ImGui::Text("atkDelayCnt : %f", atkDelayCnt);

		ImGui::Text("atkFrame : %f", atkFrame);
		ImGui::Text("hitFrame : %f", hitFrame);
		ImGui::Text("atkReadyFrame : %f", atkReadyFrame);
		ImGui::Text("atkEndFrame : %f", atkEndFrame);
		ImGui::Text("atkOrder : %d", atkOrder);
		ImGui::Text("atkMoveCnt : %f", atkMoveCnt);
		//atkCd
		ImGui::Text("atkCd : %f", atkCD);

		//stuncnt mutekicnt
		ImGui::Text("stunCnt : %f", stunCnt);
		ImGui::Text("mutekiCnt : %f", mutekiCnt);

		//show atkReadyAdjustFrame
		ImGui::Text("atkReadyAdjustFrame : %f", atkReadyAdjustFrame);

		//slider int skillId
		ImGui::SliderInt("skillId", &skillId, 0, 3);

		//show atkDmg 
		ImGui::Text("atkDmg : %f", atkDmg);

	}

	//================================================================================================

	if (ImGui::CollapsingHeader("Move,Dash,Vector")) {
		ImGui::Checkbox("isMoving", &isMoving);
		ImGui::Checkbox("isDashing", &isDashing);

		ImGui::Text("moveSpd : %f", moveSpd);

		ImGui::Text("moveVec : %f %f %f", moveVec.x, moveVec.y, moveVec.z);
		ImGui::Text("atkVecOld : %f %f %f", atkVecOld.x, atkVecOld.y, atkVecOld.z);
		ImGui::Text("atkVec : %f %f %f", atkVec.x, atkVec.y, atkVec.z);
		ImGui::Text("dashVec : %f %f %f", dashVec.x, dashVec.y, dashVec.z);

		ImGui::Text("rotAngle : %f", rotAngle);
		ImGui::Text("dashCd : %f", dashCd);
		ImGui::Text("dashDuration : %f", dashDuration);
	
	}

	//================================================================================================

	if (ImGui::CollapsingHeader("Fog,Ambient,Color")) {
		ImGui::ColorEdit4("PLayerColor", (float*)&playerColor);
		ImGui::ColorEdit4("AmbientColor", (float*)&ambientColor);
		ImGui::ColorEdit4("DirColor", (float*)&lightColor);
		ImGui::ColorEdit4("hfogColor", (float*)&hfogColor);
		ImGui::SliderFloat("dfogDensity", &dfogDensity, 0.0f, 3.f);
		ImGui::SliderFloat("topVal", &topVal, -7.0f, 7.0f);
		ImGui::SliderFloat("botVal", &botVal, -7.0f, 7.0f);
		ImGui::SliderFloat("hfogDist", &hfogDist, -50.0f, 100.0f);
		ImGui::SliderFloat("lightDirX", &lightDir.x, -1.0f, 2.0f);
		ImGui::SliderFloat("lightDirY", &lightDir.y, -1.0f, 2.0f);
		ImGui::SliderFloat("lightDirZ", &lightDir.z, -1.0f, 2.0f);
	}







//=================================================
//if (ImGui::BeginTabItem("TabItem")) {
//	ImGui::SliderFloat("HP", &hp, 0.0f, maxHp);
//	ImGui::EndTabItem();
//}
//
//if (ImGui::TreeNode("TreeNode ArrowOpen")) {
//	ImGui::SliderFloat("HP", &hp, 0.0f, maxHp);
//	ImGui::TreePop();
//}
//
//if (ImGui::CollapsingHeader("BlueOutline")) {
//	ImGui::SliderFloat("HP", &hp, 0.0f, maxHp);
//}



	ImGui::End();

}
