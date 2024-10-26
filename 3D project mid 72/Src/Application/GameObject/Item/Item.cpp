//Item.cpp file:

#include "Item.h"
#include"Application/Manager/GameManager.h"


void Item::Init()
{
	objTag = GameTag::ItemTag;

	model = make_shared<KdModelWork>();
	model->SetModelData(modelPath);

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("ItemCollision", model, KdCollider::TypeGround);

	switch (objType)
	{
	case ItemType::PotionItem:
		canConsume = true;
		isStackable = true;
		break;
	case ItemType::WeaponItem:
		canEquip = true;
		break;
	case ItemType::ArmorItem:
		canEquip = true;
		break;

	default:
		break;
	}


}

void Item::Update()
{
	rotAngle++;

	if (objType == ItemType::PotionItem) {
		Matrix scaleMat = Matrix::CreateScale(0.77, 0.77, 0.77);
		Matrix transMat = Matrix::CreateTranslation(pos);
		Matrix rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
		worldMat = scaleMat * rotMat * transMat;
	}
	else if (objType == ItemType::WeaponItem) {
		Matrix scaleMat = Matrix::CreateScale(2.1, 2.1, 2.1);
		Matrix transMat = Matrix::CreateTranslation(pos + Vector3(0,0,-1));
		Matrix rotMat = Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));

		worldMat = scaleMat * rotMat * transMat;
	}
	else if (objType == ItemType::ArmorItem) {
		Matrix scaleMat = Matrix::CreateScale(1.7, 1.7, 1.7);
		Matrix transMat = Matrix::CreateTranslation(pos + Vector3(0, -2.1, 0));
		Matrix rotMat = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotAngle));
		worldMat = scaleMat * rotMat * transMat;
	}
	

	if (!isEff) {
		//isEff = true;
		//auraEffId = KdEffekseerManager::GetInstance().m_nextUniqueId;
		//KdEffekseerManager::GetInstance().Play("changeWp.efkefc", { worldMat.Translation() + Vector3(0,-0.7,0)}, 0.4, 0.14, true);
	}


	if (isPickUp) SetExpired();

}

void Item::DrawLit()
{
	//Color itemColor = { 0.325,0.75,0.52,0.4 };
	Color itemColor = { 1,1,1,0.7 };
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat, itemColor);


}

void Item::GenerateDepthMapFromLight()
{
	
}

void Item::DrawBright()
{
	/*Color itemColor = { 0.325,0.75,0.52,0.4 };
	KdShaderManager::Instance().m_StandardShader.DrawModel(*model, worldMat,itemColor);*/

}

void Item::DrawMoji()
{
}

void Item::ItemGetPickUP()
{
	//KdEffekseerManager::GetInstance().StopEffectById(auraEffId);
	SetExpired();
}

void Item::CallImgui()
{
	ImGui::Begin("Item");
	ImGui::TextColored(imRed, "Item");
	ImGui::Text("Pos: %f, %f, %f", pos.x, pos.y, pos.z);
	ImGui::Text("ObjType: %d", objType);
	ImGui::End();
}
