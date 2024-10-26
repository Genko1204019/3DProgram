#include "Drop.h"

void Drop::Init()
{

	objTag = GameTag::DropTag;

	inModel = std::make_shared<KdModelWork>();
	inModel->SetModelData("Asset/Models/_ProjectileObj/Bullet/Bullet.gltf");

	outModel = std::make_shared<KdModelWork>();
	outModel->SetModelData("Asset/Models/_ProjectileObj/Bullet/Bullet.gltf");

	objCollider = std::make_unique<KdCollider>();
	objCollider->RegisterCollisionShape("DropCollision", inModel, KdCollider::TypeGround);

	debugWire = std::make_unique<KdDebugWireFrame>();

}

void Drop::Update()
{
	worldMat = Matrix::CreateTranslation(pos);
}

void Drop::DrawLit()
{

	KdShaderManager::Instance().m_StandardShader.DrawModel(*inModel, worldMat, objColor);
}

void Drop::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*inModel, worldMat, objColor);

}

void Drop::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*outModel, worldMat, objColor);

}

void Drop::PostUpdate()
{
	pos += moveVec * moveSpd;

	pos.y -= gravity;
	gravity += gravityPow;

	if (moveSpd >= 0) moveSpd -= moveSpdDegenerate;

	if (pos.y <= -69) {
		gravity = -bouncePow;
		bouncePow -= bouncePowDegenerate;

		if (bouncePow <= 0) {
			gravity = 0;
			moveSpd = 0;
			if(pos.y < -69) pos.y = -69;
		}
	}


	KdCollider::SphereInfo sphere(KdCollider::TypeGround,pos , 1.0f);
	debugWire->AddDebugSphereByType(sphere);
	std::list<KdCollider::CollisionResult> retList;

	for (auto& obj : SceneManager::Instance().GetObjList()) {
		
		obj->Intersects(sphere, &retList);	
		float overlap = 0;
		Math::Vector3 hitDir;
		bool isHit = false;

		for (auto& ret : retList) {
			if (overlap < ret.m_overlapDistance) {
				isHit = true;
				hitDir = ret.m_hitDir;
				overlap = ret.m_overlapDistance;

			}
		}

		if (isHit) {
		//	if (obj->GetTag() == GameTag::MapTag) {
			//	if (obj->GetType() == MapType::WallType) {
					hitDir.Normalize();
					pos.x += hitDir.x * overlap;
					pos.y += hitDir.y * overlap;
					pos.z += hitDir.z * overlap;
			//	}
				
		//	}
			
		}

	}



}

void Drop::CallImgui()
{
	//imgui begin drop
	ImGui::Begin("Drop");

	//show pos x y z
	ImGui::Text("pos x:%.2f y:%.2f z:%.2f", pos.x, pos.y, pos.z);

	//show gravity bouncePow moveSpd
	ImGui::Text("gravity:%.2f", gravity);
	ImGui::Text("bouncePow:%.2f", bouncePow);
	ImGui::Text("moveSpd:%.2f", moveSpd);
	//moveSpdDegenerate
	ImGui::Text("moveSpdDegenerate:%.2f", moveSpdDegenerate);
	//bouncePowDegenerate
	ImGui::Text("bouncePowDegenerate:%.2f", bouncePowDegenerate);

	//end imgui
	ImGui::End();
}
