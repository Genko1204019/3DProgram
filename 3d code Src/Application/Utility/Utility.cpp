#include"Utility.h"

float Utility::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

float Utility::Rnd(float _min, float _max)
{
	return _min + (rand() / (float)RAND_MAX) * (_max - _min);
}

float Utility::SinRange(float x, float minVal, float maxVal)
{
	float result = sin(x);
	result = (result + 1) / 2;
	result = result * (maxVal - minVal) + minVal;
	return result;
}

bool Utility::CheckHit2D(float a_target1X, float a_target1Y, float a_target2X, float a_target2Y, float a_hitDist)
{
	return false;
}

float Utility::min(float a, float b)
{
	return 0.0f;
}

float Utility::max(float a, float b)
{
	return 0.0f;
}

void Utility::CalOverlap(const std::list<KdCollider::CollisionResult>& _colRet, float& _overlap, bool& _isHit, Vector3& _hitPos, Vector3& _hitDir)
{
	for (const auto& result : _colRet) {
		if (result.m_overlapDistance > _overlap) {
			_overlap = result.m_overlapDistance;
			_hitPos = result.m_hitPos;
			_isHit = true;
			_hitDir = result.m_hitDir;
		}
	}
}

void Utility::CalOverlap(const std::list<KdCollider::CollisionResult>& _colRet, float& _overlap, bool& _isHit, Vector3& _hitPos, Vector3& _hitDir, Vector3& _normal)
{
	for (const auto& result : _colRet) {
		if (result.m_overlapDistance > _overlap) {
			_overlap = result.m_overlapDistance;
			_hitPos = result.m_hitPos;
			_isHit = true;
			_hitDir = result.m_hitDir;
			_normal = result.m_hitNormal;
		}
	}
}

void Utility::HitPositionAdjust(Vector3& _pos, const Vector3& _hitDir, float _overlap)
{
	_pos += _hitDir * _overlap;
}

void Utility::HitPositionAdjust2D(Vector3& _pos, const Vector3& _hitDir, float _overlap)
{
	_pos.x += _hitDir.x * _overlap;
	_pos.z += _hitDir.z * _overlap;
}
