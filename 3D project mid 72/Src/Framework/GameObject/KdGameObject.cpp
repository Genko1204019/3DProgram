#include "KdGameObject.h"
#include"Application/GameObject/Camera/GameCamera/GameCamera.h"

void KdGameObject::DrawDebug()
{
	// 早期リターン
	if (!debugWire)return;

	debugWire->Draw();
}

void KdGameObject::SetScale(float scalar)
{
	Math::Vector3 scale(scalar);

	SetScale(scale);
}

void KdGameObject::SetScale(const Math::Vector3& scale)
{
	Math::Vector3 vecX = worldMat.Right();
	Math::Vector3 vecY = worldMat.Up();
	Math::Vector3 vecZ = worldMat.Backward(); 
	
	vecX.Normalize();
	vecY.Normalize();
	vecZ.Normalize();

	worldMat.Right(vecX * scale.x);
	worldMat.Up(vecY * scale.y);
	worldMat.Backward(vecZ * scale.z);
}

Math::Vector3 KdGameObject::GetScale() const
{
	return Math::Vector3(worldMat.Right().Length(), worldMat.Up().Length(), worldMat.Backward().Length());
}

void KdGameObject::CalcDistSqrFromCamera(const Math::Vector3& camPos)
{
	m_distSqrFromCamera = (worldMat.Translation() - camPos).LengthSquared();
}

bool KdGameObject::Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!objCollider) { return false; }

	return objCollider->Intersects(targetShape, worldMat, pResults);
}

bool KdGameObject::Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!objCollider) { return false; }

	return objCollider->Intersects(targetBox, worldMat, pResults);
}

bool KdGameObject::Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults)
{
	if (!objCollider) { return false; }

	return objCollider->Intersects(targetShape, worldMat, pResults);
}
