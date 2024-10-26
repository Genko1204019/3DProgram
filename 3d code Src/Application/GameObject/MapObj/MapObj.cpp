#include"MapObj.h"
#include"Application/Manager/GameManager.h"
#include"Application/GameObject/Camera/GameCamera/GameCamera.h"
#include"Application/GameObject/Drop/Drop.h"
#include"Application/Manager/InventoryManager.h"
#include"Application/GameObject/Player/Weapon/PlayerWeapon.h"
#include"Application/Manager/DialogManager.h"
#include"Application/GameObject/Player/Player.h"

void MapObj::Init()
{
	objTag = GameTag::MapTag;


	InitMapInfo();
	
	if (rotY == 1) { rotAngle = 90; }
	else if (rotY == 0) { rotAngle = 0; }

	debugWire = std::make_unique<KdDebugWireFrame>();

	miniMapTex = make_shared<KdTexture>();
	miniMapTex->Load("Asset/Textures/miniMapChip01.png");

	coverTex = make_shared<KdTexture>();
	coverTex ->Load("Asset/Textures/miniMapChip01.png");

	chestPoly = make_shared<KdSquarePolygon>();
	chestPoly->SetMaterial("Asset/Textures/E-Key.png");
	chestPoly->SetSplit(2, 1);
	chestPoly->SetUVRect(0);
	chestPoly->SetScale(3);


	InitNPC();
}

void MapObj::Update()
{

	CameraInfo();

	if (rotY == 1) { rotAngle = 90; }
	else if (rotY == 0) { rotAngle = 0; }

	worldMat = Matrix::CreateScale(scale) * Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle)) * Matrix::CreateTranslation(pos);

	UpdateAnimation();

	AddPointLight();

	//KdCollider::SphereInfo sphere(KdCollider::TypeDamage, pos + Vector3(0,1.4,0), 2);
	//if(!isWalkable)		debugWire->AddDebugSphereByType(sphere,{1,0,0,1});


	CalDistPlayerToNPC();


	if (objType == MapType::DoorType) {

		const KdModelWork::Node* _pNode = model->FindWorkNode("AttachPoint1");
		Vector3 torchPos = _pNode->m_worldTransform.Translation();
	
		Matrix torchWorld;
		if (rotAngle == 90)	torchWorld = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle)) * Matrix::CreateTranslation(pos  + (torchPos * Vector3(1, 1, -1) + Vector3(-3.1,0, -3)));
		else torchWorld = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle)) * Matrix::CreateTranslation(pos + torchPos);
		
		if (!isfireEff1) {
			KdEffekseerManager::GetInstance().PlayById(fireEff1Id,"NA_simple_aura_large.efkefc", { torchWorld.Translation() }, 0.07, 0.7, false);
			isfireEff1 = true;
		}

		const KdModelWork::Node* _pNode2 = model->FindWorkNode("AttachPoint2");
		Vector3 torchPos2 = _pNode2->m_worldTransform.Translation();
		Matrix torchWorld2;
		if (rotAngle == 90)	torchWorld2 = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle)) * Matrix::CreateTranslation(pos + (torchPos2 * Vector3(1, 1, -1) + Vector3(1, 0, 1)));
		else torchWorld2 = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle)) * Matrix::CreateTranslation(pos + torchPos2);

		if (!isfireEff2) {
			KdEffekseerManager::GetInstance().PlayById(fireEff2Id,"NA_simple_aura_large.efkefc", { torchWorld2.Translation() }, 0.07, 0.7, false);
			isfireEff2 = true;
		}

	

	}

}

void MapObj::DrawLit()
{
	if(!hasModel || !model) return;
	if (GameManager::Instance().GetIsBattleMode()) return;

	
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor);
	
	//KdShaderManager::Instance().m_StandardShader.DrawPolygon(*chestPoly, Matrix::CreateTranslation(pos + posOffset + Vector3(0,5,0)));

	

}

void MapObj::GenerateDepthMapFromLight()
{
	if (!hasModel || !model) return;
	if (GameManager::Instance().GetIsBattleMode()) return;

	if(!isSpawn) KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, objColor); // clear shadow of bottle after anim
	
}

void MapObj::DrawUnLit()
{
	
}

void MapObj::PostUpdate()
{

	BottleCollision();
}

void MapObj::DrawBright()
{
	

}

void MapObj::DrawSprite()
{








}

void MapObj::DrawMini()
{
	switch (objType)
	{
	case MapType::LandType:
		miniMapColor = { 0,0,0,0 };
		break;
	case MapType::CorridorType:
		//miniMapColor = { 0,0,0,0.35 };
		miniMapColor = { 0,1,0,1 };
		break;
	case MapType::WallType:
		miniMapColor = { 1,1,1,miniMapAlpha };
		break;
	case MapType::GoalType:
		miniMapColor = { 0,1,0,miniMapAlpha };
		break;
	case MapType::DoorType:
		miniMapColor = { 0,0,1,1 };
		break;
	case MapType::ChestType:
		miniMapColor = { 0,1,0,0 };
		break;
	case MapType::BottleType:
		miniMapColor = { 1,1,0,0 };
		break;


	default:
		//miniMapColor = { 1,0,0,1 };
		break;
	}


	if (GameManager::Instance().GetIsMiniMap()) {
		miniMapPos2D.x = scaleFactor * pos.x + xOffset;
		miniMapPos2D.y = scaleFactor * pos.z + yOffset;

		Color subColor = { 1,1,1,0.4 };
		Color outColor = miniMapColor * subColor;

		if (objType == MapType::WallType) {
			Math::Rectangle miniMapRect = { 0,0,14,14 };
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2, 2, 2)  * Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0));
			KdShaderManager::Instance().m_spriteShader.DrawTex(iconTex,0,0, miniMapRect.width, miniMapRect.height, &miniMapRect, &kWhiteColor);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
		}
		else if (objType == MapType::BottleType) {
			Math::Rectangle miniMapRect = { 0,0,14,14 };
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2, 2, 2) * Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0));
			if(!hasTriger) KdShaderManager::Instance().m_spriteShader.DrawTex(iconTex, 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &kWhiteColor);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
		}
		else if (objType == MapType::ChestType || objType == MapType::MimicType || objType == MapType::GoalType) {
			Math::Rectangle miniMapRect = { 0,0,32,32 };
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1, 1, 1) * Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0));
			if (!hasTriger) KdShaderManager::Instance().m_spriteShader.DrawTex(iconTex, 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &kWhiteColor);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
		}
		else if (objType == MapType::DoorType) {
			Math::Rectangle miniMapRect = { 0,0,155,154 };
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(0.2, 0.2, 0.2) * Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0));
			KdShaderManager::Instance().m_spriteShader.DrawTex(iconTex, 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &kWhiteColor);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
		}
		else if (objType == MapType::TrapType) {
			Math::Rectangle miniMapRect = { 0,0,32,32 };
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(1, 1, 1) * Matrix::CreateTranslation(miniMapPos2D.x - GameManager::Instance().GetPlayerScrollX(), miniMapPos2D.y - GameManager::Instance().GetPlayerScrollY(), 0));
			KdShaderManager::Instance().m_spriteShader.DrawTex(iconTex, 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &kWhiteColor);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);
		}


		//else  KdShaderManager::Instance().m_spriteShader.DrawBox(miniMapPos2D.x - GameManager::Instance().scrollX, miniMapPos2D.y - GameManager::Instance().scrollY, 14, 14, &outColor, true);

		
		//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::CreateScale(2) * Matrix::CreateTranslation(miniMapPos2D.x, miniMapPos2D.y, 0));
		//KdShaderManager::Instance().m_spriteShader.DrawTex(miniMapTex, 0, 0, miniMapRect.width, miniMapRect.height, &miniMapRect, &miniMapColor, Math::Vector2{ 0, 0.5 });
		//KdShaderManager::Instance().m_spriteShader.SetMatrix(Matrix::Identity);





	}



}



void MapObj::InitMapInfo()
{
	
	iconTex = make_shared<KdTexture>();


	switch (objType)
	{	
	case MapType::GoalType:
		scale = { 0.3,0.3,0.3 };
		modelName = "Asset/Models/_MapObj/Goal/Goal.gltf";
		isWalkable = true;
		hasModel = true;
		iconTex->Load("Asset/Textures/GoalIcon.png");
		break;
	case MapType::WallType:
		scale = { 0.5,0.56,0.5 };
		modelName = "Asset/Models/_MapObj/Wall/Wall.gltf";
		isWalkable = false;
		hasModel = true;
		iconTex->Load("Asset/Textures/MapIconWall.png");
		break;
	case MapType::CorridorType:
		isWalkable = true;
		hasModel = false;
		break;
	case MapType::LandType:
		isWalkable = true;
		hasModel = false;
		break;
	case MapType::DoorType:
		scale = {1,1,1 };
		modelName = "Asset/Models/_MapObj/Door/Door.gltf";
		isWalkable = false;
		hasModel = true;
		hasAnime = true;
		aniSpd = 1.4;
		iconTex->Load("Asset/Textures/doorIcon.png");

		break;
	case MapType::ChestType:
		scale = { 0.7,0.7,0.7}; 
		modelName = "Asset/Models/_MapObj/ChestAni/ChestAni01.gltf";
		isWalkable = false;
		hasModel = true;
		hasAnime = true;
		aniSpd = 2.1;
		iconTex->Load("Asset/Textures/chestIcon.png");

		break;
	case MapType::MimicType:
		scale = { 0.7,0.7,0.7 };
		modelName = "Asset/Models/_MapObj/ChestMonsterAni/ChestMonsterAni.gltf";
		isWalkable = false;
		hasModel = true;
		hasAnime = true;
		aniSpd = 2.8;
		iconTex->Load("Asset/Textures/chestIcon.png");

		break;
	case MapType::BottleType:
		scale = { 0.7,0.7,0.7 };
		modelName = "Asset/Models/_MapObj/BarrelAni/BarrelAni02.gltf";
		isWalkable = false;
		hasModel = true;
		hasAnime = true;
		aniSpd = 1.4;
		iconTex->Load("Asset/Textures/BarrelIcon.png");

		break;
	case MapType::TrapType:
		scale = { 3,3,3 };
		posOffset = { 0,0.77,0 };
		modelName = "Asset/Models/_MapObj/SpikeAni2/SpikeAni2.gltf";
		isWalkable = false;
		hasModel = true;
		hasAnime = true;
		aniSpd = 4.2;
		iconTex->Load("Asset/Textures/SpikeIcon.png");
		break;
	case MapType::NPCType:
		scale = {1.77,1.77,1.77 };
		modelName = "Asset/Models/_MapObj/NPC//NPCBody/NPCBody.gltf";
		isWalkable = false;
		hasModel = true;
		break;
	case MapType::EnemyPointType:
		modelName = "Asset/Models/_MapObj/NPC//NPCBody/NPCBody.gltf";
		isWalkable = false;
		hasModel = false;
		break;


	default:
		break;
	}

	

	InitMatrix();

	if (hasModel) {
		InitModelInfo();
		InitCollider();
	}


	if (hasAnime) {
		anime = make_shared<KdAnimator>();
	}

	if (objType == MapType::NPCType) {
		anime = make_shared<KdAnimator>();
	}

	
	
}

void MapObj::InitModelInfo()
{
	model = make_shared<KdModelWork>();
	model->SetModelData(modelName);

	if (objType == MapType::ChestType) {
		subModel = make_shared<KdModelWork>();
		subModel->SetModelData("Asset/Models/_MapObj/Gold/Gold.gltf");
	}

}

void MapObj::InitCollider()
{


	
	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("MapsCollision", model, KdCollider::TypeGround);

}

void MapObj::InitMatrix()
{
	worldMat = Matrix::CreateScale(scale) *Matrix::CreateRotationY(rotY) *  Matrix::CreateTranslation(pos + posOffset);
}

void MapObj::InitNPC()
{
	if (objType == MapType::NPCType) {
		anime->SetAnimation(model->GetData()->GetAnimation("Cheer"), true);
		GameManager::Instance().SetIsBattleMode(false);
		KdEffekseerManager::GetInstance().Play("Simple_Turbulence_Fireworks.efkefc", { pos - Vector3(0,+4.2,0)}, 0.7, 1, true);
	}

}

void MapObj::BottleCollision()
{

	


	if (objType == MapType::BottleType) {
		KdCollider::SphereInfo sphere(KdCollider::TypeDamage, pos + Vector3(0,1.4,0), 1.77);
		std::list<KdCollider::CollisionResult> retList;

		for (auto& obj : SceneManager::Instance().GetObjList()) {
			if (obj->GetTag() == GameTag::PlayerWeaponTag || obj->GetTag() == GameTag::PlayerProjectileTag) {
				obj->Intersects(sphere, &retList);
			}

			float maxOverlapMap = 0;
			bool isHit = false;

			for (auto& ret : retList) {
				if (maxOverlapMap < ret.m_overlapDistance) {
					maxOverlapMap = ret.m_overlapDistance;
					isHit = true;
				}
			}

			if (isHit) {
				//cast to playerweapon class
				auto playerWeapon = std::static_pointer_cast<PlayerWeapon>(obj);
				if (playerWeapon->isWithAtkFrame == true) {
					OnHit();
				}

			}


		}
		

	}

}

void MapObj::UpdateAnimation()
{

	if (anime) {
		anime->AdvanceTime(model->WorkNodes(), aniSpd);
	}

	if(objType == GoalType || objType == TrapType) triggerResetCnt--;
	
	if (hasTriger && triggerResetCnt < 0) {
		hasTriger = false;
	}


	//bottle drop logic
	if (objType == MapType::BottleType) {
		objColor = { 1,1,1,objAlpha };
		if (hasTriger && anime->IsAnimationEnd()) { objAlpha -= 0.028; }
		if (objAlpha < 0 && !isSpawn) { 
			isSpawn = true;
			objCollider->SetEnable(KdCollider::TypeGround, false); 
			InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(0, 0.7, 0));
		}

	}


	if (objType == MapType::ChestType) { 
		if (hasTriger && anime->IsAnimationEnd() && !hasPlayEff) {
			hasPlayEff = true;
			KdEffekseerManager::GetInstance().Play("NA_v3_2.5d_shining_4.efkefc", { pos + Vector3(0,2.1,0) }, 0.7, 0.7, false);
			InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(-3.5, 0.7, 0));
			InventoryManager::Instance().DoRandomSpawnItem(pos + Vector3(3.5, 0.7, 0));
			KdAudioManager::Instance().PlayChestShineSE();

		}
	}

	if (objType == MapType::MimicType) {
		if (hasTriger && anime->IsAnimationEnd() && !hasPlayEff) {
			hasPlayEff = true;
			wpCamera.lock()->StartCameraTwist();
		}
	}

}


void MapObj::AddPointLight()
{
	if (objType == MapType::ChestType) {
		KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 2.1,2.1, 0 }, 49, {pos + Vector3(0,5.6,0)}, true);

	}
	else if (objType == MapType::TrapType) {
		KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 1.4,0, 1.4 }, 49, { pos + Vector3(0,5.6,0) }, true);

	}
	else if (objType == MapType::EnemyPointType) {
		KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 2.1,0, 0 }, 49, { pos + Vector3(0,5.6,0) }, true);

	}

}

void MapObj::CalDistPlayerToNPC()
{
	if (objType != MapType::NPCType) return;

	Vector3 playerPos = wpPlayer.lock()->GetPosition();
	dist = Vector3::Distance(playerPos, pos);

	if (dist < 2.1) canTriggerDialog = true;
	else if(dist >= 2.1) canTriggerDialog = false;
}

void MapObj::CameraInfo()
{
	const shared_ptr<GameCamera>spCam = wpCamera.lock();

	if (spCam) {
		spCam->WorkCamera()->ConvertWorldToScreenDetail(pos, texPos2D);
	}

}

void MapObj::ClearFireEff()
{
	KdEffekseerManager::GetInstance().StopEffectById(fireEff1Id);
	KdEffekseerManager::GetInstance().StopEffectById(fireEff2Id);
}

void MapObj::OnHit()
{

	/*if (objType == MapType::NPCType) {
		if (InputManager::Instance().IsKeyJustPressed(KeyFlg::EnterKey)) {
			if (canTriggerDialog) DialogManager::Instance().StartDialogTalk();
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 100);
		}
	}*/

	if (objType == MapType::NPCType) return; 

	if (!hasTriger) {
		hasTriger = true;
		if (hasAnime) anime->SetAnimation(model->GetData()->GetAnimation(0), false);

		switch (objType)
		{
		case MapType::BottleType:
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 15);
			KdAudioManager::Instance().PlayBarrelSE();
			break;
		case MapType::ChestType:
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 35);
			KdAudioManager::Instance().PlayChestSE();
			break;
		case MapType::MimicType:
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 15);
			KdAudioManager::Instance().PlayChestSE();
			break;
		case MapType::DoorType:
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 10);
			KdAudioManager::Instance().PlayDoorSE();
			break;
		case MapType::TrapType:
			triggerResetCnt = 35;
			GameManager::Instance().SetStageProgress(GameManager::Instance().GetStageProgress() + 10);
			KdAudioManager::Instance().PlayTrapSE();
			break;
		case MapType::GoalType:
			triggerResetCnt = 35;
			if (GameManager::Instance().GetStageProgress() >= 99) {
				GameManager::Instance().GoToNextFloor();
				SetExpired();
			}
			break;

		default:
			break;
		}


	}


}

void MapObj::CallImgui()
{
	//
	//if (objType == MapType::NPCType) {
	//	
	//	ImGui::Begin("MapObj");
	//	ImGui::Text("canTriggerDialog : %d", canTriggerDialog);
	//	//show dist
	//	ImGui::Text("dist : %f", dist);
	//	ImGui::End();
	//}

	//if (objType == MapType::DoorType) {
	//	ImGui::Begin("Door");
	//	//slider float rotAngle -180 to 180
	//	ImGui::SliderInt("rotAngle", &rotAngle, -180, 180);
	//	ImGui::End();


	//}


}
