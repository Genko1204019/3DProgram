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
#include"Application/Manager/UiManager.h"
#include"Application/Manager/MapLoader.h"
#include"Application/GameObject/Moji/Moji.h"

#include"Application/GameObject/Projectile/EnemyProjectile.h"
#include"Application/GameObject/Enemy/BattleEnemy/BattleEnemy.h"
#include"Application/GameObject/Enemy/WanderEnemy/WanderEnemy.h"
#include"Application/GameObject/Enemy/EnemyWeapon/EnemyWeapon.h"

bool Player::IsAtkEnable()
{
	if (atkFrame >= (atkReadyFrame / aniSpd) && atkFrame <= (atkEndFrame / aniSpd) && !isMagicWeapon)	return true;
	else																								return false;
	
}

bool Player::ReadyForAtk()
{
	if(isAttacking && IsAtkEnable())	return true;
	else if (isSkillReleased)			return true;
	else								return false;
}

bool Player::isPressingMoveKey()
{
	if (!GetAsyncKeyState('W') && (!GetAsyncKeyState('A')) && (!GetAsyncKeyState('S')) && (!GetAsyncKeyState('D'))) return false;
	else return true;
	
}

void Player::Init()
{
	objTag = GameTag::PlayerTag;
	objType = PlayerType::WarriorPlayer;

	InitModelInfo();
	InitAnimator();
	InitCollider();
	InitTexuture();
	InitStatus();
	//InitPlayerStatusFromJson();

	stateMachine.ChangeState(this, new pIdleState());
}

void Player::Update()
{
	UpdateMatrix();
	UpdateRotation();
	UpdateAnimation();
	UpdateGravity();
	UpdateEffect();
	UpdateCounter();
	UpdateStatus();
	UpdateMiniMap();

	EnemyInfo();
	CameraInfo();
	GameInfo();
	WeaponInfo();

	EquipAllGears();
	PlayerLockOn();

	MoveManager();       //must before stateMachine for rotation
	UpdateStateMachine();

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

	//static Math::Vector2 offset = { 0.0f,0.0f };
	//offset.x += 0.0001f;
	//if (offset.x > 1) offset.x -= 1;
	//offset.y += 0.0001f;
	//if (offset.y > 1) offset.y -= 1;
	//KdShaderManager::Instance().m_StandardShader.SetWaterUVOffset(offset);
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
}

void Player::DrawMoji()
{
}

void Player::DrawMini()
{
	if (GameManager::Instance().GetIsMiniMap()) {
		miniMapPos2D.x = scaleFactor * pos.x + xOffset;
		miniMapPos2D.y = scaleFactor * pos.z + yOffset;

		KdShaderManager::Instance().m_spriteShader.DrawBox(0,0, 1280, 720, &kBlackColor, true);
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

		Matrix sMat = Matrix::CreateScale(1, 1, 1);
		Matrix tmat = Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0);
		Matrix fMat = sMat * tmat;

		KdShaderManager::Instance().m_spriteShader.SetMatrix(fMat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&IconTex, 0, 0, iconRect.width, iconRect.height, &iconRect, &kWhiteColor, { 0.5,0.5 }); //fine
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);

	}
}

void Player::UpdateMiniMap()
{
	scrollX = pos.x * scaleFactor;
	scrollY = pos.z * scaleFactor;

	if (scrollX > scrollXMax) scrollX = scrollXMax;
	if (scrollX < scrollXMin) scrollX = scrollXMin;
	if (scrollY > scrollYMax) scrollY = scrollYMax;
	if (scrollY < scrollYMin) scrollY = scrollYMin;
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
	IconTex.Load("Asset/Textures/playerHelmet.png");

	m_normalTex = std::make_shared<KdTexture>();
	m_normalTex->Load("Asset/Textures/normalTex.png"); //!! error
	KdShaderManager::Instance().m_StandardShader.SetWaterNormalTexture(*m_normalTex); //send it to GPU

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
		moveSpd = j[playerType]["moveSpd"].get<float>();
		atkDmg = j[playerType]["atkDmg"].get<float>();
	}

}

void Player::SetAnime(string _aniName, bool _isLoop)
{
	bodyAnime->SetAnimation(bodyModel->GetData()->GetAnimation(_aniName), _isLoop);

	const shared_ptr<PlayerWeapon>spWeapon = wpWeapon.lock();
	if(spWeapon) spWeapon->SetAnime(_aniName, _isLoop);
	const shared_ptr<PlayerArmor>spArmor = wpArmor.lock();
	if (spArmor) spArmor->SetAnime(_aniName, _isLoop);
	const shared_ptr<PlayerShield>spShield = wpShield.lock();
	if (spShield) spShield->SetAnime(_aniName, _isLoop);

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

void Player::UpdateMatrix()
{
	scaleMat = Matrix::CreateScale(1.56);
	rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
	transMat = Matrix::CreateTranslation(pos);
	worldMat = scaleMat * rotMat * transMat;
	
	wpWorldMat = Matrix::CreateScale(2) * rotMat * Matrix::CreateTranslation(pos + Vector3(0, -0.1, 0));

	PathManager::Instance().FindPlayerPoint(pos.x, pos.z);

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
	if (!isMoving && !isDashing) return;

	Matrix rotationMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
	Vector3 nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, -rotationMat);
	Vector3 targetVec = {};

	if (isLockOn)											targetVec = camToEnemyVec;
	else {
		if (nowState == PlayerState::pDefense || isDashing) targetVec = atkVecOld;
		else												targetVec = moveVec;
	}

	float dot = nowVec.Dot(targetVec);
	dot = std::clamp(dot, -1.0f, 1.0f);

	Math::Vector3 cross = targetVec.Cross(nowVec);
	cross.Normalize();

	float angle = DirectX::XMConvertToDegrees(acos(dot));
	if (angle < 0.1f) return;
	angle = std::min(angle, 5.0f);

	if (cross.y >= 0) {
		rotAngle -= angle * 1.7; 
		if (rotAngle <= 0) rotAngle += 360;
	}
	else {
		rotAngle += angle * 1.7;
		if (rotAngle >= 360) rotAngle -= 360;
	}
		
}

void Player::UpdateGravity()
{
	if (!GameManager::Instance().GetIsStrikeSkill() && !GameManager::Instance().GetIsInventoryOpen()) {
		pos.y -= gravity;
		gravity += gravityPow;
	}

}

void Player::UpdateCounter()
{
	stateCnt -= 1.f * gameSpd;
	stunCnt -= 1.f * gameSpd;
	mutekiCnt -= 1.f * gameSpd;
	strikeCnt--;
	aniFrame++;

	atkMoveCnt -= 1.f * gameSpd;
	atkCD--;
	atkFrame += 1.f * gameSpd * aniSpd;
	//atkDelayCnt -= 1.f * gameSpd;

	dashCd -= 1 * gameSpd;
	dashDuration -=1 * gameSpd;
	defenseDuration -= 1.f * gameSpd;

	if (stamina < maxStamina) stamina += 0.15f * gameSpd;

}

void Player::UpdateStatus()
{
	//MP
	mp += 0.021;
	if (mp > maxMp) mp = maxMp;
	if (mp < 0) mp = 0;

	//HpBuffer
	hpBufferCd--;
	while (hpBuffer > 0 && hpBufferCd < 0) {
		hpBufferCd = 7;
		hpBufferMax -= 1;
		hpBuffer -= 1;
	}

}

void Player::UpdateEffect()
{
	KdEffekseerManager::GetInstance().SetPosById(dahsEffId, pos + Vector3(0, 1.4, 0));
}

void Player::UpdateStateMachine()
{
	stateMachine.Update(this);
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
		camToEnemyVec = { spCam->camToEnemy.x,0,spCam->camToEnemy.z };

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
		spWeapon->isWithAtkFrame = IsAtkEnable();
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


void Player::GroundCollision()
{
	
	const float stepHigh = 0.28f;
	KdCollider::RayInfo ray(KdCollider::TypeGround, pos + Vector3(0, stepHigh, 0),{0,-1,0}, gravity + stepHigh);
	std::list<KdCollider::CollisionResult> retRayList;
	bool isHit = false;
	float overlap = 0;
	Vector3 hitPos;
	Vector3 hitDir;

	for (auto& mapObj : SceneManager::Instance().GetObjList()) {
		if (mapObj->Intersects(ray, &retRayList)) {
			Utility::CalOverlap(retRayList, overlap, isHit, hitPos, hitDir);
			
			if (isHit) {
				HitGroundObj(hitPos + Math::Vector3(0, -0.14f, 0));
				HitTrapObj(mapObj);	
			}
		}
	}

}

void Player::HitGroundObj(Vector3 _hitPos)
{
	pos = _hitPos;
	gravity = 0;
}

void Player::HitTrapObj(const std::shared_ptr<KdGameObject>& _obj)
{

	if (_obj->GetType() == MapType::TrapType && mutekiCnt < 0) {
		_obj->OnHit();
		mutekiCnt = 70;
		hp -= 10;
		hpBuffer += 10;
		UiManager::Instance().bloodAlpha = 1;
		Vector3 blowDir = pos - _obj->GetPosition();
		blowDir.Normalize();
		BlowAwayTransition(blowDir, 2.8, -0.14);
	}
}

void Player::BodyCollision()
{
	KdCollider::SphereInfo bodySphere(KdCollider::TypeGround, pos + Vector3(0, 1.14, 0), 1.f);
	std::list<KdCollider::CollisionResult> retSphereList;
	//debugWire->AddDebugSphereByType(bodySphere,kGreenColor);
	float overlap = 0;
	bool isHit = false;
	Math::Vector3 hitDir;
	Vector3 hitPos;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->Intersects(bodySphere, &retSphereList)) {
			Utility::CalOverlap(retSphereList, overlap, isHit, hitPos, hitDir);

			if (isHit) {
				Utility::HitPositionAdjust2D(pos, hitDir, overlap);
				HitMapObj(obj);
				HitItemObj(obj);
			}


		}
	}



}


void Player::HitMapObj(const std::shared_ptr<KdGameObject>& _obj)
{
	if (_obj->GetTag() != GameTag::MapTag) return;

	if (_obj->GetType() == DoorType || _obj->GetType() == ChestType || _obj->GetType() == GoalType || _obj->GetType() == MimicType) {
		_obj->OnHit();
	}
}

void Player::HitItemObj(const std::shared_ptr<KdGameObject>& _obj)
{
	if (_obj->GetTag() != GameTag::ItemTag) return;

	shared_ptr<Item> item = static_pointer_cast<Item>(_obj);
	if (item->isPickUp == false) {
		InventoryManager::Instance().AddItem(item);
		shared_ptr<Moji> messageMoji = make_shared<Moji>();
		messageMoji->SetType(MojiType::MessageMoji);
		messageMoji->SetMojiText(item->itemName);
		messageMoji->Init();
		SceneManager::Instance().AddObject(messageMoji);
		KdAudioManager::Instance().PlayPickUpSE();
	}
}

void Player::DmgCollision()
{
	KdCollider::SphereInfo dmgSphere(KdCollider::TypeDamage,pos + Vector3(0,1,0),1.f);
	std::list<KdCollider::CollisionResult> retDmgList;
	//debugWire->AddDebugSphereByType(dmgSphere, kRedColor);

	float overlap = 0;
	bool isHit = false;
	Vector3 hitPos;
	Vector3 hitDir;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		if (obj->Intersects(dmgSphere, &retDmgList)) {
			for (auto& ret : retDmgList) {
				Utility::CalOverlap(retDmgList, overlap, isHit, hitPos, hitDir);

				if (isHit) {
					HitEnemy(obj);
					HitEnemyWeapon(obj);
					HitEnemyProjectile(obj);
				}

			}
		}
	}

}

void Player::HitEnemy(const std::shared_ptr<KdGameObject>& _obj)
{
	if (_obj->GetTag() != GameTag::EnemyTag) return;
	if (_obj->GetType() != EnemyType::eWanderer) return;

	shared_ptr<WanderEnemy> wanderEnemy = static_pointer_cast<WanderEnemy>(_obj);
	if (wanderEnemy->GetNowState() == EnemyState::eFollow || wanderEnemy->GetNowState() == EnemyState::eDizzy) {
		wpCamera.lock()->StartCameraTwist();
		wanderEnemy->WanderDeadTransition();
		KdAudioManager::Instance().PlayEncounterSE();
	}

}

void Player::HitEnemyWeapon(const std::shared_ptr<KdGameObject>& _obj)
{
	if (_obj->GetTag() != GameTag::EnemyWeaponTag) return;

	shared_ptr<EnemyWeapon> enemyWeapon = static_pointer_cast<EnemyWeapon>(_obj);
	if (enemyWeapon->GetIsAtkEnable()) {
		if (isDefensing) {
			if (mutekiCnt < 0) {
				mutekiCnt = 35;
				KdEffekseerManager::GetInstance().Play("greenShield.efkefc", { pos + Vector3(0,2.1,0) }, 0.4, 2.1, false);
			}
		}
		else OnHit();
	}

}

void Player::HitEnemyProjectile(const std::shared_ptr<KdGameObject>& _obj)
{
	if (_obj->GetTag() != GameTag::EnemyProjectileTag) return;

	_obj->OnHit();

	if (mutekiCnt < 0) {
		hp -= 5;
		hpBuffer += 5;
		mutekiCnt = 35;
		Vector3 blowDir = pos - _obj->GetPosition();
		blowDir.Normalize();
		BlowAwayTransition(blowDir, toBlowPow, blowGravity, true);
	}

	

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
			KdEffekseerManager::GetInstance().PlayById(dahsEffId,"dashEff04.efkefc", { pos + Vector3(0,2.1,0) }, 1.4, 1.4, false);
		}
	
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
	if (GameManager::Instance().GetIsInventoryOpen()) return;

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
	if (isLockOn) SetAnime(walkAni, false);
	else SetAnime(walkAni, true);
	nowState = PlayerState::pMove;
	isMoving = true;
	moveSpd = moveSpdWalk;

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
	if(isDefensing || isPressingMoveKey()) return;

	if (nowState == PlayerState::pAttack) {
		if (bodyAnime->IsAnimationEnd() && !isAttacking)	stateMachine.ChangeState(this, new pIdleState());		
	}
	else													stateMachine.ChangeState(this, new pIdleState());
	
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

	switch (atkOrder)
	{
	case 1:
		SetAnime(atkAni1, false);
		SetAtkInfo(2.f, 10, 17, 3.5, 1.7);
		break;
	case 2:
		SetAnime(atkAni2, false);
		SetAtkInfo(2.f, 12, 18, 3.5, 1.7);
		break;
	case 3:
		SetAnime(atkAni3, false);
		SetAtkInfo(2.f, 10, 17, 3.5, 1.7);
		break;
	case 4:
		SetAnime(atkAni4, false);
		SetAtkInfo(2.f, 5, 14, 3.5, 1.7);
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
		wpWeapon.lock()->slashEff = true;
		wpWeapon.lock()->atkVec = atkVecOld;
		ResetAnimeName();
	}


}

void Player::EnterAtk()
{
	aniSpd = 1.4;
	isAttacking = true;
	canAtkAnime = true;
	atkCD = 7;
	atkOrder++;
	atkFrame = 0;
	atkMoveCnt = 7;
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

void Player::SetAtkInfo(float _aniSpd, float _atkStartframe, float _atkEndFrame, float _atkMoveDist, float _pushEnemyPow)
{
	atkFrame = _atkStartframe;
	atkEndFrame = _atkEndFrame;
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
	atkMoveDist = 2.1;

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
		
		if (atkFrame > atkEndFrame) isAttacking = false;
		
	}

	

}

void Player::AtkDelay()
{

	if (atkFrame >= atkReadyFrame-2 && isAtkDelay) {
		if (atkDelayCnt > 0) {
			aniSpd = 0.14f;  
			atkDelayCnt--;   
		}

		if (atkDelayCnt <= 0) {
			isAtkDelay = false; 
			aniSpd = 1.4f;      
		}
	}

}

void Player::AtkEffect()
{

	if (IsAtkEnable() && !isAtkDelay) {
		if (canSlashEff) {
			canSlashEff = false;

			switch (atkOrder)
			{
			case 1:
				KdEffekseerManager::GetInstance().PlayById(slashEffId,"blueLeftSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 2:
				KdEffekseerManager::GetInstance().PlayById(slashEffId,"blueRightSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 3:
				KdEffekseerManager::GetInstance().PlayById(slashEffId,"blueDownSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
				break;
			case 4:
				KdEffekseerManager::GetInstance().PlayById(slashEffId,"blueRightSlash.efkefc", atkWpMovePos + Vector3(0, 1.4, 0), 1, 0.7, false);
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
	aniSpd = 1;
	SetAnime(defenseAni, false);
	isDefensing = true;
	defenseDuration = 35;
	nowState = PlayerState::pDefense;

}

void Player::PlayerBlowAway()
{

	distToBlowTarget = Vector3::Distance(pos, blowTargetPos);

	if (distToBlowTarget > 0.28f && stateCnt > 0) {
		float lerpFactor = 0.1f; // Lerp speed factor, can be adjusted
		pos.x = Utility::Lerp(pos.x, blowTargetPos.x, lerpFactor);
		pos.z = Utility::Lerp(pos.z, blowTargetPos.z, lerpFactor);

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
	blowTargetPos = pos + blowDir * blowPow;

	canRotate = _canRotate;
	KdAudioManager::Instance().PlayGetHitSE();
}

void Player::EnterBlowAway()
{
	aniSpd = 1;
	SetAnime(getHitAni, false);
	nowState = PlayerState::pBlowAway;
}

void Player::UpdateHitFrame()
{
	hitFrame = atkFrame;
	



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


	if (isSkillReleased) {
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
	aniSpd = 1;
	SetAnime(skillPrepareAni, false);
	stateCnt = 7000;
	aniFrame = 0;
	isSkillReleased = false;
	skillPrepareCnt = 350;
	moveSpd = moveSpdSkill;
	nowState = PlayerState::pSkill;

	KdEffekseerManager::GetInstance().PlayById(absordEffId,"absorbYellow.efkefc", pos + Vector3(0, 2.1, 0), 0.35, 1, false);
}

void Player::ExitSkill()
{
	if (absordEffId != -1) KdEffekseerManager::GetInstance().StopEffectById(absordEffId);
	isSkillReleased = false;
	aniSpd = 1;

}

void Player::ReleaseSkill()
{

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
	flySlash->SetType(pProjectileType::FlySlashType);
	flySlash->SetPosition(pos + Vector3(0, 2.8, 0));
	flySlash->SetMoveVec(atkVecOld);
	flySlash->SetRotY(rotAngle);
	flySlash->Init();
	SceneManager::Instance().AddObject(flySlash);

	skillCnt = 70;
	KdAudioManager::Instance().PlayPlayerSlashWaveSE();
}

void Player::MagicAtkSkill()
{
	shared_ptr<PlayerProjectile> flySlash = make_shared<PlayerProjectile>();
	flySlash->SetType(pProjectileType::FireBallType);
	flySlash->SetPosition(pos + Vector3(0, 2.8, 0));
	flySlash->SetMoveVec(atkVecOld);
	flySlash->SetRotY(rotAngle);
	flySlash->Init();
	SceneManager::Instance().AddObject(flySlash);

	skillCnt = 70;
	KdAudioManager::Instance().PlayPlayerSlashWaveSE();
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


}

void Player::StrikeKillTransition()
{
	aniSpd = 1;
	stateCnt = 770;
	strikeCombo = 0;
	strikeCnt = 0;
	pos = strikeStandPos;
	GameManager::Instance().SetIsStrikeSkill(true);
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

	aniSpd = 1;
	SetAnime(stunAni, false);
	stunCnt = 35;
	mutekiCnt = 70;
	nowState = PlayerState::pStun;
	stateMachine.ChangeState(this, new pStunState());

	KdEffekseerManager::GetInstance().Play("hitEff02.efkefc", { pos }, 1, 0.7, false);
	KdAudioManager::Instance().PlayGetHitSE();

	getHitDir = Vector3(pos.x, 0, pos.z) - Vector3(enemyPos.x, 0, enemyPos.z);
	getHitDir.Normalize();
	float rot = atan2(getHitDir.x, getHitDir.z);
	getHitPos = getHitDir * 1.4;

}

void Player::EnterStun()
{
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

	//if (GetAsyncKeyState('J') && stateCnt < 0) {
	//	stateCnt = 0;
	//	StrikeKillTransition();
	//	stateCnt = 60;
	//}

	if (GetAsyncKeyState(VK_SPACE)) {
		pos.y += 2.14 * gameSpd;
	}


	if (InputManager::Instance().IsKeyJustPressed(KeyFlg::VKey)) {
		InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(0, 3, 0));
	}



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
		bool isW = IsAtkEnable();
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
